#ifndef mcSensorSD_h
#define mcSensorSD_h 1

#include "G4VSensitiveDetector.hh"
#include "mcSensorHit.hh"
#include "mcAnalyzer.hh"

#include <vector>

#include "TFile.h"
#include "TTree.h"

class G4Step;
class G4HCofThisEvent;


class mcSensorSD : public G4VSensitiveDetector
{
public:
    mcSensorSD(G4String);
    ~mcSensorSD();
    
    mcAnalyzer* analyzer;
    
    void Initialize(G4HCofThisEvent*);
    G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    void EndOfEvent(G4HCofThisEvent*);
    
    G4double GetThresholdEnergy() const {return eThreshold;}
    G4double GetTimeResolution()  const {return tResolution;}
    
    static const G4String& GetCollectionName() {return HCname;}
    void SetAnalyzer(mcAnalyzer*);
    
    
private:
    mcSensorHitsCollection* sensorCollection;    
    G4double eThreshold;
    G4double tResolution;
    
    static const G4String HCname;
    
};


#endif

