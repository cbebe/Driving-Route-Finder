#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <SPI.h>
#include <SD.h>
#include <TouchScreen.h>
#include "lcd_image.h"
#include "coordinates.h"
#include "jcursor.h"

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

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
      tft.fillCircle(restX - CIRC_RAD, restY - CIRC_RAD, CIRC_RAD, TFT_BLACK);
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