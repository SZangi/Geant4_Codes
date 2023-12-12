#include "runAction.hh"
// #include "PrimaryGeneratorAction.hh"
// #include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include <list>

runAction::runAction()
{
  // Register accumulable to the accumulable manager; approach taken from G4 example B1
  //G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  //accumulableManager->RegisterAccumulable(fInelasticCol);
  //accumulableManager->RegisterAccumulable(fElasticCol);
  //accumulableManager->RegisterAccumulable(fIonizingCol);

  // open data output file
  processOutput.open("defaultProcessOutput.csv",std::ofstream::trunc);
}

runAction::~runAction()
{
    processOutput.close();
}

void runAction::BeginOfRunAction(const G4Run*)
{
  // reset accumulables to their initial values
  //G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  //accumulableManager->Reset();  

}

void runAction::EndOfRunAction(const G4Run* run)
{
  //G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  //accumulableManager->Merge();

  //get values from the accumulables
  //G4double InelasticCol = fInelasticCol.GetValue();
  //G4double ElasticCol = fElasticCol.GetValue();
  //G4double IoninzingCol = fIonizingCol.GetValue();

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
  if (ProcessList.size() > 0){
   processOutput << "Secondaries Per Non-DT" << std::endl;
    for (G4double secondary : ProcessList){
      processOutput << secondary << std::endl;
    }
    
  }
    
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