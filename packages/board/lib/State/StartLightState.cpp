//
// Created by Matthias Rohmer on 09.01.24.
//

#include "StartLightState.h"

StartLightState::StartLightState() {}

void StartLightState::reset() {
  state = 0;
  falseStart = false;
  falseStartToggle = false;
}