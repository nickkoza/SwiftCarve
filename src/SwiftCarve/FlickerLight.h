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

#ifndef _FLICKER_LIGHT_h
#define _FLICKER_LIGHT_h

#include "LED.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "ArduinoMock.h"
#endif

struct IntRange {
    int min;
    int max;
};

typedef IntRange BrightnessRange;
typedef IntRange IdleDurationRange;
typedef IntRange TransitionDurationRange;
typedef IntRange SustainDurationRange;

class FlickerLight
{
private:
    enum State {
        STOPPED = 0,
        IDLING = 1,
        TRANSITION_UP = 2,
        SUSTAIN = 3,
        TRANSITION_DOWN = 4
    };

    LED *led;
    unsigned long stateEntryTime;
    State state;
    BrightnessRange brightness;
    IdleDurationRange idleDuration;
    TransitionDurationRange transitionDuration;
    SustainDurationRange sustainDuration;

    int currentIdle;
    int currentBrightness;
    int currentTransition;
    int currentSustain;

public:
    FlickerLight(LED *led, BrightnessRange brightness, IdleDurationRange idleDuration, TransitionDurationRange transitionDuration, SustainDurationRange sustainDuration);
    void loop();
    void goToState(State newState);
    void start();
    void stop();
};

#endif