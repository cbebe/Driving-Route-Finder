#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <SPI.h>
#include <SD.h>
#include "coordinates.h"


void setup() {
  init();
  Serial.begin(9600);
  // SD card initialization for raw reads
  Serial.print("Initializing SPI communication for raw reads...");
  if (!card.init(SPI_HALF_SPEED, SD_CS)) {
    Serial.println("failed! Is the card inserted properly?");
    while (true) {}
  }
  else {
    Serial.println("OK!");
  }
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