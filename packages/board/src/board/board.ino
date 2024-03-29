#ifndef USE_PULLUP
#   define USE_PULLUP false
#endif
#ifndef OMIT_I2C_CONNECTION
#   define OMIT_I2C_CONNECTION false
#endif
#ifndef OMIT_PITLANE_CONNECTION
#   define OMIT_PITLANE_CONNECTION false
#endif
#ifndef OMIT_RACE_PROGRESS_CONNECTION
#   define OMIT_RACE_PROGRESS_CONNECTION false
#endif
#ifndef DATA_VIA_I2C
#   define DATA_VIA_I2C false
#endif
#ifndef FUELING_SLOT_VIA_I2C
#   define FUELING_SLOT_VIA_I2C false
#endif
#ifndef VIRTUAL_SAFETY_CAR_VIA_I2C
#   define VIRTUAL_SAFETY_CAR_VIA_I2C false
#endif

#define PITLANE_NUMPIXELS 32
#define PITLANE_1_FIRST_INDEX 0
#define PITLANE_2_FIRST_INDEX 16

#define RACE_PROGRESS_NUMPIXELS 12

#define MS_CYCLE 100
#define MS_BETWEEN_STATUS_LOG 5000
#define MS_BETWEEN_LED_TOGGLE 1000
#define MS_BETWEEN_FALSE_START_TOGGLE 200
#define MS_BETWEEN_NEEDS_TO_REFUEL_TOGGLE 200
#define MS_BETWEEN_RACE_CIRCLE_UPDATE 50
#define MS_VIRTUAL_SAFETY_CAR_PULSE_TIME 3000

#include <Arduino.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <string.h>
#include <math.h>
#include <Adafruit_MCP23X17.h>
#include <Adafruit_NeoPixel.h>

// --- Structs ---
struct Input {  // Structure declaration
    unsigned int chunks;
    unsigned int data;
    unsigned int chunksData[3][3];
};
struct SlotState {
    bool isRefueling;
    bool needsRefueling;
};
struct SlotsState {
    struct SlotState slot1;
    struct SlotState slot2;
    struct SlotState slot3;
    struct SlotState slot4;
    struct SlotState slot5;
    struct SlotState slot6;

    bool lastNeedsToRefuelToggleState;
};
struct StartLightState {
    bool falseStart;
    bool falseStartToggle;

    unsigned int state;
};
struct RaceState {
    bool chaos;
    bool isInProgress;
    bool virtualSafetyCar;
    int progress;
};
struct State {
    struct SlotsState slots;

    int pitlane1;
    int pitlane2;

    struct StartLightState startLight;

    struct RaceState race;

    bool newTrackRecord;
    bool newSessionRecord;

    bool needsUpdate;
};
struct LastExecutionState {
    unsigned long log;
    unsigned long ledToggle;
    unsigned long falseStartToggle;
    unsigned long needsToRefuelToggle;
    unsigned long raceProgressCircleUpdate;
};

// --- Variables ---
struct Input input;
struct State state;
struct RaceState propagatedRaceState;
struct LastExecutionState lastExecution;
Max72xxPanel matrix = Max72xxPanel(PIN_START_LIGHT_CS, PIN_START_LIGHT_HORIZONTAL, PIN_START_LIGHT_VERTICAL);
Adafruit_MCP23X17 mcp;

Adafruit_NeoPixel pitlane = Adafruit_NeoPixel(PITLANE_NUMPIXELS, PIN_PITLANE, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel raceProgress = Adafruit_NeoPixel(RACE_PROGRESS_NUMPIXELS, PIN_RACE_PROGRESS, NEO_GRB + NEO_KHZ800);

// --- Declarations ---
#if ESP
void IRAM_ATTR readInput();
#else

void readInput();

#endif

void setupInputPins();

void setupStatusLed();

void setupI2C();

void setupPitlane();

void setupFueling();

void setupRaceProgressCircle();

void updateRaceProgressCircle();

void cyclePrintStatusLog();

void cycleToggleStatusLed();

void cycleToggleStartLightFalseStart();

void cycleToggleNeedsToRefuel();

void cycleUpdateRaceProgressCircle();

void toggleStatusLed();

void flashStatusLed(int n);

void toggleStartLightFalseStart();

void toggleNeedsToRefuel();

void restoreState();

void updateOnStatusChange();

// --- Arduino Loop ---
void setup() {
  Serial.begin(9600);

  setupI2C();
  setupStartLight();
  setupPitlane();
  setupFueling();
  setupRaceProgressCircle();
  setupStatusLed();
  setupInputPins();

  Serial.println("Setup Done.");
  flashStatusLed(10);

  restoreState();
}

void loop() {
  if (state.needsUpdate) {
    updateOnStatusChange();
  }
  cyclePrintStatusLog();
  cycleToggleStatusLed();
  cycleToggleStartLightFalseStart();
  cycleToggleNeedsToRefuel();
  cycleUpdateRaceProgressCircle();
  delay(MS_CYCLE);
}

// --- Cycle ---
bool shouldExecuteInThisCycle(int difference, unsigned long lastExecution) {
  return ((millis() - lastExecution) % difference) < MS_CYCLE;
}

void cyclePrintStatusLog() {
  if (shouldExecuteInThisCycle(MS_BETWEEN_STATUS_LOG, lastExecution.log)) {
    Serial.println("Waiting for inputs...");
    lastExecution.log = millis();
  }
}

void cycleToggleStatusLed() {
  if (shouldExecuteInThisCycle(MS_BETWEEN_LED_TOGGLE, lastExecution.ledToggle)) {
    toggleStatusLed();
    lastExecution.ledToggle = millis();
  }
}

void cycleToggleStartLightFalseStart() {
  if (shouldExecuteInThisCycle(MS_BETWEEN_FALSE_START_TOGGLE, lastExecution.falseStartToggle)) {
    toggleStartLightFalseStart();
    lastExecution.falseStartToggle = millis();
  }
}

void cycleToggleNeedsToRefuel() {
  if (shouldExecuteInThisCycle(MS_BETWEEN_NEEDS_TO_REFUEL_TOGGLE, lastExecution.needsToRefuelToggle)) {
    toggleNeedsToRefuel();
    lastExecution.needsToRefuelToggle = millis();
  }
}

void cycleUpdateRaceProgressCircle() {
  if (shouldExecuteInThisCycle(MS_BETWEEN_RACE_CIRCLE_UPDATE, lastExecution.raceProgressCircleUpdate)) {
    updateRaceProgressCircle();
    lastExecution.raceProgressCircleUpdate = millis();
  }
}

// --- UTILS ---
char *toBinaryString(int n, int numberOfBits) {
  char *string = (char *) malloc(numberOfBits + 1);
  if (!string) {
    return NULL;
  }
  for (int i = numberOfBits - 1; i >= 0; i--) {
    string[i] = (n & 1) + '0';
    n >>= 1;
  }
  string[numberOfBits] = '\0';
  return string;
}

bool hasEvenParity(unsigned int n) {
  char *str = toBinaryString(n, 32);
  int count = 0;
  for (unsigned int i = 0; i < strlen(str); i++) {
    char compLetter = str[i];
    if (strcmp(&compLetter, "1")) {
      count++;
    }
  }
  return count % 2 == 0;
}

unsigned int clearBit(unsigned int number, unsigned int n) {
  return number & ~((unsigned int) 1 << n);
}

int min(int a, int b) {
  return a < b ? a : b;
}

int max(int a, int b) {
  return a > b ? a : b;
}

// --- I2C ---
void setupI2C() {
  if (OMIT_I2C_CONNECTION) {
    return;
  }
  if (!mcp.begin_I2C()) {
    Serial.println("Error.");
    while (1);
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

// --- Start Light ---
void setupStartLight() {
  matrix.setIntensity(0); // Use a value between 0 and 15 for brightness
  for (int i = 0; i < PIN_START_LIGHT_HORIZONTAL; ++i) {
    matrix.setRotation(i, 1);    // The first display is position upside down
  }
  matrix.fillScreen(LOW);
  matrix.write();
}

void fillStartLightFields(int n) {
  matrix.fillScreen(LOW);
  for (int i = 0; i < n; i++) {
    for (int x = 0; x < 8; x++) {
      for (int y = 0; y < 8; y++) {
        matrix.drawPixel(x + i * 8, y, HIGH);
      }
    }
  }
  matrix.write(); // Send bitmap to display
}

void clearStartLightFields() {
  matrix.fillScreen(LOW);
  matrix.write(); // Send bitmap to display
}

void updateStartLight() {
  if (state.startLight.state > 0 && state.startLight.state <= 5) {
    Serial.println(state.startLight.state);
    fillStartLightFields(state.startLight.state);
  } else {
    matrix.fillScreen(LOW);
    matrix.write();
  }
}

void toggleStartLightFalseStart() {
  if (!state.startLight.falseStart) {
    return;
  }

  if (state.startLight.falseStartToggle) {
    clearStartLightFields();
  } else {
    fillStartLightFields(PIN_START_LIGHT_HORIZONTAL);
  }
  state.startLight.falseStartToggle = !state.startLight.falseStartToggle;
}

// --- Pitlane ---
void setupPitlane() {
  if (OMIT_PITLANE_CONNECTION) {
    return;
  }

  pitlane.begin();
  pitlane.setBrightness(15);
  pitlane.clear();
  pitlane.show();
}


void setPitlaneActiveIndicatorPixel(int value, int firstIndex, bool inverted) {
  if (OMIT_PITLANE_CONNECTION) {
    return;
  }

  int index = firstIndex;
  if (inverted) {
    index = PITLANE_NUMPIXELS / 2 - 1 + firstIndex;
  }

  if (value < 0) {
    pitlane.setPixelColor(index, pitlane.Color(0, 0, 0));
    return;
  }


  pitlane.setPixelColor(index, pitlane.Color(255, 255, 255));
}

void setPitlaneBarRedPixels(int index, int amount) {
  if (amount <= 0) {
    return;
  }
  pitlane.fill(pitlane.Color(255, 0, 0), index, amount);
}

void setPitlaneBarYellowPixels(int index, int amount) {
  if (amount <= 0) {
    return;
  }
  pitlane.fill(pitlane.Color(255, 255, 0), index, amount);
}

void setPitlaneBarGreenPixels(int index, int amount) {
  if (amount <= 0) {
    return;
  }
  pitlane.fill(pitlane.Color(0, 255, 0), index, amount);
}

void updatePitlaneBar(int value, int firstIndex, bool inverted) {
  if (OMIT_PITLANE_CONNECTION) {
    return;
  }

  if (value <= 0) {
    return;
  }

  int redPixels = min(3, value);
  int yellowPixels = max(min(12, value) - 3, 0);
  int greenPixels = max(value - 12, 0);

  if (!inverted) {
    int index = firstIndex + 2;

    setPitlaneBarRedPixels(index, redPixels);
    setPitlaneBarYellowPixels(index + 3, yellowPixels);
    setPitlaneBarGreenPixels(index + 12, greenPixels);
  } else {
    int lastIndex = PITLANE_NUMPIXELS / 2 + firstIndex;
    int index = lastIndex - 2;

    setPitlaneBarRedPixels(index - redPixels, redPixels);
    setPitlaneBarYellowPixels(index - redPixels - yellowPixels, yellowPixels);
    setPitlaneBarGreenPixels(index - redPixels - yellowPixels - greenPixels, greenPixels);
  }
}

void updatePitlane(int value, int firstIndex, bool inverted) {
  setPitlaneActiveIndicatorPixel(
          value,
          firstIndex,
          inverted
  );
  updatePitlaneBar(
          value,
          firstIndex,
          inverted
  );
}

void updatePitlanes() {
  if (OMIT_PITLANE_CONNECTION) {
    return;
  }

  pitlane.clear();
  updatePitlane(
          state.pitlane1,
          PITLANE_1_FIRST_INDEX,
          false
  );
  updatePitlane(
          state.pitlane2,
          PITLANE_2_FIRST_INDEX,
          true
  );

  pitlane.show();
}

// --- Race Progress Circle ---
void setupRaceProgressCircle() {
  if (OMIT_RACE_PROGRESS_CONNECTION) {
    return;
  }

  raceProgress.begin();
  raceProgress.setBrightness(15);
  raceProgress.clear();
  raceProgress.show();
}

bool needsRaceProgressCircleUpdate() {
  return state.race.virtualSafetyCar ||
         propagatedRaceState.progress != state.race.progress ||
         propagatedRaceState.chaos != state.race.chaos;
}

void setRaceProgressCircleToProgress() {
  raceProgress.setBrightness(15);
  if (state.race.progress > 0) {
    raceProgress.fill(raceProgress.Color(255, 255, 255), 0, min(state.race.progress, 8));
    if (state.race.progress > 8) {
      raceProgress.fill(raceProgress.Color(255, 0, 0), 8, state.race.progress - 8);
    }
  }
}

void setRaceProgressCircleToChaos() {
  raceProgress.setBrightness(15);
  raceProgress.fill(raceProgress.Color(255, 255, 0), 0, RACE_PROGRESS_NUMPIXELS);
}

void setRaceProgressCircleToVirtualSafetyCar() {
  int maxBrightness = 255;
  int amplitude = 220;
  float cycle = (static_cast< float >(millis()) / static_cast< float >(MS_VIRTUAL_SAFETY_CAR_PULSE_TIME)) - (millis() / MS_VIRTUAL_SAFETY_CAR_PULSE_TIME);
  float angle = (cycle * 2) * M_PI ;
  int brightness = static_cast< float >(amplitude) / 2 * sin(angle) + static_cast< float >(amplitude) / 2 + maxBrightness - amplitude;
  raceProgress.setBrightness(15);
  raceProgress.fill(raceProgress.Color(brightness, brightness, 0), 0, RACE_PROGRESS_NUMPIXELS);
}

void updateRaceProgressCircle() {
  if (OMIT_RACE_PROGRESS_CONNECTION) {
    return;
  }

  if (!needsRaceProgressCircleUpdate()) {
    return;
  }

  raceProgress.clear();

  if (state.race.chaos) {
    setRaceProgressCircleToChaos();
  } else if (state.race.virtualSafetyCar) {
    setRaceProgressCircleToVirtualSafetyCar();
  } else {
    setRaceProgressCircleToProgress();
  }

  raceProgress.show();
  propagatedRaceState = state.race;
}

// --- Fueling ---
void setupFueling() {
  if (OMIT_I2C_CONNECTION) {
    return;
  }

  if (FUELING_SLOT_VIA_I2C) {
    mcp.pinMode(PIN_FUELING_SLOT_1, OUTPUT);
    mcp.pinMode(PIN_FUELING_SLOT_2, OUTPUT);
    mcp.pinMode(PIN_FUELING_SLOT_3, OUTPUT);
    mcp.pinMode(PIN_FUELING_SLOT_4, OUTPUT);
    mcp.pinMode(PIN_FUELING_SLOT_5, OUTPUT);
    mcp.pinMode(PIN_FUELING_SLOT_6, OUTPUT);
  } else {
    pinMode(PIN_FUELING_SLOT_1, OUTPUT);
    pinMode(PIN_FUELING_SLOT_2, OUTPUT);
    pinMode(PIN_FUELING_SLOT_3, OUTPUT);
    pinMode(PIN_FUELING_SLOT_4, OUTPUT);
    pinMode(PIN_FUELING_SLOT_5, OUTPUT);
    pinMode(PIN_FUELING_SLOT_6, OUTPUT);
  }
}

void updateSlotIsRefueling(bool isRefueling, unsigned int pin) {
  if (OMIT_I2C_CONNECTION) {
    return;
  }
  if (FUELING_SLOT_VIA_I2C) {
    mcp.digitalWrite(pin, isRefueling ? HIGH : LOW);
  } else {
    mcp.digitalWrite(pin, isRefueling ? HIGH : LOW);
  }
}

void updateIsRefueling() {
  updateSlotIsRefueling(state.slots.slot1.isRefueling, PIN_FUELING_SLOT_1);
  updateSlotIsRefueling(state.slots.slot2.isRefueling, PIN_FUELING_SLOT_2);
  updateSlotIsRefueling(state.slots.slot3.isRefueling, PIN_FUELING_SLOT_3);
  updateSlotIsRefueling(state.slots.slot4.isRefueling, PIN_FUELING_SLOT_4);
  updateSlotIsRefueling(state.slots.slot5.isRefueling, PIN_FUELING_SLOT_5);
  updateSlotIsRefueling(state.slots.slot6.isRefueling, PIN_FUELING_SLOT_6);
}

void toggleSlotNeedsToRefuel(bool needsToRefuel, bool isRefueling, bool prevState, unsigned int pin) {
  if (OMIT_I2C_CONNECTION) {
    return;
  }

  if (isRefueling) {
    // handled in interrupt
    return;
  }

  bool nextState = state.slots.lastNeedsToRefuelToggleState && needsToRefuel && state.race.isInProgress;

  if (prevState == nextState) {
    return;
  }

  mcp.digitalWrite(pin, nextState ? HIGH : LOW);
}

void toggleNeedsToRefuel() {
  bool prevState = state.slots.lastNeedsToRefuelToggleState;
  state.slots.lastNeedsToRefuelToggleState = !state.slots.lastNeedsToRefuelToggleState;
  toggleSlotNeedsToRefuel(state.slots.slot1.needsRefueling, state.slots.slot1.isRefueling, prevState,
                          PIN_FUELING_SLOT_1);
  toggleSlotNeedsToRefuel(state.slots.slot2.needsRefueling, state.slots.slot2.isRefueling, prevState,
                          PIN_FUELING_SLOT_2);
  toggleSlotNeedsToRefuel(state.slots.slot3.needsRefueling, state.slots.slot3.isRefueling, prevState,
                          PIN_FUELING_SLOT_3);
  toggleSlotNeedsToRefuel(state.slots.slot4.needsRefueling, state.slots.slot4.isRefueling, prevState,
                          PIN_FUELING_SLOT_4);
  toggleSlotNeedsToRefuel(state.slots.slot5.needsRefueling, state.slots.slot5.isRefueling, prevState,
                          PIN_FUELING_SLOT_5);
  toggleSlotNeedsToRefuel(state.slots.slot6.needsRefueling, state.slots.slot6.isRefueling, prevState,
                          PIN_FUELING_SLOT_6);
}

// --- State Update ---
void printState() {
  Serial.println("State:");
  Serial.print("Pitlane 1: ");
  Serial.print(state.pitlane1);
  Serial.print(" | Pitlane 2: ");
  Serial.println(state.pitlane2);

  Serial.print("Slot1: ");
  Serial.print(state.slots.slot1.isRefueling ? "isRefueling" : "-");
  Serial.print(" | ");
  Serial.println(state.slots.slot1.needsRefueling ? "needs to refuel" : "-");

  Serial.print("Slot2: ");
  Serial.print(state.slots.slot2.isRefueling ? "isRefueling" : "-");
  Serial.print(" | ");
  Serial.println(state.slots.slot2.needsRefueling ? "needs to refuel" : "-");

  Serial.print("Slot3: ");
  Serial.print(state.slots.slot3.isRefueling ? "isRefueling" : "-");
  Serial.print(" | ");
  Serial.println(state.slots.slot3.needsRefueling ? "needs to refuel" : "-");

  Serial.print("Slot4: ");
  Serial.print(state.slots.slot4.isRefueling ? "isRefueling" : "-");
  Serial.print(" | ");
  Serial.println(state.slots.slot4.needsRefueling ? "needs to refuel" : "-");

  Serial.print("Slot5: ");
  Serial.print(state.slots.slot5.isRefueling ? "isRefueling" : "-");
  Serial.print(" | ");
  Serial.println(state.slots.slot5.needsRefueling ? "needs to refuel" : "-");

  Serial.print("Slot6: ");
  Serial.print(state.slots.slot6.isRefueling ? "isRefueling" : "-");
  Serial.print(" | ");
  Serial.println(state.slots.slot6.needsRefueling ? "needs to refuel" : "-");

  Serial.print("Start light: ");
  Serial.print(state.startLight.state);
  Serial.print(" | ");
  Serial.println(state.startLight.falseStart ? "false start" : "-");

  Serial.print("Track Record: ");
  Serial.println(state.newTrackRecord ? "on" : "off");

  Serial.print("Session Record: ");
  Serial.println(state.newSessionRecord ? "on" : "off");

  Serial.print("Race State: ");
  Serial.println(state.race.isInProgress ? "Running" : "Stopped");
  Serial.print("Chaos: ");
  Serial.println(state.race.chaos ? "On" : "Off");
  Serial.print("VSC: ");
  Serial.println(state.race.virtualSafetyCar ? "On" : "Off");

  Serial.print("Race Progress: ");
  Serial.println(state.race.progress);
}

void resetState() {
  state = {
          .slots = {
                  .slot1 = {.isRefueling = false, .needsRefueling = false},
                  .slot2 = {.isRefueling = false, .needsRefueling = false},
                  .slot3 = {.isRefueling = false, .needsRefueling = false},
                  .slot4 = {.isRefueling = false, .needsRefueling = false},
                  .slot5 = {.isRefueling = false, .needsRefueling = false},
                  .slot6 = {.isRefueling = false, .needsRefueling = false},
                  .lastNeedsToRefuelToggleState = false
          },
          .pitlane1 = -1,
          .pitlane2 = -1,
          .startLight = {.falseStart = false, .falseStartToggle = false, .state = 0},

          .race = {
                  .chaos = false,
                  .isInProgress = false,
                  .virtualSafetyCar = false,
                  .progress = 0,
          },

          .newTrackRecord = false,
          .newSessionRecord = false,
          .needsUpdate = true,
  };
}

bool updateState(unsigned int event) {
  switch (event) {
    case 11:
      resetState();
      break;
    case 21:  // virtual safety car on
      state.race.virtualSafetyCar = true;
      break;
    case 22:  // virtual safety car off
      state.race.virtualSafetyCar = false;
      break;
    case 28:  // starting light off
      state.startLight.state = 0;
      state.startLight.falseStart = false;
      break;
    case 31:  // starting light 1
      state.startLight.state = 1;
      state.startLight.falseStart = false;
      break;
    case 35:  // starting light 2
      state.startLight.state = 2;
      state.startLight.falseStart = false;
      break;
    case 37:  // starting light 3
      state.startLight.state = 3;
      state.startLight.falseStart = false;
      break;
    case 38:  // starting light 4
      state.startLight.state = 4;
      state.startLight.falseStart = false;
      break;
    case 41:  // starting light 5
      state.startLight.state = 5;
      state.startLight.falseStart = false;
      break;
    case 42:  // starting light false start
      state.startLight.falseStart = true;
      break;
    case 44:  // starting light green
      state.startLight.state = 6;
      state.startLight.falseStart = false;
      break;
    case 49:  // new track record
      state.newTrackRecord = true;
      break;
    case 50:  // new session record
      state.newSessionRecord = true;
      break;
    case 55:  // pit lane 1 off
      state.pitlane1 = -1;
      break;
    case 59:  // pit lane 1 0%
      state.pitlane1 = 0;
      break;
    case 61:  // pit lane 1 7%
      state.pitlane1 = 1;
      break;
    case 62:  // pit lane 1 14%
      state.pitlane1 = 2;
      break;
    case 73:  // pit lane 1 21%
      state.pitlane1 = 3;
      break;
    case 74:  // pit lane 1 29%
      state.pitlane1 = 4;
      break;
    case 75:  // pit lane 1 36%
      state.pitlane1 = 5;
      break;
    case 76:  // pit lane 1 43%
      state.pitlane1 = 6;
      break;
    case 77:  // pit lane 1 50%
      state.pitlane1 = 7;
      break;
    case 78:  // pit lane 1 57%
      state.pitlane1 = 8;
      break;
    case 79:  // pit lane 1 64%
      state.pitlane1 = 9;
      break;
    case 81:  // pit lane 1 71%
      state.pitlane1 = 10;
      break;
    case 82:  // pit lane 1 79%
      state.pitlane1 = 11;
      break;
    case 83:  // pit lane 1 86%
      state.pitlane1 = 12;
      break;
    case 84:  // pit lane 1 93%
      state.pitlane1 = 13;
      break;
    case 85:  // pit lane 1 100%
      state.pitlane1 = 14;
      break;
    case 89:  // pit lane 2 off
      state.pitlane2 = -1;
      break;
    case 90:  // pit lane 2 0%
      state.pitlane2 = 0;
      break;
    case 91:  // pit lane 2 7%
      state.pitlane2 = 1;
      break;
    case 92:  // pit lane 2 14%
      state.pitlane2 = 2;
      break;
    case 93:  // pit lane 2 21%
      state.pitlane2 = 3;
      break;
    case 94:  // pit lane 2 29%
      state.pitlane2 = 4;
      break;
    case 95:  // pit lane 2 36%
      state.pitlane2 = 5;
      break;
    case 97:  // pit lane 2 43%
      state.pitlane2 = 6;
      break;
    case 98:  // pit lane 2 50%
      state.pitlane2 = 7;
      break;
    case 99:  // pit lane 2 57%
      state.pitlane2 = 8;
      break;
    case 100:  // pit lane 2 64%
      state.pitlane2 = 9;
      break;
    case 101:  // pit lane 2 71%
      state.pitlane2 = 10;
      break;
    case 102:  // pit lane 2 79%
      state.pitlane2 = 11;
      break;
    case 103:  // pit lane 2 86%
      state.pitlane2 = 12;
      break;
    case 105:  // pit lane 2 93%
      state.pitlane2 = 13;
      break;
    case 106:  // pit lane 2 100%
      state.pitlane2 = 14;
      break;
    case 108: // chaos on
      state.race.chaos = true;
      break;
    case 109: // chaos off
      state.race.chaos = false;
      break;
    case 110:  // race state set to running
      state.race.isInProgress = true;
      break;
    case 111:  // race state set to not running
      state.race.isInProgress = false;
      break;
    case 113:  // race progress 0%
      state.race.progress = 0;
      break;
    case 114:  // race progress 8%
      state.race.progress = 1;
      break;
    case 115:  // race progress 17%
      state.race.progress = 2;
      break;
    case 116:  // race progress 25%
      state.race.progress = 3;
      break;
    case 117:  // race progress 33%
      state.race.progress = 4;
      break;
    case 118:  // race progress 42%
      state.race.progress = 5;
      break;
    case 119:  // race progress 50%
      state.race.progress = 6;
      break;
    case 121:  // race progress 58%
      state.race.progress = 7;
      break;
    case 122:  // race progress 67%
      state.race.progress = 8;
      break;
    case 123:  // race progress 75%
      state.race.progress = 9;
      break;
    case 124:  // race progress 83%
      state.race.progress = 10;
      break;
    case 125:  // race progress 92%
      state.race.progress = 11;
      break;
    case 126:  // race progress 100%
      state.race.progress = 12;
      break;
    case 137:  // slot 1 needs to refuel
      state.slots.slot1.needsRefueling = true;
      break;
    case 138:  // slot 1 does not need to refuel anymore
      state.slots.slot1.needsRefueling = false;
      break;
    case 139:  // slot 2 needs to refuel
      state.slots.slot2.needsRefueling = true;
      break;
    case 140:  // slot 2 does not need to refuel anymore
      state.slots.slot2.needsRefueling = false;
      break;
    case 141:  // slot 3 needs to refuel
      state.slots.slot3.needsRefueling = true;
      break;
    case 142:  // slot 3 does not need to refuel anymore
      state.slots.slot3.needsRefueling = false;
      break;
    case 143:  // slot 4 needs to refuel
      state.slots.slot4.needsRefueling = true;
      break;
    case 145:  // slot 4 does not need to refuel anymore
      state.slots.slot4.needsRefueling = false;
      break;
    case 146:  // slot 5 needs to refuel
      state.slots.slot5.needsRefueling = true;
      break;
    case 147:  // slot 5 does not need to refuel anymore
      state.slots.slot5.needsRefueling = false;
      break;
    case 148:  // slot 6 needs to refuel
      state.slots.slot6.needsRefueling = true;
      break;
    case 149:  // slot 6 does not need to refuel anymore
      state.slots.slot6.needsRefueling = false;
      break;
    case 161:  // slot 1 starts refueling
      state.slots.slot1.isRefueling = true;
      break;
    case 162:  // slot 1 stops refueling
      state.slots.slot1.isRefueling = false;
      break;
    case 163:  // slot 2 starts refueling
      state.slots.slot2.isRefueling = true;
      break;
    case 164:  // slot 2 stops refueling
      state.slots.slot2.isRefueling = false;
      break;
    case 165:  // slot 3 starts refueling
      state.slots.slot3.isRefueling = true;
      break;
    case 166:  // slot 3 stops refueling
      state.slots.slot3.isRefueling = false;
      break;
    case 167:  // slot 4 starts refueling
      state.slots.slot4.isRefueling = true;
      break;
    case 169:  // slot 4 stops refueling
      state.slots.slot4.isRefueling = false;
      break;
    case 170:  // slot 5 starts refueling
      state.slots.slot5.isRefueling = true;
      break;
    case 171:  // slot 5 stops refueling
      state.slots.slot5.isRefueling = false;
      break;
    case 172:  // slot 6 starts refueling
      state.slots.slot6.isRefueling = true;
      break;
    case 173:  // slot 6 stops refueling
      state.slots.slot6.isRefueling = false;
      break;
    default:
      return 0;
  }

  return 1;
}

void updateOnStatusChange() {
  state.needsUpdate = false;

  updateStartLight();
  updatePitlanes();
  updateIsRefueling();
}

// --- Input Handling ---
void setupInputPins() {
  if (DATA_VIA_I2C) {
    mcp.pinMode(PIN_DATA_1, USE_PULLUP ? INPUT_PULLUP : INPUT);
    mcp.digitalWrite(PIN_DATA_1, LOW);
    mcp.pinMode(PIN_DATA_2, USE_PULLUP ? INPUT_PULLUP : INPUT);
    mcp.digitalWrite(PIN_DATA_2, LOW);
    mcp.pinMode(PIN_DATA_3, USE_PULLUP ? INPUT_PULLUP : INPUT);
    mcp.digitalWrite(PIN_DATA_3, LOW);
  } else {
    pinMode(PIN_DATA_1, USE_PULLUP ? INPUT_PULLUP : INPUT);
    digitalWrite(PIN_DATA_1, LOW);
    pinMode(PIN_DATA_2, USE_PULLUP ? INPUT_PULLUP : INPUT);
    digitalWrite(PIN_DATA_2, LOW);
    pinMode(PIN_DATA_3, USE_PULLUP ? INPUT_PULLUP : INPUT);
    digitalWrite(PIN_DATA_3, LOW);
  }

  pinMode(PIN_CLOCK, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_CLOCK), readInput, RISING);
}

void resetInputData() {
  input.chunks = 0;
  input.data = 0;
}

int calcRowValue(int val1, int val2, int val3) {
  return (val1 << 2) + (val2 << 1) + val3;
}

void execReadInput() {
  int value1 = (DATA_VIA_I2C ? mcp.digitalRead(PIN_DATA_1) : digitalRead(PIN_DATA_1)) == !USE_PULLUP;
  int value2 = (DATA_VIA_I2C ? mcp.digitalRead(PIN_DATA_2) : digitalRead(PIN_DATA_2)) == !USE_PULLUP;
  int value3 = (DATA_VIA_I2C ? mcp.digitalRead(PIN_DATA_3) : digitalRead(PIN_DATA_3)) == !USE_PULLUP;

  Serial.print("Interrupt ");
  Serial.print(value1);
  Serial.print(value2);
  Serial.println(value3);

  int value = calcRowValue(value1, value2, value3);

  if (value == 0) {
    Serial.println("Received reset signal");
    return resetInputData();
  }

  if (input.chunks >= 3) {
    //what?
    Serial.print("Error chunks int too big (");
    Serial.print(input.chunks);
    Serial.println("). Missing reset?");
    return resetInputData();
  }

  input.data += value << ((2 - input.chunks) * 3);
  Serial.print("received chunk ");
  Serial.print(input.chunks);
  Serial.print(", value (");
  Serial.print(value);
  Serial.print(" ");
  Serial.print(toBinaryString(value, 3));
  Serial.print("), data (");
  Serial.print(input.data);
  Serial.print(toBinaryString(input.data, 9));
  Serial.println(")");
  input.chunks++;

  if (input.chunks != 3) {
    // still incomplete. wait for complete data...
    return;
  }

  if (!hasEvenParity(input.data) || input.data >= 512) {
    Serial.print("Received data is invalid (");
    Serial.print(input.data);
    Serial.print(" ");
    Serial.print(toBinaryString(input.data, 9));
    Serial.println(")");
    // oooh something was wrong. better luck next time...
    return resetInputData();
  }

  unsigned int dataWithoutParityBit = clearBit(input.data, 8);
  Serial.print("Received data ");
  Serial.print(dataWithoutParityBit);
  Serial.print(" ");
  Serial.println(toBinaryString(dataWithoutParityBit, 8));
  bool updated = updateState(dataWithoutParityBit);
  printState();

  if (updated) {
    state.needsUpdate = true;
  }
}

#if ESP
void IRAM_ATTR readInput() {
  execReadInput();
}
#else

void readInput() {
  execReadInput();
}

#endif

void restoreState() {
  // todo: restore in case of power outage
  resetState();
  state.race.virtualSafetyCar = true;

  updateOnStatusChange();
}