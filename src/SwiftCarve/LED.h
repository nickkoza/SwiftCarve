#ifndef _LED_h
#define _LED_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "ArduinoMock.h"
#endif

class LED {
private:
	int pin;
public:
	LED(int pin);
	void set(int brightness);
};

#endif