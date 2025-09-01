//
// Created by kali on 9/1/25.
//

#include "Controller.h"

Controller::Controller(Input* input, Broadcast* broadcast) {
    this->input = input;
    this->broadcast = broadcast;
}
bool Controller::init() {
    if (!this->broadcast->init()) {
        return false;
    }
    this->input->init();
    return true;
}