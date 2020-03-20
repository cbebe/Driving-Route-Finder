#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <SPI.h>
#include <SD.h>

#include "coordinates.h"
#include "jcursor.h"
#include "joysel.h"

void displayText(int index) {
	// 15 is the vertical span of a size-2 character
	// (including the one pixel of padding below)
	tft.setCursor(0, 15*(index % NUM_LINES));

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

void pageUpdate(bool up) {
  tft.fillScreen(TFT_BLACK);
	tft.setTextSize(2);
	tft.setTextWrap(false);
  // chooses whether to start from the bottom or top
  if (up) {
    for (int i = selectedRest; i < (pageNum + 1) * NUM_LINES; i++) {
      Serial.println(i);
      displayText(i);
    }
  } else {
    for (int i = selectedRest; i >= (pageNum) * NUM_LINES; i--) {
      Serial.println(i);
      displayText(i);
    }
  }
}

// lets the user select a restaurant
void joySelect(int prevRest, int len) {
  int aVal = analogRead(JOY_VERT);
	if (aVal > POS_BUFFER || aVal < NEG_BUFFER) {
  	prevRest = selectedRest;
    // save the previous restaurant to draw it normally
    if (aVal > POS_BUFFER) {
      selectedRest++;
      if (selectedRest == (pageNum + 1) * NUM_LINES) {
        pageNum++;
        pageUpdate(true);
        return;
      }
		}
  	else {
      selectedRest--;
      if (selectedRest < 0) {
        selectedRest = 0;
        return;
      } else if (selectedRest - (pageNum * NUM_LINES) < 0) {
        pageNum--;
        pageUpdate(false);
        return;
      }
  	}
    // draw the old highlighted string normally
    displayText(prevRest);
    // highlight the new string
    displayText(selectedRest);

		delay(50);
	}
}