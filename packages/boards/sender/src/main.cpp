//
// Created by kali on 8/31/25.
//

#include <Arduino.h>
#include "I2C.h"
#include "Led.h"

#define INTERNAL_LED_PIN 22
#define SDA_PIN 23
#define SCL_PIN 19

void onReceive(String data);

I2C i2c(8, SDA_PIN, SCL_PIN, onReceive);
Led internalLed(INTERNAL_LED_PIN);

void setup() {
  Serial.begin(9600);

  internalLed.init();
  i2c.init();

  internalLed.flash(10);
}

void onReceive(String data) {
  Serial.print("Received data: ");
  Serial.println(data);
}

void loop() {
  internalLed.toggle();
  delay(500);
}