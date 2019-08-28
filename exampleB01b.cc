//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file biasing/B01b/exampleB01b.cc
/// \brief Main program of the biasing/B01b example
//
//
//
//
// --------------------------------------------------------------
//      GEANT 4 - exampleB01b
//
// --------------------------------------------------------------
// Comments
//
// This example intends to show how to use importance sampling and scoring
// in the mass (tracking) geometry.
// A simple geometry consisting of a 180 cm high concrete cylinder
// divided into 18 slabs of 10cm each is created.
// Importance values are assigned to the 18 concrete slabs in the
// detector construction class for simplicity.
// Pairs of G4GeometryCell and importance values are stored in
// the importance store.
// Scoring is carried out by the multifunctional detector (MFD) and
// sensitive detectors
//
// Alex Howard (alexander.howard@cern.ch):
// 22/11/13: Migrated to the new MT compliant design which moves the
//           biasing process to the physicslist constructor - here
//           via the modular physicslists
//

// --------------------------------------------------------------
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include <iostream>
#include <stdlib.h>

#include "G4Types.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4VPhysicalVolume.hh"
#include "G4UImanager.hh"
#include "G4GeometryManager.hh"

// user classes
#include "B01bDetectorConstruction.hh"
#include "FTFP_BERT.hh"
#include "G4ImportanceBiasing.hh"
#include "G4WeightWindowBiasing.hh"

#include "B01bActionInitialization.hh"

// Files specific for biasing and scoring
#include "G4GeometrySampler.hh"
#include "G4IStore.hh"
#include "G4VWeightWindowStore.hh"
#include "G4WeightWindowAlgorithm.hh"

//Visualization
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char **argv)
{
  G4int mode = 0;
  G4String macro;
  if (argc==2)  mode = atoi(argv[1]);
  if (argc==3)  mode = atoi(argv[1]), macro = argv[2];

  G4int numberOfEvents = 10;
  G4long myseed = time( NULL );

  /*
#ifdef G4MULTITHREADED
  G4MTRunManager * runManager = new G4MTRunManager;
  G4cout << " Number of cores: " << G4Threading::G4GetNumberOfCores() << G4endl;
  G4cout << " and using 2 of them! " << G4endl;
  runManager->SetNumberOfThreads(2);
  //  runManager->SetNumberOfThreads(G4Threading::G4GetNumberOfCores());
  */
// #else
  G4RunManager * runManager = new G4RunManager;
// #endif

  G4Random::setTheSeed(myseed);

  G4VWeightWindowAlgorithm *wwAlg = 0; // pointer for WeightWindow (mode>0)

  // create the detector      ---------------------------
  B01bDetectorConstruction* detector = new B01bDetectorConstruction();
  runManager->SetUserInitialization(detector);
  G4GeometrySampler mgs(detector->GetWorldVolume(),"neutron");

  G4VModularPhysicsList* physicsList = new FTFP_BERT;
  
  // Custom modes
  // 0 or no arg: Importance sampling
  // 1: Weight window
  // else, no biasing

  G4String str_mode = ""; // For display message
  if(mode == 0)
    {
      str_mode = "RUN MADE USING IMPORTANCE BIASING";
      physicsList->RegisterPhysics(new G4ImportanceBiasing(&mgs));
    }
  else if (mode == 1)
    {
      str_mode = "RUN MADE USING WEIGHT WINDOW";
      wwAlg = new G4WeightWindowAlgorithm(1,    // upper limit factor
                                          1,    // survival factor
                                          100); // max. number of splitting

      physicsList->RegisterPhysics(new G4WeightWindowBiasing
                                  (&mgs, wwAlg, onBoundary));
                                    // place of action
    }
  else
    {
      str_mode = "RUN MADE USING ANALOG";
    }
  runManager->SetUserInitialization(physicsList);

  // Initialize visualization
  //
  // G4UIExecutive *ui = new G4UIExecutive(argc, argv);
  // G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  // visManager->Initialize();

 // Set user action classes through Worker Initialization
 //
  B01bActionInitialization* actions = new B01bActionInitialization;
  runManager->SetUserInitialization(actions);

  runManager->Initialize();

  if (mode == 0)
    {
      detector->CreateImportanceStore();
    }
  else if (mode == 1)
    {
      detector->CreateWeightWindowStore();
    }

  //  runManager->BeamOn(numberOfEvents);

  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (argc < 3)
  {
    UImanager->ApplyCommand("/control/cout/setCoutFile threadOut");
    UImanager->ApplyCommand("/analysis/setFileName B01b.root");
    G4String command_nevents = "/run/beamOn " +
                        G4UIcommand::ConvertToString(numberOfEvents);;
    UImanager->ApplyCommand(command_nevents);
  }
  else  UImanager->ApplyCommand("/control/execute " + macro);

  // open geometry for clean biasing stores clean-up
  //
  G4GeometryManager::GetInstance()->OpenGeometry();

  // ui->SessionStart();
  // delete ui;
  
  if (wwAlg) {
    delete wwAlg;
  }

  // mgs.ClearSampling();

  delete runManager;

  G4cout << "Random number generator seed: " << myseed << G4endl;  
  G4cout << str_mode << G4endl;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
