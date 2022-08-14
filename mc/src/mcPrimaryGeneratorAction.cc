#include "mcPrimaryGeneratorAction.hh"
#include "mcPrimaryGeneratorMessenger.hh"
#include "mcParticleGun.hh"
#include "mcDetectorConstruction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

mcPrimaryGeneratorAction::mcPrimaryGeneratorAction()
:particleTable(G4ParticleTable::GetParticleTable())
{
    particleGun = new mcParticleGun();
    G4ParticleDefinition* particle = particleTable->FindParticle("e-");
    particleGun->SetParticleDefinition(particle);
    particleGun->SetParticleMomentumDirection(G4ThreeVector(1.0,0.0,0.0));
    particleGun->SetParticleEnergy(electron_mass_c2);
    particleGun->SetParticlePosition(G4ThreeVector(0.0,0.0,0.0));

    gps = new G4GeneralParticleSource();
    pMessenger = new mcPrimaryGeneratorMessenger(this);
}



mcPrimaryGeneratorAction::~mcPrimaryGeneratorAction()
{
    delete particleGun;
    delete gps;
    delete pMessenger;
}

void mcPrimaryGeneratorAction::SelectAction(G4int i){
  selectedAction = i;
}

void mcPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if (selectedAction==0) particleGun->GeneratePrimaryVertex(anEvent);
  if (selectedAction==1) gps->GeneratePrimaryVertex(anEvent);
  if (selectedAction==2) GenerateCustom(anEvent);
}

void mcPrimaryGeneratorAction::GenerateCustom(G4Event* anEvent){

  constexpr G4double t0 = 0.;

  G4ParticleDefinition* pD0 = particleTable->FindParticle("geantino");
  G4PrimaryVertex* vertex0 = new G4PrimaryVertex(G4ThreeVector(0, 0, 1.*m), t0);
  G4PrimaryParticle* particle0 = new G4PrimaryParticle(pD0);
  particle0->SetKineticEnergy(1.*keV);
  particle0->SetMass(pD0->GetPDGMass());
  particle0->SetMomentumDirection(G4ThreeVector(0, 0, -1));
  vertex0->SetPrimary(particle0);
  anEvent->AddPrimaryVertex(vertex0);

  G4ParticleDefinition* pD1 = particleTable->FindParticle("alpha");

  G4PrimaryVertex* vertex1 = new G4PrimaryVertex(G4ThreeVector(0, 1.*m, 1.*m), t0);
  G4PrimaryParticle* particle1 = new G4PrimaryParticle(pD1);
  particle1->SetKineticEnergy(1.*keV);
  particle1->SetMass(pD1->GetPDGMass());
  particle1->SetMomentumDirection(G4ThreeVector(0, 0, -1));
  vertex1->SetPrimary(particle1);
  anEvent->AddPrimaryVertex(vertex1);

}


