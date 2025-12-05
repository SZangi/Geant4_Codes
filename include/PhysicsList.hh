// Physics list class for including HP physics.

#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PhysicsList: public G4VModularPhysicsList
{
public:
  PhysicsList();
 ~PhysicsList();

public:
  virtual void ConstructParticle();
  virtual void SetCuts();
  //virtual void ConstructProcess();

protected:
  //void ConstructGeneral();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif