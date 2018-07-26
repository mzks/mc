#include "mcParticleGun.hh"
#include "mcParticleGunMessenger.hh"
#include "mcDetectorMessenger.hh"
#include "mcDetectorConstruction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4RotationMatrix.hh"
#include "Randomize.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <math.h>

using namespace CLHEP;

mcParticleGun::mcParticleGun()
	:G4ParticleGun(1),
	particleTable(G4ParticleTable::GetParticleTable()),
	positionFlag(0),
	particleFlag(0),
	monoEnergy(1.0*keV),pMessenger(0)

{
	pMessenger = new mcParticleGunMessenger(this);


	tableFile.open("muonTable.dat");
	double f1, f2;
	while(tableFile >> f1 >> f2){
		muonE.push_back(f1);
		muonFlux.push_back(f2);
	}

	double totalFlux = 0;
	for(decltype(muonFlux.size()) i=0;i<muonFlux.size();i++){
		totalFlux += muonFlux.at(i);
	}
	double muonFactor = 1.0 / totalFlux;
	totalFlux = 0.0;

	for(decltype(muonFlux.size()) i=0;i<muonFlux.size();i++){
		totalFlux += muonFlux.at(i);
		muonPDF.push_back(muonFactor* totalFlux);
	}

	tableFileSp.open("total_table.dat");
	//double f1, f2;
	while(tableFileSp >> f1 >> f2){
		spE.push_back(f1);
		spFlux.push_back(f2);
	}

	totalFlux = 0;
	for(decltype(spFlux.size()) i=0;i<spFlux.size();i++){
		totalFlux += spFlux.at(i);
	}
	double spFactor = 1.0 / totalFlux;
	totalFlux = 0.0;

	for(decltype(spFlux.size()) i=0;i<spFlux.size();i++){
		totalFlux += spFlux.at(i);
		spPDF.push_back(spFactor* totalFlux);
	}

	tableFileFission.open("cf.dat");
	//double f1, f2;
	while(tableFileFission >> f1 >> f2){
		fissionE.push_back(f1);
		fissionFlux.push_back(f2);
	}

	totalFlux = 0;
	for(decltype(fissionFlux.size()) i=0;i<fissionFlux.size();i++){
		totalFlux += fissionFlux.at(i);
	}
	double fissionFactor = 1.0 / totalFlux;
	totalFlux = 0.0;

	for(decltype(fissionFlux.size()) i=0;i<fissionFlux.size();i++){
		totalFlux += fissionFlux.at(i);
		fissionPDF.push_back(fissionFactor* totalFlux);
	}

}

mcParticleGun::~mcParticleGun()
{
	delete pMessenger;
}

void mcParticleGun::GeneratePrimaryVertex(G4Event* anEvent)
{
	G4ThreeVector vPos;
	if(positionFlag == Top)				vPos = PutTop();
	else if (positionFlag == Centre)	vPos = PutCentre();
	else if (positionFlag == flux)  	vPos = PutFlux();
	else 								vPos = particle_position;

	G4PrimaryVertex* vertex = new G4PrimaryVertex(vPos,particle_time);

	if (particleFlag == Muon){
		G4PrimaryParticle* particle[1]={0};
		GenerateMuon(particle);
		vertex->SetPrimary(particle[0]);
	}else if(particleFlag == Neutron){
		G4PrimaryParticle* particle[1]={0};
		GenerateNeutron(particle);
		vertex->SetPrimary(particle[0]);
	}else if(particleFlag == fluxNeutron){
		G4PrimaryParticle* particle[1]={0};
		GenerateFluxNeutron(particle,vPos);
		vertex->SetPrimary(particle[0]);
	}else if(particleFlag == fluxNeutronSp){
		G4PrimaryParticle* particle[1]={0};
		GenerateFluxNeutronSp(particle,vPos);
		vertex->SetPrimary(particle[0]);
	}else if(particleFlag == fission){
		G4PrimaryParticle* particle[1]={0};
		GenerateFission(particle);
		vertex->SetPrimary(particle[0]);
	}else{
		G4double mass = particle_definition->GetPDGMass();	
		for(G4int i=0; i<NumberOfParticlesToBeGenerated; i++){
			G4PrimaryParticle* particle = new G4PrimaryParticle(particle_definition);
			particle->SetKineticEnergy(particle_energy);
			particle->SetMass(mass);
			particle->SetMomentumDirection(particle_momentum_direction);
			particle->SetCharge(particle_charge);
			particle->SetPolarization(particle_polarization.x(),
										particle_polarization.y(),
										particle_polarization.z());
			vertex->SetPrimary(particle);
					
		}
	}
	anEvent->AddPrimaryVertex(vertex);
}

void mcParticleGun::GenerateNeutron(G4PrimaryParticle* neutron[1])
{
	G4ParticleDefinition* pD = particleTable->FindParticle("neutron");
	neutron[0] = new G4PrimaryParticle(pD);
	neutron[0]->SetKineticEnergy(1.0 * GeV);
	neutron[0]->SetMomentumDirection(G4ThreeVector(0,0,-1.0));
}

void mcParticleGun::GenerateFluxNeutron(G4PrimaryParticle* neutron[1],G4ThreeVector vPos)
{
	G4ParticleDefinition* pD = particleTable->FindParticle("neutron");
	neutron[0] = new G4PrimaryParticle(pD);
    //Set energy
	neutron[0]->SetKineticEnergy(monoEnergy);

    G4double angleX = asin(2*RandFlat::shoot(0.0,1.0)-1.0);
    G4double delta = RandFlat::shoot(0.0,pi);

    G4ThreeVector momVec;
    momVec.setX(-1*vPos.getX()/vPos.getR());
    momVec.setY(-1*vPos.getY()/vPos.getR());
    momVec.setZ(-1*vPos.getZ()/vPos.getR());

    G4double theta = momVec.getTheta();
    G4double phi = momVec.getPhi();
    momVec.rotateZ(-1*phi);
    momVec.rotateY(-1*theta);
    momVec.rotateY(angleX);
    momVec.rotateZ(delta);
    momVec.rotateY(theta);
    momVec.rotateZ(phi);

	neutron[0]->SetMomentumDirection(momVec);
}

void mcParticleGun::GenerateFluxNeutronSp(G4PrimaryParticle* neutron[1],G4ThreeVector vPos)
{
	G4ParticleDefinition* pD = particleTable->FindParticle("neutron");
	neutron[0] = new G4PrimaryParticle(pD);
    //Set energy
	G4double prob = RandFlat::shoot(0.0,1.0);
	neutron[0]->SetKineticEnergy( LogLogInterpolatorCalculateSp(prob) * MeV); //real spectrum

    G4double angleX = asin(2*RandFlat::shoot(0.0,1.0)-1.0);
    G4double delta = RandFlat::shoot(0.0,pi);

    G4ThreeVector momVec;
    momVec.setX(-1*vPos.getX()/vPos.getR());
    momVec.setY(-1*vPos.getY()/vPos.getR());
    momVec.setZ(-1*vPos.getZ()/vPos.getR());

    G4double theta = momVec.getTheta();
    G4double phi = momVec.getPhi();
    momVec.rotateZ(-1*phi);
    momVec.rotateY(-1*theta);
    momVec.rotateY(angleX);
    momVec.rotateZ(delta);
    momVec.rotateY(theta);
    momVec.rotateZ(phi);

	neutron[0]->SetMomentumDirection(momVec);
}

void mcParticleGun::GenerateFission(G4PrimaryParticle* neutron[1])
{
	G4ParticleDefinition* pD = particleTable->FindParticle("neutron");
	neutron[0] = new G4PrimaryParticle(pD);
    //Set energy
	G4double prob = RandFlat::shoot(0.0,1.0);
	neutron[0]->SetKineticEnergy( LogLogInterpolatorCalculateFission(prob) * MeV); //real spectrum

	G4double px, py, pz; 
	G4double cs, sn, phi;
	cs    =  RandFlat::shoot(-1.0,1.0);
	sn    =  std::sqrt((1.0-cs)*(1.0+cs));   
	phi   =  RandFlat::shoot(0., CLHEP::twopi);   
	px    =  sn*std::cos(phi);
	py    =  sn*std::sin(phi);
	pz    =  cs; 

	neutron[0]->SetMomentumDirection(G4ThreeVector(px, py, pz));
}
void mcParticleGun::GenerateMuon(G4PrimaryParticle* muon[1])
{
	G4ParticleDefinition* pD = particleTable->FindParticle("mu-");
	muon[0] = new G4PrimaryParticle(pD);
	muon[0]->SetMomentumDirection(G4ThreeVector(0,0,-1.0));//real shoot
	G4double prob = RandFlat::shoot(0.0,1.0);
	muon[0]->SetKineticEnergy( LogLogInterpolatorCalculate(prob) * GeV); //real spectrum
}

G4double mcParticleGun::LogLogInterpolatorCalculate(G4double x){

	G4double value = 0;

	if (x > 0.9999999 || x < 0.0) return 0.0;

	if(x < muonPDF.at(1) || x == 0.0){
		value = 0.0;
	}else if( x > 1.0){
		value = 0.0;
	}else {
		size_t i = 0;
		for(i=0;i<muonPDF.size();i++){
			if(x < muonPDF.at(i)){ break; }
		}
		G4double e1 = muonPDF.at(i-1);
		G4double e2 = muonPDF.at(i);
		G4double d1 = muonE.at(i-1);
		G4double d2 = muonE.at(i);

		value = (std::log10(d1)*std::log10(e2/x) + std::log10(d2)*std::log10(x/e1)) / std::log10(e2/e1);
		value = std::pow(10.0,value);
	}
	return value;
}



G4double mcParticleGun::LogLogInterpolatorCalculateSp(G4double x){

	G4double value = 0;

	if (x > 0.9999999 || x < 0.0) return 0.0;

	if(x < spPDF.at(1) || x == 0.0){
		value = 0.0;
	}else if( x > 1.0){
		value = 0.0;
	}else {
		size_t i = 0;
		for(i=0;i<spPDF.size();i++){
			if(x < spPDF.at(i)){ break; }
		}
		G4double e1 = spPDF.at(i-1);
		G4double e2 = spPDF.at(i);
		G4double d1 = spE.at(i-1);
		G4double d2 = spE.at(i);

		value = (std::log10(d1)*std::log10(e2/x) + std::log10(d2)*std::log10(x/e1)) / std::log10(e2/e1);
		value = std::pow(10.0,value);
	}
	return value;
}

G4double mcParticleGun::LogLogInterpolatorCalculateFission(G4double x){

	G4double value = 0;

	if (x > 0.9999999 || x < 0.0) return 0.0;

	if(x < fissionPDF.at(1) || x == 0.0){
		value = 0.0;
	}else if( x > 1.0){
		value = 0.0;
	}else {
		size_t i = 0;
		for(i=0; i<fissionPDF.size();i++){
			if(x < fissionPDF.at(i)){ break; }
		}
		G4double e1 = fissionPDF.at(i-1);
		G4double e2 = fissionPDF.at(i);
		G4double d1 = fissionE.at(i-1);
		G4double d2 = fissionE.at(i);

		value = (std::log10(d1)*std::log10(e2/x) + std::log10(d2)*std::log10(x/e1)) / std::log10(e2/e1);
		value = std::pow(10.0,value);
	}
	return value;
}

const G4ThreeVector& mcParticleGun::PutCentre(){
	static G4ThreeVector vPos(0.0,0.0,0.0);
	return vPos;
}

const G4ThreeVector& mcParticleGun::PutTop(){
	static G4ThreeVector vPos(0.0,0.0,49.9*cm);
	return vPos;
}
const G4ThreeVector& mcParticleGun::PutFlux(){
    G4double radius = 40*cm;
    G4double phi = RandFlat::shoot(0.0,twopi);
    G4double theta = RandFlat::shoot(0.0,pi);
    G4double posX =radius*sin(theta)*cos(phi);
    G4double posY =radius*sin(theta)*sin(phi);
    G4double posZ =radius*cos(theta);
	static G4ThreeVector vPos(0,0,0);
    vPos.setX(posX);
    vPos.setY(posY);
    vPos.setZ(posZ);
	return vPos;
}
