//
// Created by mrohmer on 12.12.25.
//

#include "Settings.h"

Settings* Settings::instance = nullptr;

void Settings::init() {
    this->preferences.begin("cmacenter", false);
}

void Settings::setFlagBrightness(int brightness) {
    this->preferences.putInt("FLAG_BRIGHTNESS", std::min(std::max(brightness, 0), 255));
}
int Settings::getFlagBrightness() {
    return this->preferences.getInt("FLAG_BRIGHTNESS", 34); // 22
}