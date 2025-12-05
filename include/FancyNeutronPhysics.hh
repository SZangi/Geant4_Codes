#ifndef FancyNeutronPhysics_hh
#define FancyNeutronPhysics_hh 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

class FancyNeutronPhysics : public G4VPhysicsConstructor
{
public:
    FancyNeutronPhysics(const G4String& name = "fancy");
    ~FancyNeutronPhysics();
    
public:
    virtual void ConstructParticle();
    virtual void ConstructProcess();

    void ConstructGeneral();
};

#endif