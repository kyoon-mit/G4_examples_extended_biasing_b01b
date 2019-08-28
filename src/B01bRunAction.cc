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
/// \file biasing/B01b/src/B01bRunAction.cc
/// \brief Implementation of the B01bRunAction class
//
//
// 
#include "B01bRunAction.hh"

//-- In order to obtain detector information.
#include "G4RunManager.hh"
#include "B01bDetectorConstruction.hh"
#include "G4THitsMap.hh"

#include "G4UnitsTable.hh"
#include "B01bAnalysis.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// B01bRunAction
//  
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Constructor
B01bRunAction::B01bRunAction(B01bEventAction* eventAction): 
  G4UserRunAction(),
  fEventAction(eventAction)
  //  fFieldName(15),
  // fFieldValue(14),
{
  // Print event number for each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);
  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  
  analysisManager->SetVerboseLevel(1);
  
  // Create ntuple
  analysisManager->CreateNtuple("events", "data");
  analysisManager->CreateNtupleDColumn("Weight_PreStepPoint", fEventAction->GetPreStepWeightVector() );
  analysisManager->CreateNtupleDColumn("Weight_PostStepPoint", fEventAction->GetPostStepWeightVector() );
  analysisManager->CreateNtupleIColumn("Layer_Number", fEventAction->GetLayerNumberVector() );
  analysisManager->CreateNtupleIColumn("Limiting_Process", fEventAction->GetEndProcessVector() );
  
  analysisManager->CreateNtupleIColumn("NSecondaries(unweighted)", fEventAction->GetNSecondariesVector() );
  analysisManager->CreateNtupleDColumn("Energy_Deposit(unweighted)", fEventAction->GetEdepVector() );
  analysisManager->CreateNtupleDColumn("Step_Length(unweighted)", fEventAction->GetAvgStepLengthVector() );
  analysisManager->CreateNtupleIColumn("Is_Enter(unweighted)", fEventAction->GetIsEnterVector() );
  analysisManager->CreateNtupleIColumn("Is_Exit(unweighted)", fEventAction->GetIsExitVector() );
  analysisManager->CreateNtupleIColumn("Num_Hits(unweighted)", fEventAction->GetNumHitsVector() );
  
  analysisManager->CreateNtupleDColumn("NSecondaries(weighted)", fEventAction->GetNSecondariesVector_w() );
  analysisManager->CreateNtupleDColumn("Energy_Deposit(weighted)", fEventAction->GetEdepVector_w() );
  analysisManager->CreateNtupleDColumn("Step_Length(weighted)", fEventAction->GetAvgStepLengthVector_w() );
  analysisManager->CreateNtupleDColumn("Is_Enter(weighted)", fEventAction->GetIsEnterVector_w() );
  analysisManager->CreateNtupleDColumn("Is_Exit(weighted)", fEventAction->GetIsExitVector_w() );
  analysisManager->CreateNtupleDColumn("Num_Hits(weighted)", fEventAction->GetNumHitsVector_w() );
  
  analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Destructor.
B01bRunAction::~B01bRunAction()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B01bRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run start." << G4endl;
  
  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  
  // Create output file
  analysisManager->OpenFile();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B01bRunAction::EndOfRunAction(const G4Run* aRun)
{
  G4cout << " ###### EndOfRunAction  " <<G4endl;

  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
  
  // Write
  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
