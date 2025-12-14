//
// Created by kali on 9/1/25.
//

#include "State.h"

State createState(int flags, bool sessionRecord) {
    return {
        .isRed = flags == 0,
        .isGreen = flags == 1,
        .isChaos = flags == 2,
        .isFinished = flags == 3,
        .flags = flags,
        .isSessionRecord = sessionRecord,
    };
}