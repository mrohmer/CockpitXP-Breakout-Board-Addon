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
void Settings::setInt(String key, int value) {
    this->restartTicker();
    this->debouncedInts[key] = value;
}
int Settings::getInt(String key, int defaultValue) {
    if (this->debouncedInts.find(key) != this->debouncedInts.end()) {
        return this->debouncedInts[key];
    }
    return this->preferences.getInt(key.c_str(), defaultValue);
}
void Settings::setFlagBrightness(int brightness) {
    this->setInt(KEY_FLAG_BRIGHTNESS, std::min(std::max(brightness, 0), 255));
}
int Settings::getFlagBrightness() {
    return this->getInt(KEY_FLAG_BRIGHTNESS, 34); // RRGGBB22
}
void Settings::setChaosStyle(int style) {
    this->setInt(KEY_CHAOS_STYLE, style);
}
int Settings::getChaosStyle() {
    return this->getInt(KEY_CHAOS_STYLE, 1);
}