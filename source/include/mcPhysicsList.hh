#ifndef mcPhysicsList_h
#define mcPhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"


class mcPhysicsList: public G4VUserPhysicsList
{
  public:
    mcPhysicsList();
   ~mcPhysicsList();

  protected:
    // Construct particle and physics
    void ConstructParticle();
    void ConstructProcess();
 
    void SetCuts();

   
  protected:
    // these methods Construct particles 
    void ConstructBosons();
    void ConstructLeptons();
    void ConstructMesons();
    void ConstructBaryons();

  protected:
    // these methods Construct physics processes and register them
    void ConstructGeneral();
    void ConstructEM();
    void ConstructOp();
};


#endif



