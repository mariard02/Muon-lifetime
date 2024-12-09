#include "generator.hh"
#include "Randomize.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
	fParticleGun = new G4ParticleGun(1);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
	delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition *particle = particleTable->FindParticle("mu-");

	G4double cosTheta;
    do {
        cosTheta = 2 * G4UniformRand() - 1.;  // Rango [-1, 1]
    } while (G4UniformRand() > std::pow(cosTheta, 2));  // Rechazar valores que no sigan cos^2(theta)

    G4double phi = CLHEP::twopi * G4UniformRand();
    G4double sinTheta = std::sqrt(1. - cosTheta * cosTheta);
    G4double ux = sinTheta * std::cos(phi),
             uy = sinTheta * std::sin(phi),
             uz = cosTheta;
			 
	G4ThreeVector pos(0., 0., -1.5*m);
	G4ThreeVector mom(0., 0., 1.);

	G4double E_min = 110. * MeV;
	G4double E_max = 145. * MeV;

	G4double EnergyMuon = GenerateEnergy(E_min, E_max);

	fParticleGun->SetParticlePosition(pos);
	fParticleGun->SetParticleMomentumDirection(mom);
	fParticleGun->SetParticleEnergy(EnergyMuon);
	fParticleGun->SetParticleDefinition(particle);

	fParticleGun->GeneratePrimaryVertex(anEvent);
}

G4double MyPrimaryGenerator::GenerateEnergy(G4double E_min, G4double E_max)
{
	G4double r = G4UniformRand();
	return E_max * r + E_min * (1 - r);
}