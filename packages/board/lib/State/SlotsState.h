//
// Created by Matthias Rohmer on 09.01.24.
//

#ifndef ARDUINO_COMMUNICATION_ADDON_SLOTSSTATE_H
#define ARDUINO_COMMUNICATION_ADDON_SLOTSSTATE_H
#include "SlotState.h"

class SlotsState {
public:
    SlotState slot1;
    SlotState slot2;
    SlotState slot3;
    SlotState slot4;
    SlotState slot5;
    SlotState slot6;
    bool lastNeedsToRefuelToggleState;

    SlotsState();

    void reset();
};


#endif //ARDUINO_COMMUNICATION_ADDON_SLOTSSTATE_H
