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
    if (evtNb == 0) {
        spdlog::info("Initial event 0 has been done.");
        return;
    }
    // Display each 10%
    if ((100 * evtNb / NbOfEvents % 10 == 0) && ((100 * evtNb / NbOfEvents) != (100 * (evtNb-1) / NbOfEvents))) {
        spdlog::info("{:d} percent events ({:d}) have been done.", 100 * (evtNb+1) / NbOfEvents, evtNb);
    }
}  


