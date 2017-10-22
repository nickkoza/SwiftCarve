/*
Copyright(C) 2017 Nicholas Koza

This file is part of SwiftCarve.

This program is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/

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
