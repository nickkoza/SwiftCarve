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

#ifndef _FOGGER_h
#define _FOGGER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "ArduinoMock.h"
#endif

#include "Relay.h"

class Fogger {
private:
    Relay *relay;
    unsigned long lastShutdownAt;
    unsigned long startedAt;

public:
    Fogger(Relay *relay);
    void loop();
    boolean canRun();
    boolean isRunning();
    boolean isNotRunning();
    boolean start();
    boolean shutdown();
};
#endif