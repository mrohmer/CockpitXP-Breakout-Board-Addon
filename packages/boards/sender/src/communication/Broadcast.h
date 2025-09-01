//
// Created by kali on 9/1/25.
//

#ifndef BROADCAST_H
#define BROADCAST_H

#include <esp_now.h>
#include <WiFi.h>

class Broadcast {
private:
	bool initialised = false;
    int channel;
    esp_now_peer_info_t peer;

    bool initPeer();
    bool pairPeer();
public:
    Broadcast(int channel);
    bool init();
    void send(String payload);
};



#endif //BROADCAST_H
