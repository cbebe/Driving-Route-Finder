#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <SPI.h>
#include <SD.h>
#include "mode0.h"

extern MCUFRIEND_kbv tft; 
extern int cursorX, cursorY;

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
    Mode0();
    if (digitalRead(JOY_SEL) == LOW) {
      Mode1();
      mapInit(); // reinitializes map after exiting Mode 1
    }
  }
  Serial.end();
  return 0;
}