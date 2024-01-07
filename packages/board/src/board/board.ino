#define PIN_CLOCK 5
#define PIN_DATA_1 12
#define PIN_DATA_2 13
#define PIN_DATA_3 14
#define PIN_LED 2
#define USE_PULLUP true

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

// --- Variables ---
struct Input input;
struct State state;

unsigned int cycle = 0;

// --- Declarations ---
void ICACHE_RAM_ATTR readInput();
void setupInputPins();
void setupStatusLed();
void toggleStatusLed();
void flashStatusLed(int n);

// --- Arduino Loop ---
void setup() {
  Serial.begin(9600);

  setupStatusLed();
  setupInputPins();

  Serial.println("Setup Done.");
  flashStatusLed(10);
}

void loop() {
  cycle == 0 && Serial.println("Waiting for inputs...");
  delay(1000);
  toggleStatusLed();

  cycle = (cycle + 1) % 10;
}

// --- UTILS ---
char *toBinaryString(int n, int numberOfBits) {
  char *string = (char *)malloc(numberOfBits + 1);
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
  int length = sizeof(str) / sizeof(str[0]);
  for (int i = 0; i < length; i++) {
    char compLetter = str[i];
    if (strcmp(&compLetter, "1")) {
      count++;
    }
  }
  return count % 2 == 0;
}
unsigned int clearBit(unsigned int number, unsigned int n) {
  return number & ~((unsigned int)1 << n);
}

// --- State Update ---
void printState() {
  Serial.println("State:");
  Serial.printf("Pitlane 1: %d | Pitlane 2: %d\n", state.pitlane1, state.pitlane2);
  Serial.printf("Slot1: %s | %s\n", state.slot1.isRefueling ? "isRefueling" : "-", state.slot1.needsRefueling ? "needs to refuel" : "-");
  Serial.printf("Slot2: %s | %s\n", state.slot2.isRefueling ? "isRefueling" : "-", state.slot2.needsRefueling ? "needs to refuel" : "-");
  Serial.printf("Slot3: %s | %s\n", state.slot3.isRefueling ? "isRefueling" : "-", state.slot3.needsRefueling ? "needs to refuel" : "-");
  Serial.printf("Slot4: %s | %s\n", state.slot4.isRefueling ? "isRefueling" : "-", state.slot4.needsRefueling ? "needs to refuel" : "-");
  Serial.printf("Slot5: %s | %s\n", state.slot5.isRefueling ? "isRefueling" : "-", state.slot5.needsRefueling ? "needs to refuel" : "-");
  Serial.printf("Slot6: %s | %s\n", state.slot6.isRefueling ? "isRefueling" : "-", state.slot6.needsRefueling ? "needs to refuel" : "-");
  Serial.printf("Start light: %d | %s\n", state.startLight.falseStart, state.startLight.falseStart ? "false start" : "-");
  Serial.printf("Virtual Safety Car: %s\n", state.virtualSafetyCar ? "on" : "off");
  Serial.printf("Track Record: %s\n", state.newTrackRecord ? "on" : "off");
  Serial.printf("Session Record: %s\n", state.newSessionRecord ? "on" : "off");
}
void resetState() {
  state = {
    .slot1 = { .isRefueling = 0, .needsRefueling = 0 },
    .slot2 = { .isRefueling = 0, .needsRefueling = 0 },
    .slot3 = { .isRefueling = 0, .needsRefueling = 0 },
    .slot4 = { .isRefueling = 0, .needsRefueling = 0 },
    .slot5 = { .isRefueling = 0, .needsRefueling = 0 },
    .slot6 = { .isRefueling = 0, .needsRefueling = 0 },
    .pitlane1 = 0,
    .pitlane2 = 0,
    .startLight = { .falseStart = 0, .state = 0 },

    .virtualSafetyCar = 0,
    .newTrackRecord = 0,
    .newSessionRecord = 0,
  };
}
void updateState(unsigned int event) {
  switch (event) {
    case 11:
      resetState();
      break;
    case 21:  // virtual safety car on
      state.virtualSafetyCar = 1;
      break;
    case 22:  // virtual safety car off
      state.virtualSafetyCar = 0;
      break;
    case 28:  // starting light off
      state.startLight.state = 0;
      state.startLight.falseStart = 0;
      break;
    case 31:  // starting light 1
      state.startLight.state = 1;
      state.startLight.falseStart = 0;
      break;
    case 35:  // starting light 2
      state.startLight.state = 2;
      state.startLight.falseStart = 0;
      break;
    case 37:  // starting light 3
      state.startLight.state = 3;
      state.startLight.falseStart = 0;
      break;
    case 38:  // starting light 4
      state.startLight.state = 4;
      state.startLight.falseStart = 0;
      break;
    case 41:  // starting light 5
      state.startLight.state = 5;
      state.startLight.falseStart = 0;
      break;
    case 42:  // starting light false start
      state.startLight.falseStart = 1;
      break;
    case 49:  // new track record
      state.newTrackRecord = 1;
      break;
    case 50:  // new session record
      state.newSessionRecord = 1;
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
      state.slot1.needsRefueling = 1;
      break;
    case 98:  // slot 1 does not need to refuel anymore
      state.slot1.needsRefueling = 0;
      break;
    case 99:  // slot 1 starts refueling
      state.slot1.isRefueling = 1;
      break;
    case 100:  // slot 1 stops refueling
      state.slot1.isRefueling = 0;
      break;
    case 105:  // slot 2 needs to refuel
      state.slot2.needsRefueling = 1;
      break;
    case 106:  // slot 2 does not need to refuel anymore
      state.slot2.needsRefueling = 0;
      break;
    case 107:  // slot 2 starts refueling
      state.slot2.isRefueling = 1;
      break;
    case 108:  // slot 2 stops refueling
      state.slot2.isRefueling = 0;
      break;
    case 113:  // slot 3 needs to refuel
      state.slot3.needsRefueling = 1;
      break;
    case 114:  // slot 3 does not need to refuel anymore
      state.slot3.needsRefueling = 0;
      break;
    case 115:  // slot 3 starts refueling
      state.slot3.isRefueling = 1;
      break;
    case 116:  // slot 3 stops refueling
      state.slot3.isRefueling = 0;
      break;
    case 121:  // slot 4 needs to refuel
      state.slot4.needsRefueling = 1;
      break;
    case 122:  // slot 4 does not need to refuel anymore
      state.slot4.needsRefueling = 0;
      break;
    case 123:  // slot 4 starts refueling
      state.slot4.isRefueling = 1;
      break;
    case 124:  // slot 4 stops refueling
      state.slot4.isRefueling = 0;
      break;
    case 137:  // slot 5 needs to refuel
      state.slot5.needsRefueling = 1;
      break;
    case 138:  // slot 5 does not need to refuel anymore
      state.slot5.needsRefueling = 0;
      break;
    case 139:  // slot 5 starts refueling
      state.slot5.isRefueling = 1;
      break;
    case 140:  // slot 5 stops refueling
      state.slot5.isRefueling = 0;
      break;
    case 145:  // slot 6 needs to refuel
      state.slot6.needsRefueling = 1;
      break;
    case 146:  // slot 6 does not need to refuel anymore
      state.slot6.needsRefueling = 0;
      break;
    case 147:  // slot 6 starts refueling
      state.slot6.isRefueling = 1;
      break;
    case 148:  // slot 6 stops refueling
      state.slot6.isRefueling = 0;
      break;
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
void ICACHE_RAM_ATTR readInput() {
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
  Serial.printf("received chunk %d, value (%d %x %s), data (%d %x %s)\n", input.chunks, value, value, toBinaryString(value, 3), input.data, input.data, toBinaryString(input.data, 9));
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
  updateState(dataWithoutParityBit);
  printState();
}