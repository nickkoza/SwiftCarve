// Written by Nicholas Koza
// All rights reserved. Not for redistribution or use without permission.
#define SHOW_TRIGGER_PIN         4  // INPUT. The fog, the pump, and the spark lights are together known as "show"
#define MOTOR_TRIGGER_PIN        5  // INPUT
#define MOTOR_RELAY_PIN          6  // OUTPUT
#define FOG_RELAY_PIN            7  // OUTPUT
#define PUMP_RELAY_PIN           8  // OUTPUT
#define FORWARD_LIGHT_PIN        9  // PWM
#define SPARK_LIGHT_PIN_1        10 // PWM
#define SPARK_LIGHT_PIN_2        11 // PWM

#define PREHEAT_DELAY_IN_MS       250         // Give the coils a bit to warm up before the air pump enables
#define FOG_DURATION_IN_MS        (1000 * 2)  // How long to run the fogger heat coils for
#define PUMP_COOLDOWN_DELAY_IN_MS (1000 * 1)  // Leaves the pump on for a little bit while the fogger is off, to help cool it down and pump out any last fog
#define FOG_COOLDOWN_DELAY_IN_MS  (1000 * 5)  // Lock out the fog system from running again for a duration, to give it time to cool back down

struct IntRange {
  int min;
  int max;
};

class Relay {
private:
  const int openState = HIGH;   // Yes, this relay board is reversed and has a high open
  const int closedState = LOW;
  int pin;
  boolean state;
public:
  Relay(int pin) {
    this->pin = pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, openState);
    state = openState;
  }

  void open() {
    digitalWrite(pin, openState);
    state = openState;
    Serial.print("RELAY - PIN ");
    Serial.print(pin);
    Serial.println(" - Open");
  }

  void close() {
    digitalWrite(pin, closedState);
    state = closedState;
    Serial.print("RELAY - PIN ");
    Serial.print(pin);
    Serial.println(" - Close");
  }

  boolean isOpen() {
    return state == openState;
  }

  boolean isClosed() {
    return state == closedState;
  }
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
  LED *led;
  boolean running;
  BrightnessRange brightness;
  IdleDurationRange idleDuration;
  TransitionDurationRange transitionDuration;
  SustainDurationRange sustainDuration;

public:
  FlickerLight(LED *led, BrightnessRange brightness, IdleDurationRange idleDuration,  TransitionDurationRange transitionDuration, SustainDurationRange sustainDuration) {
    this->led = led;
    this->brightness = brightness;
    this->idleDuration = idleDuration; 
    this->transitionDuration = transitionDuration;
    this->sustainDuration = sustainDuration;
    running = false;
  }

  void loop() {
    // TODO
  }

  void start() {
    running = true;
    Serial.print("FLICKERLIGHT - Start");
  }

  void stop() {
    running = true;
    Serial.print("FLICKERLIGHT - Stop");
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
    state = STOPPED;
    stateEntryTime = 0;
  }
  
  void attemptToStartShow() {
    unsigned long currentTime = millis();
    if (state == STOPPED && fogger->canRun()) {
      goToState(WARM_UP);
      fogger->start();
      sparkLight1->start();
      sparkLight2->start();
    }
  }

  void loop() {
    fogger->loop();
    sparkLight1->loop();
    sparkLight2->loop();

    unsigned long currentTime = millis();
    if (state == WARM_UP && currentTime > stateEntryTime + PREHEAT_DELAY_IN_MS) {
      goToState(FOGGING);
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

void setup() {
  Serial.begin(9600);

  BrightnessRange forwardBrightness;
  forwardBrightness.min = 200;
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
  forwardLight.start();


  BrightnessRange flickerBrightness;
  flickerBrightness.min = 0;
  flickerBrightness.max = 255;

  IdleDurationRange flickerIdleDuration;
  flickerIdleDuration.min = 250;
  flickerIdleDuration.max = 1000;
  
  TransitionDurationRange flickerTransitionDuration;
  flickerTransitionDuration.min = 1;
  flickerTransitionDuration.max = 5;
  
  SustainDurationRange flickerSustainDuration;
  flickerSustainDuration.min = 20;
  flickerSustainDuration.max = 35;

  show = new Show(
    new Fogger(new Relay(FOG_RELAY_PIN)),
    new Pump(new Relay(PUMP_RELAY_PIN)),
    new FlickerLight(new LED(SPARK_LIGHT_PIN_1), flickerBrightness, flickerIdleDuration, flickerTransitionDuration, flickerSustainDuration),
    new FlickerLight(new LED(SPARK_LIGHT_PIN_2), flickerBrightness, flickerIdleDuration, flickerTransitionDuration, flickerSustainDuration)
  );
}

void loop() {
  show->loop();
  forwardLight->loop();
}
