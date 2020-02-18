#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <SPI.h>
#include <SD.h>

#include "coordinates.h"
#include "jcursor.h"
// adjust coordinates to centre the restaurant
// also considering boundary conditions
void adjustCoordinates(int posX, int posY) {
  bool normalXBound = posX >= CENTRE_X && posX <= MAX_X; 
  bool normalYBound = posY >= CENTRE_Y && posY <= MAX_Y; 
  currentPatchX = constrain(posX - CENTRE_X, 0, MAP_MAXX);
  currentPatchY = constrain(posY - CENTRE_Y, 0, MAP_MAXY);
  // places the cursor in the middle if bounds are normal
  // otherwise simply places it on the restaurant
  if (normalXBound) {cursorX = CENTRE_X;} 
  else {cursorX = abs(posX - currentPatchX);}
  if (normalYBound) {cursorY = CENTRE_Y;} 
  else {cursorY = abs(posY - currentPatchY);}
}

// places cursor on the selected restaurant
void goToResto() {
  getRestaurantFast(rest_dist[selectedRest].index, &currentRest);
  // converts from longitude and latitude
  int positionX = map(currentRest.lon, LON_WEST, LON_EAST, 0, YEG_SIZE);
  int positionY = map(currentRest.lat, LAT_NORTH, LAT_SOUTH, 0, YEG_SIZE);
  // constrains the position inside the map for out of bounds restaurants
  positionX = constrain(positionX, CUR_RAD, YEG_SIZE - CUR_RAD - 1);
  positionY = constrain(positionY, CUR_RAD, YEG_SIZE - CUR_RAD - 1);
  adjustCoordinates(positionX, positionY);
}

void restList() {
  int len, prevRest;

  // sorts the restaurant distance array depending on chosen setting
  switch (sortSetting) {
  case quick:
    len = runSort(quick);
    break;
  case insert:
    len = runSort(insert);
    break;
  case both:
    runSort(quick);
    len = runSort(insert);
    break;
  default:
    break;
  }
  selectedRest = 0; pageNum = 0;
  Serial.print("List length: "); Serial.println(len);
  Serial.print("Number of pages: "); Serial.println(len/NUM_LINES + 1);
  Serial.print("Page "); Serial.println(pageNum);
  pageUpdate(true); // prints the first page on screen

  while (digitalRead(JOY_SEL) == HIGH) {
    joySelect(prevRest, len);
  }
  goToResto();
}
