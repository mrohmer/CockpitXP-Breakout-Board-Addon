//
// Created by kali on 9/1/25.
//

#ifndef COUNTTICKER_H
#define COUNTTICKER_H

#include <Ticker.h>

typedef std::function<void(int)> OnCount;
class CountingTicker {
private:
    Ticker ticker;
    OnCount callback;
    int count = 0;
    void tick();
public:
    CountingTicker(float rateInHz, OnCount callback);
    ~CountingTicker();
    void off();
};



#endif //COUNTTICKER_H
