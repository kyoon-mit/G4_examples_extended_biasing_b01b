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
/// \file B01bEventAction.hh
/// \brief Definition of the B01bEventAction class

#ifndef B01bEventAction_h
#define B01bEventAction_h 1

#include "G4UserEventAction.hh"

#include "B01bSDHit.hh"

#include "globals.hh"

/// Event action class
///
/// In EndOfEventAction(), it prints the accumulated quantities of the energy 
/// deposit and track lengths of charged particles in Absober and Gap layers 
/// stored in the hits collections.

class B01bEventAction : public G4UserEventAction
{
public:
  B01bEventAction();
  virtual ~B01bEventAction();

  virtual void  BeginOfEventAction(const G4Event* event);
  virtual void    EndOfEventAction(const G4Event* event);
  
  std::vector<G4double>& GetPreStepWeightVector()  {return fPreStepWeightVector; }
  std::vector<G4double>& GetPostStepWeightVector() {return fPostStepWeightVector; }
  std::vector<G4int>& GetLayerNumberVector() {return fLayerNumberVector; }
  
  std::vector<G4int>& GetNSecondariesVector() {return fNSecondariesVector; }
  std::vector<G4double>& GetEdepVector() {return fEdepVector; }
  std::vector<G4double>& GetAvgStepLengthVector() {return fAvgStepLengthVector; }
  std::vector<G4int>& GetIsEnterVector() {return fIsEnterVector; }
  std::vector<G4int>& GetIsExitVector() {return fIsExitVector; }
  std::vector<G4int>& GetNumHitsVector() {return fNumHitsVector; }
  std::vector<G4int>& GetEndProcessVector() {return fEndProcessVector; }
  
  std::vector<G4double>& GetNSecondariesVector_w() {return fNSecondariesVector_w; }
  std::vector<G4double>& GetEdepVector_w() {return fEdepVector_w; }
  std::vector<G4double>& GetAvgStepLengthVector_w() {return fAvgStepLengthVector_w; }
  std::vector<G4double>& GetIsEnterVector_w() {return fIsEnterVector_w; }
  std::vector<G4double>& GetIsExitVector_w() {return fIsExitVector_w; }
  std::vector<G4double>& GetNumHitsVector_w() {return fNumHitsVector_w; }
  
private:
  // methods
  B01bSDHitsCollection* GetHitsCollection(G4int hcID, const G4Event* event) const;
  
  // ---- data members ----- 
  G4int fSDHCID;
  G4int feventID;
  
  // Vectors
  std::vector<G4double> fPreStepWeightVector;
  std::vector<G4double> fPostStepWeightVector;
  std::vector<G4int> fLayerNumberVector;
  
  // Unweighted
  std::vector<G4int> fNSecondariesVector;
  std::vector<G4double> fEdepVector;
  std::vector<G4double> fAvgStepLengthVector;
  std::vector<G4int> fIsEnterVector;
  std::vector<G4int> fIsExitVector;
  std::vector<G4int> fNumHitsVector;
  std::vector<G4int> fEndProcessVector;
  
  // Weighted
  std::vector<G4double> fNSecondariesVector_w;
  std::vector<G4double> fEdepVector_w;
  std::vector<G4double> fAvgStepLengthVector_w;
  std::vector<G4double> fIsEnterVector_w;
  std::vector<G4double> fIsExitVector_w;
  std::vector<G4double> fNumHitsVector_w;  
};
                     
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
