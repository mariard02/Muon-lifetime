#include "detector.hh"
#include <string>
#include <G4RandomTools.hh>

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
    // In the case the file doesn't exist, we create it. If it exists, we overwrite it.
    // We have to open the file in the constructor. Otherwise, there will be no output.

    //OutputFile.open("./output/PMT.txt", std::ofstream::out | std::ofstream::app);
    //OutputFile << "Energy (eV)\tTime (ns)\n";
    //OutputFile.flush();

    // Vector to save the quantum efficiency
    quEff = new G4PhysicsFreeVector();

    std::ifstream datafile;
    datafile.open("../data/eff.dat");

    if (!datafile.is_open()) {
    G4cerr << "Error: the eff.dat file could not be opened." << G4endl;
    } else {
        G4cout << "eff.dat file opened." << G4endl;
    }

    // Save the quantum efficiency data in the quEff vector.
    while(1)
    {
        G4double en, queff;

        datafile >> en >> queff;

        if(datafile.eof())
            break;

        quEff -> InsertValues(en, queff);
    }

    datafile.close();

}

MySensitiveDetector::~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{

    G4Track *track = aStep->GetTrack();

    // Stops the photon tracking after it enters the detector
    track->SetTrackStatus(fStopAndKill);

    // Obtain pre and post step

    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

    
    // Obtain the kinetic energy of the photon
    if (preStepPoint->GetKineticEnergy() && preStepPoint->GetGlobalTime()) {

        G4double photonEnergy = preStepPoint->GetKineticEnergy();
        G4double photonTime = preStepPoint->GetGlobalTime();

        // SAVE THE DATA (now it is commented because I'm using SteppingAction to save it)

        //OutputFile << photonEnergy/ CLHEP::eV << "\t" << photonTime <<  "\n";

    }

    // Implement quantum efficiency. It is commented because we want to do this in the data analysis.
    
    //if (G4UniformRand() < quEff -> Value(photonEnergy/ CLHEP::eV)){
    //    OutputFile << photonEnergy/ CLHEP::eV << "\t" << photonTime << "\n";
    //}
    
    return true;
}