//
// Created by Matthias Rohmer on 09.01.24.
//

#include "State.h"
#include "SlotsState.h"
#include "StartLightState.h"
#include "VirtualSafetyCarState.h"

State::State() {
  slots = SlotsState();
  startLight = StartLightState();
  virtualSafetyCar = VirtualSafetyCarState();
}

void State::reset() {
  pitlane1 = 0;
  pitlane2 = 0;
  newSessionRecord = false;
  newTrackRecord = false;
  slots.reset();
  startLight.reset();
  virtualSafetyCar.reset();
}

bool State::updateFromEvent(unsigned int event) {
  switch (event) {
    case 11:
      reset();
      break;
    case 21:  // virtual safety car on
      virtualSafetyCar.state = true;
      break;
    case 22:  // virtual safety car off
      virtualSafetyCar.state = false;
      break;
    case 28:  // starting light off
      startLight.state = 0;
      startLight.falseStart = false;
      break;
    case 31:  // starting light 1
      startLight.state = 1;
      startLight.falseStart = false;
      break;
    case 35:  // starting light 2
      startLight.state = 2;
      startLight.falseStart = false;
      break;
    case 37:  // starting light 3
      startLight.state = 3;
      startLight.falseStart = false;
      break;
    case 38:  // starting light 4
      startLight.state = 4;
      startLight.falseStart = false;
      break;
    case 41:  // starting light 5
      startLight.state = 5;
      startLight.falseStart = false;
      break;
    case 42:  // starting light false start
      startLight.falseStart = true;
      break;
    case 44:  // starting light green
      startLight.state = 5;
      startLight.falseStart = false;
      break;
    case 49:  // new track record
      newTrackRecord = true;
      break;
    case 50:  // new session record
      newSessionRecord = true;
      break;
    case 55:  // pit lane 1 off
      pitlane1 = 0;
      break;
    case 59:  // pit lane 1 10%
      pitlane1 = 10;
      break;
    case 61:  // pit lane 1 20%
      pitlane1 = 20;
      break;
    case 62:  // pit lane 1 30%
      pitlane1 = 30;
      break;
    case 73:  // pit lane 1 40%
      pitlane1 = 40;
      break;
    case 74:  // pit lane 1 50%
      pitlane1 = 50;
      break;
    case 75:  // pit lane 1 60%
      pitlane1 = 60;
      break;
    case 76:  // pit lane 1 70%
      pitlane1 = 70;
      break;
    case 77:  // pit lane 1 80%
      pitlane1 = 80;
      break;
    case 78:  // pit lane 1 90%
      pitlane1 = 90;
      break;
    case 79:  // pit lane 1 100%
      pitlane1 = 100;
      break;
    case 81:  // pit lane 2 off
      pitlane2 = 0;
      break;
    case 82:  // pit lane 2 10%
      pitlane2 = 10;
      break;
    case 83:  // pit lane 2 20%
      pitlane2 = 20;
      break;
    case 84:  // pit lane 2 30%
      pitlane2 = 30;
      break;
    case 85:  // pit lane 2 40%
      pitlane2 = 40;
      break;
    case 86:  // pit lane 2 50%
      pitlane2 = 50;
      break;
    case 87:  // pit lane 2 60%
      pitlane2 = 60;
      break;
    case 89:  // pit lane 2 70%
      pitlane2 = 70;
      break;
    case 90:  // pit lane 2 80%
      pitlane2 = 80;
      break;
    case 91:  // pit lane 2 90%
      pitlane2 = 90;
      break;
    case 92:  // pit lane 2 100%
      pitlane2 = 100;
      break;
    case 97:  // slot 1 needs to refuel
      slots.slot1.needsRefueling = true;
      break;
    case 98:  // slot 1 does not need to refuel anymore
      slots.slot1.needsRefueling = false;
      break;
    case 99:  // slot 1 starts refueling
      slots.slot1.isRefueling = true;
      break;
    case 100:  // slot 1 stops refueling
      slots.slot1.isRefueling = false;
      break;
    case 105:  // slot 2 needs to refuel
      slots.slot2.needsRefueling = true;
      break;
    case 106:  // slot 2 does not need to refuel anymore
      slots.slot2.needsRefueling = false;
      break;
    case 107:  // slot 2 starts refueling
      slots.slot2.isRefueling = true;
      break;
    case 108:  // slot 2 stops refueling
      slots.slot2.isRefueling = false;
      break;
    case 113:  // slot 3 needs to refuel
      slots.slot3.needsRefueling = true;
      break;
    case 114:  // slot 3 does not need to refuel anymore
      slots.slot3.needsRefueling = false;
      break;
    case 115:  // slot 3 starts refueling
      slots.slot3.isRefueling = true;
      break;
    case 116:  // slot 3 stops refueling
      slots.slot3.isRefueling = false;
      break;
    case 121:  // slot 4 needs to refuel
      slots.slot4.needsRefueling = true;
      break;
    case 122:  // slot 4 does not need to refuel anymore
      slots.slot4.needsRefueling = false;
      break;
    case 123:  // slot 4 starts refueling
      slots.slot4.isRefueling = true;
      break;
    case 124:  // slot 4 stops refueling
      slots.slot4.isRefueling = false;
      break;
    case 137:  // slot 5 needs to refuel
      slots.slot5.needsRefueling = true;
      break;
    case 138:  // slot 5 does not need to refuel anymore
      slots.slot5.needsRefueling = false;
      break;
    case 139:  // slot 5 starts refueling
      slots.slot5.isRefueling = true;
      break;
    case 140:  // slot 5 stops refueling
      slots.slot5.isRefueling = false;
      break;
    case 145:  // slot 6 needs to refuel
      slots.slot6.needsRefueling = true;
      break;
    case 146:  // slot 6 does not need to refuel anymore
      slots.slot6.needsRefueling = false;
      break;
    case 147:  // slot 6 starts refueling
      slots.slot6.isRefueling = true;
      break;
    case 148:  // slot 6 stops refueling
      slots.slot6.isRefueling = false;
      break;
    default:
      return 0;
  }

  return 1;
}

