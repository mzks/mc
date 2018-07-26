
#include "mcSensorSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

/////////////////////////////////////////////////////

mcSensorSD::mcSensorSD(G4String name)
:G4VSensitiveDetector(name)
{
  //4String HCname;
  collectionName.insert("sensorCollection");

  //--- open file -----
  G4String fileName = "mc.out";  
  outFile.open(fileName, std::ios::out);

  eThreshold = 10.0 * eV;
  tResolution= 300.0 * ns;
}

/////////////////////////////////////////////////////

mcSensorSD::~mcSensorSD()
{ 
  outFile.close();
}

/////////////////////////////////////////////////////

void mcSensorSD::Initialize(G4HCofThisEvent* HCE)
{
  sensorCollection = new mcSensorHitsCollection
                          (SensitiveDetectorName,collectionName[0]); 
  static G4int HCID = -1;
  if(HCID<0)
  { HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
  HCE->AddHitsCollection( HCID, sensorCollection ); 

}

/////////////////////////////////////////////////////

G4bool mcSensorSD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  
  const G4Track * aTrack =  aStep->GetTrack();

  //if ( aTrack->GetDefinition()->GetPDGCharge() == 0.0) return false;
  //G4cout << aTrack->GetDefinition()->GetPDGEncoding() << G4endl;
  // Sensitive to neutron only
  //if ( aTrack->GetDefinition()->GetPDGEncoding() != 2112) return false;

  
  //Check Energy deposit
  G4double eLoss = aStep->GetTotalEnergyDeposit();
  if (eLoss <= 0.0 ) return false;
  G4double time = aTrack->GetGlobalTime(); 
  
 
  G4int copyNO = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();
  G4int NbHits = sensorCollection->entries();
  G4bool found = false;
  for (G4int iHit=0; (iHit<NbHits) && (!found) ;iHit++) {
	  found = (copyNO == (*sensorCollection)[iHit]->GetCopyNO() ) ;
	  if (found) {    
		  // check time
		  if (std::abs(time-(*sensorCollection)[iHit]->GetTime()) < tResolution) {
			  // merge hit
			  (*sensorCollection)[iHit]->AddEdep(eLoss);
			  return true;
		  } 
	  }
  }
  
  mcSensorHit* newHit = new mcSensorHit();
  newHit->Set(copyNO, aTrack, eLoss);
  sensorCollection->insert( newHit );

 // G4int NbHits = sensorCollection->entries();
 // G4bool found = false;
 // for (G4int i=0; (i<NbHits) && (!found) ;i++) {
 //   found = (copyNO == (*sensorCollection)[i]->GetCopyNO() ) 
 //        && ( aTrack ->GetTrackID() == (*sensorCollection)[i]->GetTrackID() );
 // } 
 // if (!found) {
 //   mcSensorHit* newHit = new mcSensorHit();
 //   //newHit->Set(copyNO ,aTrack);
 //   newHit->Set(copyNO ,aTrack,eLoss);
 //   sensorCollection->insert( newHit );
 //   
 //   //newHit->Print();
 //   //newHit->Draw();
 // }
  return true;
}

///////////////////////////////////////////////////////

void mcSensorSD::EndOfEvent(G4HCofThisEvent* HCE)
{
  G4int NbHits = sensorCollection->entries();
  if (verboseLevel>0) {
    G4cout << "\n-------->Hits Collection: in this event they are " 
	 << NbHits 
	 << " hits in the tracker chambers: " << G4endl;
  }

  outFile.setf( std::ios:: scientific, std::ios::floatfield );
  outFile << std::setprecision(8);
  outFile << NbHits << G4endl;

  //G4int NbHits = sensorCollection->entries();

  G4bool isFirstHit = true;  
  G4bool hasHit=false;
  for (G4int i=0;i<NbHits;i++){
	  mcSensorHit* hit = (*sensorCollection)[i];
	  if (verboseLevel>1) hit->Print();
	  // output hits other than trigger counters
	  G4int ch = hit->GetCopyNO();
	  //if (hit->GetEdep() < eThreshold ) continue;
	  if (isFirstHit) {
		  isFirstHit = false;
		  hasHit = true;
	  }
	  outFile << ch << " "
		  << hit->GetTrackID() <<" " 
		  << hit->GetPDGcode() <<" " 
		  << hit->GetEnergy()/MeV  <<" " 
		  << hit->GetPos().x()/mm << " "
		  << hit->GetPos().y()/mm << " "
		  << hit->GetPos().z()/mm << " "
		  << hit->GetTime()/ns << " "
		  << hit->GetEdep()/MeV 
		  << G4endl;
  }

	  //for (G4int i=0;i<NbHits;i++){
	  //  mcSensorHit* hit = (*sensorCollection)[i];
	  //  
	  //  if (verboseLevel>0) {
  //    hit->Print();
  //  }    
  //  
  //  outFile << hit->GetCopyNO() <<" "  // Sensor ditector #
  //      << hit->GetTrackID() <<" " 
  //      << hit->GetPDGcode() <<" " 
  //      << hit->GetEnergy()/MeV  <<" " 
  //      << hit->GetPos().x()/mm  <<" " 
  //      << hit->GetPos().y()/mm  <<" " 
  //      << hit->GetPos().z()/mm  <<" " 
  //      << hit->GetTime()/ns  <<" " 
  //      << G4endl;
  //}
  
}



