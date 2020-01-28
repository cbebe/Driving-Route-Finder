

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <SPI.h>
#include <SD.h>
#include "lcd_image.h"

#include "joy_cursor.h"

MCUFRIEND_kbv tft;



lcd_image_t yegImage = { "yeg-big.lcd", YEG_SIZE, YEG_SIZE };


// global variables for cursor position
int cursorX, cursorY;

void redrawCursor() {
  tft.fillRect(cursorX - CURSOR_SIZE/2, cursorY - CURSOR_SIZE/2,
               CURSOR_SIZE, CURSOR_SIZE, TFT_RED);
}

// redraws map background on previous cursor position to remove black trail
void redrawMapBg(uint16_t tempX, uint16_t tempY) {
  int yegMiddleX = YEG_SIZE/2 - (DISPLAY_WIDTH - 60)/2;
  int yegMiddleY = YEG_SIZE/2 - DISPLAY_HEIGHT/2;
  int screenPatchX = tempX - CURSOR_SIZE/2;
  // prevents cursor from moving to black column
  screenPatchX = constrain(screenPatchX, 0, DISPLAY_WIDTH - 60 - CURSOR_SIZE);
  int screenPatchY = tempY - CURSOR_SIZE/2;
  lcd_image_draw(&yegImage, &tft, yegMiddleX + screenPatchX, 
                  yegMiddleY + screenPatchY, screenPatchX, 
                  screenPatchY, CURSOR_SIZE, CURSOR_SIZE);
}
// processes analog data and changes the increment 
// depending on the read analog value (from +-1px to +-4px)
void processAnalog(int aVal, char dir) {
  int posBuffer = JOY_CENTER + JOY_DEADZONE;
  int negBuffer = JOY_CENTER - JOY_DEADZONE;
  int increment = 0;
  if (aVal > posBuffer) {
    increment += 1 + (aVal - posBuffer)/(JOY_DEADZONE * 2);
  } else if (aVal < negBuffer) {
    increment += -1 +(aVal - negBuffer)/(JOY_DEADZONE * 2);
  }
  // accounts for the inverted X increment
  if (dir == 'X') {
  	cursorX -= increment;
  } else {
  	cursorY += increment;	
  }
}

void processJoystick() {
  // temp variables to draw map on later
  int tempX = cursorX, tempY = cursorY;
  processAnalog(analogRead(JOY_HORIZ), 'X'); 
  processAnalog(analogRead(JOY_VERT), 'Y');
  int cRad = CURSOR_SIZE/2;
  // subtract an extra 1 to account for the "centre" cursor pixel
  cursorX = constrain(cursorX, cRad, DISPLAY_WIDTH - 61 - cRad);
  cursorY = constrain(cursorY, cRad, DISPLAY_HEIGHT - 1 - cRad);
  // will only redraw map when the cursor moves to prevent the cursor from flickering
  if (tempX != cursorX || tempY != cursorY) {
    redrawMapBg(tempX, tempY);
    redrawCursor();
  }
}

void joySetup() {
  init();
  Serial.begin(9600);
  uint16_t ID = tft.readID();    // read ID from display
  Serial.print("ID = 0x");
  Serial.println(ID, HEX);
  if (ID == 0xD3D3) ID = 0x9481; // write-only shield  
  tft.begin(ID);                 // LCD gets ready to work
	Serial.print("Initializing SD card...");
	if (!SD.begin(SD_CS)) {
		Serial.println("failed! Is it inserted properly?");
		while (true) {}
	}
	Serial.println("OK!");
	tft.setRotation(1); tft.fillScreen(TFT_BLACK);
	int yegMiddleX = YEG_SIZE/2 - (DISPLAY_WIDTH - 60)/2;
	int yegMiddleY = YEG_SIZE/2 - DISPLAY_HEIGHT/2;
	lcd_image_draw(&yegImage, &tft, yegMiddleX, yegMiddleY,
                 0, 0, DISPLAY_WIDTH - 60, DISPLAY_HEIGHT);
  // initial cursor position is the middle of the screen
  cursorX = (DISPLAY_WIDTH - 60)/2;
  cursorY = DISPLAY_HEIGHT/2;
  redrawCursor();
}