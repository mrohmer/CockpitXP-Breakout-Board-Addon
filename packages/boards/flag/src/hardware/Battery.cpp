//
// Created by kali on 9/22/25.
//

#include "Battery.h"

Battery::Battery() {
}

void Battery::init() {
    lipo.enableDebugging(); // Uncomment this line to enable helpful debug messages on Serial

    // Set up the MAX17043 LiPo fuel gauge:
    if (lipo.begin() == false) {
        Serial.println(F("MAX17043 not detected. Please check wiring. Freezing."));

        return;
    }
    lipo.quickStart();
}
double Battery::getPercentage() {
    if (this->lastSoc < 0 || (millis() - this->lastRead) > 5000) {
        this->lastRead = millis();
        this->lastSoc = this->readPercentage();
    }
    return this->lastSoc;
}
double Battery::readPercentage() {
    double soc = lipo.getSOC();
    double voltage = lipo.getVoltage();
    Serial.print("Voltage: ");
    Serial.print(voltage);  // Print the battery voltage
    Serial.println(" V");

    Serial.print("Percentage: ");
    Serial.print(soc); // Print the battery state of charge
    Serial.println(" %");

    return soc;
}