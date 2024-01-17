# Cockpit XP Breakout Board Addon - The Board

## Prerequisite

This project is using [platformio](https://platformio.org/).
Therefore, you need to have the cli installed (see [here](https://platformio.org/install/cli)).

## Wiring Esp8266

| Pin | GPIO   | Name                |
|-----|--------|---------------------|
| D4  | GPIO2  | Internal Led        |
| D7  | GPIO5  | Clock               |
| D6  | GPIO12 | Pitlane             |
| D5  | GPIO14 | Race Progress       |
| D1  | GPIO5  | SCL to MCP (Yellow) |
| D2  | GPIO4  | SDA to MCP (Blue)   |

# Wiring MCP23017

| Pin | Nbr | Name   |
|-----|-----|--------|
| A7  | 7   | Data 1 |
| A6  | 6   | Data 2 |
| A5  | 7   | Data 3 |
