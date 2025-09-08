//
// Created by kali on 8/31/25.
//

#include <Arduino.h>
#include "Led.h"
#include "Controller.h"
#include "Flags.h"

Led internalLed(INTERNAL_LED_PIN);
Controller controller;

void restart() {
    internalLed.off();
    delay(1000);
    ESP.restart();
}
void setup() {
    Serial.begin(9600);

    internalLed.init();
    internalLed.on();

    bool success = controller.addFlag(new Flags(FLAGS_PIN))
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
    internalLed.toggle();
    delay(500);
}