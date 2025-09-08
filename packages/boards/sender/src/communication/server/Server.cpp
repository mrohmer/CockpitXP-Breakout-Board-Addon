//
// Created by kali on 9/8/25.
//

#include "Server.h"

void HttpServer::init() {
    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        String html = F(
            "<html><head><title>");
        html += WiFi.getHostname();
        html += F("</title></head><body><a href=\"/update\">OTA Update</a></html>");

        request->send(200, "text/html", html);
    });

    this->server->begin();
}
void HttpServer::loop() {
    this->ota->loop();
}
void HttpServer::onUpdateProgress(OTAProgressCallback callback) {
    this->ota->onUpdateProgress(callback);
}
void HttpServer::onUpdateDone(OTADoneCallback callback) {
    this->ota->onUpdateDone(callback);
}