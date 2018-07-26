#ifndef mcDetectorConstruction_h
#define mcDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4Box;
class G4Orb;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4UniformMagField;
class G4UserLimits;
class mcDetectorMessenger;



class mcDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
    mcDetectorConstruction();
   ~mcDetectorConstruction();

  public:
     
     void SetSensorMaterial (G4String);     
     void SetSensorThickness(G4double);     
     void SetSensorSize(G4double);     
     void SetSensorLeverArm(G4double);     
  
     void SetMaxStep(G4double);     

     void SetMagField(G4double);
     
     G4VPhysicalVolume* Construct();

     void UpdateGeometry();

     void SetPESize(G4double);
     void SetBoronSize(G4double);

  public:
  
     G4double GetWorldSizeX()  const    {return WorldSizeX;}; 
     G4double GetWorldSizeYZ() const    {return WorldSizeYZ;};
     
     const G4Material* GetSensorMaterial()  const {return pSensorMaterial;};
     G4double    GetSensorThickness() const {return vSensorThickness;};      
     G4double    GetSensorSize()      const {return vSensorSize;};      
     G4double    GetSensorLeverArm()      const {return vSensorArm;};      
 
     G4double    GetMaxStep()      const {return maxStep;};      
 
     G4double    GetFieldValue()      const {return fieldValue;};      
     
     const G4VPhysicalVolume* GetphysiWorld() const {return physWorld;};           
     const G4VPhysicalVolume* GetSensor()     const {return physSensor;};
	 G4double	GetRockSize() const {return vRockSize;};

	 G4double GetPESize() const {return PEThickness;};
	 G4double GetBoronSize() const {return BoronThickness;};
                 
  private:
     
     G4Material*        pSensorMaterial;
     G4double           vSensorThickness;
     G4double           vSensorSize;
     G4double           vSensorArm;
	 G4double			vRockSize;
     
     G4Material*        defaultMaterial;
     G4double           WorldSizeYZ;
     G4double           WorldSizeX;

	 G4Tubs*			solidPolyethylene;
	 G4LogicalVolume* 	logicPolyethylene;
	 G4VPhysicalVolume*	physPolyethylene;

	 G4Tubs*			solidBoron;
	 G4LogicalVolume* 	logicBoron;
	 G4VPhysicalVolume*	physBoron;

	 G4Tubs*			solidDetector;
	 G4LogicalVolume* 	logicDetector;
	 G4VPhysicalVolume*	physDetector;
            
     G4Orb*             solidWorld;       //pointer to the solid World 
     G4LogicalVolume*   logicWorld;       //pointer to the logical World
     G4VPhysicalVolume* physWorld;        //pointer to the physical World

     G4Tubs*            solidSensor;      //pointer to the solid Sensor
     G4LogicalVolume*   logicSensor;      //pointer to the logical Sensor
     G4VPhysicalVolume* physSensor;       //pointer to the physical Sensor

     
     G4UniformMagField* magField;      //pointer to the magnetic field
     G4double           fieldValue;
 
     G4UserLimits*      pUserLimits;    //pointer to the UserLimits
	 G4double            maxStep;          // max step length
     mcDetectorMessenger* detectorMessenger;  //pointer to the Messenger

	 G4Material*		mat_3He;
	 G4Material*		mat_detector;
	 G4Material*		mat_polyethylene;
	 G4Material*		mat_boron;
	 G4Material*		mat_air;

     //G4Tubs*            solidPE;
     //G4LogicalVolume*   logicPE;
     //G4VPhysicalVolume* physPE; 

     G4Box*            solidAir;
     G4LogicalVolume*   logicAir;
     G4VPhysicalVolume* physAir; 

     G4Box*            solidShield;
     G4LogicalVolume*   logicShield;
     G4VPhysicalVolume* physShield; 

      
     //G4Material*        RockKam;
  private:
    
     void DefineMaterials();
     G4double PEThickness;
     G4double BoronThickness;
};

#endif

