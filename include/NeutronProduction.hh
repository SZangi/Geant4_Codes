# ifndef NeutronProduction_hh
# define NeutronProduction_hh 1

#include "G4VDiscreteProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VParticleChange.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"

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


        
        std::vector<G4double> GenerateCosE(G4double incident_E, G4double m_A, G4double m_B,G4double m_C,G4double m_D,G4double Q_value);
        
        G4double CosToE(G4double cos, G4double m_A, G4double m_B,G4double m_C,G4double m_D,G4double E_in,G4double Q_value);

    private:
        //G4double m_a = 13.003355;
        G4double m_a = 11.009305;
        G4double m_b = 4.002603;
        //G4double m_c = 15.994915;
        G4double m_c = 14.003074;
        G4double m_d = 1.008665;
        //G4double Q = 2.216*MeV;
        G4double Q = 0.157*MeV;

        std::vector<G4double> Particle_info = {};

        std::vector<G4double*> Hist_points;
        std::vector<G4RandGeneral*> Generators;

        G4double A_masses;
        G4double A_energy;
        G4double A;
        G4double B_masses;
        G4double B_energy;
        G4double B;
        G4double C_masses;
        G4double C_energy;
        G4double C;
        G4double D_masses;
        G4double D_energy;
        G4double D;
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