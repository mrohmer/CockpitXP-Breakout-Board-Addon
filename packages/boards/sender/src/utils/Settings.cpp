//
// Created by mrohmer on 12.12.25.
//

#include "Settings.h"

Settings* Settings::instance = nullptr;

void Settings::init() {
    this->preferences.begin("cmacenter", false);
}
void Settings::restartTicker() {
    ticker.detach();
    ticker.once_ms(30000, std::bind(&Settings::flush, this));
}
void Settings::flush() {
    ticker.detach();
    for (const auto& pair : this->debouncedInts) {
        this->preferences.putInt(pair.first.c_str(), pair.second);
    }
    this->debouncedInts.clear();
}
void Settings::setFlagBrightness(int brightness) {
    this->restartTicker();
    this->debouncedInts[KEY_FLAG_BRIGHTNESS] = std::min(std::max(brightness, 0), 255);
}
int Settings::getFlagBrightness() {
    if (this->debouncedInts.find(KEY_FLAG_BRIGHTNESS) != this->debouncedInts.end()) {
        return this->debouncedInts[KEY_FLAG_BRIGHTNESS];
    }
    return this->preferences.getInt(KEY_FLAG_BRIGHTNESS, 34); // 22
}