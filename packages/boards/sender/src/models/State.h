//
// Created by kali on 9/1/25.
//

#ifndef STATE_H
#define STATE_H

#include <Arduino.h>

struct State {
    bool isRed;
    bool isGreen;
    bool isChaos;
    bool isFinished;

    int flags;
    bool isSessionRecord;
};


inline bool operator==(const State& lhs, const State& rhs) {
    return lhs.flags == rhs.flags && lhs.isSessionRecord == rhs.isSessionRecord;
}

inline bool operator!=(const State& lhs, const State& rhs) {
    return !(lhs == rhs);
}

State createState(int flags, bool sessionRecord);

#endif //STATE_H
