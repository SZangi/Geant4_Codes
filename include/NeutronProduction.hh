# ifndef NeutronProduction_hh
# define NeutronProduction_hh 1

#include "G4VDiscreteProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VParticleChange.hh"
#include "globals.hh"

class G4VParticleChange;

class NeutronProduction : public G4VDiscreteProcess
{
    public:
        NeutronProduction(const G4String& = "nUpdate", G4ProcessType = fGeneral);
        ~NeutronProduction();

        G4double GetMeanFreePath(const G4Track&, G4double,G4ForceCondition*);

        G4VParticleChange* PostStepDoIt(const G4Track&, const G4Step&);

        G4double PostStepGetPhysicalInteractionLength(const G4Track& track, G4double, G4ForceCondition* condiiton);

        G4bool IsApplicable(const G4ParticleDefinition&);

    private:
        G4double E_incident;
        G4double E_out;
};

inline G4double NeutronProduction::GetMeanFreePath(const G4Track&,G4double,G4ForceCondition*)
{
    return DBL_MAX;
}

inline G4double NeutronProduction::PostStepGetPhysicalInteractionLength(const G4Track&,G4double,G4ForceCondition* condition)
{
    *condition = StronglyForced;

    return DBL_MAX;
}

inline G4bool NeutronProduction::IsApplicable(const G4ParticleDefinition& PD){
    return(PD.GetParticleName()=="alpha");
}

#endif