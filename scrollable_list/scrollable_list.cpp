/*
	Display a scrollable list on the screen.
*/

#include <Arduino.h>
#include <MCUFRIEND_kbv.h>

#define SD_CS 10
MCUFRIEND_kbv tft;

#define DISPLAY_WIDTH  480
#define DISPLAY_HEIGHT 320

#define JOY_VERT  A9 // should connect A9 to pin VRx
#define JOY_HORIZ A8 // should connect A8 to pin VRy
#define JOY_SEL   53

#define JOY_CENTER   512
#define JOY_DEADZONE 64

#define NUM_LINES 7

const char* stringsToDisplay[] = {
	"I wish",
	"I could",
	"show you when you are lonely or in darkness",
	"the astonishing",
	"light",
	"of your own",
	"being"
};

int highlightedString, oldNum;

// assumes the text size is already 2, text is not wrapping,
// and that 0 <= index < NUM_LINES
void displayText(int index) {
	// 15 is the vertical span of a size-2 character
	// (including the one pixel of padding below)
	tft.setCursor(0, 15*index);

	if (index == highlightedString) {
		tft.setTextColor(TFT_BLACK, TFT_WHITE);
	}
	else {
		tft.setTextColor(TFT_WHITE, TFT_BLACK);
	}
	tft.println(stringsToDisplay[index]);
}

void displayAllText() {
	tft.fillScreen(TFT_BLACK);

	tft.setTextSize(2);
	tft.setTextWrap(false); // change to true to see the result

	for (int index = 0; index < NUM_LINES; index++) {
		displayText(index);
	}
}

void setup() {
	init();

	Serial.begin(9600);

	pinMode(JOY_SEL, INPUT_PULLUP);

	uint16_t ID = tft.readID();
	tft.begin(ID);

	tft.fillScreen(TFT_BLACK);

	tft.setRotation(1);
}

void processAnalog(int aVal) {
  int posBuffer = JOY_CENTER + JOY_DEADZONE;
  int negBuffer = JOY_CENTER - JOY_DEADZONE;
	if (aVal > posBuffer) {
  	oldNum = highlightedString;
    highlightedString++;
  } else if (aVal < negBuffer) {
  	oldNum = highlightedString;
    highlightedString--;
  }
	// draw the old highlighted string normally
	displayText(oldNum);
	// highlight the new string
	displayText(highlightedString);
}


int main() {
	setup();

	highlightedString = 0;
	displayAllText();
	while (1) {
		processAnalog(digitalRead(JOY_VERT));
		// Challenge: Use joystick to scroll the list and select a line
	}

	Serial.end();

	return 0;
}
