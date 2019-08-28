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
//
/// \file B01bSensitiveDetector.cc
/// \brief Based on examples/basic/B4/B01b/src/B01bSensitiveDetector.cc

#include "B01bSensitiveDetector.hh"
#include "B01bSDHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4VProcess.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4PSPopulation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B01bSensitiveDetector::B01bSensitiveDetector(
                            const G4String& name, 
                            const G4String& hitsCollectionName)
 : G4VSensitiveDetector(name),
   fHitsCollection(nullptr)
{ 
  collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B01bSensitiveDetector::~B01bSensitiveDetector() 
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B01bSensitiveDetector::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection 
    = new B01bSDHitsCollection(SensitiveDetectorName, collectionName[0]); 
    ; 

  // Add this collection in hce
  auto hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 

  /*
  // Create hits
  // fNofCells for cells + one more for total sums 
  for (G4int i=0; i<fNofCells+1; i++ ) {
    fHitsCollection->insert(new B01bSDHit());
  }
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool B01bSensitiveDetector::ProcessHits(G4Step* step, 
                                     G4TouchableHistory*)
{
  // Create a hit every step
  auto hit = new B01bSDHit();
  
  //-----------------------
  // Sensitive Detector
  //-----------------------
  // Information available:
  // 0. [Int]    Layer number of the hit (cell ID)
  // 1. [Int]    Interaction Limiting Step
  // 2. [Int]    Number of Created Secondaries
  // 3. [Double] Weight -- PreStepPoint
  // 4. [Double] Weight -- PostStepPoint
  // 5. [Double] Energy Deposit
  // 6. [Double] Step Length
  // 7. [Bool]   Track Entering
  // 8. [Bool]   Track Exiting
  
  auto pre_step = step->GetPreStepPoint();
  auto post_step = step->GetPostStepPoint();
  
  // 0. Layer number
  G4int LayerNumber = (pre_step->GetTouchable()->GetReplicaNumber(0));

  // 1. Interaction type that limits step
  G4int EndProcess = (post_step->GetProcessDefinedStep()->GetProcessSubType());
  
  // 2. Number of Created Secondaries
  G4int NSecondaries = (step->GetSecondaryInCurrentStep())->size();
  
  // 3-4. Weights
  G4double PreStepWeight = pre_step->GetWeight();
  G4double PostStepWeight = post_step->GetWeight();
  
  // 5. Energy Deposit
  G4double Edep = step->GetTotalEnergyDeposit();
  
  // 6. Step Length
  G4double StepLength = 0.;
  if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
    StepLength = step->GetStepLength();
  }
  
  if ( Edep==0. && StepLength == 0. ) return false; 
  
  // 7. Track Entering
  G4bool IsEnter = (pre_step->GetStepStatus() == fGeomBoundary);
  
  // 8. Track Exit
  G4bool IsExit = (post_step->GetStepStatus() == fGeomBoundary);

  
  // Add values
  hit->Set(LayerNumber, EndProcess, NSecondaries, PreStepWeight, PostStepWeight, Edep, StepLength, IsEnter, IsExit);
  
  // Add to hits collection
  fHitsCollection->insert(hit);
  
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B01bSensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
  if ( verboseLevel>1 ) { 
     auto nofHits = fHitsCollection->entries();
     G4cout
       << G4endl 
       << "-------->Hits Collection: in this event they are " << nofHits 
       << " hits in the tracker chambers: " << G4endl;
     for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
