#ifndef mcEventAction_h
#define mcEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class mcRunAction;



class mcEventAction : public G4UserEventAction
{
public:
    mcEventAction(mcRunAction*);
    ~mcEventAction();
    
public:
    void  BeginOfEventAction(const G4Event*);
    void  EndOfEventAction(const G4Event*);
    
    
private:
    mcRunAction* runAction;                          
    
};


#endif


