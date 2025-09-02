//
// Created by kali on 9/2/25.
//

#include "Flags.h"

Flags::Flags(int pin) {
    pxl = new Adafruit_NeoPixel(NUM_LEDS, pin, NEO_GRB + NEO_KHZ800);
}
void Flags::init() {
    pxl->begin();
    pxl->setBrightness(255);
}
Flags* Flags::setColorString(int index, String str) {
    optional<Color> color = parseColor(std::string(str.c_str()));

    return this->setColor(index, pxl->Color(color->r, color->g, color->b));
}
Flags* Flags::setColor(int index, uint32_t color) {
    if (index < 0 || index >= NUM_LEDS / 2) {
        return this;
    }
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
