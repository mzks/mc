#ifndef mcDetectorConstruction_h
#define mcDetectorConstruction_h 1

#include "mcAnalyzer.hh"
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
    void SetShieldMaterial (G4String);
    void SetSensorSize(G4double);
    void SetShieldThickness(G4double);
    void SetMaxStep(G4double);
    void SetMagField(G4double);
    
    G4VPhysicalVolume* Construct();
    
    void UpdateGeometry();
    
public:
    
    G4double GetWorldRadius()  const    {return WorldRadius;};
    
    const G4Material* GetSensorMaterial()  const {return sensorMaterial;};
    const G4Material* GetShieldMaterial()  const {return shieldMaterial;};
    const double GetSensorSize() const {return sensorSize;};
    const double GetShieldThickness() const {return shieldThickness;};

    G4double    GetMaxStep()      const {return maxStep;};
    
    G4double    GetFieldValue()      const {return fieldValue;};
    
    const G4VPhysicalVolume* GetphysiWorld() const {return physWorld;};
    const G4VPhysicalVolume* GetSensor()     const {return physSensor;};
    
    void SetAnalyzer(mcAnalyzer*);
    
    
    
private:
    G4Material*        defaultMaterial;
    G4Material*        sensorMaterial;
    G4Material*        shieldMaterial;

    G4double           WorldRadius;
    G4double           sensorSize;
    G4double           shieldThickness;
    
    G4Orb*             solidWorld;
    G4LogicalVolume*   logicWorld;
    G4VPhysicalVolume* physWorld;
    
    G4Box*             solidSensor;
    G4LogicalVolume*   logicSensor;
    G4VPhysicalVolume* physSensor;

    G4Box*             solidShield;
    G4LogicalVolume*   logicShield;
    G4VPhysicalVolume* physShield;

    G4UniformMagField* magField;      //pointer to the magnetic field
    G4double           fieldValue;
    
    G4UserLimits*      pUserLimits;    //pointer to the UserLimits
    G4double            maxStep;          // max step length
    mcDetectorMessenger* detectorMessenger;  //pointer to the Messenger
    
    void DefineMaterials();
    mcAnalyzer* analyzer;

};

#endif

