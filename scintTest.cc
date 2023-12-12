/*
############################################################

d(euterium)t(ritium) TEST Simulation

Arthur Zangi (szangi@mit.edu)
11-03-2023

Adapted from scintTEST by
Zach Hartwig (hartwig@mit.edu)

02 MAR 09 (original)
03 APR 23 (updated for geant4-v11.1.1)

I have written "scintTest" as an introductory simulation for beginning
Geant4 users.  Each piece of code is explained in gory detail such
that the novice may understand what the code does and why it's import.
The following basic Geant4 concepts are covered in this simulation:

- Writing the main file
- Defining the geometry
    - building basic materials 
    - utilizing geometry volumes (solid,logical,physical)
- Specifying the physics processes
- Creating a source of particle
- Sensitive Detector
  
The program defines a long rectangular world volume and places 2
pieces of St Gobain BC404 Scintillator.  A beam of electrons is fired
at the scintillators and energy deposition information is
collected. Options are provided for the user to enable outputtting the
total energy deposited within each scintillator tile per event to a
text file for simple post-processing.

############################################################
*/

// Include the necessary header files.  Header files that begin with
// "G4....hh" are the Geant4 source header files...
#include "G4RunManager.hh" 
#include "G4ScoringManager.hh"
#include "G4AnalysisManager.hh"
#include "G4TScoreHistFiller.hh"
#include "G4VisExecutive.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "QGSP_BIC.hh"
#include "QGSP_BIC_AllHP.hh"
#include "G4TrajectoryDrawByParticleID.hh"
#include "G4GenericBiasingPhysics.hh"

/// ...wherease these are user-defined header files.
#include "geometryConstruction.hh"
#include "PGA.hh"
#include "steppingAction.hh"
#include "eventAction.hh"
#include "runAction.hh"
#include "PhysicsList.hh"
//#include "trackingAction.hh"

// This is the main program.  It will set up managers to run and
// handle the operation of the program and open the user interface, as
// well as deleting the managers upon termination of the program.

int main(int argc, char *argv[])
{

  // Create a runManager to handle the flow of operations in the program.  
  G4RunManager* runManager = new G4RunManager;

  // Activate command-based scorer
  G4ScoringManager::GetScoringManager();

  //activate analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Activate command based histogramming
  auto histFiller = new G4TScoreHistFiller<G4AnalysisManager>;

  // Create new "mandatory defined" class objects and tell the
  // runManager to initialize them for use
  runManager -> SetUserInitialization(new geometryConstruction);
  // new physics list with individual lists registered for different processes
  // QGSP_BIC_AllHP* physicsList = new QGSP_BIC_AllHP;
  PhysicsList *physicsList = new PhysicsList();

 // biasing physics already added in class definition
 // G4GenericBiasingPhysics* biasingPhysics = new G4GenericBiasingPhysics();
 // biasingPhysics -> Bias("deuteron");
 // physicsList -> RegisterPhysics(biasingPhysics);

  runManager -> SetUserInitialization(physicsList);
  
  runManager -> SetUserAction(new PGA);
  
  // Create new "user defined" class objects and tell the runManager
  // to initialize them for use
  runAction *rnAction = new runAction;
  runManager -> SetUserAction(rnAction);
  eventAction *evtAction = new eventAction(rnAction);
  runManager -> SetUserAction(evtAction);  
  runManager -> SetUserAction(new steppingAction(evtAction));
  
  runManager -> Initialize();
  
  // If the shell variable for visualization use is set, then create a
  // manager to handle the visualization processes  
  G4VisManager* visManager = new G4VisExecutive;
  visManager -> Initialize();
  G4TrajectoryDrawByParticleID* model = new G4TrajectoryDrawByParticleID;
  visManager->RegisterModel(model);
  visManager->SelectTrajectoryModel(model->Name());

  // Get the U(ser)I(interface) pointer to allow...*suspense*
  // ...user interface!
  G4UImanager* UI = G4UImanager::GetUIpointer();
  {
    // Create a modern UI interface with embedded OpenGL graphics
    G4UIExecutive *UIExecutive = new G4UIExecutive(argc, argv, "Qt");
    //UI-> ApplyCommand("/control/execute scintTest.vis");
    //UI-> ApplyCommand("/control/execute analysis.mac");
    
    // In Geant4 speak, verbose is the amount of information that the
    // simulation will output.  From 0 (silent) to some non-zero value
    // that is specific individual information (High number can output
    // an IMMENSE amount of information.
    UI -> ApplyCommand("/run/verbose 1");
    UI -> ApplyCommand("/event/verbose 0");
    UI -> ApplyCommand("/hits/verbose 0");
    UI -> ApplyCommand("/tracking/verbose 0");
    //UI -> ApplyCommand("/tracking/storeTrajectory 0");
    //UI -> ApplyCommand("/vis/scene/add/trajectories");

    UIExecutive -> SessionStart();

    // Make sure to "delete" anything that we have create above using
    // "new".  This is good memory management

    delete UIExecutive;
    delete UI;
  }
  delete histFiller;
  delete visManager;
  delete evtAction;
  delete runManager;
  
  return 0;
}
