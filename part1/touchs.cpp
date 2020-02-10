#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <SPI.h>
#include <SD.h>
#include "coordinates.h"
#include "touchs.h"

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

void drawDots() {
  restaurant currentRest; int restX, restY;
  int patchBoundX = currentPatchX + MAP_WIDTH - CIRC_RAD;
  int patchBoundY = currentPatchY + MAP_HEIGHT - CIRC_RAD;
  for (int i = 0; i < NUM_RESTAURANTS; i++) {
    getRestaurantFast(i, &currentRest);
    restX = map(currentRest.lon, LON_WEST, LON_EAST, 0, YEG_SIZE);
    restY = map(currentRest.lat, LAT_NORTH, LAT_SOUTH, 0, YEG_SIZE);
    if ((restX >= currentPatchX && restX <= patchBoundX) && 
        (restY >= currentPatchY && restY <= patchBoundY)) {
      tft.fillCircle(restX - currentPatchX, restY - currentPatchY, CIRC_RAD, TFT_BLUE);
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
	int16_t screen_x = map(touch.y, TS_MINX, TS_MAXX, TFT_WIDTH - 1, 0);
  // check if map was touched
  if (screen_x > 0 && screen_x < MAP_WIDTH) {
    drawDots();
    Serial.println("map touched");
  }
}