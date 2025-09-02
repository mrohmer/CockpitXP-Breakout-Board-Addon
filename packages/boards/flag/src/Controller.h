//
// Created by kali on 9/2/25.
//

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Flags.h>
#include <communication/Now.h>
#include "ArduinoJson.h"

class Controller {
private:
    Flags* flags;
    Now* now;
    void onReceiveData(String data);
public:
    Controller(Flags* flags);
    bool init();
};



#endif //CONTROLLER_H
