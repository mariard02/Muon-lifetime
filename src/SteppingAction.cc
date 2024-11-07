#include "SteppingAction.hh"
#include "G4Step.hh"
#include "G4OpticalPhoton.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"
#include "CLHEP/Units/PhysicalConstants.h" 
#include <string>
#include <cmath> // For M_PI


SteppingAction::SteppingAction()
{
	// Open a file to save the energies of the photons
	_StepOutputFile1.open("./output/PMT.txt", std::ofstream::out | std::ofstream::app);
    
}

// Create destructor
SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
	// Define what we do at each step
	auto track = step->GetTrack();

    //track->SetTrackStatus(fStopAndKill);

    auto particle = track->GetParticleDefinition();
    auto volume = track->GetVolume();
    G4StepPoint *preStepPoint = step->GetPreStepPoint();
    G4StepPoint *postStepPoint = step->GetPostStepPoint();

    if (preStepPoint->GetPhysicalVolume()->GetName() == "physDetector") {

            G4int trackID = track -> GetTrackID();

            G4double electronEnergy = track->GetKineticEnergy();
            G4double electronTime = track->GetGlobalTime();
            G4int detectorID = preStepPoint->GetTouchableHandle()->GetCopyNumber();
            
            _StepOutputFile1 << electronEnergy / eV << "\t" << electronTime / ns << "\t" << detectorID << "\t" << eventID << "\t" << trackID << "\n";

            _StepOutputFile1.flush();

            track->SetTrackStatus(fStopAndKill);

    } 
}


