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
	_StepOutputFile1.open("./output/electrons_in_LogicLead.txt", std::ofstream::out | std::ofstream::trunc);
    
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

    if (track->GetDefinition()->GetParticleName() == "mu-"){
        _StepOutputFile1 << preStepPoint->GetPhysicalVolume()->GetName() << "\n";
    }

        // Comprobamos si se ha creado un electrón en LogicLead
    if (track->GetDefinition()->GetParticleName() == "mu-" &&
        preStepPoint->GetPhysicalVolume()->GetName() == "physLead") {
        // Abrir archivo para electrones en LogicLead
        if (_StepOutputFile1.is_open()) {
            G4double electronEnergy = track->GetKineticEnergy();
            G4double electronTime = track->GetGlobalTime();
            _StepOutputFile1 << "Energy (eV): " << electronEnergy / CLHEP::eV
                         << "\tTime (ns): " << electronTime << "\n";

            _StepOutputFile1.flush();
            //_StepOutputFile1 << "There is a muon\n"; 
        }
    } else {
        _StepOutputFile1 << "No electrons in this run \n";
        _StepOutputFile1.flush();
    }
}

void SteppingAction::SavePhotonData(const G4Step* step, G4String ProcessName){
	auto track = step->GetTrack();
    //G4double energy = track->GetKineticEnergy();
    //G4double wavelength = (CLHEP::twopi * CLHEP::hbarc) / (energy / MeV);  // Wavelength in nm
    //G4double time = track->GetGlobalTime();
    //G4int trackID = track->GetTrackID();

    //_StepOutputFile1 << trackID << "\t" << energy / eV  << "\t" << wavelength / nm << "\t" << time << "\n";

    // Check if it was created through scintillation
    //if (ProcessName == "Scintillation") {
    		//_StepOutputFile1 << trackID << "\t" << energy / eV  << "\t" << wavelength / nm << "\t" << time << "\n";
    		
		//}
}

