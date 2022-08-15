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
// $Id$
//
//-----------------------------------------------------------------------------
//
// GEANT4 Class header file 
//
// File name:  G4EmCaptureCascade
//
// Author:        V.Ivanchenko (Vladimir.Ivantchenko@cern.ch)
// 
// Creation date: 22 April 2012 on base of G4MuMinusCaptureCascade
//
// Class Description: 
//
// Simulation of electromagnetic cascade from capture level to K-shell
// of the mesonic atom
//
// Probabilities of gamma and Auger transitions from
// N.C.Mukhopadhyay Phys. Rep. 30 (1977) 1.
//
//-----------------------------------------------------------------------------
//
// Modifications: 
//
//-----------------------------------------------------------------------------

#ifndef CustomG4EmCaptureCascade_h
#define CustomG4EmCaptureCascade_h 1

#include "globals.hh"
#include "G4Nucleus.hh"
#include "G4Track.hh"
#include "G4HadProjectile.hh"
#include "G4HadSecondary.hh"
#include "G4HadFinalState.hh"
#include "G4HadronicInteraction.hh"
#include "G4RandomDirection.hh"
#include "G4ParticleDefinition.hh"
#include "G4DynamicParticle.hh"
#include "G4ThreeVector.hh"

class CustomG4EmCaptureCascade : public G4HadronicInteraction
{ 
public:
 
  explicit CustomG4EmCaptureCascade();
 
  virtual ~CustomG4EmCaptureCascade();

  virtual G4HadFinalState* ApplyYourself(const G4HadProjectile &aTrack, 
					 G4Nucleus & targetNucleus );

  virtual void ModelDescription(std::ostream& outFile) const; 

private:

  inline void AddNewParticle(G4ParticleDefinition* aParticle,
			     G4double kinEnergy);

  // hide assignment operator as private 
  CustomG4EmCaptureCascade& operator=(const CustomG4EmCaptureCascade &right) = delete;
  CustomG4EmCaptureCascade(const CustomG4EmCaptureCascade& ) = delete;

  G4HadFinalState result;
  G4ParticleDefinition* theElectron;
  G4ParticleDefinition* theGamma;
  G4double fMuMass;
  G4double fPbarMass;
  G4double fNbarMass;
  G4double fDbarMass;
  G4double fAntiParticleMass;
  G4int fAntiParticleType;
  G4double fTime;
  G4double fLevelEnergy[60];
  G4double fKLevelEnergy[93];
  
  G4double pbar_si[1000][4];
  G4double dbar_si[2000][4];
  G4double pbar_al[1000][4];
  G4double dbar_al[2000][4];
  G4double pbar_c[1000][4];
  G4double dbar_c[2000][4];  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void 
CustomG4EmCaptureCascade::AddNewParticle(G4ParticleDefinition* aParticle,
				   G4double kinEnergy)
{
  G4DynamicParticle* dp = new G4DynamicParticle(aParticle,
                                                G4RandomDirection(),
                                                kinEnergy);
  G4HadSecondary hs(dp);
  hs.SetTime(fTime);
  result.AddSecondary(hs);
}

#endif

