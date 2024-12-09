#ifndef RUNACTION_HH
#define RUNACTION_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4VUserActionInitialization.hh"
#include "G4SystemOfUnits.hh"
#include <fstream>

class RunAction : public G4UserRunAction {
public:
    RunAction();
    virtual ~RunAction();
    virtual void BeginOfRunAction(const G4Run* run);
    virtual void EndOfRunAction(const G4Run* run);
    
    void AddTotalEnergyDeposit(G4double energy);
    
private:
    std::ofstream RunOutputFile;
    
    G4double _TotalEnergyDeposit;
};

#endif