#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <SPI.h>
#include <SD.h>
#include <TouchScreen.h>
#include "lcd_image.h"
#include "coordinates.h"
#include "jcursor.h"

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// setup for tft and map patch
void joySetup() {
  uint16_t ID = tft.readID();    // read ID from display
  if (ID == 0xD3D3) ID = 0x9481; // write-only shield  
  tft.begin(ID);                 // LCD gets ready to work
	if (!SD.begin(SD_CS)) {
		while (true) {}
	}
	tft.setRotation(1); 
  // sets the current map patch to the middle of the Edmonton map
  currentPatchX = YEG_SIZE/2 - MAP_WIDTH/2;
	currentPatchY = YEG_SIZE/2 - MAP_HEIGHT/2;
  // initial cursor position is the middle of the screen
  cursorX = MAP_WIDTH/2;
  cursorY = MAP_HEIGHT/2;
  mapInit();
}

void drawDots() {
  restaurant currentRest; int restX, restY;
  int patchBoundX = currentPatchX + MAP_WIDTH - 1;
  int patchBoundY = currentPatchY + MAP_HEIGHT - 1;
  for (int i = 0; i < NUM_RESTAURANTS; i++) {
    getRestaurantFast(i, &currentRest);
    restX = map(currentRest.lon, LON_WEST, LON_EAST, 0, YEG_SIZE);
    restY = map(currentRest.lat, LAT_NORTH, LAT_SOUTH, 0, YEG_SIZE);
    if ((restX >= currentPatchX && restX <= patchBoundX) &&
        (restY >= currentPatchY && restY <= patchBoundY)) {
      tft.
    }
  }
}

void processTouchScreen() {
	TSPoint touch = ts.getPoint();
	pinMode(YP, OUTPUT); 
	pinMode(XM, OUTPUT); 
	if (touch.z < MINPRESSURE || touch.z > MAXPRESSURE) {
		return;
	}
	int16_t screen_x = map(touch.y, TS_MINX, TS_MAXX, TFT_WIDTH-1, 0);
	int16_t screen_y = map(touch.x, TS_MINY, TS_MAXY, TFT_HEIGHT-1, 0);

  // check if rightmost column was touched
  if (screen_x > 0 && screen_x < MAP_WIDTH) {
    drawDots();
  }
}

void Mode0() {
  processJoystick();
  processTouchScreen();
}