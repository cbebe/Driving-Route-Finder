#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <SPI.h>
#include <SD.h>
#include "joy_cursor.h"

void setup() {
  joySetup();
}

int main() {
  setup();
  while (1) {
    processJoystick();
  }
  return 0;
}