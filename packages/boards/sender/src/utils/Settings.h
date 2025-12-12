//
// Created by mrohmer on 12.12.25.
//

#ifndef COCKPITXP_BREAKOUT_BOARD_ADDON_SETTINGS_H
#define COCKPITXP_BREAKOUT_BOARD_ADDON_SETTINGS_H

#include <Preferences.h>

class Settings {
private:
    static Settings* instance;
    Preferences preferences;
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