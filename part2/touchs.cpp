#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <SPI.h>
#include <SD.h>

#include "coordinates.h"
#include "touchs.h"

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// displays nearby restaurants as dots on the screen
void drawDots() {
  restaurant currentRest; int restX, restY;
  int patchBoundX = currentPatchX + MAP_WIDTH - CIRC_RAD;
  int patchBoundY = currentPatchY + MAP_HEIGHT - CIRC_RAD;

  for (int i = 0; i < NUM_RESTAURANTS; i++) {
    getRestaurantFast(i, &currentRest);
    // converts from longitude and latitude to pixels relative to map size
    restX = map(currentRest.lon, LON_WEST, LON_EAST, 0, YEG_SIZE);
    restY = map(currentRest.lat, LAT_NORTH, LAT_SOUTH, 0, YEG_SIZE);
    // only draws the dots if it is within the screen patch
    if ((restX >= currentPatchX + CIRC_RAD && restX <= patchBoundX) && 
        (restY >= currentPatchY + CIRC_RAD && restY <= patchBoundY)) {
      tft.fillCircle(restX - currentPatchX, restY - currentPatchY, CIRC_RAD, TFT_BLUE);
      tft.drawCircle(restX - currentPatchX, restY - currentPatchY, CIRC_RAD, TFT_WHITE);
    }
  }
}

void btnSetup() {
  tft.fillRect(MAP_WIDTH + 1, 0, 60, MAP_HEIGHT/2 - 4, TFT_WHITE);
  tft.drawRect(MAP_WIDTH + 1, 0, 60, MAP_HEIGHT/2 - 4, TFT_RED);
  tft.fillRect(MAP_WIDTH + 1, MAP_HEIGHT/2 + 4, 60, MAP_HEIGHT/2 - 4, TFT_WHITE);
  tft.drawRect(MAP_WIDTH + 1, 0, 60, MAP_HEIGHT/2 - 4, TFT_GREEN);
}

void processTouchScreen() {
	TSPoint touch = ts.getPoint();
	pinMode(YP, OUTPUT); 
	pinMode(XM, OUTPUT); 
	if (touch.z < MINPRESSURE || touch.z > MAXPRESSURE) {return;}

	int16_t screen_x = map(touch.y, TS_MINX, TS_MAXX, TFT_WIDTH - 1, 0);
  // draw dots if map was touched
  if (screen_x > 0 && screen_x < MAP_WIDTH) {drawDots();}
}

