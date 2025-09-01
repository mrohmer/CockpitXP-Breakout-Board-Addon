//
// Created by kali on 9/1/25.
//

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Input.h"
#include "communication/Broadcast.h"
#include "models/State.h"
#include "models/LightDto.h"
#include "utils/CountingTicker.h"

class Controller {
private:
    Input* input;
    Broadcast* broadcast;
    CountingTicker* finished;
    CountingTicker* chaos;
    CountingTicker* green;
    CountingTicker* red;
    void onChange(State* state);
    void endAllTickers();
    void startGreen();
    void startRed();
    void startChaos();
    void startFinished();
    void setGreen(int count);
    void setRed(int count);
    void setChaos(int count);
    void setFinished(int count);
    void send(LightDto* dto);
public:
    Controller(Input* input, Broadcast* broadcast);
    bool init();
};


#endif //CONTROLLER_H
