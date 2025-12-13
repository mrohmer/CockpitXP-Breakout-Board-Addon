//
// Created by mrohmer on 12.12.25.
//

#ifndef COCKPITXP_BREAKOUT_BOARD_ADDON_SETTINGS_H
#define COCKPITXP_BREAKOUT_BOARD_ADDON_SETTINGS_H

#include <Preferences.h>
#include <map>
#include "Ticker.h"

#define KEY_FLAG_BRIGHTNESS "FLAG_BRIGHTNESS"

class Settings {
private:
    static Settings* instance;
    Ticker ticker;
    std::map<String, int> debouncedInts;
    Preferences preferences;
    void restartTicker();
    void flush();
public:
    static Settings* getInstance() {
        if (!instance) {
            instance = new Settings();
        }
        return instance;
    };
    void init();
    int getFlagBrightness();
    void setFlagBrightness(int brightness);
};


#endif //COCKPITXP_BREAKOUT_BOARD_ADDON_SETTINGS_H