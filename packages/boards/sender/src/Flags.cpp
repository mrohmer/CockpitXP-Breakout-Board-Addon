//
// Created by kali on 9/2/25.
//

#include "Flags.h"

Flags::Flags(int pin) {
    pxl = new Adafruit_NeoPixel(NUM_LEDS, pin, NEO_GRB + NEO_KHZ800);
}
Flags* Flags::init() {
    pxl->begin();
    pxl->setBrightness(255);
    return this;
}
Flags* Flags::setColorString(int index, String str) {
    optional<Color> color = parseColor(std::string(str.c_str()));

    return this->setColor(index, color->r, color->g, color->b);
}
Flags* Flags::setColor(int index, int r, int g, int b) {
    if (index < 0 || index >= NUM_LEDS / 2) {
        return this;
    }
    uint32_t color = pxl->Color(r, g, b);
    pxl->setPixelColor(index, color);
    int otherSideIndex = NUM_LEDS - index - 1;
    pxl->setPixelColor(otherSideIndex, color);
    return this;
}
Flags* Flags::show() {
    pxl->show();
    return this;
}
Flags* Flags::clear() {
    pxl->clear();
    return this;
}
