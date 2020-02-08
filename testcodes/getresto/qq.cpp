// ---------------------------------------------------
// Name : Charles Ancheta
// ID: 1581672
// CMPUT 275 , Winter 2020
//
// Weekly Exercise 2: Restaurants and Pointers
// ---------------------------------------------------

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <SD.h>
#include <TouchScreen.h>

#define SD_CS 10

// physical dimensions of the tft display (# of pixels)
#define TFT_WIDTH  480
#define TFT_HEIGHT 320

// touch screen pins, obtained from the documentation
#define YP A3 // must be an analog pin, use "An" notation!
#define XM A2 // must be an analog pin, use "An" notation!
#define YM 9  // can be a digital pin
#define XP 8  // can be a digital pin

// dimensions of the part allocated to the map display
#define MAP_DISP_WIDTH (TFT_WIDTH - 60)
#define MAP_DISP_HEIGHT TFT_HEIGHT

#define REST_START_BLOCK 4000000
#define NUM_RESTAURANTS 1066

// calibration data for the touch screen, obtained from documentation
// the minimum/maximum possible readings from the touch point
#define TS_MINX 100
#define TS_MINY 120
#define TS_MAXX 940
#define TS_MAXY 920
// thresholds to determine if there was a touch
#define MINPRESSURE   10
#define MAXPRESSURE 1000

MCUFRIEND_kbv tft;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
// different than SD
Sd2Card card;

struct restaurant {
  int32_t lat;
  int32_t lon;
  uint8_t rating; // from 0 to 10
  char name[55];
};

// declare global variables for calculating average time
uint32_t slowTotalTime = 0, slowTotalCalls = 0;
uint32_t fastTotalTime = 0, fastTotalCalls = 0;
uint32_t pastBlockNum; // declare for getRestaurantFast
restaurant restBlock[8];

// forward declaration of setup functions
void setupRecords(), setupButtons();

void setup() {
  init();
  Serial.begin(9600);
  // tft display initialization
  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  // SD card initialization for raw reads
  Serial.print("Initializing SPI communication for raw reads...");
  if (!card.init(SPI_HALF_SPEED, SD_CS)) {
    Serial.println("failed! Is the card inserted properly?");
    while (true) {}
  }
  else {
    Serial.println("OK!");
  }
  setupRecords();
  setupButtons();
}
// prints labels on screen
void setupRecords() {
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  const char* toPrint[] {
    "RECENT SLOW RUN:","SLOW RUN AVG:",
    "RECENT FAST RUN:","FAST RUN AVG:"
  };
  for (int i = 0; i < 4; i++) {
    tft.println(toPrint[i]);
    tft.println("Not yet run\n");
  }
}
// function to write 4 characters vertically
void writeVertical(char text[], int X, int Y) {
  for (int i = 0; i < 4; i++) {
    tft.setCursor(X, Y + (16 * i));
    tft.print(text[i]);
  }
}
// print fast and slow buttons on screen
void setupButtons() {
  tft.drawRect(MAP_DISP_WIDTH, 0, 60, MAP_DISP_HEIGHT/2, TFT_RED);
  tft.drawRect(MAP_DISP_WIDTH, MAP_DISP_HEIGHT/2, 60, MAP_DISP_HEIGHT/2, TFT_RED);
  char slow[] = "SLOW", fast[] = "FAST";
  writeVertical(slow, MAP_DISP_WIDTH + 25, 45);
  writeVertical(fast, MAP_DISP_WIDTH + 25, MAP_DISP_HEIGHT/2 + 45);
}
// the implementation from class
void getRestaurant(int restIndex, restaurant* restPtr) {
  uint32_t blockNum = REST_START_BLOCK + restIndex/8;
  restaurant restBlock[8];
  while (!card.readBlock(blockNum, (uint8_t*) restBlock)) {
    Serial.println("Read block failed, trying again.");
  }
  *restPtr = restBlock[restIndex % 8];
}

void getRestaurantFast(int restIndex, restaurant* restPtr) {
  uint32_t blockNum = REST_START_BLOCK + restIndex/8;
  // only loads from SD card when not on the same block
  if (blockNum != pastBlockNum) {
    while (!card.readBlock(blockNum, (uint8_t*) restBlock)) {
      Serial.println("Read block failed, trying again.");
    }
    pastBlockNum = blockNum;
  }
  *restPtr = restBlock[restIndex % 8];
}
// records results depending on which mode was called
void recordTimes(uint32_t delta, bool isFast) {
  int totalCalls, totalTime, cursorY = 16;
  if (isFast) {
    totalCalls = fastTotalCalls; 
    totalTime = fastTotalTime;
    cursorY *= 7; // writes on last two lines
  } else {
    totalCalls = slowTotalCalls; 
    totalTime = slowTotalTime;
  }
  // clear previous entries
  tft.fillRect(0, cursorY, 130, 16, TFT_BLACK);
  tft.fillRect(0, cursorY + 48, 130, 16, TFT_BLACK);
  // print results
  tft.setCursor(0, cursorY);
  tft.print(delta); tft.println(" ms\n\n");
  tft.print(totalTime/totalCalls); tft.print(" ms");
}

void processGetRestaurant(bool isFast) {  
  restaurant rest;
  uint32_t timeBefore = millis();
  // calls function depending on mode
  if (isFast) {
    for (int i = 0; i < NUM_RESTAURANTS; ++i) {
      getRestaurantFast(i, &rest);
    }
    fastTotalCalls += 1;
  } else {
    for (int i = 0; i < NUM_RESTAURANTS; ++i) {
      getRestaurant(i, &rest);
    }
    slowTotalCalls += 1;
  }
  uint32_t timeAfter = millis();
  // calculates time by getting the difference between millis calls 
  uint32_t delta = timeAfter - timeBefore;
  if (isFast) { // adds total time depending on mode
  	fastTotalTime += delta;
  } else {
  	slowTotalTime += delta;	
  } 
  recordTimes(delta, isFast);
}

void processTouchScreen() {
	TSPoint touch = ts.getPoint();
	pinMode(YP, OUTPUT); 
	pinMode(XM, OUTPUT); 
	if (touch.z < MINPRESSURE || touch.z > MAXPRESSURE) {
		return;
	}
	int16_t screen_x = map(touch.y, TS_MINX, TS_MAXX, TFT_WIDTH-1, 0);
	int16_t screen_y = map(touch.x, TS_MINY, TS_MAXY, TFT_HEIGHT-1, 0);

  // check if rightmost column was touched
  if (screen_x > MAP_DISP_WIDTH && screen_x < TFT_WIDTH) {
    if (screen_y > 0 && screen_y < TFT_HEIGHT/2) {
      processGetRestaurant(false); // slow touch area
    } else {
      processGetRestaurant(true); // fast touch area
    }
  }
  delay(100);
}

int main() {
  setup();
  while (true) {
    processTouchScreen();
  }
  Serial.end();
  return 0;
}