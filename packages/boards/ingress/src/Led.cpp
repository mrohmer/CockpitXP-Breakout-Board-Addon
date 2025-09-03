#include "Led.h"

Led::Led(uint8_t pin) {
    this->pin = pin;
}
void Led::init() {
    pinMode(this->pin, OUTPUT);
    digitalWrite(this->pin, HIGH);
}

void Led::on() {
    digitalWrite(this->pin, LOW);
}
void Led::off() {
    digitalWrite(this->pin, HIGH);
}
void Led::toggle() {
    digitalWrite(this->pin, !digitalRead(this->pin));
}
void Led::flash(int times) {
    while (times-- > 0) {
        this->toggle();
        delay(100);
        this->toggle();
        delay(100);
    }
}