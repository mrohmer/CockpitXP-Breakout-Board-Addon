//
// Created by kali on 8/31/25.
//

#include <Arduino.h>
#include "Input.h"
#include "Led.h"

#define INTERNAL_LED_PIN 22
#define SDA_PIN 23
#define SCL_PIN 19

State* state = new State();
Input input(8, SDA_PIN, SCL_PIN, state);
Led internalLed(INTERNAL_LED_PIN);

void setup() {
  Serial.begin(9600);

  internalLed.init();
  input.init();

  internalLed.flash(10);
}

void loop() {
  internalLed.toggle();
  delay(500);
}