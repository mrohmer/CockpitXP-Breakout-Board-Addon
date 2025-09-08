//
// Created by kali on 9/8/25.
//

#ifndef OTA_H
#define OTA_H

#include <ElegantOTA.h>

#define WEBSERVER_H
#include <ESPAsyncWebServer.h>

typedef std::function<void(float)> OTAProgressCallback;
typedef std::function<void()> OTADoneCallback;

class OTA {
public:
    OTA(AsyncWebServer* server);
    void loop();
    void onUpdateProgress(OTAProgressCallback callback);
    void onUpdateDone(OTADoneCallback callback);
};



#endif //OTA_H
