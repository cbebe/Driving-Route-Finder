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
}

int main() {
  setup();
  while (1) {
    processJoystick();
  }
  Serial.end();
  return 0;
}