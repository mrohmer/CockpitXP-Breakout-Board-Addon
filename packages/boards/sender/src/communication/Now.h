//
// Created by kali on 9/1/25.
//

#ifndef NOW_H
#define NOW_H

#include <esp_now.h>
#include <WiFi.h>

class Now {
private:
	bool initialised = false;
    int channel;
    esp_now_peer_info_t peer;

    bool initPeer();
    bool pairPeer();
public:
    Now(int channel);
    bool init();
    void send(String payload);
};

#endif //NOW_H
