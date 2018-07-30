#include "mcPrimaryGeneratorAction.hh"
#include "mcParticleGun.hh"
#include "mcDetectorConstruction.hh"

#include "G4Event.hh"
#include "G4MuonMinus.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

mcPrimaryGeneratorAction::mcPrimaryGeneratorAction(const mcDetectorConstruction* mcDC)
:particleTable(G4ParticleTable::GetParticleTable())
//,mcDetector(mcDC)
{
    particleGun = new mcParticleGun();
    G4ParticleDefinition* particle = particleTable->FindParticle("geantino");
    particleGun->SetParticleDefinition(particle);
    particleGun->SetParticleMomentumDirection(G4ThreeVector(1.0,0.0,0.0));
    particleGun->SetParticleEnergy(electron_mass_c2);
    particleGun->SetParticlePosition(G4ThreeVector(0.0,0.0,0.0));
    
    
}



mcPrimaryGeneratorAction::~mcPrimaryGeneratorAction()
{
    delete particleGun;
}



void mcPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    particleGun->GeneratePrimaryVertex(anEvent);
    
}



