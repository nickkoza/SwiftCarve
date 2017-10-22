#pragma once
//#ifndef _FLICKER_LIGHT_h
//#define _FLICKER_LIGHT_h

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

//#endif