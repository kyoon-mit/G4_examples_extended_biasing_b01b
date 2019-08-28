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
/// \file B01bEventAction.cc
/// \brief Derived from examples/basic/B4/B4c/src/B4cEventAction.cc

#include "B01bEventAction.hh"
#include "B01bSensitiveDetector.hh"
#include "B01bSDHit.hh"
#include "B01bAnalysis.hh"
#include "B01bDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>
#include <string>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B01bEventAction::B01bEventAction()
 : G4UserEventAction(),
   fSDHCID(-1), // Sensitive Detector Hit Collection ID
   feventID(0), // Event ID
   fPreStepWeightVector{},
   fPostStepWeightVector{},
   fLayerNumberVector{},
   fNSecondariesVector(20),
   fEdepVector(20),
   fAvgStepLengthVector(20),
   fIsEnterVector(20),
   fIsExitVector(20),
   fNumHitsVector(20),
   fEndProcessVector{},
   fNSecondariesVector_w(20),
   fEdepVector_w(20),
   fAvgStepLengthVector_w(20),
   fIsEnterVector_w(20),
   fIsExitVector_w(20),
   fNumHitsVector_w(20)
{
  // Create analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(2);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B01bEventAction::~B01bEventAction()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B01bSDHitsCollection* 
B01bEventAction::GetHitsCollection(G4int hcID,
                                  const G4Event* event) const
{
  auto hitsCollection 
    = static_cast<B01bSDHitsCollection*>(
        event->GetHCofThisEvent()->GetHC(hcID));
  
  if ( ! hitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID; 
    G4Exception("B01bEventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }         

  return hitsCollection;
}    

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B01bEventAction::BeginOfEventAction(const G4Event* event)
{
  
  // ID of this event
  feventID = event->GetEventID();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B01bEventAction::EndOfEventAction(const G4Event* event)
{
  // Get hits collection ID
  if ( fSDHCID == -1) {
    fSDHCID
      = G4SDManager::GetSDMpointer()->GetCollectionID("SDHitsCollection");
  }
  
  // Get sensitive detector hits collections
  auto SDHC = GetHitsCollection(fSDHCID, event);
  auto nhits = SDHC->entries();
  G4cout << "Number of hits: " << nhits << G4endl;
  
  // Resize Vectors
  fPreStepWeightVector.resize(nhits);
  fPostStepWeightVector.resize(nhits);
  fLayerNumberVector.resize(nhits);
  
  // Empty Vectors
  fNSecondariesVector.clear(); fNSecondariesVector.resize(20);
  fEdepVector.clear(); fEdepVector.resize(20);
  fAvgStepLengthVector.clear(); fAvgStepLengthVector.resize(20);
  fIsEnterVector.clear(); fIsEnterVector.resize(20);
  fIsExitVector.clear(); fIsExitVector.resize(20);
  fNumHitsVector.clear(); fNumHitsVector.resize(20);
  fEndProcessVector.resize(nhits);
  
  fNSecondariesVector_w.clear(); fNSecondariesVector_w.resize(20);
  fEdepVector_w.clear(); fEdepVector_w.resize(20);
  fAvgStepLengthVector_w.clear(); fAvgStepLengthVector_w.resize(20);
  fIsEnterVector_w.clear(); fIsEnterVector_w.resize(20);
  fIsExitVector_w.clear(); fIsExitVector_w.resize(20);
  fNumHitsVector_w.clear(); fNumHitsVector_w.resize(20);
  
  // Read hits and store in vector
  for (auto i = 0; i < nhits; i++) {
    auto hit = (*SDHC)[i];    
    G4int l = hit->GetLayerNumber();
    G4double w = hit->GetPreStepWeight();
    
    // Vectors
    fPreStepWeightVector[i] = w;
    fPostStepWeightVector[i] = hit->GetPostStepWeight();
    fLayerNumberVector[i] = hit->GetLayerNumber();
    
    // Unweighted Vectors
    fNSecondariesVector[l]   += hit->GetNSecondaries();
    fEdepVector[l]           += hit->GetEdep()*CLHEP::MeV;
    fAvgStepLengthVector[l]  += hit->GetAvgStepLength()*CLHEP::mm;
    fIsEnterVector[l]        += hit->IsEnter();
    fIsExitVector[l]         += hit->IsExit();
    fNumHitsVector[l]        += 1;
    fEndProcessVector[i]      = hit->GetEndProcess();

    // Weighted Vectors
    fNSecondariesVector_w[l]  += hit->GetNSecondaries()*w;
    fEdepVector_w[l]          += hit->GetEdep()*CLHEP::MeV*w;
    fAvgStepLengthVector_w[l] += hit->GetAvgStepLength()*CLHEP::mm*w;
    fIsEnterVector_w[l]       += hit->IsEnter()*w;
    fIsExitVector_w[l]        += hit->IsExit()*w;
    fNumHitsVector_w[l]       += w;
  }
  
  // Get average step length
  for (auto l = 0; l < 20; ++l)
  {
    fAvgStepLengthVector[l] = fAvgStepLengthVector[l]/fNumHitsVector[l];
    fAvgStepLengthVector_w[l] = fAvgStepLengthVector_w[l]/fNumHitsVector_w[l];
  }
  
  
  // Get analysis manager instance
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->AddNtupleRow();
  
  if(event->GetNumberOfPrimaryVertex()==0)
  {
    G4cout << "Event is empty." << G4endl;
    return;
  }
  
  // Print per event (modulo n)
  //
  auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( feventID % printModulo == 0 ) ) {
    G4cout << "---> End of event: " << feventID << G4endl;
  }
  
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
