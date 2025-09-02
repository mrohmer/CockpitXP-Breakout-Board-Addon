//
// Created by kali on 8/31/25.
//

#include <Arduino.h>
#include "Led.h"
#include "communication/Now.h"

#ifdef ESP32
#define INTERNAL_LED_PIN 22
#else
#define INTERNAL_LED_PIN 2
#endif


Led internalLed(INTERNAL_LED_PIN);

void restart() {
    internalLed.off();
    delay(1000);
    ESP.restart();
}
void setup() {
    Serial.begin(9600);

    internalLed.init();
    internalLed.on();

    bool success = Now::getInstance()
        ->init();

    if (!success) {
        restart();
    }

    internalLed.flash(10);
}

void loop() {
    internalLed.toggle();
    delay(500);
}