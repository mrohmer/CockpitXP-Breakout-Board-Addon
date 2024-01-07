#define PIN_CLOCK 5
#define PIN_DATA_1 12
#define PIN_DATA_2 3
#define PIN_DATA_3 4
#define PIN_LED 2
#define USE_PULLUP true
#define PIN_START_LIGHT_CS 15
#define PIN_START_LIGHT_HORIZONTAL 5
#define PIN_START_LIGHT_VERTICAL 1

#define MS_CYCLE 100
#define MS_BETWEEN_STATUS_LOG 5000
#define MS_BETWEEN_LED_TOGGLE 1000
#define MS_BETWEEN_FALSE_START_TOGGLE 200

#include <Arduino.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <string.h>


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
struct StartLightState {
    bool falseStart;
    bool falseStartToggle;

    unsigned int state;
};
struct State {
    struct SlotState slot1;
    struct SlotState slot2;
    struct SlotState slot3;
    struct SlotState slot4;
    struct SlotState slot5;
    struct SlotState slot6;

    unsigned int pitlane1;
    unsigned int pitlane2;

    struct StartLightState startLight;

    bool virtualSafetyCar;
    bool newTrackRecord;
    bool newSessionRecord;
};
struct LastExecutionState {
    unsigned long log;
    unsigned long ledToggle;
    unsigned long falseStartToggle;
};

// --- Variables ---
struct Input input;
struct State state;
struct LastExecutionState lastExecution;
Max72xxPanel matrix = Max72xxPanel(PIN_START_LIGHT_CS, PIN_START_LIGHT_HORIZONTAL, PIN_START_LIGHT_VERTICAL);

// --- Declarations ---
void IRAM_ATTR

readInput();

void setupInputPins();

void setupStatusLed();

void cyclePrintStatusLog();

void cycleToggleStatusLed();

void cycleToggleStartLightFalseStart();

void toggleStatusLed();

void flashStatusLed(int n);

void toggleStartLightFalseStart();

// --- Arduino Loop ---
void setup() {
  Serial.begin(9600);

  setupStatusLed();
  setupInputPins();
  setupStartLight();

  Serial.println("Setup Done.");
  flashStatusLed(10);
}

void loop() {
  cyclePrintStatusLog();
  cycleToggleStatusLed();
  cycleToggleStartLightFalseStart();
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

// --- State Update ---
void printState() {
  Serial.println("State:");
  Serial.printf("Pitlane 1: %d | Pitlane 2: %d\n", state.pitlane1, state.pitlane2);
  Serial.printf("Slot1: %s | %s\n", state.slot1.isRefueling ? "isRefueling" : "-",
                state.slot1.needsRefueling ? "needs to refuel" : "-");
  Serial.printf("Slot2: %s | %s\n", state.slot2.isRefueling ? "isRefueling" : "-",
                state.slot2.needsRefueling ? "needs to refuel" : "-");
  Serial.printf("Slot3: %s | %s\n", state.slot3.isRefueling ? "isRefueling" : "-",
                state.slot3.needsRefueling ? "needs to refuel" : "-");
  Serial.printf("Slot4: %s | %s\n", state.slot4.isRefueling ? "isRefueling" : "-",
                state.slot4.needsRefueling ? "needs to refuel" : "-");
  Serial.printf("Slot5: %s | %s\n", state.slot5.isRefueling ? "isRefueling" : "-",
                state.slot5.needsRefueling ? "needs to refuel" : "-");
  Serial.printf("Slot6: %s | %s\n", state.slot6.isRefueling ? "isRefueling" : "-",
                state.slot6.needsRefueling ? "needs to refuel" : "-");
  Serial.printf("Start light: %d | %s\n", state.startLight.state, state.startLight.falseStart ? "false start" : "-");
  Serial.printf("Virtual Safety Car: %s\n", state.virtualSafetyCar ? "on" : "off");
  Serial.printf("Track Record: %s\n", state.newTrackRecord ? "on" : "off");
  Serial.printf("Session Record: %s\n", state.newSessionRecord ? "on" : "off");
}

void resetState() {
  state = {
          .slot1 = {.isRefueling = false, .needsRefueling = false},
          .slot2 = {.isRefueling = false, .needsRefueling = false},
          .slot3 = {.isRefueling = false, .needsRefueling = false},
          .slot4 = {.isRefueling = false, .needsRefueling = false},
          .slot5 = {.isRefueling = false, .needsRefueling = false},
          .slot6 = {.isRefueling = false, .needsRefueling = false},
          .pitlane1 = 0,
          .pitlane2 = 0,
          .startLight = {.falseStart = false, .falseStartToggle = false, .state = 0},

          .virtualSafetyCar = false,
          .newTrackRecord = false,
          .newSessionRecord = false,
  };
}

bool updateState(unsigned int event) {
  switch (event) {
    case 11:
      resetState();
      break;
    case 21:  // virtual safety car on
      state.virtualSafetyCar = true;
      break;
    case 22:  // virtual safety car off
      state.virtualSafetyCar = false;
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
    case 49:  // new track record
      state.newTrackRecord = true;
      break;
    case 50:  // new session record
      state.newSessionRecord = true;
      break;
    case 55:  // pit lane 1 off
      state.pitlane1 = 0;
      break;
    case 59:  // pit lane 1 10%
      state.pitlane1 = 10;
      break;
    case 61:  // pit lane 1 20%
      state.pitlane1 = 20;
      break;
    case 62:  // pit lane 1 30%
      state.pitlane1 = 30;
      break;
    case 73:  // pit lane 1 40%
      state.pitlane1 = 40;
      break;
    case 74:  // pit lane 1 50%
      state.pitlane1 = 50;
      break;
    case 75:  // pit lane 1 60%
      state.pitlane1 = 60;
      break;
    case 76:  // pit lane 1 70%
      state.pitlane1 = 70;
      break;
    case 77:  // pit lane 1 80%
      state.pitlane1 = 80;
      break;
    case 78:  // pit lane 1 90%
      state.pitlane1 = 90;
      break;
    case 79:  // pit lane 1 100%
      state.pitlane1 = 100;
      break;
    case 81:  // pit lane 2 off
      state.pitlane2 = 0;
      break;
    case 82:  // pit lane 2 10%
      state.pitlane2 = 10;
      break;
    case 83:  // pit lane 2 20%
      state.pitlane2 = 20;
      break;
    case 84:  // pit lane 2 30%
      state.pitlane2 = 30;
      break;
    case 85:  // pit lane 2 40%
      state.pitlane2 = 40;
      break;
    case 86:  // pit lane 2 50%
      state.pitlane2 = 50;
      break;
    case 87:  // pit lane 2 60%
      state.pitlane2 = 60;
      break;
    case 89:  // pit lane 2 70%
      state.pitlane2 = 70;
      break;
    case 90:  // pit lane 2 80%
      state.pitlane2 = 80;
      break;
    case 91:  // pit lane 2 90%
      state.pitlane2 = 90;
      break;
    case 92:  // pit lane 2 100%
      state.pitlane2 = 100;
      break;
    case 97:  // slot 1 needs to refuel
      state.slot1.needsRefueling = true;
      break;
    case 98:  // slot 1 does not need to refuel anymore
      state.slot1.needsRefueling = false;
      break;
    case 99:  // slot 1 starts refueling
      state.slot1.isRefueling = true;
      break;
    case 100:  // slot 1 stops refueling
      state.slot1.isRefueling = false;
      break;
    case 105:  // slot 2 needs to refuel
      state.slot2.needsRefueling = true;
      break;
    case 106:  // slot 2 does not need to refuel anymore
      state.slot2.needsRefueling = false;
      break;
    case 107:  // slot 2 starts refueling
      state.slot2.isRefueling = true;
      break;
    case 108:  // slot 2 stops refueling
      state.slot2.isRefueling = false;
      break;
    case 113:  // slot 3 needs to refuel
      state.slot3.needsRefueling = true;
      break;
    case 114:  // slot 3 does not need to refuel anymore
      state.slot3.needsRefueling = false;
      break;
    case 115:  // slot 3 starts refueling
      state.slot3.isRefueling = true;
      break;
    case 116:  // slot 3 stops refueling
      state.slot3.isRefueling = false;
      break;
    case 121:  // slot 4 needs to refuel
      state.slot4.needsRefueling = true;
      break;
    case 122:  // slot 4 does not need to refuel anymore
      state.slot4.needsRefueling = false;
      break;
    case 123:  // slot 4 starts refueling
      state.slot4.isRefueling = true;
      break;
    case 124:  // slot 4 stops refueling
      state.slot4.isRefueling = false;
      break;
    case 137:  // slot 5 needs to refuel
      state.slot5.needsRefueling = true;
      break;
    case 138:  // slot 5 does not need to refuel anymore
      state.slot5.needsRefueling = false;
      break;
    case 139:  // slot 5 starts refueling
      state.slot5.isRefueling = true;
      break;
    case 140:  // slot 5 stops refueling
      state.slot5.isRefueling = false;
      break;
    case 145:  // slot 6 needs to refuel
      state.slot6.needsRefueling = true;
      break;
    case 146:  // slot 6 does not need to refuel anymore
      state.slot6.needsRefueling = false;
      break;
    case 147:  // slot 6 starts refueling
      state.slot6.isRefueling = true;
      break;
    case 148:  // slot 6 stops refueling
      state.slot6.isRefueling = false;
      break;
    default:
      return 0;
  }

  return 1;
}

void updateOnStatusChange() {
  updateStartLight();
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
    updateOnStatusChange();
  }
}