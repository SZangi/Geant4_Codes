// Physics List methods from Hadr03, xs verification example.

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PhysicsList.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4NuclideTable.hh"

#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronElasticPhysicsPHP.hh"
#include "G4HadronElasticPhysicsXS.hh"

#include "G4HadronInelasticQBBC.hh"
#include "G4HadronPhysicsINCLXX.hh"
#include "G4HadronPhysicsShielding.hh"

#include "G4IonElasticPhysics.hh"
#include "G4IonPhysicsXS.hh"
#include "G4IonQMDPhysics.hh"
#include "G4IonPhysicsPHP.hh"
#include "G4IonINCLXXPhysics.hh"

#include "G4EmLowEPPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"

#include "G4RadioactiveDecayPhysics.hh"
#include "G4DecayPhysics.hh"

#include "G4GenericBiasingPhysics.hh"

#include "G4PhysicsListHelper.hh"
#include "NeutronProduction.hh"

#include "FancyNeutronPhysics.hh"

// particles

#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList()
:G4VModularPhysicsList()
{
  SetVerboseLevel(1);
  
  //add new units
  //
  new G4UnitDefinition( "mm2/g",  "mm2/g", "Surface/Mass", mm2/g);
  new G4UnitDefinition( "um2/mg", "um2/mg","Surface/Mass", um*um/mg);
    
  // mandatory for G4NuclideTable
  //
  const G4double meanLife = 1*picosecond;  
  G4NuclideTable::GetInstance()->SetMeanLifeThreshold(meanLife);  
    
  // Construct Specific Physics
    // Hadronic Physics
     RegisterPhysics( new G4HadronInelasticQBBC(0));
     RegisterPhysics( new G4HadronElasticPhysicsHP(0));  

    // EM Physics should be removed to get xs to match exactly with recorded vales
     RegisterPhysics (new G4EmLowEPPhysics(0)); 
     //RegisterPhysics(new G4EmStandardPhysics_option4 (0));

    // Ion Physics
     RegisterPhysics( new G4IonElasticPhysics(0));
     RegisterPhysics (new G4IonPhysicsPHP(0));

    // Decay Physics
     RegisterPhysics(new G4DecayPhysics(0));
     RegisterPhysics(new G4RadioactiveDecayPhysics(0));

     

  // Add Biasing Physics
    G4GenericBiasingPhysics* biasingPhysics = new G4GenericBiasingPhysics();
    biasingPhysics -> Bias("alpha");
    RegisterPhysics(biasingPhysics);

  // A scheme?

    //RegisterPhysics(new FancyNeutronPhysics());


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructParticle()
{
  G4BosonConstructor  pBosonConstructor;
  pBosonConstructor.ConstructParticle();

  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();

  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();  
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{
//  SetCutValue(0*mm, "proton");
// setting this value down will bring the neutron production closer to the expected
// value but not quite there, still a little low.
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......