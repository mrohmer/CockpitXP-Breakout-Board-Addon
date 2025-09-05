//
// Created by kali on 8/31/25.
//

#include <Arduino.h>
#include "input/Input.h"
#include "Led.h"
#include "communication/Now.h"
#include "Controller.h"
#include "Flags.h"

#define CHANNEL 0

#ifdef USE_I2C_INPUT
#include "input/I2CInput.h"
Input* input = new I2CInput((uint8_t)0x55, PIN_SDA, PIN_SCL);
#else
#include "input/UsbBoxInput.h"
Input* input = new UsbBoxInput(PIN_FLAG1, PIN_FLAG2, PIN_SESSION_RECORD);
#endif

Controller controller(input, new Now(CHANNEL));
Led internalLed(INTERNAL_LED_PIN);

void restart() {
  internalLed.off();
  delay(1000);
  ESP.restart();
}
void setup() {
  Serial.begin(115200);

  internalLed.init();
  internalLed.on();

#ifdef INTERNAL_RGB_LED_PIN
  controller.addFlag(new Flags(INTERNAL_RGB_LED_PIN));
#endif
  if (!controller.init()) {
    return restart();
  }

  internalLed.flash(10);
}

void loop() {
  internalLed.toggle();
  controller.loop();
  delay(100);
}