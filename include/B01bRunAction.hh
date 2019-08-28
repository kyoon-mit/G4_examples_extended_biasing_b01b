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
/// \file biasing/B01b/include/B01bRunAction.hh
/// \brief Definition of the B01bRunAction class
//
//
// 

#ifndef B01bRunAction_h
#define B01bRunAction_h 1

#include "G4UserRunAction.hh"

#include "B01bEventAction.hh"

#include "globals.hh"

class G4Run;

//=======================================================================
// B01bRunAction
//   
//
//
//=======================================================================
//
class B01bRunAction : public G4UserRunAction
{
public:
  // constructor and destructor
  B01bRunAction(B01bEventAction*);
  virtual ~B01bRunAction();

public:
  // virtual method from G4UserRunAction.
  virtual void BeginOfRunAction(const G4Run* aRun);
  virtual void EndOfRunAction(const G4Run* aRun);

/*
public:
  void PrintHeader(std::ostream *out);
  std::string FillString(const std::string &name, char c, G4int n
                        , G4bool back=true);
*/
private:
  // Data member 
  B01bEventAction* fEventAction;
  // G4String fSDName;  
  //  G4int fFieldName;
  // G4int fFieldValue;

};

//

#endif
