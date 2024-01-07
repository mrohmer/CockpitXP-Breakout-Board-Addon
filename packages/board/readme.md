# Cockpit XP Breakout Board Addon - The Board

## Prerequisite

This project is using [platformio](https://platformio.org/).
Therefore, you need to have the cli installed (see [here](https://platformio.org/install/cli)).

## Wiring Esp8266

| Pin | GPIO   | Name            |
|-----|--------|-----------------|
| D4  | GPIO2  | Internal Led    |
| D1  | GPIO5  | Clock           |
| D6  | GPIO12 | Data 1          |
| RX  | GPIO3  | Data 2          |
| D2  | GPIO4  | Data 3          |
| D8  | GPIO15 | Start Light CS  |
| D7  | GPIO13 | Start Light DIN |
| D5  | GPIO14 | Start Light CLK |