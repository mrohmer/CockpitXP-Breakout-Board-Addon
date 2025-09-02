//
// Created by kali on 9/1/25.
//

#include "Controller.h"

Controller::Controller(Input* input, Broadcast* broadcast) {
    this->input = input;
    this->broadcast = broadcast;

    this->input->onChange(std::bind(&Controller::onChange, this, std::placeholders::_1));
}
bool Controller::init() {
    if (!this->broadcast->init()) {
        return false;
    }
    this->input->init();
    Serial.println("Initialized Controller");
    return true;
}
void Controller::onChange(State* state) {
    if (state->isGreen()) {
        return this->startGreen();
    }
    if (state->isChaos()) {
        return this->startChaos();
    }
    if (state->isFinished()) {
        return this->startFinished();
    }
    return this->startRed();
}
void Controller::endAllTickers() {
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
void Controller::startRed() {
    if (this->red != nullptr) {
        return;
    }
    this->endAllTickers();
    this->red = new CountingTicker(0.5, std::bind(&Controller::setRed, this, std::placeholders::_1));
}
void Controller::startGreen() {
    if (this->green != nullptr) {
        return;
    }
    this->endAllTickers();
    this->green = new CountingTicker(2, std::bind(&Controller::setGreen, this, std::placeholders::_1));
}
void Controller::startChaos() {
    if (this->chaos != nullptr) {
        return;
    }
    this->endAllTickers();
    this->chaos = new CountingTicker(4, std::bind(&Controller::setChaos, this, std::placeholders::_1));
}
void Controller::startFinished() {
    if (this->finished != nullptr) {
        return;
    }
    this->endAllTickers();
    this->chaos = new CountingTicker(2, std::bind(&Controller::setFinished, this, std::placeholders::_1));
}
void Controller::setRed(int count) {
    this->send(LightDto::createRed());
}
void Controller::setGreen(int count) {
    bool on = count <= (5 * 2) && count % 2 == 0;
    this->send(LightDto::createGreen(on));
}
void Controller::setChaos(int count) {
    bool initial = count % 2 == 0;
    this->send(LightDto::createChaos(initial));
}
void Controller::setFinished(int count) {
    bool initial = count % 2 == 0;
    this->send(LightDto::createFinished(initial));
}
void Controller::send(LightDto* dto) {
    this->broadcast->send(dto->serialize());
}