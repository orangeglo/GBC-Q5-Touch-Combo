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
  setSensorPinsHigh();

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
      if (!digitalRead(A_PIN)) {
        setSensorPinLow(A_PIN);
      }
      if (!digitalRead(B_PIN)){
        setSensorPinLow(B_PIN);
      }
    }
    delay(5);
  }

  setSensorPinsHigh();
}

void startPressed() {
  detachInterrupt(0);
}

void setSensorPinLow(byte pin) {
  digitalWrite(PALETTE_PIN, LOW);
  pinMode(PALETTE_PIN, OUTPUT);
}

void setSensorPinsHigh() {
  if (PULL_SENSOR_PINS_HIGH) {
    digitalWrite(PALETTE_PIN, HIGH);
    pinMode(PALETTE_PIN, OUTPUT);
    digitalWrite(BRIGHTNESS_PIN, HIGH);
    pinMode(BRIGHTNESS_PIN, OUTPUT);
  } else {
    pinMode(PALETTE_PIN, INPUT);
    pinMode(BRIGHTNESS_PIN, INPUT);
  }
}
