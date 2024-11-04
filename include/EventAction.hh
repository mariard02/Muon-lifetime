#ifndef EVENTACTION_HH
#define EVENTACTION_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "SteppingAction.hh"
#include <string>

class EventAction : public G4UserEventAction {
public:
    EventAction(SteppingAction* steppingAction);
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

private:
    SteppingAction* fSteppingAction;
};

#endif