//
// Created by kali on 9/1/25.
//

#ifndef STATE_H
#define STATE_H

#include <Arduino.h>

class State {
private:
    int flags = 0;
    bool sessionRecord = false;
    long lastChangeFlags = -1;
    long lastChangeSessionRecord = -1;

public:
    bool isRed();
    bool isGreen();
    bool isChaos();
    bool isFinished();

    void setFlags(int flags);
    void setSessionRecord(bool sessionRecord);
};



#endif //STATE_H
