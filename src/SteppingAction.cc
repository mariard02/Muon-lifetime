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
    G4int TimeInFiber = 25; // Approximate time that the photons spend in the fiber

    G4int TimeInterval = 25; // This is the time interval during which we will consider the events CHECK THIS

    G4int TimeWindow1 = TimeInFiber + TimeInterval; // Time window for the primary particle

    G4int SeparationDecay = 20;

	// Define what we do at each step
	auto track = step->GetTrack();

    //track->SetTrackStatus(fStopAndKill);

    auto particle = track->GetParticleDefinition();
    auto volume = track->GetVolume();
    G4StepPoint *preStepPoint = step->GetPreStepPoint();
    G4StepPoint *postStepPoint = step->GetPostStepPoint();
    G4double electronTime = track->GetGlobalTime();

    if (preStepPoint->GetPhysicalVolume()->GetName() == "physDetector") {

           G4int trackID = track -> GetTrackID();

           G4double electronEnergy = track->GetKineticEnergy();
           G4double electronTime = track->GetGlobalTime();
           G4int detectorID = preStepPoint->GetTouchableHandle()->GetCopyNumber();
            
           _StepOutputFile1 << electronTime / ns << "\t" << detectorID << "\t" << eventID << "\t" << "\n";

           _StepOutputFile1.flush();

           track->SetTrackStatus(fStopAndKill);

    } 

    //if (preStepPoint->GetPhysicalVolume()->GetName() == "physDetector" && electronTime < TimeWindow1 * ns) {

          //  G4int detectorID = preStepPoint->GetTouchableHandle()->GetCopyNumber();
            
           // energyDepositedMap[detectorID] += 1;
            
          //  track->SetTrackStatus(fStopAndKill);

    //} 
}

void SteppingAction::EndOfEventAction()
{
    //for (const auto& entry : energyDepositedMap) {
      //  G4int detectorID = entry.first;
        //G4double totalEnergyDeposited = entry.second;
        
        //_StepOutputFile2 << detectorID << "\t" << std::fixed << totalEnergyDeposited << "\t" << eventID  << "\n";

    //    _StepOutputFile2.flush();
    //}

    // Limpiar el mapa para el próximo evento
    //energyDepositedMap.clear();
    
    //_StepOutputFile1.flush();
}