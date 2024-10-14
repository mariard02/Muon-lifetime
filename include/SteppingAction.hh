#ifndef STEPPINGACTION_HH
#define STEPPINGACTION_HH

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"

class SteppingAction : public G4UserSteppingAction {
public:
    SteppingAction();
    virtual ~SteppingAction();
    
    virtual void UserSteppingAction(const G4Step* step);

private:
    //void SaveMuonStepData(const G4Step* step);
    void SavePhotonData(const G4Step* step, G4String ProcessName);

    std::ofstream _StepOutputFile1;
    //std::ofstream _StepOutputFile2;
};

#endif