// Written by Nicholas Koza
// All rights reserved. Not for redistribution or use without permission.
#define SHOW_TRIGGER_PIN         4  // INPUT. The fog, the pump, and the spark lights are together known as "show"
#define MOTOR_TRIGGER_PIN        5  // INPUT
#define MOTOR_RELAY_PIN          6  // OUTPUT
#define FOG_RELAY_PIN            7  // OUTPUT
#define PUMP_RELAY_PIN           8  // OUTPUT
#define FORWARD_LIGHT_PIN        9  // PWM
#define SPARK_LIGHT_PIN          10 // PWM

#define PUMP_DURATION_IN_MS      (1000 * 3) // Give the ability to leave the pump on for longer than the fogger, to cool it down and pump out and remaining fog
#define FOG_COOLDOWN_DELAY_IN_MS (1000 * 5) // Lock out the fog system from running again for a duration, to give it time to cool back down
#define FOG_DURATION_IN_MS       (1000 * 2) // How long to run the fogger heat coils for
#define PREHEAT_DELAY_IN_MS      250        // Give the coils a bit to warm up before the air pump enables

class Relay {
private:
  const int openState = HIGH;   // Yes, this relay board is reversed from what you'd expect
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
  }

  void close() {
    digitalWrite(pin, closedState);
    state = closedState;
  }

  boolean isOpen() {
    return state == openState;
  }

  boolean isClosed() {
    return state == closedState;
  }
};


class Pump {
private:
Relay* relay;
unsigned long lastStartedAt;

public:
  Pump(Relay *relay) {
    this->relay = relay;
    lastStartedAt = 0;
  }

  boolean canRun() {
    return isNotRunning() && 
  }

  boolean isRunning() {
    return relay->isClosed();
  }

  boolean isNotRunning() {
    return !isRunning();
  }

  void start() {
    relay->close();
  }

  void shutdown() {
    relay->open();
  }
};

class Fogger {
public:
  Fogger(int relayPin) {
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, LOW);
  }

  boolean isRunning() {
    
  }

  boolean start() {
    
  }

  boolean shutdown() {
    
  }
};

class Show {
  unsigned long startTime;
  boolean showRunning;
  Fogger *fogger;
  Pump *pump;

public:
  Show(Fogger *fogger, Pump *pump, int sparkLightPin) {
    this->fogger = fogger;
    this->pump = pump;
    pinMode(sparkLightPin, OUTPUT);
    analogWrite(sparkLightPin, 0);
  }
  
  void attemptToStartShow() {
    unsigned long currentTime = millis();
    if (!showRunning && currentTime > startTime + PREHEAT_DELAY_IN_MS + PUMP_DURATION_IN_MS + FOG_COOLDOWN_DELAY_IN_MS) {
      showRunning = true;
      startTime = currentTime;
    }
  }

  void stepShow() {
    
  }
};

void setup() {
  
}

void loop() {
  

}
