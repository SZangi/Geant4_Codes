#ifndef runAction_hh
#define runAction_hh 1

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"

#include <fstream>
#include <list>

class G4Run;

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

class runAction : public G4UserRunAction
{
  public:
    runAction();
    ~runAction();

    void BeginOfRunAction(const G4Run*) override;
    void   EndOfRunAction(const G4Run*) override;

    void AddProcess (G4double ProcessNum, int ProcessType);

    void AddSecondaries(G4double NumSecondaries);

    void AddNeutron(G4double NumNeutrons);

    void SetIncidentEnergy(G4double IncidentE) {IncidentEne = IncidentE;};

  private:
    G4double InelasticCol = 0.;
    G4double ElasticCol = 0.;
    G4double DT = 0.;
    G4double IonizingCol = 0.;
    std::list<G4double> ProcessList = {};
    std::ofstream processOutput;
    G4double Neutrons = 0.;
    G4double IncidentEne = -1.;
};



#endif