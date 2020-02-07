#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <SPI.h>
#include <SD.h>
#include "joy_cursor.h"

void setup() {
  init();
  Serial.begin(9600);
  // sets up Arduino for Mode 0
  joySetup();
  pinMode(JOY_SEL, INPUT);
  digitalWrite(JOY_SEL, HIGH);
}

int main() {
  setup();
  while (1) {
    processJoystick();
    if (digitalRead(JOY_SEL) == LOW) {
      Mode1();
      mapInit();
    }
  }
  Serial.end();
  return 0;
}