//
// Created by Matthias Rohmer on 09.01.24.
//

#ifndef ARDUINO_COMMUNICATION_ADDON_STARTLIGHTSTATE_H
#define ARDUINO_COMMUNICATION_ADDON_STARTLIGHTSTATE_H


class StartLightState {
public:
    bool falseStart;
    bool falseStartToggle;

    unsigned int state;

    StartLightState();

    void reset();
};


#endif //ARDUINO_COMMUNICATION_ADDON_STARTLIGHTSTATE_H
