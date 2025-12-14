//
// Created by kali on 8/31/25.
//

#include <Arduino.h>
#include <Wire.h>
#include "Led.h"
#include "Controller.h"
#include "Flags.h"
#include "hardware/Battery.h"

Led internalLed(INTERNAL_LED_PIN);
Controller controller;
Battery battery;

void restart() {
    internalLed.off();
    delay(1000);
    ESP.restart();
}
void setup() {
    Serial.begin(9600);

    Wire.begin(PIN_SDA, PIN_SCL, 0);

    internalLed.init();
    internalLed.on();

    battery.init();

    bool success = controller
        .setBattery(&battery)
        ->addFlag(new Flags(FLAGS_PIN))
#ifdef INTERNAL_RGB_LED_PIN
        ->addFlag(new Flags(INTERNAL_RGB_LED_PIN))
#endif
        ->init();

    if (!success) {
        restart();
    }

    internalLed.flash(10);
}

void loop() {
    // internalLed.toggle();
    delay(500);
}