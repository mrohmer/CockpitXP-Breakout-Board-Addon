//
// Created by kali on 9/1/25.
//

#include "State.h"

bool State::isRed() {
    return this->flags == 0;
}
bool State::isGreen() {
    return this->flags == 1;
}
bool State::isChaos() {
    return this->flags == 2;
}
bool State::isFinished() {
    return this->flags == 3;
}
bool State::setFlags(int flags) {
    if (this->flags == flags) {
        return false;
    }
    this->flags = flags;
    this->lastChangeFlags = millis();
    return true;
}
bool State::setSessionRecord(bool sessionRecord) {
    if (this->sessionRecord == sessionRecord) {
        return false;
    }
    this->sessionRecord = sessionRecord;
    this->lastChangeSessionRecord = millis();
    return true;
}