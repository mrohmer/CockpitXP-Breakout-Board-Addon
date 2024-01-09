//
// Created by Matthias Rohmer on 09.01.24.
//

#ifndef ARDUINO_COMMUNICATION_ADDON_SLOTSTATE_H
#define ARDUINO_COMMUNICATION_ADDON_SLOTSTATE_H


class SlotState {
public:
    bool isRefueling;
    bool needsRefueling;

    SlotState();

    void reset();
};


#endif //ARDUINO_COMMUNICATION_ADDON_SLOTSTATE_H
