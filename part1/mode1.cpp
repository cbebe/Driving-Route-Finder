#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <SPI.h>
#include <SD.h>
#include "coordinates.h"

void displayText(int index) {
	// 15 is the vertical span of a size-2 character
	// (including the one pixel of padding below)
	restaurant currentRest;
	tft.setCursor(0, 15*index);
  getRestaurantFast(rest_dist[index].index, &currentRest);
	tft.println(currentRest.name);
}

void displayAllText() {
	tft.fillScreen(TFT_BLACK);
	tft.setTextColor(TFT_WHITE, TFT_BLACK);
	tft.setTextSize(2);
	tft.setTextWrap(false);

	for (int i = 0; i < NUM_LINES; ++i) {
    displayText(i);
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
  int16_t restX = map(rest->lat, LAT_NORTH, LAT_SOUTH, 0, YEG_SIZE); 
  int16_t restY = map(rest->lon, LON_WEST, LON_EAST, 0, YEG_SIZE);
  return abs(restX - cursorX) + abs(restY - cursorY);
}

void loadAllRestaurants() {
	restaurant currentRest;
  for (int i = 0; i < NUM_RESTAURANTS; i++) {
    rest_dist[i].index = i;
    getRestaurantFast(i, &currentRest);
    rest_dist[i].dist = calculateDist(&currentRest);
  }
}

void Mode1() {
	int prevRest;
  loadAllRestaurants();
  iSort(rest_dist, NUM_RESTAURANTS);
	displayAllText();
	while (digitalRead(JOY_SEL) == HIGH) {}
}