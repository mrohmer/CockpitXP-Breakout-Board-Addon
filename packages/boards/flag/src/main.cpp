//
// Created by kali on 8/31/25.
//

#ifdef ESP32
#define INTERNAL_LED_PIN 22
#else
#define INTERNAL_LED_PIN 2
#endif

#include <Arduino.h>
#include "Led.h"
#include "Controller.h"
#include "Flags.h"

Led internalLed(INTERNAL_LED_PIN);
Controller controller(new Flags(FLAGS_PIN));

void restart() {
    internalLed.off();
    delay(1000);
    ESP.restart();
}
void setup() {
    Serial.begin(9600);

    internalLed.init();
    internalLed.on();

    bool success = controller.init();

    if (!success) {
        restart();
    }

    internalLed.flash(10);
}

void loop() {
    internalLed.toggle();
    delay(500);
}