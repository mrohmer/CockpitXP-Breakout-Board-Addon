//
// Created by kali on 9/1/25.
//

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Input.h"
#include "communication/Broadcast.h"

class Controller {
private:
    Input* input;
    Broadcast* broadcast;
public:
    Controller(Input* input, Broadcast* broadcast);
    bool init();
};



#endif //CONTROLLER_H
