#ifndef _PUMP_h
#define _PUMP_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "ArduinoMock.h"
#endif

#include "Relay.h"

class Pump {
private:
	Relay *relay;

public:
	Pump(Relay *relay);
	boolean isRunning();
	boolean isNotRunning();
	void start();
	void shutdown();
};

#endif