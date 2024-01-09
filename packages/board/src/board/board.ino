#define PIN_CLOCK 5
#define PIN_DATA_1 12
#define PIN_DATA_2 3
#define PIN_DATA_3 4
#define PIN_LED 2
#define USE_PULLUP true
#define PIN_START_LIGHT_CS 15
#define PIN_START_LIGHT_HORIZONTAL 5
#define PIN_START_LIGHT_VERTICAL 1
#define PIN_PITLANE_1_20 0 // VIA I2C
#define PIN_PITLANE_1_40 1 // VIA I2C
#define PIN_PITLANE_1_60 2 // VIA I2C
#define PIN_PITLANE_1_80 3 // VIA I2C
#define PIN_PITLANE_1_100 4 // VIA I2C
#define PIN_PITLANE_2_20 5 // VIA I2C
#define PIN_PITLANE_2_40 6 // VIA I2C
#define PIN_PITLANE_2_60 7 // VIA I2C
#define PIN_PITLANE_2_80 8 // VIA I2C
#define PIN_PITLANE_2_100 9 // VIA I2C
#define PIN_FUELING_SLOT_1 10 // VIA I2C
#define PIN_FUELING_SLOT_2 11 // VIA I2C
#define PIN_FUELING_SLOT_3 12 // VIA I2C
#define PIN_FUELING_SLOT_4 13 // VIA I2C
#define PIN_FUELING_SLOT_5 14 // VIA I2C
#define PIN_FUELING_SLOT_6 15 // VIA I2C
#define PIN_VIRTUAL_SAFETY_CAR 16 // VIA I2C

#define MS_CYCLE 100
#define MS_BETWEEN_STATUS_LOG 5000
#define MS_BETWEEN_LED_TOGGLE 1000
#define MS_BETWEEN_FALSE_START_TOGGLE 200
#define MS_BETWEEN_NEEDS_TO_REFUEL_TOGGLE 200
#define MS_BETWEEN_VIRTUAL_SAFETY_CAR_TOGGLE 150

#define HAS_I2C_CONNECTED false

#include <Arduino.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <string.h>
#include <Adafruit_MCP23X17.h>
#include <State.h>


// --- Structs ---
struct Input {  // Structure declaration
    unsigned int chunks;
    unsigned int data;
    unsigned int chunksData[3][3];
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
struct LastExecutionState lastExecution;
Max72xxPanel matrix = Max72xxPanel(PIN_START_LIGHT_CS, PIN_START_LIGHT_HORIZONTAL, PIN_START_LIGHT_VERTICAL);
Adafruit_MCP23X17 mcp;
State state = State();

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
  if (!HAS_I2C_CONNECTED) {
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
  if (state.startLight.state > 0) {
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
  if (!HAS_I2C_CONNECTED) {
    return;
  }

  mcp.pinMode(PIN_PITLANE_1_20, OUTPUT);
  mcp.pinMode(PIN_PITLANE_1_40, OUTPUT);
  mcp.pinMode(PIN_PITLANE_1_60, OUTPUT);
  mcp.pinMode(PIN_PITLANE_1_80, OUTPUT);
  mcp.pinMode(PIN_PITLANE_1_100, OUTPUT);
  mcp.pinMode(PIN_PITLANE_2_20, OUTPUT);
  mcp.pinMode(PIN_PITLANE_2_40, OUTPUT);
  mcp.pinMode(PIN_PITLANE_2_60, OUTPUT);
  mcp.pinMode(PIN_PITLANE_2_80, OUTPUT);
  mcp.pinMode(PIN_PITLANE_2_100, OUTPUT);
}

void updatePitlane(
        unsigned int value,
        unsigned int pin20,
        unsigned int pin40,
        unsigned int pin60,
        unsigned int pin80,
        unsigned int pin100
) {
  if (!HAS_I2C_CONNECTED) {
    return;
  }
  mcp.digitalWrite(pin20, value >= 15 ? HIGH : LOW);
  mcp.digitalWrite(pin40, value >= 35 ? HIGH : LOW);
  mcp.digitalWrite(pin60, value >= 55 ? HIGH : LOW);
  mcp.digitalWrite(pin80, value >= 75 ? HIGH : LOW);
  mcp.digitalWrite(pin100, value >= 95 ? HIGH : LOW);
}

void updatePitlanes() {
  updatePitlane(
          state.pitlane1,
          PIN_PITLANE_1_20,
          PIN_PITLANE_1_40,
          PIN_PITLANE_1_60,
          PIN_PITLANE_1_80,
          PIN_PITLANE_1_100
  );
  updatePitlane(
          state.pitlane2,
          PIN_PITLANE_2_20,
          PIN_PITLANE_2_40,
          PIN_PITLANE_2_60,
          PIN_PITLANE_2_80,
          PIN_PITLANE_2_100
  );
}

// --- Fueling ---
void setupFueling() {
  if (!HAS_I2C_CONNECTED) {
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
  if (!HAS_I2C_CONNECTED) {
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
  if (!HAS_I2C_CONNECTED) {
    return;
  }

  if (isRefueling) {
    // handled in interrupt
    return;
  }

  bool nextState = state.slots.lastNeedsToRefuelToggleState && needsToRefuel;

  if (prevState == nextState) {
    return;
  }

  mcp.digitalWrite(pin, nextState ? HIGH : LOW);
}

void toggleNeedsToRefuel() {
  bool prevState = state.slots.lastNeedsToRefuelToggleState;
  state.slots.lastNeedsToRefuelToggleState = !state.slots.lastNeedsToRefuelToggleState;
  toggleSlotNeedsToRefuel(state.slots.slot1.needsRefueling, state.slots.slot1.isRefueling, prevState, PIN_FUELING_SLOT_1);
  toggleSlotNeedsToRefuel(state.slots.slot2.needsRefueling, state.slots.slot2.isRefueling, prevState, PIN_FUELING_SLOT_2);
  toggleSlotNeedsToRefuel(state.slots.slot3.needsRefueling, state.slots.slot3.isRefueling, prevState, PIN_FUELING_SLOT_3);
  toggleSlotNeedsToRefuel(state.slots.slot4.needsRefueling, state.slots.slot4.isRefueling, prevState, PIN_FUELING_SLOT_4);
  toggleSlotNeedsToRefuel(state.slots.slot5.needsRefueling, state.slots.slot5.isRefueling, prevState, PIN_FUELING_SLOT_5);
  toggleSlotNeedsToRefuel(state.slots.slot6.needsRefueling, state.slots.slot6.isRefueling, prevState, PIN_FUELING_SLOT_6);
}

// --- Virtual Safety Car
void setupVirtualSafetyCar() {
  if (!HAS_I2C_CONNECTED) {
    return;
  }
  mcp.pinMode(PIN_VIRTUAL_SAFETY_CAR, OUTPUT);
}
void toggleVirtualSafetyCar() {
  if (!HAS_I2C_CONNECTED) {
    return;
  }

  bool prevState = state.virtualSafetyCar.lastToggleState;
  state.virtualSafetyCar.lastToggleState = !state.virtualSafetyCar.lastToggleState;
  bool nextState = state.virtualSafetyCar.state && state.virtualSafetyCar.lastToggleState;

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
}
void updateOnStatusChange() {
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
  bool updated = state.updateFromEvent(dataWithoutParityBit);
  printState();
  if (updated) {
    updateOnStatusChange();
  }
}

void restoreState() {
  // todo: restore in case of power outage
  state.reset();
  updateOnStatusChange();
}