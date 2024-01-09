//
// Created by Matthias Rohmer on 09.01.24.
//

#include "SlotsState.h"
#include "SlotState.h"

SlotsState::SlotsState() {
  slot1 = SlotState();
  slot2 = SlotState();
  slot3 = SlotState();
  slot4 = SlotState();
  slot5 = SlotState();
  slot6 = SlotState();
}

void SlotsState::reset() {
  slot1.reset();
  slot2.reset();
  slot3.reset();
  slot4.reset();
  slot5.reset();
  slot6.reset();
  lastNeedsToRefuelToggleState = false;
}