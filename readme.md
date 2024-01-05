# Cockpit XP Breakout Board Addon

## General Things

4 Output Pins are available.

To be able to send more than 15 different signals, we want to split a bigger number into smaller chunks of bits.
But as we can't rely on a certain execution time in a cockpit xp addon we need to have one bit as clock, with a
raising & falling flank as the signal.

As I would like to keep the general execution time as low as possible, I would like to decrease the initialisation
signal as short as possible.
So every sent piece of data is prefixed with a single event of all zero values.
**This eliminates all numbers with a chunk of 3 bits all zero**

Besides that it would be nice to have one bit as parity bit to ensure correct data to be transferred.

### Events

1. Reset all

1. Slot 1 needs to refuel
1. Slot 1 does not need to refuel anymore
1. Slot 1 starts refueling
1. Slot 1 stops refueling

1. Slot 2 needs to refuel
1. Slot 2 does not need to refuel anymore
1. Slot 2 starts refueling
1. Slot 2 stops refueling

1. Slot 3 needs to refuel
1. Slot 3 does not need to refuel anymore
1. Slot 3 starts refueling
1. Slot 3 stops refueling

1. Slot 4 needs to refuel
1. Slot 4 does not need to refuel anymore
1. Slot 4 starts refueling
1. Slot 4 stops refueling

1. Slot 5 needs to refuel
1. Slot 5 does not need to refuel anymore
1. Slot 5 starts refueling
1. Slot 5 stops refueling

1. Slot 6 needs to refuel
1. Slot 6 does not need to refuel anymore
1. Slot 6 starts refueling
1. Slot 6 stops refueling

1. Pitlane 1 of
1. Pitlane 1 10%
1. Pitlane 1 20%
1. Pitlane 1 30%
1. Pitlane 1 40%
1. Pitlane 1 50%
1. Pitlane 1 60%
1. Pitlane 1 70%
1. Pitlane 1 80%
1. Pitlane 1 90%
1. Pitlane 1 100%

1. Pitlane 2 off
1. Pitlane 2 10%
1. Pitlane 2 20%
1. Pitlane 2 30%
1. Pitlane 2 40%
1. Pitlane 2 50%
1. Pitlane 2 60%
1. Pitlane 2 70%
1. Pitlane 2 80%
1. Pitlane 2 90%
1. Pitlane 2 100%

1. Starting light 1
1. Starting light 2
1. Starting light 3
1. Starting light 4
1. Starting light 5
1. Starting light off
1. Starting light false start

1. New track record
1. Mew session record

1. Virtual Safety Car on
1. Virtual Safety Car off

### Mapping

Even though we would need 6bit to encode the numbers for every event we need to take into account that we can't use
every possible value due to the conflicts with the sync event (eg every value below 8 as it starts with a block of 000).

As we want to add another bit for parity the next step would be 9bit long events & 8bit for the raw payload.

This results in total 256 possible values. 85 are eliminated as it would include a sequence of 000 but leaves 171 valid
values.

| binary            | dec | valid | mapping                                |
|-------------------|-----|-------|----------------------------------------|
| (0)00 - 000 - 000 | ..0 | ❌     |                                        |
| (1)00 - 000 - 001 | ..1 | ❌     |                                        |
| (1)00 - 000 - 010 | ..2 | ❌     |                                        |
| (0)00 - 000 - 011 | ..3 | ❌     |                                        |
| (1)00 - 000 - 100 | ..4 | ❌     |                                        |
| (0)00 - 000 - 101 | ..5 | ❌     |                                        |
| (0)00 - 000 - 110 | ..6 | ❌     |                                        |
| (1)00 - 000 - 111 | ..7 | ❌     |                                        |
| (1)00 - 001 - 000 | ..8 | ❌     |                                        |
| (0)00 - 001 - 001 | ..9 | ❌     |                                        |
| (0)00 - 001 - 010 | .10 | ❌     |                                        |
| (1)00 - 001 - 011 | .11 | ✅     | Reset all                              |
| (0)00 - 001 - 100 | .12 | ❌     |                                        |
| (1)00 - 001 - 101 | .13 | ✅     |                                        |
| (1)00 - 001 - 110 | .14 | ✅     |                                        |
| (0)00 - 001 - 111 | .15 | ❌     |                                        |
| (1)00 - 010 - 000 | .16 | ❌     |                                        |
| (0)00 - 010 - 001 | .17 | ❌     |                                        |
| (0)00 - 010 - 010 | .18 | ❌     |                                        |
| (1)00 - 010 - 011 | .19 | ✅     |                                        |
| (0)00 - 010 - 100 | .20 | ❌     |                                        |
| (1)00 - 010 - 101 | .21 | ✅     | Virtual Safety Car On                  |
| (1)00 - 010 - 110 | .22 | ✅     | Virtual Safety Car Off                 |
| (0)00 - 010 - 111 | .23 | ❌     |                                        |
| (0)00 - 011 - 000 | .24 | ❌     |                                        |
| (1)00 - 011 - 001 | .25 | ✅     |                                        |
| (1)00 - 011 - 010 | .26 | ✅     |                                        |
| (0)00 - 011 - 011 | .27 | ❌     |                                        |
| (1)00 - 011 - 100 | .28 | ✅     | Starting light off                     |
| (0)00 - 011 - 101 | .29 | ❌     |                                        |
| (0)00 - 011 - 110 | .30 | ❌     |                                        |
| (1)00 - 011 - 111 | .31 | ✅     | Starting light 1                       |
| (1)00 - 100 - 000 | .32 | ❌     |                                        |
| (0)00 - 100 - 001 | .33 | ❌     |                                        |
| (0)00 - 100 - 010 | .34 | ❌     |                                        |
| (1)00 - 100 - 011 | .35 | ✅     | Starting light 2                       |
| (0)00 - 100 - 100 | .36 | ❌     |                                        |
| (1)00 - 100 - 101 | .37 | ✅     | Starting light 3                       |
| (1)00 - 100 - 110 | .38 | ✅     | Starting light 4                       |
| (0)00 - 100 - 111 | .39 | ❌     |                                        |
| (0)00 - 101 - 000 | .40 | ❌     |                                        |
| (1)00 - 101 - 001 | .41 | ✅     | Starting light 5                       |
| (1)00 - 101 - 010 | .42 | ✅     | Starting light false start             |
| (0)00 - 101 - 011 | .43 | ❌     |                                        |
| (1)00 - 101 - 100 | .44 | ✅     |                                        |
| (0)00 - 101 - 101 | .45 | ❌     |                                        |
| (0)00 - 101 - 110 | .46 | ❌     |                                        |
| (1)00 - 101 - 111 | .47 | ✅     |                                        |
| (0)00 - 110 - 000 | .48 | ❌     |                                        |
| (1)00 - 110 - 001 | .49 | ✅     | New track record                       |
| (1)00 - 110 - 010 | .50 | ✅     | New session record                     |
| (0)00 - 110 - 011 | .51 | ❌     |                                        |
| (1)00 - 110 - 100 | .52 | ✅     |                                        |
| (0)00 - 110 - 101 | .53 | ❌     |                                        |
| (0)00 - 110 - 110 | .54 | ❌     |                                        |
| (1)00 - 110 - 111 | .55 | ✅     | Pitlane 1 off                          |
| (1)00 - 111 - 000 | .56 | ❌     |                                        |
| (0)00 - 111 - 001 | .57 | ❌     |                                        |
| (0)00 - 111 - 010 | .58 | ❌     |                                        |
| (1)00 - 111 - 011 | .59 | ✅     | Pitlane 1 10%                          |
| (0)00 - 111 - 100 | .60 | ❌     |                                        |
| (1)00 - 111 - 101 | .61 | ✅     | Pitlane 1 20%                          |
| (1)00 - 111 - 110 | .62 | ✅     | Pitlane 1 30%                          |
| (0)00 - 111 - 111 | .63 | ❌     |                                        |
| (1)01 - 000 - 000 | .64 | ❌     |                                        |
| (0)01 - 000 - 001 | .65 | ❌     |                                        |
| (0)01 - 000 - 010 | .66 | ❌     |                                        |
| (1)01 - 000 - 011 | .67 | ❌     |                                        |
| (0)01 - 000 - 100 | .68 | ❌     |                                        |
| (1)01 - 000 - 101 | .69 | ❌     |                                        |
| (1)01 - 000 - 110 | .70 | ❌     |                                        |
| (0)01 - 000 - 111 | .71 | ❌     |                                        |
| (0)01 - 001 - 000 | .72 | ❌     |                                        |
| (1)01 - 001 - 001 | .73 | ✅     | Pitlane 1 40%                          |
| (1)01 - 001 - 010 | .74 | ✅     | Pitlane 1 50%                          |
| (0)01 - 001 - 011 | .75 | ✅     | Pitlane 1 60%                          |
| (1)01 - 001 - 100 | .76 | ✅     | Pitlane 1 70%                          |
| (0)01 - 001 - 101 | .77 | ✅     | Pitlane 1 80%                          |
| (0)01 - 001 - 110 | .78 | ✅     | Pitlane 1 90%                          |
| (1)01 - 001 - 111 | .79 | ✅     | Pitlane 1 100%                         |
| (0)01 - 010 - 000 | .80 | ❌     |                                        |
| (1)01 - 010 - 001 | .81 | ✅     | Pitlane 2 off                          |
| (1)01 - 010 - 010 | .82 | ✅     | Pitlane 2 10%                          |
| (0)01 - 010 - 011 | .83 | ✅     | Pitlane 2 20%                          |
| (1)01 - 010 - 100 | .84 | ✅     | Pitlane 2 30%                          |
| (0)01 - 010 - 101 | .85 | ✅     | Pitlane 2 40%                          |
| (0)01 - 010 - 110 | .86 | ✅     | Pitlane 2 50%                          |
| (1)01 - 010 - 111 | .87 | ✅     | Pitlane 2 60%                          |
| (1)01 - 011 - 000 | .88 | ❌     |                                        |
| (0)01 - 011 - 001 | .89 | ✅     | Pitlane 2 70%                          |
| (0)01 - 011 - 010 | .90 | ✅     | Pitlane 2 80%                          |
| (1)01 - 011 - 011 | .91 | ✅     | Pitlane 2 90%                          |
| (0)01 - 011 - 100 | .92 | ✅     | Pitlane 2 100%                         |
| (1)01 - 011 - 101 | .93 | ✅     |                                        |
| (1)01 - 011 - 110 | .94 | ✅     |                                        |
| (0)01 - 011 - 111 | .95 | ✅     |                                        |
| (0)01 - 100 - 000 | .96 | ❌     |                                        |
| (1)01 - 100 - 001 | .97 | ✅     | Slot 1 needs to refuel                 |
| (1)01 - 100 - 010 | .98 | ✅     | Slot 1 does not need to refuel anymore |
| (0)01 - 100 - 011 | .99 | ✅     | Slot 1 starts refueling                |
| (1)01 - 100 - 100 | 100 | ✅     | Slot 1 stops refueling                 |
| (0)01 - 100 - 101 | 101 | ✅     |                                        |
| (0)01 - 100 - 110 | 102 | ✅     |                                        |
| (1)01 - 100 - 111 | 103 | ✅     |                                        |
| (1)01 - 101 - 000 | 104 | ❌     |                                        |
| (0)01 - 101 - 001 | 105 | ✅     | Slot 2 needs to refuel                 |
| (0)01 - 101 - 010 | 106 | ✅     | Slot 2 does not need to refuel anymore |
| (1)01 - 101 - 011 | 107 | ✅     | Slot 2 starts refueling                |
| (0)01 - 101 - 100 | 108 | ✅     | Slot 2 stops refueling                 |
| (1)01 - 101 - 101 | 109 | ✅     |                                        |
| (1)01 - 101 - 110 | 110 | ✅     |                                        |
| (0)01 - 101 - 111 | 111 | ✅     |                                        |
| (1)01 - 110 - 000 | 112 | ❌     |                                        |
| (0)01 - 110 - 001 | 113 | ✅     | Slot 3 needs to refuel                 |
| (0)01 - 110 - 010 | 114 | ✅     | Slot 3 does not need to refuel anymore |
| (1)01 - 110 - 011 | 115 | ✅     | Slot 3 starts refueling                |
| (0)01 - 110 - 100 | 116 | ✅     | Slot 3 stops refueling                 |
| (1)01 - 110 - 101 | 117 | ✅     |                                        |
| (1)01 - 110 - 110 | 118 | ✅     |                                        |
| (0)01 - 110 - 111 | 119 | ✅     |                                        |
| (0)01 - 111 - 000 | 120 | ❌     |                                        |
| (1)01 - 111 - 001 | 121 | ✅     | Slot 4 needs to refuel                 |
| (1)01 - 111 - 010 | 122 | ✅     | Slot 4 does not need to refuel anymore |
| (0)01 - 111 - 011 | 123 | ✅     | Slot 4 starts refueling                |
| (1)01 - 111 - 100 | 124 | ✅     | Slot 4 stops refueling                 |
| (0)01 - 111 - 101 | 125 | ✅     |                                        |
| (0)01 - 111 - 110 | 126 | ✅     |                                        |
| (1)01 - 111 - 111 | 127 | ✅     |                                        |
| (1)10 - 000 - 000 | 128 | ❌     |                                        |
| (0)10 - 000 - 001 | 129 | ❌     |                                        |
| (0)10 - 000 - 010 | 130 | ❌     |                                        |
| (1)10 - 000 - 011 | 131 | ❌     |                                        |
| (0)10 - 000 - 100 | 132 | ❌     |                                        |
| (1)10 - 000 - 101 | 133 | ❌     |                                        |
| (1)10 - 000 - 110 | 134 | ❌     |                                        |
| (0)10 - 000 - 111 | 135 | ❌     |                                        |
| (0)10 - 001 - 000 | 136 | ❌     |                                        |
| (1)10 - 001 - 001 | 137 | ✅     | Slot 5 needs to refuel                 |
| (1)10 - 001 - 010 | 138 | ✅     | Slot 5 does not need to refuel anymore |
| (0)10 - 001 - 011 | 139 | ✅     | Slot 5 starts refueling                |
| (1)10 - 001 - 100 | 140 | ✅     | Slot 5 stops refueling                 |
| (0)10 - 001 - 101 | 141 | ✅     |                                        |
| (0)10 - 001 - 110 | 142 | ✅     |                                        |
| (1)10 - 001 - 111 | 143 | ✅     |                                        |
| (0)10 - 010 - 000 | 144 | ❌     |                                        |
| (1)10 - 010 - 001 | 145 | ✅     | Slot 6 needs to refuel                 |
| (1)10 - 010 - 010 | 146 | ✅     | Slot 6 does not need to refuel anymore |
| (0)10 - 010 - 011 | 147 | ✅     | Slot 6 starts refueling                |
| (1)10 - 010 - 100 | 148 | ✅     | Slot 6 stops refueling                 |
| (0)10 - 010 - 101 | 149 | ✅     |                                        |
| (0)10 - 010 - 110 | 150 | ✅     |                                        |
| (1)10 - 010 - 111 | 151 | ✅     |                                        |
| (1)10 - 011 - 000 | 152 | ❌     |                                        |
| (0)10 - 011 - 001 | 153 | ✅     |                                        |
| (0)10 - 011 - 010 | 154 | ✅     |                                        |
| (1)10 - 011 - 011 | 155 | ✅     |                                        |
| (0)10 - 011 - 100 | 156 | ✅     |                                        |
| (1)10 - 011 - 101 | 157 | ✅     |                                        |
| (1)10 - 011 - 110 | 158 | ✅     |                                        |
| (0)10 - 011 - 111 | 159 | ✅     |                                        |
| (0)10 - 100 - 000 | 160 | ❌     |                                        |
| (1)10 - 100 - 001 | 161 | ✅     |                                        |
| (1)10 - 100 - 010 | 162 | ✅     |                                        |
| (0)10 - 100 - 011 | 163 | ✅     |                                        |
| (1)10 - 100 - 100 | 164 | ✅     |                                        |
| (0)10 - 100 - 101 | 165 | ✅     |                                        |
| (0)10 - 100 - 110 | 166 | ✅     |                                        |
| (1)10 - 100 - 111 | 167 | ✅     |                                        |
| (1)10 - 101 - 000 | 168 | ❌     |                                        |
| (0)10 - 101 - 001 | 169 | ✅     |                                        |
| (0)10 - 101 - 010 | 170 | ✅     |                                        |
| (1)10 - 101 - 011 | 171 | ✅     |                                        |
| (0)10 - 101 - 100 | 172 | ✅     |                                        |
| (1)10 - 101 - 101 | 173 | ✅     |                                        |
| (1)10 - 101 - 110 | 174 | ✅     |                                        |
| (0)10 - 101 - 111 | 175 | ✅     |                                        |
| (1)10 - 110 - 000 | 176 | ❌     |                                        |
| (0)10 - 110 - 001 | 177 | ✅     |                                        |
| (0)10 - 110 - 010 | 178 | ✅     |                                        |
| (1)10 - 110 - 011 | 179 | ✅     |                                        |
| (0)10 - 110 - 100 | 180 | ✅     |                                        |
| (1)10 - 110 - 101 | 181 | ✅     |                                        |
| (1)10 - 110 - 110 | 182 | ✅     |                                        |
| (0)10 - 110 - 111 | 183 | ✅     |                                        |
| (0)10 - 111 - 000 | 184 | ❌     |                                        |
| (1)10 - 111 - 001 | 185 | ✅     |                                        |
| (1)10 - 111 - 010 | 186 | ✅     |                                        |
| (0)10 - 111 - 011 | 187 | ✅     |                                        |
| (1)10 - 111 - 100 | 188 | ✅     |                                        |
| (0)10 - 111 - 101 | 189 | ✅     |                                        |
| (0)10 - 111 - 110 | 190 | ✅     |                                        |
| (1)10 - 111 - 111 | 191 | ✅     |                                        |
| (0)11 - 000 - 000 | 192 | ❌     |                                        |
| (1)11 - 000 - 001 | 193 | ❌     |                                        |
| (1)11 - 000 - 010 | 194 | ❌     |                                        |
| (0)11 - 000 - 011 | 195 | ❌     |                                        |
| (1)11 - 000 - 100 | 196 | ❌     |                                        |
| (0)11 - 000 - 101 | 197 | ❌     |                                        |
| (0)11 - 000 - 110 | 198 | ❌     |                                        |
| (1)11 - 000 - 111 | 199 | ❌     |                                        |
| (1)11 - 001 - 000 | 200 | ❌     |                                        |
| (0)11 - 001 - 001 | 201 | ✅     |                                        |
| (0)11 - 001 - 010 | 202 | ✅     |                                        |
| (1)11 - 001 - 011 | 203 | ✅     |                                        |
| (0)11 - 001 - 100 | 204 | ✅     |                                        |
| (1)11 - 001 - 101 | 205 | ✅     |                                        |
| (1)11 - 001 - 110 | 206 | ✅     |                                        |
| (0)11 - 001 - 111 | 207 | ✅     |                                        |
| (1)11 - 010 - 000 | 208 | ❌     |                                        |
| (0)11 - 010 - 001 | 209 | ✅     |                                        |
| (0)11 - 010 - 010 | 210 | ✅     |                                        |
| (1)11 - 010 - 011 | 211 | ✅     |                                        |
| (0)11 - 010 - 100 | 212 | ✅     |                                        |
| (1)11 - 010 - 101 | 213 | ✅     |                                        |
| (1)11 - 010 - 110 | 214 | ✅     |                                        |
| (0)11 - 010 - 111 | 215 | ✅     |                                        |
| (0)11 - 011 - 000 | 216 | ❌     |                                        |
| (1)11 - 011 - 001 | 217 | ✅     |                                        |
| (1)11 - 011 - 010 | 218 | ✅     |                                        |
| (0)11 - 011 - 011 | 219 | ✅     |                                        |
| (1)11 - 011 - 100 | 220 | ✅     |                                        |
| (0)11 - 011 - 101 | 221 | ✅     |                                        |
| (0)11 - 011 - 110 | 222 | ✅     |                                        |
| (1)11 - 011 - 111 | 223 | ✅     |                                        |
| (1)11 - 100 - 000 | 224 | ❌     |                                        |
| (0)11 - 100 - 001 | 225 | ✅     |                                        |
| (0)11 - 100 - 010 | 226 | ✅     |                                        |
| (1)11 - 100 - 011 | 227 | ✅     |                                        |
| (0)11 - 100 - 100 | 228 | ✅     |                                        |
| (1)11 - 100 - 101 | 229 | ✅     |                                        |
| (1)11 - 100 - 110 | 230 | ✅     |                                        |
| (0)11 - 100 - 111 | 231 | ✅     |                                        |
| (0)11 - 101 - 000 | 232 | ❌     |                                        |
| (1)11 - 101 - 001 | 233 | ✅     |                                        |
| (1)11 - 101 - 010 | 234 | ✅     |                                        |
| (0)11 - 101 - 011 | 235 | ✅     |                                        |
| (1)11 - 101 - 100 | 236 | ✅     |                                        |
| (0)11 - 101 - 101 | 237 | ✅     |                                        |
| (0)11 - 101 - 110 | 238 | ✅     |                                        |
| (1)11 - 101 - 111 | 239 | ✅     |                                        |
| (0)11 - 110 - 000 | 240 | ❌     |                                        |
| (1)11 - 110 - 001 | 241 | ✅     |                                        |
| (1)11 - 110 - 010 | 242 | ✅     |                                        |
| (0)11 - 110 - 011 | 243 | ✅     |                                        |
| (1)11 - 110 - 100 | 244 | ✅     |                                        |
| (0)11 - 110 - 101 | 245 | ✅     |                                        |
| (0)11 - 110 - 110 | 246 | ✅     |                                        |
| (1)11 - 110 - 111 | 247 | ✅     |                                        |
| (1)11 - 111 - 000 | 248 | ❌     |                                        |
| (0)11 - 111 - 001 | 249 | ✅     |                                        |
| (0)11 - 111 - 010 | 250 | ✅     |                                        |
| (1)11 - 111 - 011 | 251 | ✅     |                                        |
| (0)11 - 111 - 100 | 252 | ✅     |                                        |
| (1)11 - 111 - 101 | 253 | ✅     |                                        |
| (1)11 - 111 - 110 | 254 | ✅     |                                        |
| (0)11 - 111 - 111 | 255 | ✅     |                                        |