#define MS_CYCLE 250

#include <Arduino.h>
#include "communication/I2C.h"
#include "communication/UsbBox.h"
#include "Led.h"


// --- Declarations ---
void onFlagsChange(int flags);

void onSessionRecordChange(int sessionRecord);

void sendStateToI2C();

struct State {
  uint8_t flags;
  uint8_t sessionRecord;
};

struct State state;
Led internalLed(PIN_LED);
struct UsbBox usbBox(PIN_FLAG1, PIN_FLAG2, PIN_FLAG_SESSION_RECORD);
#ifdef ESP32
I2C i2c(PIN_SDA, PIN_SCL);
#else
I2C i2c
#endif
bool send = false;

// --- Arduino Loop ---
void setup() {
  Serial.begin(9600);

  internalLed.init();
  i2c.init();

  usbBox
    .onFlagsChange(onFlagsChange)
    ->onSessionRecordChange(onSessionRecordChange)
    ->init();

  Serial.println("Setup Done.");
  internalLed.flash(10);
}

void loop() {
  long start = millis();
  internalLed.toggle();
  sendStateToI2C();

  delay(MS_CYCLE);
}

// --- I2C ---
void sendStateToI2C() {
  if (!send) {
    return;
  }
  send = false;

  Serial.println("Sending state");
  Serial.print("[flags, sessionRecord]: ");

  char json[10];
  sprintf(json, "[%d,%d]", state.flags, state.sessionRecord);
  Serial.println(json);

  i2c.send(json);
}

// --- interrupts ---
void onFlagsChange(int flags) {
  if (flags == state.flags) {
    return;
  }
  state.flags = flags;
  send = true;
}

void onSessionRecordChange(int sessionRecord) {
  if (sessionRecord == state.sessionRecord) {
    return;
  }
  state.sessionRecord = sessionRecord;
  send = true;
}