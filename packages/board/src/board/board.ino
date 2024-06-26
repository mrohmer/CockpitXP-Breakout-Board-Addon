
#define PITLANE_NUMPIXELS_PER_PITLANE 16
#define PITLANE_NUMPIXELS PITLANE_NUMPIXELS_PER_PITLANE * 2
#define PITLANE_1_FIRST_INDEX 0
#define PITLANE_2_FIRST_INDEX 16

#define FLAGS_PIXELS_PER_FLAG 8
#define FLAGS_NUM 5
#define FLAGS_NUMPIXELS FLAGS_PIXELS_PER_FLAG * FLAGS_NUM

#define MS_CYCLE 100
#define MS_BETWEEN_LED_TOGGLE 1000

#include <Arduino.h>
#include <string.h>
#include <math.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel pitlane = Adafruit_NeoPixel(PITLANE_NUMPIXELS, PIN_FLAGS, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel falgs = Adafruit_NeoPixel(FLAGS_NUMPIXELS, PIN_FLAGS, NEO_GRB + NEO_KHZ800);

struct LastExecutionState {
    unsigned long ledToggle;
};

struct LastExecutionState lastExecution;

// --- Declarations ---
#if ESP
void IRAM_ATTR readPitlanePins();
void IRAM_ATTR readFlagPins();
#else

void readPitlanePins();

void readFlagPins();

#endif

void setupInputPins();

void setupStatusLed();

void setupPitlane();

void setupFlags();

void cycleToggleStatusLed();

void toggleStatusLed();

void flashStatusLed(int n);

// --- Arduino Loop ---
void setup() {
  Serial.begin(9600);

  setupStatusLed();
  setupInputPins();
  setupPitlane();
  setupFlags();

  Serial.println("Setup Done.");
  flashStatusLed(10);
}

void loop() {
  cycleToggleStatusLed();

  delay(MS_CYCLE);
}

// --- Cycle ---
bool shouldExecuteInThisCycle(int difference, unsigned long lastExecution) {
  return ((millis() - lastExecution) % difference) < MS_CYCLE;
}

void cycleToggleStatusLed() {
  if (shouldExecuteInThisCycle(MS_BETWEEN_LED_TOGGLE, lastExecution.ledToggle)) {
    toggleStatusLed();
    lastExecution.ledToggle = millis();
  }
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


// --- Pitlane ---
void setupPitlane() {
  pitlane.begin();
  pitlane.setBrightness(15);
  pitlane.clear();
  pitlane.show();
}

void updatePitlane(bool value, int firstIndex) {
  if (!value) {
    return;
  }

  pitlane.fill(pitlane.Color(255, 255, 255), firstIndex, PITLANE_NUMPIXELS_PER_PITLANE);
}

void updatePitlanes(bool lane1, bool lane2) {
  pitlane.clear();
  updatePitlane(
          lane1,
          PITLANE_1_FIRST_INDEX
  );
  updatePitlane(
          lane2,
          PITLANE_2_FIRST_INDEX
  );

  pitlane.show();
}

// --- FLAGS ---
void setupFlags() {
  flags.begin();
  flags.setBrightness(15);
  flags.clear();
  flags.show();
}

void setFlagsYellow() {
  flags.clear();
  flags.fill(flags.Color(255, 255, 255), 0, FLAGS_NUMPIXELS);
  flags.show();
}

void setFlagsRed() {
  flags.clear();
  flags.fill(flags.Color(255, 0, 0), 0, FLAGS_NUMPIXELS);
  flags.show();
}

void setFlagsGreen() {
  flags.clear();
  flags.fill(flags.Color(0, 255, 0), 0, FLAGS_NUMPIXELS);
  flags.show();
}

// --- inputs ---
void setupInputPins() {
  pinMode(PIN_PITLANE1, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_PITLANE1), readPitlanePins, CHANGE);
  pinMode(PIN_PITLANE2, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_PITLANE2), readPitlanePins, CHANGE);
  pinMode(PIN_FLAG1, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_FLAG1), readFlagPins, CHANGE);
  pinMode(PIN_FLAG2, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_FLAG1), readFlagPins, CHANGE);
}

// --- interrupts ---
void execReadPitlanePins() {
  bool lane1 = digitalRead(PIN_PITLANE1) == 1;
  bool lane2 = digitalRead(PIN_PITLANE2) == 1;

  updatePitlanes(lane1, lane2);
}

void execReadFlagPins() {
  int value1 = digitalRead(PIN_FLAG1);
  int value2 = digitalRead(PIN_FLAG2);

  switch(value1 + value2) {
    case 0:
      setFlagsRed();
      break;
    case 1:
      setFlagsGreen();
      break;
    case 2:
      setFlagsYellow();
      break;
  }
}

#if ESP
void IRAM_ATTR readPitlanePins(){
  execReadPitlanePins();
}
void IRAM_ATTR readFlagPins(){
  execReadFlagPins();
}
#else

void readPitlanePins() {
  execReadPitlanePins();
}

void readFlagPins() {
  execReadFlagPins();
}

#endif