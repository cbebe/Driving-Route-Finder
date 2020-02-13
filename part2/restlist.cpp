#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <SPI.h>
#include <SD.h>

#include "coordinates.h"
#include "jcursor.h"
#include "restlist.h"
#include "sorting.h"
#include "touchs.h"

int8_t selectedRest;
restaurant currentRest;
RestDist rest_dist[NUM_RESTAURANTS];

void displayText(int index) {
	// 15 is the vertical span of a size-2 character
	// (including the one pixel of padding below)
	tft.setCursor(0, 15*index);

  // highlights selected restaurant
	if (index == selectedRest) { 
    tft.setTextColor(TFT_BLACK, TFT_WHITE); 
  } else {
		tft.setTextColor(TFT_WHITE, TFT_BLACK);
	}
  // loads restaurants name and prints
  getRestaurantFast(rest_dist[index].index, &currentRest);
	tft.println(currentRest.name);
}

// sets up the screen to display the nearest 21 restaurants
void displayAllText() {
	tft.fillScreen(TFT_BLACK);
	tft.setTextSize(2);
	tft.setTextWrap(false);
  selectedRest = 0; // highlights the nearest restaurant
  for (int i = 0; i < NUM_LINES; i++) {
    displayText(i);
  } 
}

// lets the user select a restaurant
void joySelect(int prevRest) {
  int aVal = analogRead(JOY_VERT);
	if (aVal > POS_BUFFER || aVal < NEG_BUFFER) {
    // save the previous restaurant to draw it normally
  	prevRest = selectedRest;
    if (aVal > POS_BUFFER) {
      selectedRest++; // highlight moves down
			if (selectedRest == NUM_LINES) {
				selectedRest = 0; // wraps back to the top
			}
		}
  	else {
	    selectedRest--; // highlight moves up
			if (selectedRest < 0) {
				selectedRest = NUM_LINES - 1; // wraps back to the botttom
			}
  	} 
		// draw the old highlighted string normally
		displayText(prevRest);
		// highlight the new string
		displayText(selectedRest);
		delay(400);
	}
}

// converts restaurant longitude and latitude 
// and calculates the Manhattan distance from cursor 
int16_t calculateDist(restaurant rest) {
  int16_t restX = map(rest.lon, LON_WEST, LON_EAST, 0, YEG_SIZE);
  int16_t restY = map(rest.lat, LAT_NORTH, LAT_SOUTH, 0, YEG_SIZE); 
  int16_t cursorX_coord = cursorX + currentPatchX;
  int16_t cursorY_coord = cursorY + currentPatchY;
  return abs(restX - cursorX_coord) + abs(restY - cursorY_coord);
}

void sortDistArray(int len) {
  int tStart;
  switch (sortSetting) {
  case quick:
    tStart = millis();
    qSort(rest_dist, len);
    Serial.print("");
    break;
  
  default:
    break;
  }
}

// loads all the restaurants from sd card to a RestDist struct array
// the array is then sorted and the first 21 restaurants are displayed
void loadAllRestaurants() {
  int actualLen = 0;
  for (int i = 0; i < NUM_RESTAURANTS; i++) {
    getRestaurantFast(i, &currentRest);
    if ((currentRest.rating+1)/2 >= ratingSel - 1) {
      rest_dist[i].index = i;
      rest_dist[i].dist = calculateDist(currentRest);
      actualLen++;
    }
  }
  sortDistArray(actualLen);
  displayAllText();
}

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