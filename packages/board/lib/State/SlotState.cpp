//
// Created by Matthias Rohmer on 09.01.24.
//

#include "SlotState.h"

SlotState::SlotState() {}

void SlotState::reset() {
  isRefueling = false;
  needsRefueling = false;
}
