#include <avr/sleep.h>

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
  pinMode(PALETTE_PIN, OUTPUT);
  digitalWrite(PALETTE_PIN, HIGH);
  pinMode(BRIGHTNESS_PIN, OUTPUT);
  digitalWrite(BRIGHTNESS_PIN, HIGH);

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
      digitalWrite(PALETTE_PIN, digitalRead(A_PIN));
      digitalWrite(BRIGHTNESS_PIN, digitalRead(B_PIN));
    }
    delay(5);
  }

  digitalWrite(PALETTE_PIN, HIGH);
  digitalWrite(BRIGHTNESS_PIN, HIGH);
}

void startPressed() {
  detachInterrupt(0);
}
