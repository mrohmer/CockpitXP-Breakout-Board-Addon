//
// Created by kali on 9/1/25.
//

#include "LightDto.h"

LightDto::LightDto(String l1, String l2, String l3, String l4){
    this->lights[0] = l1;
    this->lights[1] = l2;
    this->lights[2] = l3;
    this->lights[3] = l4;
}
LightDto* LightDto::createGreen(bool on) {
    String v = on ? "8cc800" : "000";
   return new LightDto(v, v, v, v);
}
LightDto* LightDto::createRed() {
    String v = "ff0000";
    return new LightDto(v, v, v, v);
}
LightDto* LightDto::createCheckered(bool initial, String vOn, String vOff) {
    String v1 = initial ? vOn : vOff;
    String v2 = initial ? vOff : vOn;
    return new LightDto(v1, v2, v1, v2);
}
LightDto* LightDto::createChaos(bool initial) {
    return createCheckered(initial, "ff9600", "000");
}
LightDto* LightDto::createFinished(bool initial) {
    return createCheckered(initial, "fff", "000");
}
LightDto* LightDto::createProgress(float progress) {
    int v = (progress / 4) * 256;
    int diff = v % 4;
    int base = v - diff;
    String color = "ff9600";
    String v1 = color + toHexStr(base);
    String v2 = color + toHexStr(base + (diff > 0 ? 1 : 0));
    String v3 = color + toHexStr(base + (diff > 1 ? 1 : 0));
    String v4 = color + toHexStr(base + (diff > 2 ? 1 : 0));
    return new LightDto(v1, v2, v1, v2);
}
LightDto* LightDto::createBleConnection() {
    String v = "0082fc22";
    return new LightDto(v, v, v, v);
}
String LightDto::toHexStr(int value) {
    String hexStr = String(value, HEX);

    if (hexStr.length() < 2) {
        hexStr = "0" + hexStr;
    }

    return hexStr;
}
String LightDto::serialize() {
    JsonDocument doc;

    doc["t"] = "c";
    JsonArray arr = doc["d"].to<JsonArray>(); // turn data into array

    for (int i = 0; i < 4; i++) {
        arr.add(lights[i]);
    }

    String output;
    serializeJson(doc, output);
    return output;
}