//
// Created by Matthias Rohmer on 09.01.24.
//

#include "VirtualSafetyCarState.h"

VirtualSafetyCarState::VirtualSafetyCarState() {}

void VirtualSafetyCarState::reset() {
  state = false;
  lastToggleState = false;
}