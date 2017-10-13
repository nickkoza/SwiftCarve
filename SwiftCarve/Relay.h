// Relay.h

#ifndef _RELAY_h
#define _RELAY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class Relay {
private:
	const int openState = HIGH;
	const int closedState = LOW;
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
