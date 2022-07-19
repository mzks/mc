

#ifndef mcRunAction_h
#define mcRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"


class G4Run;

class mcRunAction : public G4UserRunAction
{
public:
    mcRunAction();
    ~mcRunAction();
    
public:
    void BeginOfRunAction(const G4Run*);
    void EndOfRunAction(const G4Run*);
    
private:
};


#endif

