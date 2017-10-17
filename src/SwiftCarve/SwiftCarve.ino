// Written by Nicholas Koza
// All rights reserved. Not for redistribution or use without permission.
#include <pcmRF.h>
#include <pcmConfig.h>
#include "SD.h"
#define SD_ChipSelectPin 4
#include "TMRpcm.h"
#include "SPI.h"

#include "Relay.h"
#define SHOW_TRIGGER_PIN         14 // INPUT. The fog, the pump, and the spark lights are together known as "show"
#define MOTOR_TRIGGER_PIN        15 // INPUT
#define MOTOR_RELAY_PIN          8  // OUTPUT
#define FOG_RELAY_PIN            7  // OUTPUT
#define PUMP_RELAY_PIN           2  // OUTPUT
#define FORWARD_LIGHT_PIN        3  // PWM
#define SPARK_LIGHT_PIN_1        5  // PWM
#define SPARK_LIGHT_PIN_2        9  // PWM

#define PREHEAT_DELAY_IN_MS       500         // Give the coils a bit to warm up before the air pump enables
#define FOG_DURATION_IN_MS        (1000 * 1)  // How long to run the fogger heat coils for
#define PUMP_COOLDOWN_DELAY_IN_MS (1000 * 3)  // Leaves the pump on for a little bit while the fogger is off, to help cool it down and pump out any last fog
#define FOG_COOLDOWN_DELAY_IN_MS  (1000 * 5)  // Lock out the fog system from running again for a duration, to give it time to cool back down

struct IntRange {
  int min;
  int max;
};


class LED {
private:
  int pin;
public:
  LED(int pin) {
    this->pin = pin;
    pinMode(pin, OUTPUT);
    analogWrite(pin, 0);
  }

  void set(int brightness) {
    analogWrite(pin, brightness);
  }
};


class Pump {
private:
  Relay *relay;

public:
  Pump(Relay *relay) {
    this->relay = relay;
  }

  boolean isRunning() {
    return relay->isClosed();
  }

  boolean isNotRunning() {
    return !isRunning();
  }

  void start() {
    relay->close();
    Serial.println("PUMP - Start");
  }

  void shutdown() {
    relay->open();
    Serial.println("PUMP - Shutdown");
  }
};


class Fogger {
private:
  Relay *relay;
  unsigned long lastShutdownAt;
  unsigned long startedAt;
  
public:
  Fogger(Relay *relay) {
    this->relay = relay;
    lastShutdownAt = 0;
  }

  void loop() {

    // Make sure to automatically shutdown if we've hit the max run duration
    if (isRunning() && startedAt + FOG_DURATION_IN_MS) {
      shutdown();
    }
  }

  boolean canRun() {
    // Make sure we don't turn back on within the cooldown period
    return isNotRunning() && millis() > lastShutdownAt + FOG_COOLDOWN_DELAY_IN_MS;
  }

  boolean isRunning() {
    relay->isClosed();
  }

  boolean isNotRunning() {
    return !isRunning();
  }

  boolean start() {
    if (canRun()) {
      relay->close();
      startedAt = millis();
      Serial.println("FOGGER - Start");
    }
  }

  boolean shutdown() {
    relay->open();
    lastShutdownAt = millis();
    Serial.println("FOGGER - Shutdown");
  }
};

typedef IntRange BrightnessRange;
typedef IntRange IdleDurationRange;
typedef IntRange TransitionDurationRange;
typedef IntRange SustainDurationRange;

class FlickerLight {
private:
  enum State {
    STOPPED = 0,
    IDLING = 1,
    TRANSITION_UP = 2,
    SUSTAIN = 3,
    TRANSITION_DOWN = 4
  };

  LED *led;
  unsigned long stateEntryTime;
  State state;
  BrightnessRange brightness;
  IdleDurationRange idleDuration;
  TransitionDurationRange transitionDuration;
  SustainDurationRange sustainDuration;

  int currentIdle;
  int currentBrightness;
  int currentTransition;
  int currentSustain;

public:
  FlickerLight(LED *led, BrightnessRange brightness, IdleDurationRange idleDuration,  TransitionDurationRange transitionDuration, SustainDurationRange sustainDuration) {
    this->led = led;
    this->brightness = brightness;
    this->idleDuration = idleDuration; 
    this->transitionDuration = transitionDuration;
    this->sustainDuration = sustainDuration;
    goToState(STOPPED);

	
  }

  void loop() {
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

  void goToState(State newState) {
    state = newState;
    stateEntryTime = millis();

    if (state == IDLING) {
      currentIdle = random(idleDuration.min, idleDuration.max);
    }
    else if(state == TRANSITION_UP || state == TRANSITION_DOWN) {
      currentTransition = random(transitionDuration.min, transitionDuration.max);
    }
    else if(state == SUSTAIN) {
      currentSustain = random(sustainDuration.min, sustainDuration.max);
    }
    
    //Serial.print("FLICKERLIGHT - went to state ");
    //Serial.print(newState);
    //Serial.println("");
  }

  void start() {
    goToState(IDLING);
    Serial.println("FLICKERLIGHT - Start");
  }

  void stop() {
    goToState(STOPPED);
    Serial.println("FLICKERLIGHT - Stop");
  }
};

class Show {
private:
  enum State {
    STOPPED = 0,
    WARM_UP = 1,
    FOGGING = 2,
    COOLDOWN = 3
  };
  
  unsigned long stateEntryTime;
  Fogger *fogger;
  Pump *pump;
  FlickerLight *sparkLight1;
  FlickerLight *sparkLight2;
  State state;

  void goToState(State newState) {
    state = newState;
    stateEntryTime = millis();
    Serial.print("SHOW - went to state ");
    Serial.print(newState);
    Serial.println("");
  }

public:
  Show(Fogger *fogger, Pump *pump, FlickerLight *sparkLight1, FlickerLight *sparkLight2) {
    this->fogger = fogger;
    this->pump = pump;
    this->sparkLight1 = sparkLight1;
    this->sparkLight2 = sparkLight2;
    sparkLight1->stop();
    sparkLight2->stop();
    //sparkLight1->start();
    //sparkLight2->start();
    state = STOPPED;
    stateEntryTime = 0;
  }
  
  void attemptToStartShow() {
    unsigned long currentTime = millis();
    if (state == STOPPED && fogger->canRun()) {
      goToState(WARM_UP);
      fogger->start();
    }
  }

  void loop() {
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
    }
  }
};


FlickerLight *forwardLight;
Show *show;
Relay *motor;
TMRpcm *audio;
unsigned long lastOutputTime;

void setup() {
	Serial.begin(9600);
	lastOutputTime = millis();

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

	show = new Show(
		new Fogger(new Relay(FOG_RELAY_PIN)),
		new Pump(new Relay(PUMP_RELAY_PIN)),
		new FlickerLight(new LED(SPARK_LIGHT_PIN_1), flickerBrightness, flickerIdleDuration, flickerTransitionDuration, flickerSustainDuration),
		new FlickerLight(new LED(SPARK_LIGHT_PIN_2), flickerBrightness, flickerIdleDuration, flickerTransitionDuration, flickerSustainDuration)
	);

	pinMode(SHOW_TRIGGER_PIN, INPUT);
	pinMode(MOTOR_TRIGGER_PIN, INPUT);

	if (SD.begin(SD_ChipSelectPin)) {
		Serial.println("Booting up audio");
		audio = new TMRpcm();
		audio->speakerPin = 9;
	}
	else {
		audio = NULL;
		Serial.println("Could not get SD card. Not initializing audio.");
		return;   // don't do anything more if not
	}
	
	if (audio != NULL) {
		audio->play("6.wav");
		delay(1000);
		audio->play("6.wav");
	}
}

void loop() {
	/*show->attemptToStartShow();
	if (motor->isOpen()) {
		motor->close();
	}*/

	if (audio != NULL) {
		if (!audio->isPlaying()) {
			audio->play("4.wav");
		}
	}

	if (digitalRead(SHOW_TRIGGER_PIN) == HIGH) {
	Serial.println("SHOW TRIGGER PIN HIGH");
	show->attemptToStartShow();
	}

	/*if (digitalRead(MOTOR_TRIGGER_PIN) == HIGH) {
	Serial.println("MOTOR TRIGGER PIN HIGH");
	if (motor->isOpen()) {
		motor->close();
	}
	}
	else {
	if (motor->isClosed()) {
		motor->open();
	}
	}*/
  
	show->loop();
	forwardLight->loop();
}
