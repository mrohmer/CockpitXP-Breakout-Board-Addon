//
// Created by kali on 9/2/25.
//

#ifndef FLAGS_H
#define FLAGS_H

#define NUM_LEDS 8

#include <Adafruit_NeoPixel.h>
#include <utils/ParseColor.h>

class Flags {
private:
    Adafruit_NeoPixel* pxl = nullptr;
public:
    Flags(int pin);
    Flags* init();
    Flags* setColorString(int index, String color);
    Flags* setColor(int index, int r, int g, int b);
    Flags* show();
    Flags* clear();
};

#endif //FLAGS_H
