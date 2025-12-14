//
// Created by kali on 9/5/25.
//

#ifndef INPUT_H
#define INPUT_H

#include <models/State.h>

typedef std::function<void(State)> OnInputChange;

class Input {
public:
    virtual void init() = 0;
    virtual void loop() = 0;
    virtual void onChange(OnInputChange onChange) = 0;
    virtual State getState() = 0;
};



#endif //INPUT_H
