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
/// \file B01bSDHit.cc
/// \brief Based on examples/basic/B4/B01b/src/B01bCalorHit.cc

#include "B01bSDHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<B01bSDHit>* B01bSDHitAllocator = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B01bSDHit::B01bSDHit()
 : G4VHit(),
   fLayerNumber(0),
   fEndProcess(0),
   fNSecondaries(0),
   fPreStepWeight(0.),
   fPostStepWeight(0.),
   fEdep(0.),
   fAvgStepLength(0.),
   fIsEnter(0),
   fIsExit(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B01bSDHit::~B01bSDHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B01bSDHit::B01bSDHit(const B01bSDHit& right)
  : G4VHit()
{
  fLayerNumber = right.fLayerNumber;
  fEndProcess  = right.fEndProcess;
  fNSecondaries = right.fNSecondaries;
  fPreStepWeight   = right.fPreStepWeight;
  fPostStepWeight  = right.fPostStepWeight;
  fEdep        = right.fEdep;
  fAvgStepLength  = right.fAvgStepLength;
  fIsEnter     = right.fIsEnter;
  fIsExit      = right.fIsExit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const B01bSDHit& B01bSDHit::operator=(const B01bSDHit& right)
{
  fLayerNumber = right.fLayerNumber;
  fEndProcess  = right.fEndProcess;
  fNSecondaries = right.fNSecondaries;
  fPreStepWeight   = right.fPreStepWeight;
  fPostStepWeight  = right.fPostStepWeight;
  fEdep        = right.fEdep;
  fAvgStepLength  = right.fAvgStepLength;
  fIsEnter     = right.fIsEnter;
  fIsExit      = right.fIsExit;
  
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool B01bSDHit::operator==(const B01bSDHit& right) const
{
  return ( this == &right ) ? true : false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B01bSDHit::Print() // Please Edit
{
  G4cout
     << "Edep: " 
     << std::setw(7) << G4BestUnit( fEdep,"Energy")
     << " step length: "
     << std::setw(7) << G4BestUnit( fAvgStepLength,"Length")
     << " interaction limiting step: "
     << fEndProcess
     << " layer number: " 
     << fLayerNumber
     << " weight (prestep): "
     << fPreStepWeight
     << " weight (poststep): "
     << fPostStepWeight
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
