//
// Created by kali on 9/8/25.
//

#ifndef SERVER_H
#define SERVER_H

#include <WiFi.h>
#include <AsyncTCP.h>


#include <ElegantOTA.h>

#define WEBSERVER_H
#include <ESPAsyncWebServer.h>
#include "OTA.h"
#include <WebSerial.h>

class HttpServer {
private:
    AsyncWebServer* server = new AsyncWebServer(80);
    OTA* ota;
public:
	void init();
	void loop();
    void onUpdateProgress(OTAProgressCallback callback);
    void onUpdateDone(OTADoneCallback callback);
};



#endif //SERVER_H
