/*
Copyright(C) 2017 Nicholas Koza

This file is part of SwiftCarve.

This program is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/

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
