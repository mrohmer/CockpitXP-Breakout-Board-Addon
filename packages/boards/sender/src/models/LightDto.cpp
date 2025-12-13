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
LightDto* LightDto::createChaosStyle1(bool initial) {
    String on = "ff9600";
    String off = "000";
    return new LightDto(on, initial ? on : off, on, initial ? off : on);
}
LightDto* LightDto::createChaosStyle2(bool initial) {
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
LightDto* LightDto::setBrightnessOnAllLights(int brightness) {
    this->lights[0] = this->adjustColorToBrightness(this->lights[0], brightness);
    this->lights[1] = this->adjustColorToBrightness(this->lights[1], brightness);
    this->lights[2] = this->adjustColorToBrightness(this->lights[2], brightness);
    this->lights[3] = this->adjustColorToBrightness(this->lights[3], brightness);
    return this;
}
String LightDto::adjustColorToBrightness(String color, int brightness) {
    // invalid brightness values
    if (brightness < 0 || brightness > 255) {
        return color;
    }

    // brighntess zero can also be treated as black
    if (brightness == 0) {
        return "000";
    }
    if (color == "000" || color == "000000") {
        return "000";
    }

    // remove alpha from color string
    if (color.length() == 4) {
        return this->adjustColorToBrightness(color.substring(0, 3), brightness);
    }
    if (color.length() == 8) {
        return this->adjustColorToBrightness(color.substring(0, 6), brightness);
    }

    // brightness is max so no need to mention it
    if (brightness == 255) {
        return color;
    }

    char buff[2];
    sprintf(buff, "%02x", brightness);
    String brightnessStr = String(buff);

    // short format and brightness is compatible
    if (color.length() == 3 && brightness % 16 == 0) {
        char buff2[4];
        sprintf(buff2, "%s%c", color.c_str(), brightnessStr[0]);
        return String(buff2);
    }

    // short format but brightness requires long format
    if (color.length() == 3) {
        char buff2[8];
        sprintf(
            buff2,
            "%c%c%c%c%c%c%s",
            color.charAt(0),
            color.charAt(0),
            color.charAt(1),
            color.charAt(1),
            color.charAt(2),
            color.charAt(2),
            brightnessStr.c_str()
        );
        return String(buff2);
    }

    // long format
    if (color.length() == 6) {
        char buff2[8];
        sprintf(buff2, "%s%s", color.c_str(), brightnessStr.c_str());
        return String(buff2);
    }

    // fallback to same char
    return color;
}