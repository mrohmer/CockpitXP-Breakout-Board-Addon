//
// Created by kali on 9/1/25.
//

#include "FlagController.h"

FlagController::FlagController(Input* input, Now* now, Ble* ble) {
    this->input = input;
    this->now = now;
    this->ble = ble;

    this->input->onChange(std::bind(&FlagController::onChange, this, std::placeholders::_1));
}
void FlagController::init() {
    this->input->init();
    this->initFlags();
    Serial.println("Initialized Controller");
    this->startRed();

    this->ble->onConnectionChange(std::bind(&FlagController::setBleConnected, this, std::placeholders::_1));
    this->ble->getControlFlags()->onChange(std::bind(&FlagController::updateBleControl, this, std::placeholders::_1, std::placeholders::_2));
}
void FlagController::onChange(State state) {
	if (this->isBleControl) {
		return;
	}

    this->execUpdate(state);
}
void FlagController::execUpdate(State state) {
    if (this->updating) {
        return;
    }
    if (state.isGreen) {
        return this->startGreen();
    }
    if (state.isChaos) {
        return this->startChaos();
    }
    if (state.isFinished) {
        return this->startFinished();
    }
    return this->startRed();
}
void FlagController::endAllTickers() {
    if (this->red != nullptr) {
        this->red->off();
        this->red = nullptr;
    }
    if (this->green != nullptr) {
        this->green->off();
        this->green = nullptr;
    }
    if (this->chaos != nullptr) {
        this->chaos->off();
        this->chaos = nullptr;
    }
    if (this->finished != nullptr) {
        this->finished->off();
        this->finished = nullptr;
    }
}
void FlagController::startRed() {
    if (this->red != nullptr) {
        return;
    }
    this->endAllTickers();
    this->red = new CountingTicker(0.5f, std::bind(&FlagController::setRed, this, std::placeholders::_1));
}
void FlagController::startGreen() {
    if (this->green != nullptr) {
        return;
    }
    this->endAllTickers();
    this->green = new CountingTicker(2, std::bind(&FlagController::setGreen, this, std::placeholders::_1));
}
void FlagController::startChaos() {
    if (this->chaos != nullptr) {
        return;
    }
    this->endAllTickers();
    this->chaos = new CountingTicker(4, std::bind(&FlagController::setChaos, this, std::placeholders::_1));
}
void FlagController::startFinished() {
    if (this->finished != nullptr) {
        return;
    }
    this->endAllTickers();
    this->finished = new CountingTicker(2, std::bind(&FlagController::setFinished, this, std::placeholders::_1));
}
void FlagController::setRed(int count) {
    this->send(LightDto::createRed());
}
void FlagController::setGreen(int count) {
    bool on = count <= (5 * 2) && count % 2 == 1;
    this->send(LightDto::createGreen(on));
}
void FlagController::setChaos(int count) {
    bool initial = count % 2 == 0;
    this->send(LightDto::createChaos(initial));
}
void FlagController::setFinished(int count) {
    bool initial = count % 2 == 0;
    this->send(LightDto::createFinished(initial));
}
void FlagController::send(LightDto* dto) {
    this->now->send(dto->serialize());
    this->updateFlags(dto);
}
void FlagController::loop() {
    this->input->loop();
}
FlagController* FlagController::addFlag(Flags* flag) {
    this->flags.insert(this->flags.end(), flag);
    return this;
}
FlagController* FlagController::addStatusFlag(Flags* flag) {
    this->statusFlags.insert(this->statusFlags.end(), flag);
    return this;
}
void FlagController::initFlags() {
    for (auto & element : this->flags) {
        element->init();
    }
    for (auto & element : this->statusFlags) {
        element->init();
    }
}
void FlagController::updateFlags(LightDto* dto) {
    for (auto & element : this->flags) {
        element
            ->clear()
            ->setColorString(0, dto->lights[0])
            ->setColorString(1, dto->lights[1])
            ->setColorString(2, dto->lights[2])
            ->setColorString(3, dto->lights[3])
            ->show();
    }
    if (this->mirrorFlagToStatusFlag()) {
        this->updateStateFlags(dto);
    }
}
void FlagController::updateStateFlags(LightDto* dto) {
    for (auto & element : this->statusFlags) {
        element
            ->clear()
            ->setColorString(0, dto->lights[0])
            ->setColorString(1, dto->lights[1])
            ->setColorString(2, dto->lights[2])
            ->setColorString(3, dto->lights[3])
            ->show();
    }
}
void FlagController::setUpdateProgress(float progress) {
	this->updating = true;
	this->endAllTickers();

    this->send(LightDto::createProgress(progress));
}
void FlagController::setUpdateDone() {
	this->updating = true;
	this->endAllTickers();

    this->send(LightDto::createProgress(100));
}
void FlagController::updateBleControl(bool enabled, State state) {
    this->isBleControl = enabled;

    if (this->isBleControl) {
        this->execUpdate(state);
    } else {
        this->execUpdate(this->input->getState());
    }
}
void FlagController::setBleConnected(bool connected) {
    this->isBleConnected = connected;

    Serial.printf("Ble connected: %d\n", this->isBleConnected);

    if (!this->mirrorFlagToStatusFlag()) {
        this->updateStateFlags(LightDto::createBleConnection());
    }
}
bool FlagController::mirrorFlagToStatusFlag() {
    return !this->isBleConnected;
}