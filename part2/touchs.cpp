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

// function to write 5 characters vertically
void writeVertical(char text[], int x, int y, int n) {
  for (int i = 0; i < n; i++) {
    tft.setCursor(x, y + (16 * i));
    tft.print(text[i]);
  }
}

void btnSetup() {
  tft.fillRect(MAP_WIDTH + 1, 0, 60, MAP_HEIGHT/2 - 4, TFT_WHITE);
  tft.drawRect(MAP_WIDTH + 1, 0, 60, MAP_HEIGHT/2 - 4, TFT_RED);
  tft.fillRect(MAP_WIDTH + 1, MAP_HEIGHT/2 + 4, 60, MAP_HEIGHT/2 - 4, TFT_WHITE);
  tft.drawRect(MAP_WIDTH + 1, 0, 60, MAP_HEIGHT/2 - 4, TFT_GREEN);

  tft.setTextSize(2);
  tft.setColor(TFT_BLACK);
  tft.setCursor(MAP_WIDTH + 25, MAP_HEIGHT/4);
  tft.print("1");
  writeVertical("QSORT", MAP_WIDTH + 25, MAP_HEIGHT/2 + 30, 5);
}

void setRating() {

}

void changeSort() {

}

void processTouchScreen() {
	TSPoint touch = ts.getPoint();
	pinMode(YP, OUTPUT); 
	pinMode(XM, OUTPUT); 
	if (touch.z < MINPRESSURE || touch.z > MAXPRESSURE) {return;}

	int16_t screen_x = map(touch.y, TS_MINX, TS_MAXX, TFT_WIDTH - 1, 0);
	int16_t screen_y = map(touch.x, TS_MINY, TS_MAXY, TFT_HEIGHT - 1, 0);
  // draw dots if map was touched
  if (screen_x > 0 && screen_x < MAP_WIDTH) {drawDots();}
  if (screen_x < TFT_WIDTH && screen_x > MAP_WIDTH) {
    if (screen_y < TFT_HEIGHT/2) {setRating();}
    else {changeSort();}
  }
  
}

