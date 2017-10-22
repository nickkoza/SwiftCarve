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