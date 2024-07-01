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

#define MS_CYCLE 250
#define MS_BETWEEN_LED_TOGGLE 1000
#define MS_BETWEEN_FLAGS_GREEN 500
#define MS_BETWEEN_FLAGS_CHAOS 250
#define MS_BETWEEN_FLAGS_RED 1000

#include <Arduino.h>
#include <string.h>
#include <math.h>
#include <Adafruit_NeoPixel.h>

#if IS_ESP
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "ArduinoJson.h"
#endif

struct LastExecutionState {
    unsigned long ledToggle;
    unsigned long flagsGreen;
    unsigned long flagsChaos;
#if IS_ESP
    unsigned long demo;
#endif
};
struct FlagsState {
    int state;
    int toggles;
};
#if IS_ESP
struct DemoState {
    bool active;
    int magicNumber;
};
struct DemoState demoState;
#endif

Adafruit_NeoPixel pitlane = Adafruit_NeoPixel(PITLANE_NUMPIXELS, PIN_PITLANE, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel flags = Adafruit_NeoPixel(FLAGS_NUMPIXELS, PIN_FLAGS, NEO_GRB + NEO_KHZ800);

struct LastExecutionState lastExecution;
struct FlagsState flagsState;

// --- Declarations ---
#if IS_ESP
void IRAM_ATTR readPitlanePins();
void IRAM_ATTR readFlagPins();
void setupServer();
void setupDemo();
void cycleDemo();
#else

void readPitlanePins();

void readFlagPins();

#endif

void setupInputPins();

void setupStatusLed();

void setupPitlane();

void setupFlags();

void cycleToggleStatusLed();

void cycleUpdateFlagsGreen();

void cycleUpdateFlagsChaos();

void cycleUpdateFlagsRed();

void toggleStatusLed();

void flashStatusLed(int n);

void updateFlagsGreen();

void updateFlagsChaos();

void updateFlagsRed();

// --- Arduino Loop ---
void setup() {
  Serial.begin(9600);

  setupStatusLed();
  setupInputPins();
  setupPitlane();
  setupFlags();

#if IS_ESP
  setupDemo();
  setupServer();
#endif

  Serial.println("Setup Done.");
  flashStatusLed(10);
}

void loop() {
  cycleToggleStatusLed();
  cycleUpdateFlagsGreen();
  cycleUpdateFlagsChaos();
  cycleUpdateFlagsRed();

#if IS_ESP
  cycleDemo();
#endif

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
  if (shouldExecuteInThisCycle(MS_BETWEEN_FLAGS_CHAOS, lastExecution.flagsChaos)) {
    updateFlagsRed();
    lastExecution.flagsChaos = millis();
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

void setFlagsGreen() {
  flagsState = {
          .state = FLAGS_STATE_GREEN,
          .toggles = 0
  };
}

void updateFlagsGreen() {
  if (flagsState.state != FLAGS_STATE_GREEN || flagsState.toggles > (5 * 2 - 1)) {
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

void updateFlagsChaos() {
  if (flagsState.state != FLAGS_STATE_CHAOS) {
    return;
  }

  int shift = flagsState.toggles % 2;
  flags.clear();

  for (int flag = 0; flag < FLAGS_NUM; flag++) {
    for (int pixel = shift; pixel < FLAGS_PIXELS_PER_FLAG; pixel += 2) {
      int index = flag * FLAGS_PIXELS_PER_FLAG + pixel;

      flags.setPixelColor(index, flags.Color(255, 255, 0));
    }
  }

  flags.show();

  flagsState.toggles = (flagsState.toggles + 1) % 2;
}

// --- inputs ---
void setupInputPins() {
  return; // todo fix
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
  }
}

#if IS_ESP
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


#if IS_ESP
void setupDemo() {
  demoState = {
          .active = true,
          .magicNumber = -1
  };
}
void startDemo() {
  demoState = {
          .active = true,
          .magicNumber = -1
  };
}
void stopDemo() {
  demoState = {
          .active = false,
          .magicNumber = -1
  };

  setFlagsRed();
  updatePitlanes(false, false);

  execReadPitlanePins();
  execReadFlagPins();
}
void updateDemoFlags() {
  switch(demoState.magicNumber % 9) {
    case 0:
      Serial.println("Red Flags.");
      setFlagsRed();
      break;
    case 1:
      Serial.println("Green Flags.");
      setFlagsGreen();
      break;
    case 4:
      Serial.println("Chaos Flags.");
      setFlagsChaos();
      break;
    case 7:
      Serial.println("Green Flags.");
      setFlagsGreen();
      break;
  }
}
void updateDemoPitlanes() {
  switch(demoState.magicNumber % 13) {
    case 0:
      Serial.println("Pitlane [false, false].");
      updatePitlanes(false, false);
      break;
    case 1:
      Serial.println("Pitlane [true, false].");
      updatePitlanes(true, false);
      break;
    case 2:
      Serial.println("Pitlane [false, false].");
      updatePitlanes(false, false);
      break;
    case 3:
      Serial.println("Pitlane [false, true].");
      updatePitlanes(false, true);
      break;
    case 4:
      Serial.println("Pitlane [false, false].");
      updatePitlanes(false, false);
      break;
    case 5:
      Serial.println("Pitlane [false, true].");
      updatePitlanes(false, true);
      break;
    case 6:
      Serial.println("Pitlane [true, true].");
      updatePitlanes(true, true);
      break;
    case 7:
      Serial.println("Pitlane [true, false].");
      updatePitlanes(true, false);
      break;
    case 8:
      Serial.println("Pitlane [false, false].");
      updatePitlanes(false, false);
      break;
    case 9:
      Serial.println("Pitlane [true, false].");
      updatePitlanes(true, false);
      break;
    case 10:
      Serial.println("Pitlane [true, true].");
      updatePitlanes(true, true);
      break;
    case 11:
      Serial.println("Pitlane [false, true].");
      updatePitlanes(false, true);
      break;
  }
}
void updateDemo() {
  if (!demoState.active) {
    return;
  }
  demoState.magicNumber = (demoState.magicNumber + 1) % 10000;

  updateDemoFlags();
  updateDemoPitlanes();
}
void cycleDemo() {
  if (shouldExecuteInThisCycle(2500, lastExecution.demo)) {
    updateDemo();

    IPAddress ip = WiFi.softAPIP();
    Serial.print("IP address: ");
    Serial.println(ip);

    lastExecution.demo = millis();
  }
}

AsyncWebServer server(80);

const char* hostname = "cma";
const char* password = "cmacmacma";

class StaticAppRewrite : public AsyncWebRewrite
{
  public:
  StaticAppRewrite(const char* from)
    : AsyncWebRewrite(from, "/") {
  }

  bool match(AsyncWebServerRequest *request) override {
    if (!request->url().startsWith(from())) {
      return false;
    }

    _toUrl = request->url();
    _toUrl.replace(from(), "");
    return true;
  }
};

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}
void setupServer() {
  Serial.setDebugOutput(true);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(hostname, password);

  IPAddress ip = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(ip);

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  SPIFFS.begin();

  server.on("/api/demo", HTTP_GET, [](AsyncWebServerRequest *request) {
    StaticJsonDocument<100> data;

    data["active"] = demoState.active;

    String response;
    serializeJson(data, response);
    request->send(200, "application/json", response);
  });
  server.on("/api/demo/activate", HTTP_POST, [](AsyncWebServerRequest *request) {
    startDemo();

    StaticJsonDocument<100> data;

    data["active"] = demoState.active;

    String response;
    serializeJson(data, response);
    request->send(200, "application/json", response);
  });
  server.on("/api/demo/deactivate", HTTP_POST, [](AsyncWebServerRequest *request) {
    stopDemo();

    StaticJsonDocument<100> data;

    data["active"] = demoState.active;

    String response;
    serializeJson(data, response);
    request->send(200, "application/json", response);
  });

  server.addRewrite( new StaticAppRewrite("/_app/immutable") );
  server.addRewrite( new StaticAppRewrite("/_app") );

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  server.begin();
}
#endif