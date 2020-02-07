#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <SPI.h>
#include <SD.h>
#include "joy_cursor.h"

MCUFRIEND_kbv tft;
void setup() {
  init();
  Serial.begin(9600);
  // sets up Arduino for Mode 0
  uint16_t ID = tft.readID();    // read ID from display
  if (ID == 0xD3D3) ID = 0x9481; // write-only shield  
    tft.begin(ID);                 // LCD gets ready to work
	if (!SD.begin(SD_CS)) {
		while (true) {}
	}
	tft.setRotation(1); tft.fillScreen(TFT_BLACK);
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