#include "mcDetectorConstruction.hh"
#include "mcDetectorMessenger.hh"
#include "mcSensorSD.hh"
#include "mcAnalyzer.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Orb.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UniformMagField.hh"
#include "G4SDManager.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4NistManager.hh"


#include <iostream>

mcDetectorConstruction::mcDetectorConstruction()
:defaultMaterial(0),sensorMaterial(0),shieldMaterial(0),
WorldRadius(5.0*m),sensorRadius(10.0*cm),sensorHeight(20.0*cm),shieldThickness(20*cm),
solidWorld(0),logicWorld(0),physWorld(0),
solidSensor(0),logicSensor(0),physSensor(0),
magField(0),pUserLimits(0),maxStep(100.0*cm)
{
    
    // default parameter values of Sensor
    DefineMaterials();
    
    SetSensorMaterial("NaI");
    SetShieldMaterial("Lead");

    // create commands for interactive definition of the calorimeter
    detectorMessenger = new mcDetectorMessenger(this);
}

mcDetectorConstruction::~mcDetectorConstruction()
{ 
    delete detectorMessenger;
}

//------------------------------------------------------------------------//
// Begin of Construct()
G4VPhysicalVolume* mcDetectorConstruction::Construct()
{
    // Clean old geometry, if any
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();
    
    // create geometry
    
    // World
    solidWorld = new G4Orb("World",WorldRadius);
    logicWorld = new G4LogicalVolume(solidWorld,     //its solid
                                     defaultMaterial,//its material
                                     "World");		 //its name
    
    physWorld = new G4PVPlacement(0,			     //no rotation
                                  G4ThreeVector(),	 //at (0,0,0)
                                  logicWorld,		 //its logical volume
                                  "World",		     //its name
                                  0,			     //its mother  volume
                                  false,			 //no boolean operation
                                  0);			     //copy number
    
    // Sensor
    solidSensor = new G4Tubs("Sensor", 0, sensorRadius, sensorHeight/2., 0, CLHEP::twopi);
    logicSensor = new G4LogicalVolume(solidSensor,sensorMaterial,"Sensor");
    
    physSensor = new G4PVPlacement(0,G4ThreeVector(0, 0, 2.0*m+sensorHeight/2.),logicSensor,"Sensor",logicWorld,false,1);
    //physSensor = new G4PVPlacement(0,G4ThreeVector(20*cm,0,0),logicSensor,"Sensor",logicWorld,false,2);
    //physSensor = new G4PVPlacement(0,G4ThreeVector(40*cm,0,0),logicSensor,"Sensor",logicWorld,false,3);

    solidShield = new G4Box("Shield", 1.0*m, 1.0*m, shieldThickness/2.);
    logicShield = new G4LogicalVolume(solidShield, shieldMaterial, "Shield");
    physShield =  new G4PVPlacement(0, G4ThreeVector(0, 0, 1.0*m+shieldThickness/2.), logicShield, "Shield", logicWorld, false, 101);
    
    //------------------------------------------------
    // Sensitive detectors
    //------------------------------------------------
    
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    mcSensorSD* aSensorSD = (mcSensorSD*)SDman->FindSensitiveDetector("mc/SensorSD");
    if ( aSensorSD == 0){
        aSensorSD = new mcSensorSD("mc/SensorSD");
        SDman->AddNewDetector( aSensorSD );
    }
    aSensorSD->SetAnalyzer(analyzer);
    
    logicSensor->SetSensitiveDetector(aSensorSD);
    
    // Set UserLimits
    G4double maxTrkLen = 10.0*WorldRadius;
    G4double maxTime   = 1000.0 * ns;
    pUserLimits = new G4UserLimits(maxStep, maxTrkLen, maxTime);
    logicWorld->SetUserLimits(pUserLimits);
    
    // Visualization attributes
    logicWorld->SetVisAttributes (G4VisAttributes::GetInvisible());
    
    G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
    simpleBoxVisAtt->SetVisibility(true);
    logicSensor->SetVisAttributes(simpleBoxVisAtt);
    
    return physWorld;
}
// End of Construct()
//------------------------------------------------------------------------//


///////////////////////////////////////////////////////
void mcDetectorConstruction::DefineMaterials()
{ 
    //This function illustrates the possible ways to define materials
    
    G4String symbol, name;
    G4double a, z, density, temperature, pressure;
    // No use temperature & pressure in Geant4
    G4int iz, n, in;
    
    G4int ncomponents, natoms;
    G4double abundance, fractionmass;
    
    // a =mass of a mole;
    // z =mean number of protons;
    // iz=number of protons in an isotope;
    // n =number of nucleons in an isotope;

    auto nm = G4NistManager::Instance();

    // All elements
    G4Element* H  = nm->FindOrBuildElement("H");  G4Element* He = nm->FindOrBuildElement("He"); G4Element* Li = nm->FindOrBuildElement("Li");
    G4Element* Be = nm->FindOrBuildElement("Be"); G4Element* B  = nm->FindOrBuildElement("B");  G4Element* C  = nm->FindOrBuildElement("C");
    G4Element* N  = nm->FindOrBuildElement("N");  G4Element* O  = nm->FindOrBuildElement("O");  G4Element* F  = nm->FindOrBuildElement("F");
    G4Element* Ne = nm->FindOrBuildElement("Ne"); G4Element* Na = nm->FindOrBuildElement("Na"); G4Element* Mg = nm->FindOrBuildElement("Mg");
    G4Element* Al = nm->FindOrBuildElement("Al"); G4Element* Si = nm->FindOrBuildElement("Si"); G4Element* P  = nm->FindOrBuildElement("P");
    G4Element* S  = nm->FindOrBuildElement("S");  G4Element* Cl = nm->FindOrBuildElement("Cl"); G4Element* Ar = nm->FindOrBuildElement("Ar");
    G4Element* K  = nm->FindOrBuildElement("K");  G4Element* Ca = nm->FindOrBuildElement("Ca"); G4Element* Sc = nm->FindOrBuildElement("Sc");
    G4Element* Ti = nm->FindOrBuildElement("Ti"); G4Element* V  = nm->FindOrBuildElement("V");  G4Element* Cr = nm->FindOrBuildElement("Cr");
    G4Element* Mn = nm->FindOrBuildElement("Mn"); G4Element* Fe = nm->FindOrBuildElement("Fe"); G4Element* Co = nm->FindOrBuildElement("Co");
    G4Element* Ni = nm->FindOrBuildElement("Ni"); G4Element* Cu = nm->FindOrBuildElement("Cu"); G4Element* Zn = nm->FindOrBuildElement("Zn");
    G4Element* Ga = nm->FindOrBuildElement("Ga"); G4Element* Ge = nm->FindOrBuildElement("Ge"); G4Element* As = nm->FindOrBuildElement("As");
    G4Element* Se = nm->FindOrBuildElement("Se"); G4Element* Br = nm->FindOrBuildElement("Br"); G4Element* Kr = nm->FindOrBuildElement("Kr");
    G4Element* Rb = nm->FindOrBuildElement("Rb"); G4Element* Sr = nm->FindOrBuildElement("Sr"); G4Element* Y  = nm->FindOrBuildElement("Y");
    G4Element* Zr = nm->FindOrBuildElement("Zr"); G4Element* Nb = nm->FindOrBuildElement("Nb"); G4Element* Mo = nm->FindOrBuildElement("Mo");
    G4Element* Tc = nm->FindOrBuildElement("Tc"); G4Element* Ru = nm->FindOrBuildElement("Ru"); G4Element* Rh = nm->FindOrBuildElement("Rh");
    G4Element* Pd = nm->FindOrBuildElement("Pd"); G4Element* Ag = nm->FindOrBuildElement("Ag"); G4Element* Cd = nm->FindOrBuildElement("Cd");
    G4Element* In = nm->FindOrBuildElement("In"); G4Element* Sn = nm->FindOrBuildElement("Sn"); G4Element* Sb = nm->FindOrBuildElement("Sb");
    G4Element* Te = nm->FindOrBuildElement("Te"); G4Element* I  = nm->FindOrBuildElement("I");  G4Element* Xe = nm->FindOrBuildElement("Xe");
    G4Element* Cs = nm->FindOrBuildElement("Cs"); G4Element* Ba = nm->FindOrBuildElement("Ba"); G4Element* La = nm->FindOrBuildElement("La");
    G4Element* Ce = nm->FindOrBuildElement("Ce"); G4Element* Pr = nm->FindOrBuildElement("Pr"); G4Element* Nd = nm->FindOrBuildElement("Nd");
    G4Element* Pm = nm->FindOrBuildElement("Pm"); G4Element* Sm = nm->FindOrBuildElement("Sm"); G4Element* Eu = nm->FindOrBuildElement("Eu");
    G4Element* Gd = nm->FindOrBuildElement("Gd"); G4Element* Tb = nm->FindOrBuildElement("Tb"); G4Element* Dy = nm->FindOrBuildElement("Dy");
    G4Element* Ho = nm->FindOrBuildElement("Ho"); G4Element* Er = nm->FindOrBuildElement("Er"); G4Element* Tm = nm->FindOrBuildElement("Tm");
    G4Element* Yb = nm->FindOrBuildElement("Yb"); G4Element* Lu = nm->FindOrBuildElement("Lu"); G4Element* Hf = nm->FindOrBuildElement("Hf");
    G4Element* Ta = nm->FindOrBuildElement("Ta"); G4Element* W  = nm->FindOrBuildElement("W");  G4Element* Re = nm->FindOrBuildElement("Re");
    G4Element* Os = nm->FindOrBuildElement("Os"); G4Element* Ir = nm->FindOrBuildElement("Ir"); G4Element* Pt = nm->FindOrBuildElement("Pt");
    G4Element* Au = nm->FindOrBuildElement("Au"); G4Element* Hg = nm->FindOrBuildElement("Hg"); G4Element* Tl = nm->FindOrBuildElement("Tl");
    G4Element* Pb = nm->FindOrBuildElement("Pb"); G4Element* Bi = nm->FindOrBuildElement("Bi"); G4Element* Po = nm->FindOrBuildElement("Po");
    G4Element* At = nm->FindOrBuildElement("At"); G4Element* Rn = nm->FindOrBuildElement("Rn"); G4Element* Ac = nm->FindOrBuildElement("Ac");
    G4Element* Th = nm->FindOrBuildElement("Th"); G4Element* Pa = nm->FindOrBuildElement("Pa"); G4Element* U  = nm->FindOrBuildElement("U");
    G4Element* Np = nm->FindOrBuildElement("Np"); G4Element* Pu = nm->FindOrBuildElement("Pu"); G4Element* Am = nm->FindOrBuildElement("Am");
    G4Element* Cm = nm->FindOrBuildElement("Cm"); G4Element* Bk = nm->FindOrBuildElement("Bk"); G4Element* Cf = nm->FindOrBuildElement("Cf");

    // define an Element from isotopes, by relative abundance
    G4Isotope* U5 = new G4Isotope("U235", iz=92, n=235, a=235.01*g/mole);
    G4Isotope* U8 = new G4Isotope("U238", iz=92, n=238, a=238.03*g/mole);
    
    G4Element* en_U  = new G4Element("enriched Uranium",symbol="en_U",ncomponents=2);
    en_U->AddIsotope(U5, abundance= 90.*perCent);
    en_U->AddIsotope(U8, abundance= 10.*perCent);
    
    // Case 1 : define simple materials
    new G4Material("liquidArgon", z=18., a= 39.95*g/mole, density= 1.390*g/cm3);
    new G4Material("Lead"     , z=82., a= 207.19*g/mole, density= 11.35*g/cm3);
    new G4Material("Tungsten" , z=74., a= 183.84*g/mole, density= 19.25*g/cm3);
    
    // Case 2 : define a material from elements.   
    // case 2-1: chemical molecule
    G4Material* H2O =
    new G4Material("Water", density= 1.000*g/cm3, ncomponents=2);
    H2O->AddElement(H, natoms=2);
    H2O->AddElement(O, natoms=1);
    // overwrite computed meanExcitationEnergy with ICRU recommended value
    H2O->GetIonisation()->SetMeanExcitationEnergy(75.0*eV);

    G4Material* SiO2 = new G4Material("quartz",density= 2.200*g/cm3, ncomponents=2);
    SiO2->AddElement(Si, natoms=1);
    SiO2->AddElement(O , natoms=2);

    // case 2-2: mixture by fractional mass
    G4Material* Air = new G4Material("Air"  , density= 1.290*mg/cm3, ncomponents=2);
    Air->AddElement(N, fractionmass=0.7);
    Air->AddElement(O, fractionmass=0.3);
    
    // case 3: define a material from elements and/or others materials (mixture of mixtures)
    G4Material* Aerogel = new G4Material("Aerogel", density= 0.200*g/cm3, ncomponents=3);
    Aerogel->AddMaterial(SiO2, fractionmass=62.5*perCent);
    Aerogel->AddMaterial(H2O , fractionmass=37.4*perCent);
    Aerogel->AddElement (C   , fractionmass= 0.1*perCent);

    // case 4: define a material from isotope
    G4Isotope* he3 = new G4Isotope(name="he3", iz=2, in=3, a=3.0160293191*g/mole);
    G4Element* He3 = new G4Element(name="He3", symbol="He3", ncomponents=1);
    He3->AddIsotope(he3, abundance=100.*perCent);
    G4Material* Herium3 = new G4Material(name="Herium3",density=1.34644166*mg/cm3,ncomponents=1,kStateGas,temperature=300.15*kelvin,pressure=10.*atmosphere);
    Herium3->AddElement(He3, 1);
    
    // case 5: Find material in NIST database
    // https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/Appendix/materialNames.html#nist-compounds

    G4Material* G4_lXe = nm->FindOrBuildMaterial("G4_lXe");
    G4Material* G4_lH2 = nm->FindOrBuildMaterial("G4_lH2");
    G4Material* G4_lO2 = nm->FindOrBuildMaterial("G4_lO2");
    G4Material* G4_lN2  = nm->FindOrBuildMaterial("G4_lN2");
    G4Material* G4_lAr = nm->FindOrBuildMaterial("G4_lAr");
    G4Material* G4_Galactic = nm->FindOrBuildMaterial("G4_Galactic");
    G4Material* G4_ACETONE = nm->FindOrBuildMaterial("G4_ACETONE");
    G4Material* G4_STAINLESS_STEEL = nm->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    // 
    // Define materials for your experiment
    //

    // Plastic scintillator 
    G4Material* Scintillator = new G4Material("Scintillator", density= 1.032*g/cm3, ncomponents=2);
    Scintillator->AddElement(C, natoms=9);
    Scintillator->AddElement(H, natoms=10);
    
    G4Material* Myl = new G4Material("Mylar", density= 1.397*g/cm3, ncomponents=3);
    Myl->AddElement(C, natoms=10);
    Myl->AddElement(H, natoms= 8);
    Myl->AddElement(O, natoms= 4);
    
    
    // Stainless-Steel 304 
    G4Material* sus304 = new G4Material(name="SUS304", density=8.03*g/cm3, ncomponents=5);
    sus304->AddElement(Ni, 0.09);
    sus304->AddElement(C, 0.005);
    sus304->AddElement(Mn, 0.01);
    sus304->AddElement(Cr, 0.18);
    sus304->AddElement(Fe, 0.715);

    G4Material* polyethylene=new G4Material(name="polyethylene",density=0.92*g/cm3,ncomponents=2);
    polyethylene->AddElement(C,1);
    polyethylene->AddElement(H,2);

    
    // examples of gas in non STP conditions
    G4Material* CO2 = new G4Material("CarbonicGas", density= 27.*mg/cm3, ncomponents=2, kStateGas, 325.*kelvin, 2.*atmosphere);
    CO2->AddElement(C, natoms=1);
    CO2->AddElement(O, natoms=2);
    
    G4Material* NaI = new G4Material("NaI", density= 3.67*g/cm3, ncomponents=2);
    NaI->AddElement(Na, natoms=1);
    NaI->AddElement(I, natoms=1);
    
    G4Material* steam = new G4Material("WaterSteam", density= 0.3*mg/cm3, ncomponents=1, kStateGas, 500.*kelvin, 2.*atmosphere);
    steam->AddMaterial(H2O, fractionmass=1.);
    
    // examples of vacuum
    G4Material* Vacuum =
    new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                   kStateGas, 2.73*kelvin, 3.e-18*pascal);
    
    G4Material* beam = new G4Material("Beam", density= 1.e-5*g/cm3, ncomponents=1, kStateGas, STP_Temperature, 2.e-2*bar);
    beam->AddMaterial(Air, fractionmass=1.);

    //
    // other materials
    //

    // NIST materials : These are not all materials included in the database
    G4Material* G4_A_150_TISSUE = nm->FindOrBuildMaterial("G4_A-150_TISSUE");
    G4Material* G4_ACETYLENE = nm->FindOrBuildMaterial("G4_ACETYLENE");
    G4Material* G4_ADENINE = nm->FindOrBuildMaterial("G4_ADENINE");
    G4Material* G4_AIR = nm->FindOrBuildMaterial("G4_AIR");
    G4Material* G4_ALANINE = nm->FindOrBuildMaterial("G4_ALANINE");
    G4Material* G4_ALUMINUM_OXIDE = nm->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
    G4Material* G4_AMBER = nm->FindOrBuildMaterial("G4_AMBER");
    G4Material* G4_AMMONIA = nm->FindOrBuildMaterial("G4_AMMONIA");
    G4Material* G4_ANILINE = nm->FindOrBuildMaterial("G4_ANILINE");
    G4Material* G4_ANTHRACENE = nm->FindOrBuildMaterial("G4_ANTHRACENE");
    G4Material* G4_BAKELITE = nm->FindOrBuildMaterial("G4_BAKELITE");
    G4Material* G4_BENZENE = nm->FindOrBuildMaterial("G4_BENZENE");
    G4Material* G4_BERYLLIUM_OXIDE = nm->FindOrBuildMaterial("G4_BERYLLIUM_OXIDE");
    G4Material* G4_BGO = nm->FindOrBuildMaterial("G4_BGO");
    G4Material* G4_BORON_CARBIDE = nm->FindOrBuildMaterial("G4_BORON_CARBIDE");
    G4Material* G4_BORON_OXIDE = nm->FindOrBuildMaterial("G4_BORON_OXIDE");
    G4Material* G4_BUTANE = nm->FindOrBuildMaterial("G4_BUTANE");
    G4Material* G4_N_BUTYL_ALCOHOL = nm->FindOrBuildMaterial("G4_N-BUTYL_ALCOHOL");
    G4Material* G4_CADMIUM_TELLURIDE = nm->FindOrBuildMaterial("G4_CADMIUM_TELLURIDE");
    G4Material* G4_CADMIUM_TUNGSTATE = nm->FindOrBuildMaterial("G4_CADMIUM_TUNGSTATE");
    G4Material* G4_CALCIUM_CARBONATE = nm->FindOrBuildMaterial("G4_CALCIUM_CARBONATE");
    G4Material* G4_CALCIUM_FLUORIDE = nm->FindOrBuildMaterial("G4_CALCIUM_FLUORIDE");
    G4Material* G4_CALCIUM_OXIDE = nm->FindOrBuildMaterial("G4_CALCIUM_OXIDE");
    G4Material* G4_CALCIUM_SULFATE = nm->FindOrBuildMaterial("G4_CALCIUM_SULFATE");
    G4Material* G4_CALCIUM_TUNGSTATE = nm->FindOrBuildMaterial("G4_CALCIUM_TUNGSTATE");
    G4Material* G4_CARBON_DIOXIDE = nm->FindOrBuildMaterial("G4_CARBON_DIOXIDE");
    G4Material* G4_CARBON_TETRACHLORIDE = nm->FindOrBuildMaterial("G4_CARBON_TETRACHLORIDE");
    G4Material* G4_CHLOROBENZENE = nm->FindOrBuildMaterial("G4_CHLOROBENZENE");
    G4Material* G4_CHLOROFORM = nm->FindOrBuildMaterial("G4_CHLOROFORM");
    G4Material* G4_ETHANE = nm->FindOrBuildMaterial("G4_ETHANE");
    G4Material* G4_ETHYL_ALCOHOL = nm->FindOrBuildMaterial("G4_ETHYL_ALCOHOL");
    G4Material* G4_KAPTO = nm->FindOrBuildMaterial("G4_KAPTO");
    G4Material* G4_LEAD_OXIDE = nm->FindOrBuildMaterial("G4_LEAD_OXIDE");
    G4Material* G4_METHANE = nm->FindOrBuildMaterial("G4_METHANE");
    G4Material* G4_METHANOL = nm->FindOrBuildMaterial("G4_METHANOL");
    G4Material* G4_POLYETHYLENE = nm->FindOrBuildMaterial("G4_POLYETHYLENE");
    G4Material* G4_MYLAR = nm->FindOrBuildMaterial("G4_MYLAR");
    G4Material* G4_POLYPROPYLENE = nm->FindOrBuildMaterial("G4_POLYPROPYLENE");
    G4Material* G4_POLYSTYRENE = nm->FindOrBuildMaterial("G4_POLYSTYRENE");
    G4Material* G4_TEFLON = nm->FindOrBuildMaterial("G4_TEFLON");
    G4Material* G4_N_PROPYL_ALCOHOL = nm->FindOrBuildMaterial("G4_N-PROPYL_ALCOHOL");
    G4Material* G4_SILICON_DIOXIDE = nm->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
    G4Material* G4_RUBBER_NATURAL = nm->FindOrBuildMaterial("G4_RUBBER_NATURAL");
    G4Material* G4_PbWO4 = nm->FindOrBuildMaterial("G4_PbWO4");

    G4Material* G4_H  = nm->FindOrBuildMaterial("G4_H");  G4Material* G4_He = nm->FindOrBuildMaterial("G4_He"); G4Material* G4_Li = nm->FindOrBuildMaterial("G4_Li");
    G4Material* G4_Be = nm->FindOrBuildMaterial("G4_Be"); G4Material* G4_B  = nm->FindOrBuildMaterial("G4_B");  G4Material* G4_C  = nm->FindOrBuildMaterial("G4_C");
    G4Material* G4_N  = nm->FindOrBuildMaterial("G4_N");  G4Material* G4_O  = nm->FindOrBuildMaterial("G4_O");  G4Material* G4_F  = nm->FindOrBuildMaterial("G4_F");
    G4Material* G4_Ne = nm->FindOrBuildMaterial("G4_Ne"); G4Material* G4_Na = nm->FindOrBuildMaterial("G4_Na"); G4Material* G4_Mg = nm->FindOrBuildMaterial("G4_Mg");
    G4Material* G4_Al = nm->FindOrBuildMaterial("G4_Al"); G4Material* G4_Si = nm->FindOrBuildMaterial("G4_Si"); G4Material* G4_P  = nm->FindOrBuildMaterial("G4_P");
    G4Material* G4_S  = nm->FindOrBuildMaterial("G4_S");  G4Material* G4_Cl = nm->FindOrBuildMaterial("G4_Cl"); G4Material* G4_Ar = nm->FindOrBuildMaterial("G4_Ar");
    G4Material* G4_K  = nm->FindOrBuildMaterial("G4_K");  G4Material* G4_Ca = nm->FindOrBuildMaterial("G4_Ca"); G4Material* G4_Sc = nm->FindOrBuildMaterial("G4_Sc");
    G4Material* G4_Ti = nm->FindOrBuildMaterial("G4_Ti"); G4Material* G4_V  = nm->FindOrBuildMaterial("G4_V");  G4Material* G4_Cr = nm->FindOrBuildMaterial("G4_Cr");
    G4Material* G4_Mn = nm->FindOrBuildMaterial("G4_Mn"); G4Material* G4_Fe = nm->FindOrBuildMaterial("G4_Fe"); G4Material* G4_Co = nm->FindOrBuildMaterial("G4_Co");
    G4Material* G4_Ni = nm->FindOrBuildMaterial("G4_Ni"); G4Material* G4_Cu = nm->FindOrBuildMaterial("G4_Cu"); G4Material* G4_Zn = nm->FindOrBuildMaterial("G4_Zn");
    G4Material* G4_Ga = nm->FindOrBuildMaterial("G4_Ga"); G4Material* G4_Ge = nm->FindOrBuildMaterial("G4_Ge"); G4Material* G4_As = nm->FindOrBuildMaterial("G4_As");
    G4Material* G4_Se = nm->FindOrBuildMaterial("G4_Se"); G4Material* G4_Br = nm->FindOrBuildMaterial("G4_Br"); G4Material* G4_Kr = nm->FindOrBuildMaterial("G4_Kr");
    G4Material* G4_Rb = nm->FindOrBuildMaterial("G4_Rb"); G4Material* G4_Sr = nm->FindOrBuildMaterial("G4_Sr"); G4Material* G4_Y  = nm->FindOrBuildMaterial("G4_Y");
    G4Material* G4_Zr = nm->FindOrBuildMaterial("G4_Zr"); G4Material* G4_Nb = nm->FindOrBuildMaterial("G4_Nb"); G4Material* G4_Mo = nm->FindOrBuildMaterial("G4_Mo");
    G4Material* G4_Tc = nm->FindOrBuildMaterial("G4_Tc"); G4Material* G4_Ru = nm->FindOrBuildMaterial("G4_Ru"); G4Material* G4_Rh = nm->FindOrBuildMaterial("G4_Rh");
    G4Material* G4_Pd = nm->FindOrBuildMaterial("G4_Pd"); G4Material* G4_Ag = nm->FindOrBuildMaterial("G4_Ag"); G4Material* G4_Cd = nm->FindOrBuildMaterial("G4_Cd");
    G4Material* G4_In = nm->FindOrBuildMaterial("G4_In"); G4Material* G4_Sn = nm->FindOrBuildMaterial("G4_Sn"); G4Material* G4_Sb = nm->FindOrBuildMaterial("G4_Sb");
    G4Material* G4_Te = nm->FindOrBuildMaterial("G4_Te"); G4Material* G4_I  = nm->FindOrBuildMaterial("G4_I");  G4Material* G4_Xe = nm->FindOrBuildMaterial("G4_Xe");
    G4Material* G4_Cs = nm->FindOrBuildMaterial("G4_Cs"); G4Material* G4_Ba = nm->FindOrBuildMaterial("G4_Ba"); G4Material* G4_La = nm->FindOrBuildMaterial("G4_La");
    G4Material* G4_Ce = nm->FindOrBuildMaterial("G4_Ce"); G4Material* G4_Pr = nm->FindOrBuildMaterial("G4_Pr"); G4Material* G4_Nd = nm->FindOrBuildMaterial("G4_Nd");
    G4Material* G4_Pm = nm->FindOrBuildMaterial("G4_Pm"); G4Material* G4_Sm = nm->FindOrBuildMaterial("G4_Sm"); G4Material* G4_Eu = nm->FindOrBuildMaterial("G4_Eu");
    G4Material* G4_Gd = nm->FindOrBuildMaterial("G4_Gd"); G4Material* G4_Tb = nm->FindOrBuildMaterial("G4_Tb"); G4Material* G4_Dy = nm->FindOrBuildMaterial("G4_Dy");
    G4Material* G4_Ho = nm->FindOrBuildMaterial("G4_Ho"); G4Material* G4_Er = nm->FindOrBuildMaterial("G4_Er"); G4Material* G4_Tm = nm->FindOrBuildMaterial("G4_Tm");
    G4Material* G4_Yb = nm->FindOrBuildMaterial("G4_Yb"); G4Material* G4_Lu = nm->FindOrBuildMaterial("G4_Lu"); G4Material* G4_Hf = nm->FindOrBuildMaterial("G4_Hf");
    G4Material* G4_Ta = nm->FindOrBuildMaterial("G4_Ta"); G4Material* G4_W  = nm->FindOrBuildMaterial("G4_W");  G4Material* G4_Re = nm->FindOrBuildMaterial("G4_Re");
    G4Material* G4_Os = nm->FindOrBuildMaterial("G4_Os"); G4Material* G4_Ir = nm->FindOrBuildMaterial("G4_Ir"); G4Material* G4_Pt = nm->FindOrBuildMaterial("G4_Pt");
    G4Material* G4_Au = nm->FindOrBuildMaterial("G4_Au"); G4Material* G4_Hg = nm->FindOrBuildMaterial("G4_Hg"); G4Material* G4_Tl = nm->FindOrBuildMaterial("G4_Tl");
    G4Material* G4_Pb = nm->FindOrBuildMaterial("G4_Pb"); G4Material* G4_Bi = nm->FindOrBuildMaterial("G4_Bi"); G4Material* G4_Po = nm->FindOrBuildMaterial("G4_Po");
    G4Material* G4_At = nm->FindOrBuildMaterial("G4_At"); G4Material* G4_Rn = nm->FindOrBuildMaterial("G4_Rn"); G4Material* G4_Ac = nm->FindOrBuildMaterial("G4_Ac");
    G4Material* G4_Th = nm->FindOrBuildMaterial("G4_Th"); G4Material* G4_Pa = nm->FindOrBuildMaterial("G4_Pa"); G4Material* G4_U  = nm->FindOrBuildMaterial("G4_U");
    G4Material* G4_Np = nm->FindOrBuildMaterial("G4_Np"); G4Material* G4_Pu = nm->FindOrBuildMaterial("G4_Pu"); G4Material* G4_Am = nm->FindOrBuildMaterial("G4_Am");
    G4Material* G4_Cm = nm->FindOrBuildMaterial("G4_Cm"); G4Material* G4_Bk = nm->FindOrBuildMaterial("G4_Bk"); G4Material* G4_Cf = nm->FindOrBuildMaterial("G4_Cf");


    // Ce:Gd3Al2Ga3O12  -- https://www.c-and-a.jp/assets/img/products/101210514_GAGG.pdf
    G4Material* GAGG = new G4Material("GAGG", density= 6.63*g/cm3, ncomponents=4);
    GAGG->AddElement(Gd, natoms=3);
    GAGG->AddElement(Al, natoms=2);
    GAGG->AddElement(Ga, natoms=3);
    GAGG->AddElement(O, natoms=12);

    G4Material* CeGAGG = new G4Material("CeGAGG", density= 6.63*g/cm3, ncomponents=2);
    CeGAGG->AddMaterial(GAGG, fractionmass=99.*perCent);
    CeGAGG->AddElement (Ce,   fractionmass=1.*perCent); // typical doped-Ce
    
    G4Material* RockKam =
    new G4Material("RockKam", density= 3.0*g/cm3, ncomponents=13);
    RockKam->AddElement(H, fractionmass=0.006);
    RockKam->AddElement(C, fractionmass=0.003);
    RockKam->AddElement(O, fractionmass=0.607);
    RockKam->AddElement(Na, fractionmass=0.039);
    RockKam->AddElement(Mg, fractionmass=0.003);
    RockKam->AddElement(Al, fractionmass=0.106);
    RockKam->AddElement(Si, fractionmass=0.185);
    RockKam->AddElement(P, fractionmass=0.001);
    RockKam->AddElement(K, fractionmass=0.021);
    RockKam->AddElement(Ca, fractionmass=0.018);
    RockKam->AddElement(Ti, fractionmass=0.001);
    RockKam->AddElement(Mn, fractionmass=0.000);
    RockKam->AddElement(Fe, fractionmass=0.010);

    
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
    
    
    //default materials of the World
    defaultMaterial  = Vacuum;
}

///////////////////////////////////////////////////////
void mcDetectorConstruction::SetMaxStep(G4double value)
{
    //--------- example of User Limits -------------------------------
    // below is an example of how to set tracking constraints in a given
    // logical volume
    
    if (value >0.) {
        maxStep = value;
    } else {
        maxStep = DBL_MAX;
    }
    if (pUserLimits) {
        delete pUserLimits;
    }
    UpdateGeometry();
}
/////////////////////////////////////////////////////////////

void mcDetectorConstruction::SetSensorMaterial(G4String materialChoice)
{
    // search the material by its name
    G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);
    if (pttoMaterial) {
        sensorMaterial = pttoMaterial;
        G4cout << " mcDetectorConstruction::SetSensorMaterial:  ";
        G4cout << "Sensor material is " << materialChoice << G4endl;
        UpdateGeometry();
    } else {
        G4cout << " mcDetectorConstruction::SetSensorMaterial:  ";
        G4cout << materialChoice << " is not in the Material Table.";
        G4cout <<G4endl;
    }
}

void mcDetectorConstruction::SetShieldMaterial(G4String materialChoice)
{
    // search the material by its name
    G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);
    if (pttoMaterial) {
        shieldMaterial = pttoMaterial;
        G4cout << " mcDetectorConstruction::SetShileMaterial:  ";
        G4cout << "Shield material is " << materialChoice << G4endl;
        UpdateGeometry();
    } else {
        G4cout << " mcDetectorConstruction::SetShieldMaterial:  ";
        G4cout << materialChoice << " is not in the Material Table.";
        G4cout <<G4endl;
    }
}

void mcDetectorConstruction::SetSensorRadius(G4double size)
{
    sensorRadius = size;
    UpdateGeometry();
}
void mcDetectorConstruction::SetSensorHeight(G4double size)
{
    sensorHeight = size;
    UpdateGeometry();
}
void mcDetectorConstruction::SetShieldThickness(G4double thickness)
{
    shieldThickness = thickness;
    UpdateGeometry();
}

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

void mcDetectorConstruction::SetMagField(G4double value)
{
    //apply a global uniform magnetic field along Z axis
    G4FieldManager* fieldMgr
    = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    
    if(magField) delete magField;		//delete the existing magn field
    
    if(value!=0.){			// create a new one if non nul
        fieldValue = value;
        magField = new G4UniformMagField(G4ThreeVector(0.,0.,fieldValue));
        fieldMgr->SetDetectorField(magField);
        fieldMgr->CreateChordFinder(magField);
    } else {
        magField = 0;
        fieldMgr->SetDetectorField(magField);
    }
}

#include "G4RunManager.hh"

void mcDetectorConstruction::UpdateGeometry()
{
    G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

void mcDetectorConstruction::SetAnalyzer(mcAnalyzer * analyzer_in){
    analyzer = analyzer_in;
}

