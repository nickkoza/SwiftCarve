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

#include "Config.h"
#include "Relay.h"
#include "LED.h"
#include "Pump.h"
#include "AudioPlayer.h"
#include "FlickerLight.h"
#include "Fogger.h"
#include "Show.h"

FlickerLight *forwardLight;
Show *show;
Relay *motor;
unsigned long motorStartedAt;
boolean playMotorSustain;
AudioPlayer *audioPlayer;

void setup() {
	Serial.begin(9600);

	motor = new Relay(MOTOR_RELAY_PIN);
	motor->open();

	BrightnessRange forwardBrightness;
	forwardBrightness.min = 100;
	forwardBrightness.max = 255;

	IdleDurationRange forwardIdleDuration;
	forwardIdleDuration.min = 250;
	forwardIdleDuration.max = 2500;

	TransitionDurationRange forwardTransitionDuration;
	forwardTransitionDuration.min = 250;
	forwardTransitionDuration.max = 2000;

	SustainDurationRange forwardSustainDuration;
	forwardSustainDuration.min = 250;
	forwardSustainDuration.max = 2500;
	forwardLight = new FlickerLight(new LED(FORWARD_LIGHT_PIN), forwardBrightness, forwardIdleDuration, forwardTransitionDuration, forwardSustainDuration);
	forwardLight->start();


	BrightnessRange flickerBrightness;
	flickerBrightness.min = 0;
	flickerBrightness.max = 255;

	IdleDurationRange flickerIdleDuration;
	flickerIdleDuration.min = 250;
	flickerIdleDuration.max = 1250;

	TransitionDurationRange flickerTransitionDuration;
	flickerTransitionDuration.min = 20;
	flickerTransitionDuration.max = 50;

	SustainDurationRange flickerSustainDuration;
	flickerSustainDuration.min = 20;
	flickerSustainDuration.max = 50;

    audioPlayer = new AudioPlayer(SANDISK_CHIP_SELECT_PIN, SPEAKER_PIN);

	show = new Show(
		new Fogger(new Relay(FOG_RELAY_PIN)),
		new Pump(new Relay(PUMP_RELAY_PIN)),
		new FlickerLight(new LED(SPARK_LIGHT_PIN_1), flickerBrightness, flickerIdleDuration, flickerTransitionDuration, flickerSustainDuration),
		new FlickerLight(new LED(SPARK_LIGHT_PIN_2), flickerBrightness, flickerIdleDuration, flickerTransitionDuration, flickerSustainDuration),
		audioPlayer
	);

	pinMode(SHOW_TRIGGER_PIN, INPUT);
	pinMode(MOTOR_TRIGGER_PIN, INPUT);
}

void loop() {
    //audioPlayer->playLoopedIfSilent("SUSTAIN");
    //audioPlayer->playIfSilent("SUSTAIN.wav");

	if (digitalRead(SHOW_TRIGGER_PIN) == HIGH) {
	    Serial.println("SHOW TRIGGER PIN HIGH");
	    show->attemptToStartShow();
	}

	if (digitalRead(MOTOR_TRIGGER_PIN) == HIGH) {
	    if (motor->isOpen()) {
		    motor->close();
            motorStartedAt = millis();
            playMotorSustain = true;
            audioPlayer->play("START.wav");
	    }

        if (millis() - motorStartedAt >= 150 && playMotorSustain) {
            Serial.println("MOTOR TRIGGER");
            audioPlayer->playLooped("SUSTAIN.wav");
            playMotorSustain = false;
        }
	}
	else {
	    if (motor->isClosed()) {
		    motor->open();
            audioPlayer->play("END.wav");
	    }
	}
  
	//show->loop();
	//forwardLight->loop();
}
