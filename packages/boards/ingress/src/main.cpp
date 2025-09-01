#define FLAGS_PIXELS_PER_FLAG 8
#define FLAGS_NUM 5
#define FLAGS_NUMPIXELS FLAGS_PIXELS_PER_FLAG * FLAGS_NUM

#define FLAGS_STATE_RED 0
#define FLAGS_STATE_GREEN 1
#define FLAGS_STATE_CHAOS 2
#define FLAGS_STATE_FINISHED 3

#define MS_CYCLE 250

#include <Arduino.h>
#include "I2C.h"


// --- Declarations ---

void readFlagPins();

void readFlagSessionRecordPin();

void setupInputPins();

void setupStatusLed();

void toggleStatusLed();

void flashStatusLed(int n);

void execRedPins(int n);

void sendStateToI2C();

struct State {
  uint8_t flags;
  uint8_t sessionRecord;
};

struct State state;
#ifdef ESP32
I2C i2c(PIN_SDA, PIN_SCL);
#else
I2C i2c
#endif
bool send = false;

// --- Arduino Loop ---
void setup() {
  Serial.begin(9600);

  setupStatusLed();
  setupInputPins();
  i2c.init();

  Serial.println("Setup Done.");
  flashStatusLed(10);
}

void loop() {
  long start = millis();
  toggleStatusLed();
  sendStateToI2C();

  delay(MS_CYCLE);
}

// --- Status Led ---
void setupStatusLed() {
  pinMode(PIN_LED, OUTPUT);

  digitalWrite(PIN_LED, HIGH);
}

void toggleStatusLed() {
  digitalWrite(PIN_LED, !digitalRead(PIN_LED));
}

void flashStatusLed(int n) {
  while (n-- > 0) {
    digitalWrite(PIN_LED, !digitalRead(PIN_LED));
    delay(100);
    digitalWrite(PIN_LED, !digitalRead(PIN_LED));
    delay(100);
  }
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

// --- inputs ---
void setupInputPins() {
  pinMode(PIN_FLAG1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_FLAG1), readFlagPins, CHANGE);
  pinMode(PIN_FLAG2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_FLAG2), readFlagPins, CHANGE);
  pinMode(PIN_FLAG_SESSION_RECORD, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_FLAG_SESSION_RECORD), readFlagSessionRecordPin, CHANGE);
}

// --- interrupts ---

void execReadFlagPins() {
  uint8_t value1 = 1 - digitalRead(PIN_FLAG1);
  uint8_t value2 = 1 - digitalRead(PIN_FLAG2);

  state.flags = value1 + value2 * 2;
  send = true;
}

void execReadFlagSessionRecordPin() {
  state.sessionRecord = 1 - digitalRead(PIN_FLAG_SESSION_RECORD);
  send = true;
}

void readFlagPins() {
  execReadFlagPins();
}

void readFlagSessionRecordPin() {
  execReadFlagSessionRecordPin();
}