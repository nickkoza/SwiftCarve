#include "Relay.h"

Relay::Relay(int pin) {
	this->pin = pin;
	pinMode(pin, OUTPUT);
	digitalWrite(pin, openState);
	state = openState;
}

void Relay::open() {
	digitalWrite(pin, openState);
	state = openState;
	Serial.print("RELAY - PIN ");
	Serial.print(pin);
	Serial.println(" - Open");
}

void Relay::close() {
	digitalWrite(pin, closedState);
	state = closedState;
	Serial.print("RELAY - PIN ");
	Serial.print(pin);
	Serial.println(" - Close");
}

boolean Relay::isOpen() {
	return state == openState;
}

boolean Relay::isClosed() {
	return state == closedState;
}
