#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <SPI.h>
#include <SD.h>
#include "jcursor.h"
#include "coordinates.h"

int8_t selectedRest = 0;
restaurant currentRest;

void displayText(int index) {
	// 15 is the vertical span of a size-2 character
	// (including the one pixel of padding below)
	tft.setCursor(0, 15*index);

	if (index == selectedRest) {
		tft.setTextColor(TFT_BLACK, TFT_WHITE);
	}
	else {
		tft.setTextColor(TFT_WHITE, TFT_BLACK);
	}
  getRestaurantFast(rest_dist[index].index, &currentRest);

	tft.println(currentRest.name);
}

void displayAllText() {
	tft.fillScreen(TFT_BLACK);
	tft.setTextSize(2);
	tft.setTextWrap(false);

	for (int i = 0; i < NUM_LINES; ++i) {
    getRestaurantFast(rest_dist[i].index, &currentRest);
    displayText(i);
	}
}

void joySelect(int prevRest) {
  int aVal = analogRead(JOY_VERT);
	if (aVal > POS_BUFFER || aVal < NEG_BUFFER) {
  	prevRest = selectedRest;
    if (aVal > POS_BUFFER) {
			selectedRest++;
			if (selectedRest == NUM_LINES) {
				selectedRest = 0; // wraps back to the top
			}
		}
  	else {
	    selectedRest--;
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

void swapRest(RestDist *restA, RestDist *restB) {
  RestDist tmp = *restA;
  *restA = *restB; *restB = tmp;
}

// implementation of insertion sort from assignment pdf
void iSort(RestDist array[], int length) {
  int i = 1;
  while (i < length) {
    int j = i;
    while (j > 0 && array[j-1].dist > array[j].dist) {
      swapRest(&array[j], &array[j-1]); j--;
    }
    i++;
  }
}

int16_t calculateDist(restaurant *rest) {
  int16_t restX = map(rest->lon, LON_WEST, LON_EAST, 0, YEG_SIZE);
  int16_t restY = map(rest->lat, LAT_NORTH, LAT_SOUTH, 0, YEG_SIZE); 
  int16_t cursorX_coord = cursorX + currentPatchX;
  int16_t cursorY_coord = cursorY + currentPatchY;
  return abs(restX - cursorX_coord) + abs(restY - cursorY_coord);
}

void loadAllRestaurants() {
	restaurant currentRest;
  for (int i = 0; i < NUM_RESTAURANTS; i++) {
    rest_dist[i].index = i;
    getRestaurantFast(i, &currentRest);
    rest_dist[i].dist = calculateDist(&currentRest);
  }
}

void adjustCoordinates() {
  getRestaurantFast(rest_dist[selectedRest].index, &currentRest);
  int positionX = map(currentRest.lon, LON_WEST, LON_EAST, 0, YEG_SIZE);
  int positionY = map(currentRest.lat, LAT_NORTH, LAT_SOUTH, 0, YEG_SIZE);

}

void Mode1() {
	int prevRest;
  loadAllRestaurants();
  iSort(rest_dist, NUM_RESTAURANTS);
	displayAllText();
	while (digitalRead(JOY_SEL) == HIGH) {
		joySelect(prevRest);
	}
  adjustCoordinates();
}