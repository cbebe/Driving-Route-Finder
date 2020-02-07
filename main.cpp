#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <SPI.h>
#include <SD.h>
#include "joy_cursor.h"

#define MODE_SWITCH 53
void setup() {
  init();
  Serial.begin(9600);
  // sets up Arduino for Mode 0
  joySetup();
  pinMode(MODE_SWITCH, INPUT);
  digitalWrite(MODE_SWITCH, HIGH);
}

int main() {
  setup();
  while (1) {
    processJoystick();
    if (digitalRead(MODE_SWITCH) == LOW) {
      Mode1();
    }
  }
  Serial.end();
  return 0;
}