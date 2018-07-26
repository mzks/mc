#ifndef mcParticleGun_h
#define mcParticleGun_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleGunMessenger.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include <fstream>
#include <vector>

class G4ParticleTable;
class G4Event;
class mcDetectorConstraction;
class mcParticleGunMessenger;


class mcParticleGun : public G4ParticleGun
{
	friend class mcParticleGunMessenger;
	public:
	mcParticleGun();
	~mcParticleGun();

	public:
	virtual void GeneratePrimaryVertex(G4Event*);
	void GenerateNeutron(G4PrimaryParticle* neutron[1]);
	void GenerateMuon(G4PrimaryParticle* muon[1]);
	void GenerateFluxNeutron(G4PrimaryParticle* neutron[1], G4ThreeVector vPos);
	void GenerateFluxNeutronSp(G4PrimaryParticle* neutron[1], G4ThreeVector vPos);
	void GenerateFission(G4PrimaryParticle* neutron[1]);

	const G4ThreeVector& PutCentre();
	const G4ThreeVector& PutTop();
	const G4ThreeVector& PutFlux();

	G4double LogLogInterpolatorCalculate(G4double);
	G4double LogLogInterpolatorCalculateSp(G4double);
	G4double LogLogInterpolatorCalculateFission(G4double);

	private:
	std::ifstream tableFile;
	std::vector<long double> muonE;
	std::vector<long double> muonFlux;
	std::vector<long double> muonPDF;

	std::ifstream tableFileSp;
	std::vector<long double> spE;
	std::vector<long double> spFlux;
	std::vector<long double> spPDF;

	std::ifstream tableFileFission;
	std::vector<long double> fissionE;
	std::vector<long double> fissionFlux;
	std::vector<long double> fissionPDF;

	G4ParticleTable*					particleTable;
	const mcDetectorConstraction*		mcDC;

	G4int positionFlag;
	enum{ UserPos=0, Top, Centre,flux};
	G4int particleFlag;
	enum{ User=0, Muon, Neutron,fluxNeutron,fluxNeutronSp,fission};
    G4double monoEnergy;

	mcParticleGunMessenger*	pMessenger;


};

#endif
