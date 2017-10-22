#include "Pump.h"

Pump::Pump(Relay *relay) {
	this->relay = relay;
}

boolean Pump::isRunning() {
	return relay->isClosed();
}

boolean Pump::isNotRunning() {
	return !isRunning();
}

void Pump::start() {
	relay->close();
	Serial.println("PUMP - Start");
}

void Pump::shutdown() {
	relay->open();
	Serial.println("PUMP - Shutdown");
}