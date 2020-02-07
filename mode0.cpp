#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <SPI.h>
#include <SD.h>
#include "lcd_image.h"
#include "mode0.h"

extern MCUFRIEND_kbv tft; 

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

void mapInit() {
  tft.fillScreen(TFT_BLACK);
  currentPatchX = YEG_SIZE/2 - MAP_WIDTH/2;
	currentPatchY = YEG_SIZE/2 - MAP_HEIGHT/2;
  drawMap();
  // initial cursor position is the middle of the screen
  cursorX = MAP_WIDTH/2;
  cursorY = MAP_HEIGHT/2;
  redrawCursor();
}

// setup for tft and map patch
void joySetup() {
  uint16_t ID = tft.readID();    // read ID from display
  if (ID == 0xD3D3) ID = 0x9481; // write-only shield  
  tft.begin(ID);                 // LCD gets ready to work
	if (!SD.begin(SD_CS)) {
		while (true) {}
	}
	tft.setRotation(1); 
  // sets the current map patch to the middle of the Edmonton map
  mapInit();
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
  int increment = 0;
  if (aVal > POS_BUFFER) {
    increment += 1 + (aVal - POS_BUFFER)/(JOY_DEADZONE * 2);
  } else if (aVal < negBuffer) {
    increment += -1 +(aVal - NEG_BUFFER)/(JOY_DEADZONE * 2);
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

void Mode0() {
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

