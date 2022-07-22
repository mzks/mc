#include "mcEventAction.hh"

#include "mcRunAction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VTrajectory.hh"
#include "G4VVisManager.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>

#include <smart_loop_logger.hh>


mcEventAction::mcEventAction(mcRunAction* action)
{
    runAction = action;
}


mcEventAction::~mcEventAction()
{
}


void mcEventAction::BeginOfEventAction(const G4Event* evt)
{  
}


void mcEventAction::EndOfEventAction(const G4Event* evt)
{
    NbOfEvents = runAction->NbOfEventToBeProcessed;
    if (NbOfEvents <= 0) spdlog::error("No events to be processed.");

    G4int evtNb = evt->GetEventID();
    smart_loop_logger(NbOfEvents, evtNb);
}  


