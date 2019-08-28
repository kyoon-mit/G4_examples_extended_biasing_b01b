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
/// \file B01bSDHit.hh
/// \brief Definition of the B01bSDHit class

#ifndef B01bSDHit_h
#define B01bSDHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"

/// Calorimeter hit class
///
/// It defines data members to store the the energy deposit and track lengths
/// of charged particles in a selected volume:
/// - fEdep, fTrackLength

class B01bSDHit : public G4VHit
{
  public:
    B01bSDHit();
    B01bSDHit(const B01bSDHit&);
    virtual ~B01bSDHit();

    // operators
    const B01bSDHit& operator=(const B01bSDHit&);
    G4bool operator==(const B01bSDHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw() {}
    virtual void Print();

    // methods to handle data
    void Set(G4int, G4int, G4int, G4double, G4double, G4double, G4double, G4bool, G4bool);
    // void Add(G4double de, G4double dl);

    // get methods
    G4int GetNLayers() const;
    G4int GetLayerNumber() const;
    G4int GetEndProcess() const;
    G4int GetNSecondaries() const;    
    G4double GetPreStepWeight() const;
    G4double GetPostStepWeight() const;
    G4double GetEdep() const;
    G4double GetAvgStepLength() const;
    G4bool IsEnter() const;
    G4bool IsExit() const;
      
  private:
    G4int fLayerNumber;    ///< The ID number of the detector cell
    G4int fEndProcess;     ///< Interaction limiting the step
    G4int fNSecondaries;         ///< Track ID
    G4double fPreStepWeight;   ///< The weight assigned to each cell & PreStepPoint
    G4double fPostStepWeight;  ///< The weight assined to each cell & PostStepPoint
    G4double fEdep;        ///< Energy deposit in the sensitive volume
    G4double fAvgStepLength;  ///< Step length in the hit
    G4bool fIsEnter;       ///< Track is entering the layer
    G4bool fIsExit;        ///< Track is exiting the layer
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using B01bSDHitsCollection = G4THitsCollection<B01bSDHit>;

extern G4ThreadLocal G4Allocator<B01bSDHit>* B01bSDHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* B01bSDHit::operator new(size_t)
{
  if (!B01bSDHitAllocator) {
    B01bSDHitAllocator = new G4Allocator<B01bSDHit>;
  }
  void *hit;
  hit = (void *) B01bSDHitAllocator->MallocSingle();
  return hit;
}

inline void B01bSDHit::operator delete(void *hit)
{
  if (!B01bSDHitAllocator) {
    B01bSDHitAllocator = new G4Allocator<B01bSDHit>;
  }
  B01bSDHitAllocator->FreeSingle((B01bSDHit*) hit);
}

inline void B01bSDHit::Set(G4int    layernumber,
                           G4int    endprocess,
                           G4int    nsecondaries,
                           G4double prestepweight,
                           G4double poststepweight,  
                           G4double edep,
                           G4double steplength,
                           G4bool   isenter,
                           G4bool   isexit       ) {
  
  fLayerNumber = layernumber;
  fEndProcess  = endprocess;
  fNSecondaries = nsecondaries;
  fPreStepWeight   = prestepweight;
  fPostStepWeight  = poststepweight;
  fEdep        = edep;
  fAvgStepLength  = steplength;
  fIsEnter     = isenter;
  fIsExit      = isexit;
}


inline G4int B01bSDHit::GetLayerNumber() const {
  return fLayerNumber;
}


inline G4int B01bSDHit::GetEndProcess() const {
  return fEndProcess;
}


inline G4int B01bSDHit::GetNSecondaries() const {
  return fNSecondaries;
}


inline G4double B01bSDHit::GetPreStepWeight() const {
  return fPreStepWeight;
}


inline G4double B01bSDHit::GetPostStepWeight() const {
  return fPostStepWeight;
}


inline G4double B01bSDHit::GetEdep() const { 
  return fEdep; 
}


inline G4double B01bSDHit::GetAvgStepLength() const {
  return fAvgStepLength;
}


inline G4bool B01bSDHit::IsEnter() const {
  return fIsEnter;
}


inline G4bool B01bSDHit::IsExit() const {
  return fIsExit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
