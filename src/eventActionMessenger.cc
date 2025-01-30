#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

#include "eventAction.hh"
#include "eventActionMessenger.hh"

// eventActionMessenger is how the user can communicate with the
// eventAction class at runtime, meaning that the user can change
// variables from the scintTest command line.  

eventActionMessenger::eventActionMessenger(eventAction *eventAction)
  : EA(eventAction) //what is this?
{
  // Creates a new directory where the commands will live
  outputDir = new G4UIdirectory("/scintTest/output/");
  outputDir -> SetGuidance("Data output control");

  // Command will let the user specify the name of the output file
  fileCommand = new G4UIcmdWithAString("/scintTest/output/setFileName",this);
  fileCommand -> SetGuidance("Set the output data file name");
  fileCommand -> SetParameterName("choice",true);
  fileCommand -> SetDefaultValue("scintTest.dat");
  fileCommand -> AvailableForStates(G4State_Idle);

  // Command will let the user turn data output 'on' or 'off'
  dataCommand = new G4UIcmdWithAString("/scintTest/output/setDataOutput",this);
  dataCommand -> SetGuidance("Set the data output on/off");
  dataCommand -> SetParameterName("choice",true);
  dataCommand -> SetDefaultValue("off");
  dataCommand -> SetCandidates("on off");
  dataCommand -> AvailableForStates(G4State_Idle);
}

eventActionMessenger::~eventActionMessenger()
{
  delete dataCommand;
  delete fileCommand;
  delete outputDir;
}


// The SetNewValue command is responsible for calling the appropriate
// function in eventAction.cc/.hh once the user has selected a command
// and a value...
void eventActionMessenger::SetNewValue(G4UIcommand *command, 
				       G4String newCommand)
{
  // if(true), then pass commands to eventAction.hh functions
  if(command == fileCommand)
    EA -> SetOutputFileName("Outputs/"+newCommand);
  
  if(command == dataCommand)
    EA -> SetDataOutput(newCommand);
}
