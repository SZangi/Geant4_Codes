#include "runAction.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "PGA.hh"

#include <list>
#include <iostream>

runAction::runAction()
{
  // open data output file
  processOutput.open("Outputs/MonoEXSOut.csv",std::ofstream::trunc);
}

runAction::~runAction()
{
    processOutput.close();
}

void runAction::BeginOfRunAction(const G4Run*)
{
  IncidentEne = -1.;
  Neutrons = 0;
}

void runAction::EndOfRunAction(const G4Run* run)
{
  G4RunManager *runManager = G4RunManager::GetRunManager();
  PGA *thePGA = (PGA*) runManager->GetUserPrimaryGeneratorAction();
  G4ParticleGun* GPS = thePGA->GetSource();

  // Print the data to the command line (and dump it to a file in case we want
  // to dump more things from runAction later)
  if (InelasticCol > 0){
        G4cout << "Inelastic: " << InelasticCol << " Elastic: " << ElasticCol << " Ionizing: " << IonizingCol << std::endl;
        G4cout << "DT: " << DT << std::endl;
        processOutput << InelasticCol << std::endl;
        processOutput << ElasticCol << std::endl;
        processOutput << IonizingCol << std::endl;
        processOutput << DT << std::endl;
  }

  if (Neutrons > 0){
    SetIncidentEnergy(GPS->GetParticleEnergy()/keV);
    processOutput << IncidentEne << "," << Neutrons << std::endl;
  }
  // This prints number of secondaries per event to a list.
  //if (ProcessList.size() > 0){
  // processOutput << "Secondaries Per Non-DT" << std::endl;
  //  for (G4double secondary : ProcessList){
  //    processOutput << secondary << std::endl;
  //  }
    
  // }
}

void runAction::AddProcess(G4double ProcessNum, int ProcessType)
{
    switch (ProcessType)
    {
    case 1:
        InelasticCol += ProcessNum;
        break;
    case 2:
        ElasticCol += ProcessNum;
        break;
    case 3:
        IonizingCol += ProcessNum;
        break;
    case 4:
        DT += ProcessNum;
    }
}

void runAction::AddSecondaries(G4double NumSecondaries){
  ProcessList.push_back(NumSecondaries);
}

void runAction::AddNeutron(G4double Neutron){
  Neutrons += Neutron;
}