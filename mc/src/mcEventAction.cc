
#include "mcEventAction.hh"

#include "mcRunAction.hh"

#include "G4Event.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VTrajectory.hh"
#include "G4VVisManager.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>


mcEventAction::mcEventAction(mcRunAction* action)
{
    runAction = action;
}


mcEventAction::~mcEventAction()
{
}


void mcEventAction::BeginOfEventAction(const G4Event*)
{  
}


void mcEventAction::EndOfEventAction(const G4Event* evt)
{
    G4int evtNb = evt->GetEventID();   
    if (evtNb%1000  == 0) {
        G4cout << " Event #" << evtNb << G4endl;
    }
}  


