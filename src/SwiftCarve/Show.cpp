#include "Show.h"
#include "Config.h"

Show::Show(Fogger *fogger, Pump *pump, FlickerLight *sparkLight1, FlickerLight *sparkLight2, AudioPlayer *audioPlayer) {
    this->fogger = fogger;
    this->pump = pump;
    this->sparkLight1 = sparkLight1;
    this->sparkLight2 = sparkLight2;
    this->audioPlayer = audioPlayer;
    sparkLight1->stop();
    sparkLight2->stop();
    //sparkLight1->start();
    //sparkLight2->start();
    state = STOPPED;
    stateEntryTime = 0;
}

void Show::goToState(State newState) {
    state = newState;
    stateEntryTime = millis();
    Serial.print("SHOW - went to state ");
    Serial.print(newState);
    Serial.println("");
}

void Show::attemptToStartShow() {
    unsigned long currentTime = millis();
    if (state == STOPPED && fogger->canRun()) {
        goToState(WARM_UP);
        fogger->start();
        //audioPlayer->playLooped("SPARK");
    }
}

void Show::loop() {
    fogger->loop();
    sparkLight1->loop();
    //sparkLight2->loop(); // This is causing an issue with the audio. I suspect they're sharing the same interupt.

    unsigned long currentTime = millis();
    if (state == WARM_UP && currentTime > stateEntryTime + PREHEAT_DELAY_IN_MS) {
        goToState(FOGGING);
        sparkLight1->start();
        sparkLight2->start();
        pump->start();
    }
    else if (state == FOGGING && currentTime > stateEntryTime + FOG_DURATION_IN_MS) {
        goToState(COOLDOWN);
        fogger->shutdown();
    }
    else if (state == COOLDOWN && currentTime > stateEntryTime + PUMP_COOLDOWN_DELAY_IN_MS) {
        goToState(STOPPED);
        pump->shutdown();
        sparkLight1->stop();
        sparkLight2->stop();
        audioPlayer->stop();
    }
}
