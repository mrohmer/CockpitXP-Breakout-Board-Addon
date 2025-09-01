//
// Created by kali on 9/1/25.
//

#include "CountingTicker.h"

CountingTicker::CountingTicker(float rateInHz, OnCount callback) {
    this->callback = callback;

    ticker.attach(1 / (rateInHz * 1000), std::bind(&CountingTicker::tick, this));
    this->tick();
}
CountingTicker::~CountingTicker() {
    this->off();
}
void CountingTicker::off() {
    ticker.detach();
}
void CountingTicker::tick() {
    this->callback(++this->count);
}