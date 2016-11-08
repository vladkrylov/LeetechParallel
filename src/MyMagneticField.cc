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
// $Id: B5MagneticField.cc 77656 2013-11-27 08:52:57Z gcosmo $
//
/// \file B5MagneticField.cc
/// \brief Implementation of the B5MagneticField class

#include "MyMagneticField.hh"

#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyMagneticField::MyMagneticField()
: G4MagneticField(), fMessenger(0), fBy(1.0*tesla)
{
    // define commands for this class
    DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyMagneticField::~MyMagneticField()
{ 
    delete fMessenger; 
}

void MyMagneticField::GetFieldValue(const G4double point[4],double *bField) const
{
    bField[0] = 0.;
    bField[1] = fBy;
    bField[2] = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyMagneticField::DefineCommands()
{
    // Define /B5/field command directory using generic messenger class
    fMessenger = new G4GenericMessenger(this, 
                                        "/Leetech/det/",
                                        "Field control");

    // fieldValue command 
    G4GenericMessenger::Command& valueCmd
      = fMessenger->DeclareMethodWithUnit("setField","tesla",
                                  &MyMagneticField::SetField,
                                  "Set field strength.");
    valueCmd.SetParameterName("field", true);
    valueCmd.SetDefaultValue("1.");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
