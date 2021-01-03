#include <avr/sleep.h>

#define PULL_SENSOR_PINS_HIGH true // change to false if leaving touch sensor ICs installed

#define START_PIN 2 // interrupt pin
#define A_PIN 0
#define B_PIN 1
#define PALETTE_PIN 4 
#define BRIGHTNESS_PIN 3

unsigned long pressedMillis = 0;

void setup() {
  pinMode(START_PIN, INPUT);
  pinMode(A_PIN, INPUT);
  pinMode(B_PIN, INPUT);
  setSensorPinHigh(PALETTE_PIN);
  setSensorPinHigh(BRIGHTNESS_PIN);

  delay(5);
}

void loop() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  cli();
  attachInterrupt(0, startPressed, LOW);
  sei();
  sleep_cpu();

  // wakes here with interrupt
  
  sleep_disable();

  pressedMillis = millis();
  while (!digitalRead(START_PIN)) {
    if ((millis() - pressedMillis) > 1000) {
      setSensorPinFromButtonPin(PALETTE_PIN, A_PIN);
      setSensorPinFromButtonPin(BRIGHTNESS_PIN, B_PIN);
    }
    delay(5);
  }

  setSensorPinHigh(PALETTE_PIN);
  setSensorPinHigh(BRIGHTNESS_PIN);
}

void startPressed() {
  detachInterrupt(0);
}

void setSensorPinFromButtonPin(byte sensorPin, byte buttonPin) {
  if (digitalRead(buttonPin)) {
    setSensorPinHigh(sensorPin);
  } else {
    digitalWrite(sensorPin, LOW);
    pinMode(sensorPin, OUTPUT);
  }
}

void setSensorPinHigh(byte sensorPin) {
  if (PULL_SENSOR_PINS_HIGH) {
    digitalWrite(sensorPin, HIGH);
    pinMode(sensorPin, OUTPUT); 
  } else {
    pinMode(sensorPin, INPUT); 
  }
}
