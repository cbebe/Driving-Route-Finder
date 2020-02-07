#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <SPI.h>
#include <SD.h>
#include "lcd_image.h"
#include "joy_cursor.h"

MCUFRIEND_kbv tft;

lcd_image_t yegImage = { "yeg-big.lcd", YEG_SIZE, YEG_SIZE };

// global variables for cursor and map positions 
int cursorX, cursorY, currentPatchX, currentPatchY;

// redraws the cursor on the given cursor position
void redrawCursor() {
  tft.fillRect(cursorX - CURSOR_SIZE/2, cursorY - CURSOR_SIZE/2,
               CURSOR_SIZE, CURSOR_SIZE, TFT_RED);
}

// draws an entire map patch on the screen
void drawMap() {
  lcd_image_draw(&yegImage, &tft, currentPatchX, currentPatchY,
                 0, 0, MAP_WIDTH, MAP_HEIGHT);
}

// setup for tft and map patch
void joySetup() {
  uint16_t ID = tft.readID();    // read ID from display
  if (ID == 0xD3D3) ID = 0x9481; // write-only shield  
  tft.begin(ID);                 // LCD gets ready to work
	if (!SD.begin(SD_CS)) {
		while (true) {}
	}
	tft.setRotation(1); tft.fillScreen(TFT_BLACK);
  // sets the current map patch to the middle of the Edmonton map
	currentPatchX = YEG_SIZE/2 - MAP_WIDTH/2;
	currentPatchY = YEG_SIZE/2 - MAP_HEIGHT/2;
  drawMap();
  // initial cursor position is the middle of the screen
  cursorX = MAP_WIDTH/2;
  cursorY = MAP_HEIGHT/2;
  redrawCursor();
}

// redraws map background on previous cursor position to remove black trail
void redrawMapBg(uint16_t tempX, uint16_t tempY) {
  int screenPatchX = tempX - CURSOR_SIZE/2;
  // prevents cursor from moving to black column
  screenPatchX = constrain(screenPatchX, 0, MAP_WIDTH - CURSOR_SIZE);
  int screenPatchY = tempY - CURSOR_SIZE/2;
  lcd_image_draw(&yegImage, &tft, currentPatchX + screenPatchX, 
                  currentPatchY + screenPatchY, screenPatchX, 
                  screenPatchY, CURSOR_SIZE, CURSOR_SIZE);
}
// processes analog data and changes the increment 
// depending on the read analog value (from +-1px to +-4px)
void processAnalog(int aVal, char dir) {
  int posBuffer = JOY_CENTER + JOY_DEADZONE;
  int negBuffer = JOY_CENTER - JOY_DEADZONE;
  int increment = 0;
  if (aVal > posBuffer) {
    increment += 1 + (aVal - posBuffer)/(JOY_DEADZONE * 2);
  } else if (aVal < negBuffer) {
    increment += -1 +(aVal - negBuffer)/(JOY_DEADZONE * 2);
  }
  // accounts for the inverted X increment
  if (dir == 'X') {
  	cursorX -= increment;
  } else {
  	cursorY += increment;	
  }
}

// moves map patch depending on which edge of the screen the cursor was touching
void moveMapPatch(bool xl, bool xr, bool yt, bool yb) {
  if (!xl) {currentPatchX -= MAP_WIDTH;}
  if (!xr) {currentPatchX += MAP_WIDTH;}
  if (!yt) {currentPatchY -= MAP_HEIGHT;}
  if (!yb) {currentPatchY += MAP_HEIGHT;}
  currentPatchX = constrain(currentPatchX, 0, YEG_SIZE - MAP_WIDTH);
  currentPatchY = constrain(currentPatchY, 0, YEG_SIZE - MAP_HEIGHT);
  cursorX = MAP_WIDTH/2; cursorY = MAP_HEIGHT/2;
  drawMap(); redrawCursor();
}

// clamps the cursor to the map bounds
void clampMapBounds() {
  if (currentPatchX == 0 && cursorX < C_RAD) {
    cursorX = C_RAD;  
  }   
  if (currentPatchX == YEG_SIZE - MAP_WIDTH && cursorX > BOUND_WIDTH) {
    cursorX = BOUND_HEIGHT;  
  }
  if (currentPatchY == 0 && cursorY < C_RAD) {
    cursorY = C_RAD;  
  }
  if (currentPatchY == YEG_SIZE - MAP_HEIGHT && cursorY > BOUND_HEIGHT) {
    cursorY = BOUND_HEIGHT;  
  } 
} 

void screenBoundCheck() {
  // checks if the cursor is still in bounds of the screen
  bool xInLeftBnd = cursorX >= C_RAD; 
  bool xInRightBnd = cursorX <= BOUND_WIDTH;
  bool yInTopBnd = cursorY >= C_RAD; 
  bool yInBotBnd = cursorY <= BOUND_HEIGHT;
  if (!xInLeftBnd || !xInRightBnd || !yInTopBnd || !yInBotBnd) {
    moveMapPatch(xInLeftBnd, xInRightBnd, yInTopBnd, yInBotBnd);
  }
}

void processJoystick() {
  // temp variables to draw map on later
  int tempX = cursorX, tempY = cursorY;
  processAnalog(analogRead(JOY_HORIZ), 'X'); 
  processAnalog(analogRead(JOY_VERT), 'Y');
  clampMapBounds();
  screenBoundCheck();
  // will only redraw map when the cursor moves to prevent the cursor from flickering
  if (tempX != cursorX || tempY != cursorY) {
    redrawMapBg(tempX, tempY);
    redrawCursor();
  }
   
}


const char* stringsToDisplay[] = {
	"I wish",
	"I could",
	"show you when you are lonely or in darkness",
	"the astonishing",
	"light",
	"of your own",
	"being",
	"I wish",
	"I could",
	"show you when you are lonely or in darkness",
	"the astonishing",
	"light",
	"of your own",
	"being",
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

void processAnalogMode1(int aVal) {
  int posBuffer = JOY_CENTER + JOY_DEADZONE;
  int negBuffer = JOY_CENTER - JOY_DEADZONE;
	if (aVal > posBuffer || aVal < negBuffer) {
  	oldNum = highlightedString;
    if (aVal > posBuffer) {
			highlightedString++;
			if (highlightedString == NUM_LINES) {
				highlightedString = 0;
			}
		}
  	else {
	    highlightedString--;
			if (highlightedString < 0) {
				highlightedString = NUM_LINES - 1;
			}
  	} 
		// draw the old highlighted string normally
		displayText(oldNum);
		// highlight the new string
		displayText(highlightedString);
		delay(500);
	}
}


void Mode1() {
	highlightedString = 0;
	displayAllText();
	while (digitalRead(JOY_SEL) == HIGH) {
		processAnalogMode1(analogRead(JOY_VERT));
		// Challenge: Use joystick to scroll the list and select a line
	}
}
