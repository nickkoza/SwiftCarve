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

#include "Pump.h"

Pump::Pump(Relay *relay) {
	this->relay = relay;
}

boolean Pump::isRunning() {
	return relay->isClosed();
}

boolean Pump::isNotRunning() {
	return !isRunning();
}

void Pump::start() {
	relay->close();
	Serial.println("PUMP - Start");
}

void Pump::shutdown() {
	relay->open();
	Serial.println("PUMP - Shutdown");
}