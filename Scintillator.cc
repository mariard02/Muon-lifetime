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

	if (argc != 2 && argc != 3){
		G4cout << " ERROR of the input parameters !!! " << G4endl;
		return 0;
	} else {
		G4cout << "INPUT PARAMETERS " << G4endl;
		G4cout << "Number of events: " << argv[1] << G4endl;
		if (argv[2]){
			G4cout << "With visualization" << G4endl;
		} else {
			G4cout << "No visualization" << G4endl;
		}
	}

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

	if (argv[2]){
		G4UIExecutive *ui = new G4UIExecutive(argc, argv);

		G4VisManager *visManager = new G4VisExecutive();
		visManager->Initialize();

		G4UImanager *UImanager = G4UImanager::GetUIpointer();

    	UImanager->ApplyCommand("/vis/open OGL");
		UImanager->ApplyCommand("/vis/view/set/viewpointVector 1 1 1");
		UImanager->ApplyCommand("/vis/drawVolume");
		UImanager->ApplyCommand("/vis/scene/add/trajectories optical");
		UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");
		UImanager->ApplyCommand("/vis/viewer/set/autorefresh true");
		UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate");
		UImanager->ApplyCommand("/tracking/verbose 0");
		UImanager->ApplyCommand("/event/verbose 0");
		UImanager->ApplyCommand("/run/verbose 0");
		UImanager->ApplyCommand("/control/verbose  0");

		runManager -> BeamOn(atoi(argv[1]));

		ui->SessionStart();
	} else {
		G4UImanager *UImanager = G4UImanager::GetUIpointer();
		UImanager->ApplyCommand("/tracking/verbose 0");
		UImanager->ApplyCommand("/event/verbose 0");
		UImanager->ApplyCommand("/run/verbose 0");
		UImanager->ApplyCommand("/control/verbose  0");

		runManager -> BeamOn(atoi(argv[1]));
	}

	// -------------------------------------------------------------------------------------
	// NO VISUALIZATION

	
	// -------------------------------------------------------------------------------------

	// -------------------------------------------------------------------------------------
	//  VISUALIZATION 

	//G4UIExecutive *ui = new G4UIExecutive(argc, argv);

	//G4VisManager *visManager = new G4VisExecutive();
	//visManager->Initialize();

	//G4UImanager *UImanager = G4UImanager::GetUIpointer();

    //UImanager->ApplyCommand("/vis/open OGL");
	//UImanager->ApplyCommand("/vis/view/set/viewpointVector 1 1 1");
	//UImanager->ApplyCommand("/vis/drawVolume");
	//UImanager->ApplyCommand("/vis/scene/add/trajectories optical");
	//UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");
	//UImanager->ApplyCommand("/vis/viewer/set/autorefresh true");
	//UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate");

	//ui->SessionStart();
	// -------------------------------------------------------------------------------------

	G4cout << "The simulation has finished. \n";

	delete runManager;

	return 0;
}