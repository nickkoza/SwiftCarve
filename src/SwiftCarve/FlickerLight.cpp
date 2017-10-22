#include "FlickerLight.h"

FlickerLight::FlickerLight(LED *led, BrightnessRange brightness, IdleDurationRange idleDuration, TransitionDurationRange transitionDuration, SustainDurationRange sustainDuration) {
    this->led = led;
    this->brightness = brightness;
    this->idleDuration = idleDuration;
    this->transitionDuration = transitionDuration;
    this->sustainDuration = sustainDuration;
    goToState(STOPPED);
}

void FlickerLight::FlickerLight::loop() {
    unsigned long currentTime = millis();

    if (state == STOPPED) {
        led->set(0);
    }
    else if (state == IDLING) {
        led->set(brightness.min);
        if (currentTime > stateEntryTime + currentIdle) {
            goToState(TRANSITION_UP);
        }
    }
    else if (state == TRANSITION_UP) {
        float transitionBrightness = ((float)currentTime - (float)stateEntryTime) / (float)currentTransition;
        led->set(brightness.min + ((float)brightness.max - (float)brightness.min) * transitionBrightness);
        if (currentTime > stateEntryTime + currentTransition) {
            goToState(SUSTAIN);
        }
    }
    else if (state == SUSTAIN) {
        led->set(brightness.max);
        if (currentTime > stateEntryTime + currentSustain) {
            goToState(TRANSITION_DOWN);
        }
    }
    else if (state == TRANSITION_DOWN) {
        float transitionBrightness = 1.f - (((float)currentTime - (float)stateEntryTime) / (float)currentTransition);
        led->set(brightness.min + ((float)brightness.max - (float)brightness.min) * transitionBrightness);
        if (currentTime > stateEntryTime + currentTransition) {
            goToState(IDLING);
        }
    }
}

void FlickerLight::goToState(State newState) {
    state = newState;
    stateEntryTime = millis();

    if (state == IDLING) {
        currentIdle = random(idleDuration.min, idleDuration.max);
    }
    else if (state == TRANSITION_UP || state == TRANSITION_DOWN) {
        currentTransition = random(transitionDuration.min, transitionDuration.max);
    }
    else if (state == SUSTAIN) {
        currentSustain = random(sustainDuration.min, sustainDuration.max);
    }

    //Serial.print("FLICKERLIGHT - went to state ");
    //Serial.print(newState);
    //Serial.println("");
}

void FlickerLight::start() {
    goToState(IDLING);
    Serial.println("FLICKERLIGHT - Start");
}

void FlickerLight::stop() {
    goToState(STOPPED);
    Serial.println("FLICKERLIGHT - Stop");
}
