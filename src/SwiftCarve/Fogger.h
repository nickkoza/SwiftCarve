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