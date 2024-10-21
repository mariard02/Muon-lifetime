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
	_StepOutputFile1.open("./output/step_output_Scintillator1.txt", std::ios::app);
	if (!_StepOutputFile1.is_open()) {
        G4cerr << "Error: No se pudo abrir el archivo para guardar datos de los fotones.\n";
    } else {
        //_StepOutputFile1 << "Track_photon_ID\tEnergy_MeV\tWavelength_nm\tTime_ns\n";
        _StepOutputFile1.flush();  // Forzar la escritura al archivo
    }
}

// Create destructor
SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
	// Define what we do at each step
	auto track = step->GetTrack();
    auto particle = track->GetParticleDefinition();
    auto volume = track->GetVolume();

    // Save the data from the photons
    if (volume -> GetName() == "physScin") {
	    if (particle == G4OpticalPhoton::OpticalPhotonDefinition()) {

	        G4String creatorProcess = track->GetCreatorProcess()->GetProcessName();
	        
	        SavePhotonData(step, creatorProcess);
	    }
    }
}

void SteppingAction::SavePhotonData(const G4Step* step, G4String ProcessName){
	auto track = step->GetTrack();
    G4double energy = track->GetKineticEnergy();
    G4double wavelength = (CLHEP::twopi * CLHEP::hbarc) / (energy / MeV);  // Wavelength in nm
    G4double time = track->GetGlobalTime();
    G4int trackID = track->GetTrackID();

    //_StepOutputFile1 << trackID << "\t" << energy / eV  << "\t" << wavelength / nm << "\t" << time << "\n";

    // Check if it was created through scintillation
    if (ProcessName == "Scintillation") {
    		_StepOutputFile1 << trackID << "\t" << energy / eV  << "\t" << wavelength / nm << "\t" << time << "\n";
    		
		}
}

