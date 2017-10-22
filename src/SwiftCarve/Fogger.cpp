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

#include "Fogger.h"
#include "Config.h"

Fogger::Fogger(Relay *relay) {
    this->relay = relay;
    lastShutdownAt = 0;
}

void Fogger::loop() {
    // Make sure to automatically shutdown if we've hit the max run duration
    if (isRunning() && startedAt + FOG_DURATION_IN_MS) {
        shutdown();
    }
}

boolean Fogger::canRun() {
    // Make sure we don't turn back on within the cooldown period
    return isNotRunning() && millis() > lastShutdownAt + FOG_COOLDOWN_DELAY_IN_MS;
}

boolean Fogger::isRunning() {
    relay->isClosed();
}

boolean Fogger::isNotRunning() {
    return !isRunning();
}

boolean Fogger::start() {
    if (canRun()) {
        relay->close();
        startedAt = millis();
        Serial.println("FOGGER - Start");
    }
}

boolean Fogger::shutdown() {
    relay->open();
    lastShutdownAt = millis();
    Serial.println("FOGGER - Shutdown");
}
