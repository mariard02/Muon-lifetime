#include "detector.hh"
#include <string>
#include <G4RandomTools.hh>

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
    // In the case the file doesn't exist, we create it. If it exists, we overwrite it.
    // We have to open the file in the constructor. Otherwise, there will be no output.

    //newFile.open("./output/new_file.txt");

    OutputFile.open("./output/PMT.txt", std::ofstream::out | std::ofstream::app);
    //OutputFile << "Energy (eV)\tTime (ns)\n";
    //OutputFile.flush();

    quEff = new G4PhysicsFreeVector();

    std::ifstream datafile;
    datafile.open("../data/eff.dat");

    if (!datafile.is_open()) {
    G4cerr << "Error: no se pudo abrir el archivo eff.dat" << G4endl;
    } else {
        G4cout << "Archivo eff.dat abierto correctamente" << G4endl;
    }


    while(1)
    {
        G4double en, queff;

        datafile >> en >> queff;

        if(datafile.eof())
            break;

        G4cout << en << " " << queff << G4endl;

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

        //G4cout << photonEnergy/ CLHEP::eV << "\t" << photonTime <<  "\n";

        //OutputFile << photonEnergy/ CLHEP::eV << "\t" << photonTime <<  "\n";

        //newFile << photonEnergy/ CLHEP::eV << "\t" << photonTime <<  "\n";
    }
    
    //if (G4UniformRand() < quEff -> Value(photonEnergy/ CLHEP::eV)){
    //    OutputFile << photonEnergy/ CLHEP::eV << "\t" << photonTime << "\n";
    //}
    
    return true;
}