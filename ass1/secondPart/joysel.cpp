#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <SPI.h>
#include <SD.h>

#include "coordinates.h"
#include "jcursor.h"

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

// clears screen
void clearScreen() {
  tft.fillScreen(TFT_BLACK);
	tft.setTextSize(2);
	tft.setTextWrap(false);
}

// updates the page with a new list of restaurants
void pageUpdate(bool top) {
  clearScreen();
  if (top) { // if cursor should start from the top
    for (int i = selectedRest; i < (pageNum + 1) * NUM_LINES; i++) {
      displayText(i);
    }
  } else { // if should start from the bottom
    for (int i = selectedRest; i >= pageNum * NUM_LINES; i--) {
      displayText(i);
    }
  }
}

void lastPage(int len) {
  clearScreen();
  for (int i = selectedRest; i < selectedRest + (len % NUM_LINES); i++) {
    displayText(i);
  }
}

// lets the user select a restaurant
void joySelect(int prevRest, int len) {
  int aVal = analogRead(JOY_VERT);
	if (aVal > POS_BUFFER || aVal < NEG_BUFFER) {
  	prevRest = selectedRest; // save the previous restaurant to draw it normally
    if (aVal > POS_BUFFER) {
      selectedRest++;
      // prevents scrolling past the list limit
      if (selectedRest > len - 1) {
        selectedRest = len - 1;
        return;
      }
      if (selectedRest == (pageNum + 1) * NUM_LINES) {
        // if scrolled past the bottom of the page
        pageNum++;
        Serial.print("Page "); Serial.println(pageNum);
        if (pageNum == len/NUM_LINES) {
          lastPage(len);
        } else {
          pageUpdate(true);
        }
        return;
      }
		}
  	else if (aVal < NEG_BUFFER) {
      selectedRest--;
      // prevents scrolling past the list limit
      if (selectedRest < 0) {
        selectedRest = 0;
        return;
      } else if (selectedRest - (pageNum * NUM_LINES) < 0) {
        // if scrolled past the top of the page
        pageNum--;
        Serial.print("Page "); Serial.println(pageNum);
        pageUpdate(false);
        return;
      }
  	}
    // draw the old highlighted string normally
    displayText(prevRest);
    // highlight the new string
    displayText(selectedRest);

	}
}
