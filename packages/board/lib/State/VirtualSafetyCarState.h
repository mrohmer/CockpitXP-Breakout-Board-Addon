//
// Created by Matthias Rohmer on 09.01.24.
//

#ifndef ARDUINO_COMMUNICATION_ADDON_VIRTUALSAFETYCARSTATE_H
#define ARDUINO_COMMUNICATION_ADDON_VIRTUALSAFETYCARSTATE_H


class VirtualSafetyCarState {
public:
    bool state;
    bool lastToggleState;

    VirtualSafetyCarState();

    void reset();
};


#endif //ARDUINO_COMMUNICATION_ADDON_VIRTUALSAFETYCARSTATE_H
