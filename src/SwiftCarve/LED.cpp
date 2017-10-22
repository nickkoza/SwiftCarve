#include "LED.h"

LED::LED(int pin) {
	this->pin = pin;
	pinMode(pin, OUTPUT);
	analogWrite(pin, 0);
}

void LED::set(int brightness) {
	analogWrite(pin, brightness);
}