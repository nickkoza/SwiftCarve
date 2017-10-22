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

#ifndef _SHOW_h
#define _SHOW_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "ArduinoMock.h"
#endif

#include "Fogger.h"
#include "Pump.h"
#include "FlickerLight.h"
#include "AudioPlayer.h"

class Show {
private:
    enum State {
        STOPPED = 0,
        WARM_UP = 1,
        FOGGING = 2,
        COOLDOWN = 3
    };

    unsigned long stateEntryTime;
    Fogger *fogger;
    Pump *pump;
    FlickerLight *sparkLight1;
    FlickerLight *sparkLight2;
    AudioPlayer *audioPlayer;
    State state;

    void goToState(State newState);

public:
    Show(Fogger *fogger, Pump *pump, FlickerLight *sparkLight1, FlickerLight *sparkLight2, AudioPlayer *audioPlayer);
    void attemptToStartShow();
    void loop();
};
#endif