#include "Fogger.h"
#include "Config.h"

Fogger::Fogger(Relay *relay) {
    this->relay = relay;
    lastShutdownAt = 0;
}

void Fogger::loop() {
    // Make sure to automatically shutdown if we've hit the max run duration
    if (isRunning() && startedAt + FOG_DURATION_IN_MS) {
        shutdown();
    }
}

boolean Fogger::canRun() {
    // Make sure we don't turn back on within the cooldown period
    return isNotRunning() && millis() > lastShutdownAt + FOG_COOLDOWN_DELAY_IN_MS;
}

boolean Fogger::isRunning() {
    relay->isClosed();
}

boolean Fogger::isNotRunning() {
    return !isRunning();
}

boolean Fogger::start() {
    if (canRun()) {
        relay->close();
        startedAt = millis();
        Serial.println("FOGGER - Start");
    }
}

boolean Fogger::shutdown() {
    relay->open();
    lastShutdownAt = millis();
    Serial.println("FOGGER - Shutdown");
}
