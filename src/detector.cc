#include "detector.hh"
#include <string>

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
    // In the case the file doesn't exist, we create it. If it exists, we overwrite it.
    // We have to open the file in the constructor. Otherwise, there will be no output.
    OutputFile.open("./output/step_output_Scintillator.txt", std::ofstream::out | std::ofstream::trunc);
    OutputFile << "Energy (eV)\tTime (ns)\n";
    OutputFile.flush();
}

MySensitiveDetector::~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{

    G4Track *track = aStep->GetTrack();

    // Detener el seguimiento del fotón después de este paso
    track->SetTrackStatus(fStopAndKill);

    // Obtener los puntos de pre-paso y post-paso
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

    // Obtener la posición del fotón
    G4ThreeVector posPhoton = preStepPoint->GetPosition();
    
    // Obtener la energía cinética del fotón
    G4double photonEnergy = preStepPoint->GetKineticEnergy();

    G4double photonTime = preStepPoint->GetGlobalTime();

    OutputFile << photonEnergy/ CLHEP::eV << "\t" << photonTime << "\n";

    //G4cout << "Photon energy: " << photonEnergy / CLHEP::eV << " eV" << G4endl;

    return true;
}