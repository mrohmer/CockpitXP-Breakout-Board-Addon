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
    String v = on ? "8cc800" : "000000";
   return new LightDto(v, v, v, v);
}
LightDto* LightDto::createRed() {
    String v = "ff0000";
    return new LightDto(v, v, v, v);
}
LightDto* LightDto::createCheckered(bool initial, String vOn, String vOff) {
    String v1 = initial ? vOn : vOff;
    String v2 = initial ? vOff : vOn;
    return new LightDto(v1, v2, v2, v1);
}
LightDto* LightDto::createChaos(bool initial) {
    return createCheckered(initial, "ff0000", "000000");
}
LightDto* LightDto::createFinished(bool initial) {
    return createCheckered(initial, "ff9600", "000000");
}
String LightDto::serialize() {
    JsonDocument doc;

    JsonArray arr = doc.to<JsonArray>(); // turn root into array

    for (int i = 0; i < 4; i++) {
        arr.add(lights[i]);
    }

    char output[256];
    serializeJson(doc, output);
    return String(output);
}