//
// Created by kali on 9/8/25.
//

#include "OTA.h"

OTA::OTA(AsyncWebServer* server) {
    ElegantOTA.begin(server);
}
void OTA::loop() {
    ElegantOTA.loop();
}
void OTA::onUpdateProgress(OTAProgressCallback callback) {
    ElegantOTA.onStart([=]() {
        callback(0);
    });
    ElegantOTA.onProgress([=](size_t current, size_t final) {
      callback(static_cast< float >(current) / static_cast< float >(final));
    });
}
void OTA::onUpdateDone(OTADoneCallback callback) {
    ElegantOTA.onEnd([=](bool success) {
      if (success) {
        callback();
      }
    });
}