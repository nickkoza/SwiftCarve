// Relay.h

#ifndef _RELAY_h
#define _RELAY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "ArduinoMock.h"
#endif

class Relay {
private:
	const int openState = LOW;
	const int closedState = HIGH;
	int pin;
	boolean state;
public:
	Relay(int pin);
	void open();
	void close();
	boolean isOpen();
	boolean isClosed();
};

#endif
