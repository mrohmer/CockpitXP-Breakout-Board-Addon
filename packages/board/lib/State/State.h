//
// Created by Matthias Rohmer on 09.01.24.
//

#ifndef ARDUINO_COMMUNICATION_ADDON_STATE_H
#define ARDUINO_COMMUNICATION_ADDON_STATE_H
#include "SlotsState.h"
#include "StartLightState.h"
#include "VirtualSafetyCarState.h"

class State {
public:
    SlotsState slots;
    StartLightState startLight;
    VirtualSafetyCarState virtualSafetyCar;
    unsigned int pitlane1;
    unsigned int pitlane2;
    bool newTrackRecord;
    bool newSessionRecord;

    State();
    void reset();
    bool updateFromEvent(unsigned int event);
};


#endif //ARDUINO_COMMUNICATION_ADDON_STATE_H
