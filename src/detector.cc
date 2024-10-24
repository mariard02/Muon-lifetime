#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

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

    G4cout << "Photon energy: " << photonEnergy / CLHEP::eV << " eV" << G4endl;

    return true;
}