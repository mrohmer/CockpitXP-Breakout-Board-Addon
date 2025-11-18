//
// Created by kali on 8/31/25.
//

#include <Arduino.h>
#include "secrets.hpp"
#include "communication/server/Server.h"
#include "input/Input.h"
#include "Led.h"
#include "communication/Now.h"
#include "Controller.h"
#include "Flags.h"
#include <WiFi.h>

#define CHANNEL 0
#define HOSTNAME "carrera"

#ifdef USE_I2C_INPUT
#include "input/I2CInput.h"
Input* input = new I2CInput((uint8_t)0x55, PIN_SDA, PIN_SCL);
#else
#include "input/UsbBoxInput.h"
Input* input = new UsbBoxInput(PIN_FLAG1, PIN_FLAG2, PIN_SESSION_RECORD);
#endif

HttpServer server;
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

  WiFi.setHostname(HOSTNAME);
  WiFi.begin(SECRET_WIFI_SSID, SECRET_WIFI_PASSKEY);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  server
    .onUpdateProgress(std::bind(&Controller::setUpdateProgress, &controller, std::placeholders::_1))
    ->onUpdateDone(std::bind(&Controller::setUpdateDone, &controller))
    ->init();

  bool success = controller.addFlag(new Flags(FLAGS_PIN))
#ifdef INTERNAL_RGB_LED_PIN
    ->addFlag(new Flags(INTERNAL_RGB_LED_PIN))
#endif
    ->init();
  if (!success) {
    return restart();
  }

  internalLed.flash(10);
}

void loop() {
  internalLed.toggle();
  controller.loop();
  server.loop();
  delay(100);
}