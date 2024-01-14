#ifndef USE_PULLUP
#   define USE_PULLUP false
#endif
#ifndef OMIT_I2C_CONNECTION
#   define OMIT_I2C_CONNECTION false
#endif
#ifndef OMIT_PITLANE_CONNECTION
#   define OMIT_PITLANE_CONNECTION false
#endif

#define PIN_CLOCK 5
#define PIN_DATA_1 12
#define PIN_DATA_2 3
#define PIN_DATA_3 4
#define PIN_LED 2
#define PIN_START_LIGHT_CS 15
#define PIN_START_LIGHT_HORIZONTAL 5
#define PIN_START_LIGHT_VERTICAL 1
#define PIN_PITLANE 1
#define PIN_FUELING_SLOT_1 10 // VIA I2C
#define PIN_FUELING_SLOT_2 11 // VIA I2C
#define PIN_FUELING_SLOT_3 12 // VIA I2C
#define PIN_FUELING_SLOT_4 13 // VIA I2C
#define PIN_FUELING_SLOT_5 14 // VIA I2C
#define PIN_FUELING_SLOT_6 15 // VIA I2C
#define PIN_VIRTUAL_SAFETY_CAR 16 // VIA I2C

#define PITLANE_NUMPIXELS 32
#define PITLANE_1_FIRST_INDEX 0
#define PITLANE_2_FIRST_INDEX 16

#define MS_CYCLE 100
#define MS_BETWEEN_STATUS_LOG 5000
#define MS_BETWEEN_LED_TOGGLE 1000
#define MS_BETWEEN_FALSE_START_TOGGLE 200
#define MS_BETWEEN_NEEDS_TO_REFUEL_TOGGLE 200
#define MS_BETWEEN_VIRTUAL_SAFETY_CAR_TOGGLE 150

#include <Arduino.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <string.h>
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
struct VirtualSafetyCarState {
    bool state;

    bool lastToggleState;
};
struct State {
    struct SlotsState slots;

    unsigned int pitlane1;
    unsigned int pitlane2;

    struct StartLightState startLight;

    struct VirtualSafetyCarState virtualSafetyCar;
    bool newTrackRecord;
    bool newSessionRecord;
    bool raceIsInProgress;

    bool needsUpdate;
};
struct LastExecutionState {
    unsigned long log;
    unsigned long ledToggle;
    unsigned long falseStartToggle;
    unsigned long needsToRefuelToggle;
    unsigned long virtualSafetyCarToggle;
};

// --- Variables ---
struct Input input;
struct State state;
struct LastExecutionState lastExecution;
Max72xxPanel matrix = Max72xxPanel(PIN_START_LIGHT_CS, PIN_START_LIGHT_HORIZONTAL, PIN_START_LIGHT_VERTICAL);
Adafruit_MCP23X17 mcp;

Adafruit_NeoPixel pitlane = Adafruit_NeoPixel(PITLANE_NUMPIXELS, PIN_PITLANE, NEO_GRB + NEO_KHZ800);

// --- Declarations ---
void IRAM_ATTR

readInput();

void setupInputPins();

void setupStatusLed();

void setupI2C();

void setupPitlane();

void setupFueling();

void setupVirtualSafetyCar();

void cyclePrintStatusLog();

void cycleToggleStatusLed();

void cycleToggleStartLightFalseStart();

void cycleToggleNeedsToRefuel();

void cycleToggleVirtualSafetyCar();

void toggleStatusLed();

void flashStatusLed(int n);

void toggleStartLightFalseStart();

void toggleNeedsToRefuel();

void toggleVirtualSafetyCar();

void restoreState();

void updateOnStatusChange();

// --- Arduino Loop ---
void setup() {
  Serial.begin(9600);

  setupStartLight();
  setupI2C();
  setupPitlane();
  setupFueling();
  setupVirtualSafetyCar();
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
  cycleToggleVirtualSafetyCar();
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

void cycleToggleVirtualSafetyCar() {
  if (shouldExecuteInThisCycle(MS_BETWEEN_VIRTUAL_SAFETY_CAR_TOGGLE, lastExecution.virtualSafetyCarToggle)) {
    toggleVirtualSafetyCar();
    lastExecution.virtualSafetyCarToggle = millis();
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
    Serial.printf(" Display Start Light %d\n", state.startLight.state);
    fillStartLightFields(state.startLight.state);
  } else {
    Serial.println(" Clearing Start Light");
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
  pitlane.setBrightness(0);
  pitlane.show();
}


void setPitlaneActiveIndicatorPixel(int value, int firstIndex) {
  if (OMIT_PITLANE_CONNECTION) {
    return;
  }

  if (value < 0) {
    pitlane.setPixelColor(firstIndex, pitlane.Color(0, 0, 0));
    return;
  }


  pitlane.setPixelColor(firstIndex, pitlane.Color(255, 255, 255));
}

void updatePitlaneBar(int value, int firstIndex) {
  if (OMIT_PITLANE_CONNECTION) {
    return;
  }

  int index = firstIndex;
  pitlane.fill(pitlane.Color(255, 0, 0), index, min(3, value));
  if (value > 3) {
    pitlane.fill(pitlane.Color(255, 255, 0), index + 3, min(12, value) - 3);
  }
  if (value > 12) {
    pitlane.fill(pitlane.Color(0, 255, 0), index + 12, value - 12);
  }
}

void updatePitlane(int value, int firstIndex) {
  setPitlaneActiveIndicatorPixel(
          value,
          firstIndex
  );
  updatePitlaneBar(
          value,
          firstIndex
  );
}

void updatePitlanes() {
  if (OMIT_PITLANE_CONNECTION) {
    return;
  }

  pitlane.clear();
  updatePitlane(
          state.pitlane1,
          PITLANE_1_FIRST_INDEX
  );
  updatePitlane(
          state.pitlane2,
          PITLANE_2_FIRST_INDEX
  );

  pitlane.show();
}

// --- Fueling ---
void setupFueling() {
  if (OMIT_I2C_CONNECTION) {
    return;
  }
  mcp.pinMode(PIN_FUELING_SLOT_1, OUTPUT);
  mcp.pinMode(PIN_FUELING_SLOT_2, OUTPUT);
  mcp.pinMode(PIN_FUELING_SLOT_3, OUTPUT);
  mcp.pinMode(PIN_FUELING_SLOT_4, OUTPUT);
  mcp.pinMode(PIN_FUELING_SLOT_5, OUTPUT);
  mcp.pinMode(PIN_FUELING_SLOT_6, OUTPUT);
}

void updateSlotIsRefueling(bool isRefueling, unsigned int pin) {
  if (OMIT_I2C_CONNECTION) {
    return;
  }
  mcp.digitalWrite(pin, isRefueling ? HIGH : LOW);
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

  bool nextState = state.slots.lastNeedsToRefuelToggleState && needsToRefuel && state.raceIsInProgress;

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

// --- Virtual Safety Car
void setupVirtualSafetyCar() {
  if (OMIT_I2C_CONNECTION) {
    return;
  }
  mcp.pinMode(PIN_VIRTUAL_SAFETY_CAR, OUTPUT);
}

void toggleVirtualSafetyCar() {
  if (OMIT_I2C_CONNECTION) {
    return;
  }

  bool prevState = state.virtualSafetyCar.lastToggleState;
  state.virtualSafetyCar.lastToggleState = !state.virtualSafetyCar.lastToggleState;
  bool nextState = state.virtualSafetyCar.state && state.virtualSafetyCar.lastToggleState && state.raceIsInProgress;

  if (nextState == prevState) {
    return;
  }

  mcp.digitalWrite(PIN_VIRTUAL_SAFETY_CAR, nextState ? HIGH : LOW);
  state.virtualSafetyCar.lastToggleState = nextState;
}

// --- State Update ---
void printState() {
  Serial.println("State:");
  Serial.printf("Pitlane 1: %d | Pitlane 2: %d\n", state.pitlane1, state.pitlane2);
  Serial.printf("Slot1: %s | %s\n", state.slots.slot1.isRefueling ? "isRefueling" : "-",
                state.slots.slot1.needsRefueling ? "needs to refuel" : "-");
  Serial.printf("Slot2: %s | %s\n", state.slots.slot2.isRefueling ? "isRefueling" : "-",
                state.slots.slot2.needsRefueling ? "needs to refuel" : "-");
  Serial.printf("Slot3: %s | %s\n", state.slots.slot3.isRefueling ? "isRefueling" : "-",
                state.slots.slot3.needsRefueling ? "needs to refuel" : "-");
  Serial.printf("Slot4: %s | %s\n", state.slots.slot4.isRefueling ? "isRefueling" : "-",
                state.slots.slot4.needsRefueling ? "needs to refuel" : "-");
  Serial.printf("Slot5: %s | %s\n", state.slots.slot5.isRefueling ? "isRefueling" : "-",
                state.slots.slot5.needsRefueling ? "needs to refuel" : "-");
  Serial.printf("Slot6: %s | %s\n", state.slots.slot6.isRefueling ? "isRefueling" : "-",
                state.slots.slot6.needsRefueling ? "needs to refuel" : "-");
  Serial.printf("Start light: %d | %s\n", state.startLight.state, state.startLight.falseStart ? "false start" : "-");
  Serial.printf("Virtual Safety Car: %s\n", state.virtualSafetyCar.state ? "on" : "off");
  Serial.printf("Track Record: %s\n", state.newTrackRecord ? "on" : "off");
  Serial.printf("Session Record: %s\n", state.newSessionRecord ? "on" : "off");
  Serial.printf("Race State: %s\n", state.raceIsInProgress ? "Running" : "Stopped");
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
          .pitlane1 = 0,
          .pitlane2 = 0,
          .startLight = {.falseStart = false, .falseStartToggle = false, .state = 0},

          .virtualSafetyCar = {.state = false, .lastToggleState = false},
          .newTrackRecord = false,
          .newSessionRecord = false,
          .raceIsInProgress = false
          .needsUpdate = true,
  };
}

bool updateState(unsigned int event) {
  switch (event) {
    case 11:
      resetState();
      break;
    case 21:  // virtual safety car on
      state.virtualSafetyCar.state = true;
      break;
    case 22:  // virtual safety car off
      state.virtualSafetyCar.state = false;
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
    case 106:  // race state set to running
      state.raceIsInProgress = true;
      break;
    case 106:  // race state set to not running
      state.raceIsInProgress = false;
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
  pinMode(PIN_DATA_1, USE_PULLUP ? INPUT_PULLUP : INPUT);
  digitalWrite(PIN_DATA_1, LOW);
  pinMode(PIN_DATA_2, USE_PULLUP ? INPUT_PULLUP : INPUT);
  digitalWrite(PIN_DATA_2, LOW);
  pinMode(PIN_DATA_3, USE_PULLUP ? INPUT_PULLUP : INPUT);
  digitalWrite(PIN_DATA_3, LOW);
  pinMode(PIN_CLOCK, USE_PULLUP ? INPUT_PULLUP : INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_CLOCK), readInput, RISING);
}

void resetInputData() {
  input.chunks = 0;
  input.data = 0;
}

int calcRowValue(int val1, int val2, int val3) {
  return (val1 << 2) + (val2 << 1) + val3;
}

void IRAM_ATTR

readInput() {
  int value1 = digitalRead(PIN_DATA_1) == !USE_PULLUP;
  int value2 = digitalRead(PIN_DATA_2) == !USE_PULLUP;
  int value3 = digitalRead(PIN_DATA_3) == !USE_PULLUP;

  Serial.printf("Interrupt %d%d%d\n", value1, value2, value3);
  int value = calcRowValue(value1, value2, value3);

  if (value == 0) {
    Serial.printf("Received reset signal\n");
    return resetInputData();
  }

  if (input.chunks >= 3) {
    //what?
    Serial.printf("Error chunks int too big (%d). Missing reset?\n", input.chunks);
    return resetInputData();
  }

  input.data += value << ((2 - input.chunks) * 3);
  Serial.printf("received chunk %d, value (%d %x %s), data (%d %x %s)\n", input.chunks, value, value,
                toBinaryString(value, 3), input.data, input.data, toBinaryString(input.data, 9));
  input.chunks++;

  if (input.chunks != 3) {
    // still incomplete. wait for complete data...
    return;
  }

  if (!hasEvenParity(input.data) || input.data >= 512) {
    Serial.printf("Received data is invalid (%x %d %s)\n", input.data, input.data, toBinaryString(input.data, 9));
    // oooh something was wrong. better luck next time...
    return resetInputData();
  }

  unsigned int dataWithoutParityBit = clearBit(input.data, 8);
  Serial.printf("Received data %d %s\n", dataWithoutParityBit, toBinaryString(dataWithoutParityBit, 8));
  bool updated = updateState(dataWithoutParityBit);
  printState();

  if (updated) {
    state.needsUpdate = true;
  }
}

void restoreState() {
  // todo: restore in case of power outage
  resetState();
  updateOnStatusChange();
}