# Cockpit XP Breakout Board Addon

## General Things

4 Output Pins are available.

To be able to send more than 15 different signals, we want to split a bigger number into smaller chunks of bits.
But as we can't rely on a certain execution time in a cockpit xp addon we need to have one bit as clock, with a
raising flank.

As I would like to keep the general execution time as low as possible, I would like to decrease the initialisation
signal as short as possible.
So every sent piece of data is prefixed with a single event of all zero values.
**This eliminates all numbers with a chunk of 3 bits all zero**

Besides that it would be nice to have one bit as parity bit to ensure correct data to be transferred.

### Events

1. Reset all (addon: ✅, board: ❌)

1. Slot 1 needs to refuel (addon: ✅, board: ☑️)
1. Slot 1 does not need to refuel anymore (addon: ✅, board: ☑️)
1. Slot 1 starts refueling (addon: ✅, board: ☑️)
1. Slot 1 stops refueling (addon: ✅, board: ☑️)

1. Slot 2 needs to refuel (addon: ✅, board: ☑️)
1. Slot 2 does not need to refuel anymore (addon: ✅, board: ☑️)
1. Slot 2 starts refueling (addon: ✅, board: ☑️)
1. Slot 2 stops refueling (addon: ✅, board: ☑️)

1. Slot 3 needs to refuel (addon: ✅, board: ☑️)
1. Slot 3 does not need to refuel anymore (addon: ✅, board: ☑️)
1. Slot 3 starts refueling (addon: ✅, board: ☑️)
1. Slot 3 stops refueling (addon: ✅, board: ☑️)

1. Slot 4 needs to refuel (addon: ✅, board: ☑️)
1. Slot 4 does not need to refuel anymore (addon: ✅, board: ☑️)
1. Slot 4 starts refueling (addon: ✅, board: ☑️)
1. Slot 4 stops refueling (addon: ✅, board: ☑️)

1. Slot 5 needs to refuel (addon: ✅, board: ☑️)
1. Slot 5 does not need to refuel anymore (addon: ✅, board: ☑️)
1. Slot 5 starts refueling (addon: ✅, board: ☑️)
1. Slot 5 stops refueling (addon: ✅, board: ☑️)

1. Slot 6 needs to refuel (addon: ✅, board: ☑️)
1. Slot 6 does not need to refuel anymore (addon: ✅, board: ☑️)
1. Slot 6 starts refueling (addon: ✅, board: ☑️)
1. Slot 6 stops refueling (addon: ✅, board: ☑️)

1. Pitlane 1 off (addon: ✅, board: ☑️)
1. Pitlane 1 10% (addon: ✅, board: ☑️)
1. Pitlane 1 20% (addon: ✅, board: ☑️)
1. Pitlane 1 30% (addon: ✅, board: ☑️)
1. Pitlane 1 40% (addon: ✅, board: ☑️)
1. Pitlane 1 50% (addon: ✅, board: ☑️)
1. Pitlane 1 60% (addon: ✅, board: ☑️)
1. Pitlane 1 70% (addon: ✅, board: ☑️)
1. Pitlane 1 80% (addon: ✅, board: ☑️)
1. Pitlane 1 90% (addon: ✅, board: ☑️)
1. Pitlane 1 100% (addon: ✅, board: ☑️)

1. Pitlane 2 off (addon: ✅, board: ☑️)
1. Pitlane 2 10% (addon: ✅, board: ☑️)
1. Pitlane 2 20% (addon: ✅, board: ☑️)
1. Pitlane 2 30% (addon: ✅, board: ☑️)
1. Pitlane 2 40% (addon: ✅, board: ☑️)
1. Pitlane 2 50% (addon: ✅, board: ☑️)
1. Pitlane 2 60% (addon: ✅, board: ☑️)
1. Pitlane 2 70% (addon: ✅, board: ☑️)
1. Pitlane 2 80% (addon: ✅, board: ☑️)
1. Pitlane 2 90% (addon: ✅, board: ☑️)
1. Pitlane 2 100% (addon: ✅, board: ☑️)

1. Starting light 1 (addon: ✅, board: ✅)
1. Starting light 2 (addon: ✅, board: ✅)
1. Starting light 3 (addon: ✅, board: ✅)
1. Starting light 4 (addon: ✅, board: ✅)
1. Starting light 5 (addon: ✅, board: ✅)
1. Starting light off (addon: ✅, board: ✅)
1. Starting light green (addon: ✅, board: ✅)
1. False start (addon: ✅, board: ✅)

1. New track record (addon: ✅, board: ❌)
1. New session record (addon: ✅, board: ❌)

1. Virtual Safety Car on (addon: ✅, board: ❌)
1. Virtual Safety Car off (addon: ✅, board: ❌)

### Mapping

Even though we would need 6bit to encode the numbers for every event we need to take into account that we can't use
every possible value due to the conflicts with the sync event (eg every value below 8 as it starts with a block of 000).

As we want to add another bit for parity the next step would be 9bit long events & 8bit for the raw payload.

This results in total 256 possible values. 85 are eliminated as it would include a sequence of 000 but leaves 171 valid
values.

See mapping table [here](docs/mapping.md).