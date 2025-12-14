//
// Created by kali on 9/1/25.
//

#include "Now.h"

Now* Now::instance = nullptr;

bool Now::init() {
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

	esp_now_register_recv_cb(staticOnReceiveData);

	this->initialised = true;
    Serial.println("Initialized Broadcast");
	return true;
}
bool Now::initBroadcastPeer() {
	if (this->broadcastInitialised) {
		return true;
	}
	// clear peer data
	memset(&this->broadcastPeer, 0, sizeof(this->broadcastPeer));
	for (int ii = 0; ii < 6; ++ii) {
		this->broadcastPeer.peer_addr[ii] = (uint8_t)0xff;
	}
	this->broadcastPeer.channel = 0;
	this->broadcastPeer.encrypt = 0;

	this->broadcastInitialised = this->initPeer(this->broadcastPeer);
	return this->broadcastInitialised;
}
bool Now::initPeer(esp_now_peer_info_t peer) {
    Serial.print("Peer Status: ");
    const uint8_t *peer_addr = peer.peer_addr;
    // check if the peer exists
    bool exists = esp_now_is_peer_exist(peer_addr);
    if (exists) {
        // Peer already paired.
        Serial.println("Already Paired");
        return true;
    }

    // Peer not paired, attempt pair
    return this->pairPeer(peer);
}
bool Now::pairPeer(esp_now_peer_info_t peer) {
    esp_err_t addStatus = esp_now_add_peer(&peer);
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

bool Now::sendWithPeer(esp_now_peer_info_t peer, String payload) {
	if (!this->initialised) {
		return false;
	}
	const uint8_t *peer_addr = peer.peer_addr;

	Serial.print("Sending: "); Serial.println(payload);
	esp_err_t result = esp_now_send(peer_addr, (uint8_t*)payload.c_str(), payload.length());
	Serial.print("Send Status: ");
	if (result == ESP_OK) {
		Serial.println("Success");
		return true;
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
	return false;
}
bool Now::sendBroadcast(String payload) {
	if (!this->initBroadcastPeer()) {
		return false;
	}
	return this->sendWithPeer(this->broadcastPeer, payload);
}
bool Now::macStringToBytes(const String &macStr, uint8_t mac[6]) {
	if (macStr.length() == 17 || macStr.length() == 12) {
		int segmentWidth = macStr.length() == 17 ? 3 : 2;
		for (int i = 0; i < 6; i++) {
			char high = macStr[i * segmentWidth];
			char low  = macStr[i * segmentWidth + 1];

			mac[i] = (uint8_t) strtol((String(high) + String(low)).c_str(), NULL, 16);
		}

		return true;
	}

	Serial.printf("Invalid mac address format: %s", macStr.c_str());
	return false;
}
bool Now::send(String macAdress, String payload) {
	esp_now_peer_info_t peer;
	// clear peer data
	memset(&peer, 0, sizeof(peer));

	if (!this->macStringToBytes(macAdress, peer.peer_addr)) {
		return false;
	}
	Serial.printf(
		"Mac address: %02X:%02X:%02X:%02X:%02X:%02X\n",
		peer.peer_addr[0],
		peer.peer_addr[1],
		peer.peer_addr[2],
		peer.peer_addr[3],
		peer.peer_addr[4],
		peer.peer_addr[5]
	);

	peer.channel = 0;
	peer.encrypt = 0;
	if (!this->initPeer(peer)) {
		return false;
	}
	return this->sendWithPeer(peer, payload);
}
Now* Now::onReceive(OnReceiveCallback callback) {
    this->onReceiveCallbacks.insert(this->onReceiveCallbacks.end(), callback);
	return this;
}
void Now::onReceiveData(EspNowRecvInfo *macAddr, Data *data, Length len) {
	char* buff = (char*) data;
	String buffStr = String(buff);
	Serial.printf(
		"reveived from %02x:%02x:%02x:%02x:%02x:%02x: %s\n",
		macAddr->src_addr[0], macAddr->src_addr[1], macAddr->src_addr[2],
		macAddr->src_addr[3], macAddr->src_addr[4], macAddr->src_addr[5],
		buffStr.c_str()
	);

	for (auto & element : this->onReceiveCallbacks) {
		element(macAddr->src_addr, buffStr);
	}
}