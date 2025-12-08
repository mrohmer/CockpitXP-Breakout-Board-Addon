//
// Created by kali on 9/1/25.
//

#ifndef FLAG_CONTROLLER_H
#define FLAG_CONTROLLER_H

#include "input/Input.h"
#include "communication/Now.h"
#include "communication/ble/Ble.h"
#include "models/State.h"
#include "models/LightDto.h"
#include "utils/CountingTicker.h"
#include "Flags.h"
#include <vector>

class FlagController {
private:
    Input* input;
    Now* now;
    Ble* ble;
    CountingTicker* finished;
    CountingTicker* chaos;
    CountingTicker* green;
    CountingTicker* red;
    CountingTicker* progress;
	bool updating = false;
	bool isBleControl = false;
	bool isBleConnected = false;
    std::vector<Flags*> flags;
    std::vector<Flags*> statusFlags;
    void onChange(State state);
    void execUpdate(State state);
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
    void initFlags();
    void updateFlags(LightDto* dto);
    void updateStateFlags(LightDto* dto);
    void updateBleControl(bool enabled, State state);
    void setBleConnected(bool connected);
	bool mirrorFlagToStatusFlag();
public:
    FlagController(Input* input, Ble* ble);
    bool init();
    void loop();
    FlagController* addFlag(Flags* flag);
    FlagController* addStatusFlag(Flags* flag);
	void setUpdateProgress(float progress);
	void setUpdateDone();
};


#endif //FLAG_CONTROLLER_H
