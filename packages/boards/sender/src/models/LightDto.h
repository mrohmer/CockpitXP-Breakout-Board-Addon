//
// Created by kali on 9/1/25.
//

#ifndef LIGHTDTO_H
#define LIGHTDTO_H

#include <string>
#include <Arduino.h>
#include "ArduinoJson.h"


class LightDto {
private:
    LightDto(String l1, String l2, String l3, String l4);
    static LightDto* createCheckered(bool initial, String vOn, String vOff);
public:
    String lights[4];
    static LightDto* createGreen(bool on);
    static LightDto* createRed();
    static LightDto* createChaos(bool initial);
    static LightDto* createFinished(bool initial);
    LightDto* setSessionRecord(bool on);
    String serialize();
};



#endif //LIGHTDTO_H
