#include <iostream>
#include <string>


#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "construction.hh"
#include "physics.hh"
#include "action.hh"
#include "detector.hh"

#include "RunAction.hh"
#include "SteppingAction.hh"
#include "EventAction.hh"


int main(int argc, char** argv){

	G4RunManager *runManager = new G4RunManager();

	runManager->SetUserInitialization(new MyDetectorConstruction());
	runManager->SetUserInitialization(new MyPhysicsList());
	runManager->SetUserInitialization(new MyactionInitialization());

    RunAction* runAction = new RunAction();
    runManager->SetUserAction(runAction);
    
	SteppingAction* steppingAction = new SteppingAction();
	EventAction* eventAction = new EventAction(steppingAction);

	runManager->SetUserAction(eventAction);
  	runManager->SetUserAction(steppingAction);
  	
  	runManager->Initialize();

	G4UIExecutive *ui = new G4UIExecutive(argc, argv);

	//G4VisManager *visManager = new G4VisExecutive();
	//visManager->Initialize();

	G4UImanager *UImanager = G4UImanager::GetUIpointer();

	//RunOutputFile.open("./output/step_output_PMT.txt", std::ofstream::out | std::ofstream::trunc);
	//RunOutputFile << "Energy (eV)\tTime (ns)\tRun\n";

    //UImanager->ApplyCommand("/vis/open OGL");
	//UImanager->ApplyCommand("/vis/view/set/viewpointVector 1 1 1");
	//UImanager->ApplyCommand("/vis/drawVolume");
	//UImanager->ApplyCommand("/vis/scene/add/trajectories optical");
	//UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");
	//UImanager->ApplyCommand("/vis/viewer/set/autorefresh true");
	//UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate");

	UImanager->ApplyCommand("/run/beamOn 20");

	//ui->SessionStart();

	G4cout << "The simulation has finished. \n";

	delete runManager;

	return 0;
}