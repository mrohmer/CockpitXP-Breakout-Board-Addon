//
// Created by kali on 9/2/25.
//

#include "Controller.h"

Controller::Controller(Flags* flags) {
    this->flags = flags;
    this->now = Now::getInstance()
        ->onReceive(std::bind(&Controller::onReceiveData, this, std::placeholders::_1));
}
bool Controller::init() {
    this->flags->init();
    return this->now->init();
}
void Controller::onReceiveData(String data) {
    JsonDocument doc;
    deserializeJson(doc, data);
    JsonArray array = doc.as<JsonArray>();

    this->flags
        ->clear()
        ->setColorString(0, array[0])
        ->setColorString(1, array[1])
        ->setColorString(2, array[2])
        ->setColorString(3, array[3])
        ->show();
}