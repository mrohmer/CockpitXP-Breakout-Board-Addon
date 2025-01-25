#define PITLANE_NUMPIXELS_PER_PITLANE 16
#define PITLANE_NUMPIXELS PITLANE_NUMPIXELS_PER_PITLANE * 2
#define PITLANE_1_FIRST_INDEX 0
#define PITLANE_2_FIRST_INDEX 16

#define FLAGS_PIXELS_PER_FLAG 8
#define FLAGS_NUM 5
#define FLAGS_NUMPIXELS FLAGS_PIXELS_PER_FLAG * FLAGS_NUM

#define FLAGS_STATE_RED 0
#define FLAGS_STATE_GREEN 1
#define FLAGS_STATE_CHAOS 2
#define FLAGS_STATE_FINISHED 3

#define MS_CYCLE 250
#define MS_BETWEEN_LED_TOGGLE 1000
#define MS_BETWEEN_FLAGS_GREEN 500
#define MS_BETWEEN_FLAGS_CHAOS 250
#define MS_BETWEEN_FLAGS_RED 1000
#define MS_BETWEEN_FLAGS_FINISHED 500

#include <Arduino.h>
#include <string.h>
#include <math.h>
#include <Adafruit_NeoPixel.h>


struct LastExecutionState {
    unsigned long ledToggle;
    unsigned long flagsGreen;
    unsigned long flagsChaos;
    unsigned long flagsRed;
    unsigned long flagsFinished;
};
struct FlagsState {
    int state;
    int toggles;
};
struct PitlaneState {
    bool lane1;
    bool lane2;
};

Adafruit_NeoPixel pitlane = Adafruit_NeoPixel(PITLANE_NUMPIXELS, PIN_PITLANE, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel flags = Adafruit_NeoPixel(FLAGS_NUMPIXELS, PIN_FLAGS, NEO_GRB + NEO_KHZ800);

struct LastExecutionState lastExecution;
struct FlagsState flagsState;
struct PitlaneState pitlaneState;

// --- Declarations ---

void readPitlanePins();

void readFlagPins();

void setupInputPins();

void setupStatusLed();

void setupPitlane();

void setupFlags();

void cycleToggleStatusLed();

void cycleUpdateFlagsGreen();

void cycleUpdateFlagsChaos();

void cycleUpdateFlagsRed();

void cycleUpdateFlagsFinished();

void toggleStatusLed();

void flashStatusLed(int n);

void updateFlagsGreen();

void updateFlagsChaos();

void updateFlagsRed();

void updateFlagsFinished();

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
  cycleUpdateFlagsGreen();
  cycleUpdateFlagsChaos();
  cycleUpdateFlagsRed();
  cycleUpdateFlagsFinished();

  delay(MS_CYCLE);
}

// --- Cycle ---
bool shouldExecuteInThisCycle(int difference, unsigned long lastExecutionMillis) {
  return ((millis() - lastExecutionMillis) % difference) < MS_CYCLE;
}

void cycleToggleStatusLed() {
  if (shouldExecuteInThisCycle(MS_BETWEEN_LED_TOGGLE, lastExecution.ledToggle)) {
    toggleStatusLed();
    lastExecution.ledToggle = millis();
  }
}

void cycleUpdateFlagsGreen() {
  if (shouldExecuteInThisCycle(MS_BETWEEN_FLAGS_GREEN, lastExecution.flagsGreen)) {
    updateFlagsGreen();
    lastExecution.flagsGreen = millis();
  }
}

void cycleUpdateFlagsChaos() {
  if (shouldExecuteInThisCycle(MS_BETWEEN_FLAGS_CHAOS, lastExecution.flagsChaos)) {
    updateFlagsChaos();
    lastExecution.flagsChaos = millis();
  }
}

void cycleUpdateFlagsRed() {
  if (shouldExecuteInThisCycle(MS_BETWEEN_FLAGS_RED, lastExecution.flagsRed)) {
    updateFlagsRed();
    lastExecution.flagsRed = millis();
  }
}

void cycleUpdateFlagsFinished() {
  if (shouldExecuteInThisCycle(MS_BETWEEN_FLAGS_FINISHED, lastExecution.flagsFinished)) {
    updateFlagsFinished();
    lastExecution.flagsFinished = millis();
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
  pitlaneState = {
          .lane1 = false,
          .lane2 = false
  };
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
  pitlaneState.lane1 = lane1;
  pitlaneState.lane2 = lane2;

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
  flagsState = {
          .state = FLAGS_STATE_RED,
          .toggles = 0
  };
  flags.begin();
  flags.setBrightness(15);
  flags.clear();
  flags.show();
}

void setFlagsChaos() {
  flagsState = {
          .state = FLAGS_STATE_CHAOS,
          .toggles = 0
  };
}

void setFlagsRed() {
  flagsState = {
          .state = FLAGS_STATE_RED,
          .toggles = 0
  };
}

void setFlagsFinished() {
  flagsState = {
          .state = FLAGS_STATE_FINISHED,
          .toggles = 0
  };
}

void setFlagsGreen() {
  flagsState = {
          .state = FLAGS_STATE_GREEN,
          .toggles = 0
  };
}

void updateFlagsGreen() {
  if (flagsState.state != FLAGS_STATE_GREEN || flagsState.toggles > (5 * 2)) {
    return;
  }

  bool on = flagsState.toggles % 2 == 0;
  flags.clear();
  if (on) {
    flags.fill(flags.Color(0, 255, 0), 0, FLAGS_NUMPIXELS);
  }
  flags.show();

  flagsState.toggles++;
}

void updateFlagsRed() {
  if (flagsState.state != FLAGS_STATE_RED || flagsState.toggles > 0) {
    return;
  }

  flags.clear();
  flags.fill(flags.Color(255, 0, 0), 0, FLAGS_NUMPIXELS);
  flags.show();

  flagsState.toggles++;
}

void setFlagsCheckered(int shift, int r, int g, int b) {
  flags.clear();

  for (int flag = 0; flag < FLAGS_NUM; flag++) {
    for (int pixel = shift; pixel < FLAGS_PIXELS_PER_FLAG / 2; pixel += 2) {
      int index = flag * FLAGS_PIXELS_PER_FLAG + pixel;
      int index2 = FLAGS_PIXELS_PER_FLAG - index - 1;

      flags.setPixelColor(index, flags.Color(r, g, b));
      flags.setPixelColor(index2, flags.Color(r, g, b));
    }
  }

  flags.show();
}

void updateFlagsFinished() {
  if (flagsState.state != FLAGS_STATE_FINISHED) {
    return;
  }

  int shift = flagsState.toggles % 2;
  setFlagsCheckered(shift, 255, 255, 255);

  flagsState.toggles = (flagsState.toggles + 1) % 2;
}

void updateFlagsChaos() {
  if (flagsState.state != FLAGS_STATE_CHAOS) {
    return;
  }

  int shift = flagsState.toggles % 2;
  setFlagsCheckered(shift, 255, 255, 0);

  flagsState.toggles = (flagsState.toggles + 1) % 2;
}

// --- inputs ---
void setupInputPins() {
  pinMode(PIN_PITLANE1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_PITLANE1), readPitlanePins, CHANGE);
  pinMode(PIN_PITLANE2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_PITLANE2), readPitlanePins, CHANGE);
  pinMode(PIN_FLAG1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_FLAG1), readFlagPins, CHANGE);
  pinMode(PIN_FLAG2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_FLAG2), readFlagPins, CHANGE);
}

// --- interrupts ---
void execReadPitlanePins() {
  bool lane1 = digitalRead(PIN_PITLANE1) == 0;
  bool lane2 = digitalRead(PIN_PITLANE2) == 0;

  updatePitlanes(lane1, lane2);
}

void execReadFlagPins() {
  int value1 = 1 - digitalRead(PIN_FLAG1);
  int value2 = 1 - digitalRead(PIN_FLAG2);

  switch (value1 + value2 * 2) {
    case 0:
      setFlagsRed();
      break;
    case 1:
      setFlagsGreen();
      break;
    case 2:
      setFlagsChaos();
      break;
    case 3:
      setFlagsFinished();
      break;
  }
}


void readPitlanePins() {
  execReadPitlanePins();
}

void readFlagPins() {
  execReadFlagPins();
}