#include "FancyNeutronPhysics.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessTable.hh"
#include "NeutronProduction.hh"

#include "G4BosonConstructor.hh"
#include "G4IonConstructor.hh"

#include "G4PhysicsListHelper.hh"
#include "G4ParticleDefinition.hh"
#include "G4Alpha.hh"



FancyNeutronPhysics::FancyNeutronPhysics(const G4String& name):G4VPhysicsConstructor(name){;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FancyNeutronPhysics::~FancyNeutronPhysics(){;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FancyNeutronPhysics::ConstructParticle(){
  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();

  G4BosonConstructor  pBosonConstructor;
  pBosonConstructor.ConstructParticle();
}

void FancyNeutronPhysics::ConstructProcess(){
  ConstructGeneral();
}

void FancyNeutronPhysics::ConstructGeneral()
{
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

  G4ParticleDefinition* particle = G4Alpha::AlphaDefinition();

  ph->RegisterProcess(new NeutronProduction(), particle);
}