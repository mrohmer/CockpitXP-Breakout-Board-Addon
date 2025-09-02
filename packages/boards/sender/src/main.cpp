//
// Created by kali on 8/31/25.
//

#include <Arduino.h>
#include "Input.h"
#include "Led.h"
#include "communication/Now.h"
#include "Controller.h"

#define INTERNAL_LED_PIN 22
#define SDA_PIN 23
#define SCL_PIN 19
#define CHANNEL 0

Controller controller(new Input((uint8_t)0x55, SDA_PIN, SCL_PIN), new Now(CHANNEL));
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

  if (!controller.init()) {
    return restart();
  }

  internalLed.flash(10);
}

void loop() {
  internalLed.toggle();
  delay(500);
}