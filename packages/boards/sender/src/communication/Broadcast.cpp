//
// Created by kali on 9/1/25.
//

#include "Broadcast.h"

Broadcast::Broadcast(int channel) {
    this->channel = channel;
}

bool Broadcast::init() {
	WiFi.mode(WIFI_STA);
	Serial.printf("STA MAC: %\n", WiFi.macAddress().c_str());

    if (esp_now_init() != ESP_OK) {
        Serial.println("ESPNow Init Failed");
        // Retry InitESPNow, add a counte and then restart?
        // InitESPNow();
        // or Simply Restart
        // ESP.restart();
        return false;
    }

    // todo
    // esp_now_register_send_cb(OnDataSent);
    // esp_now_register_recv_cb(OnDataRecv);

    bool result = this->initPeer();
	if(!result) {
		return false;
	}
	this->initialised = true;
    Serial.println("Initialized Broadcast");
	return true;
}
bool Broadcast::initPeer() {
    // clear peer data
    memset(&peer, 0, sizeof(peer));
    for (int ii = 0; ii < 6; ++ii) {
        peer.peer_addr[ii] = (uint8_t)0xff;
    }
    peer.channel = this->channel;
    peer.encrypt = 0;

    Serial.print("Peer Status: ");
    const uint8_t *peer_addr = this->peer.peer_addr;
    // check if the peer exists
    bool exists = esp_now_is_peer_exist(peer_addr);
    if (exists) {
        // Peer already paired.
        Serial.println("Already Paired");
        return true;
    }

    // Peer not paired, attempt pair
    return this->pairPeer();
}
bool Broadcast::pairPeer() {
    esp_err_t addStatus = esp_now_add_peer(&this->peer);
    if (addStatus == ESP_OK) {
        // Pair success
        Serial.println("Pair success");
        return true;
    }
    else if (addStatus == ESP_ERR_ESPNOW_NOT_INIT) {
        // How did we get so far!!
        Serial.println("ESPNOW Not Init");
        return false;
    }
    else if (addStatus == ESP_ERR_ESPNOW_ARG) {
        Serial.println("Invalid Argument");
        return false;
    }
    else if (addStatus == ESP_ERR_ESPNOW_FULL) {
        Serial.println("Peer list full");
        return false;
    }
    else if (addStatus == ESP_ERR_ESPNOW_NO_MEM) {
        Serial.println("Out of memory");
        return false;
    }
    else if (addStatus == ESP_ERR_ESPNOW_EXIST) {
        Serial.println("Peer Exists");
        return true;
    }
    else {
        Serial.println("Not sure what happened");
        return false;
    }
}

void Broadcast::send(String payload) {
	if (!this->initialised) {
		return;
	}
	const uint8_t *peer_addr = this->peer.peer_addr;

	Serial.print("Sending: "); Serial.println(payload);
	esp_err_t result = esp_now_send(peer_addr, (uint8_t*)payload.c_str(), payload.length());
	Serial.print("Send Status: ");
	if (result == ESP_OK) {
		Serial.println("Success");
	}
	else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
		// How did we get so far!!
		Serial.println("ESPNOW not Init.");
	}
	else if (result == ESP_ERR_ESPNOW_ARG) {
		Serial.println("Invalid Argument");
	}
	else if (result == ESP_ERR_ESPNOW_INTERNAL) {
		Serial.println("Internal Error");
	}
	else if (result == ESP_ERR_ESPNOW_NO_MEM) {
		Serial.println("ESP_ERR_ESPNOW_NO_MEM");
	}
	else if (result == ESP_ERR_ESPNOW_NOT_FOUND) {
		Serial.println("Peer not found.");
	}
	else {
		Serial.println("Not sure what happened");
	}
}