//---------------------------------------
// Name: Poulomi Ganguly
// ID: 1598887
// CMPUT 275, Winter 2020
//
// Weekly Assignment 2: Restaurant and Pointers: getRestaurantFast
//---------------------------------------

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <SD.h>
#include <TouchScreen.h>

#define SD_CS 10

// physical dimensions of the tft display (# of pixels)
#define DISPLAY_WIDTH  480
#define DISPLAY_HEIGHT 320

// touch screen pins, obtained from the documentaion
#define YP A3 // must be an analog pin, use "An" notation!
#define XM A2 // must be an analog pin, use "An" notation!
#define YM 9  // can be a digital pin
#define XP 8  // can be a digital pin

// dimensions of the part allocated to the map display
#define MAP_DISP_WIDTH (DISPLAY_WIDTH - 60)
#define MAP_DISP_HEIGHT DISPLAY_HEIGHT

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

// GLOBAL VARIABLES FOR AVERAGES
int S_Slow = 0;
int n_Slow = 0;
int S_Fast = 0;
int n_Fast = 0;

MCUFRIEND_kbv tft;

// a multimeter reading says there are 300 ohms of resistance across the plate,
// so initialize with this to get more accurate readings
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// different than SD
Sd2Card card;

struct restaurant {
  int32_t lat;
  int32_t lon;
  uint8_t rating; // from 0 to 10
  char name[55];
};


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

uint32_t oldNum = 0;

void getRestaurantFast(int restIndex, restaurant* restPtr) {
	uint32_t blockNum = REST_START_BLOCK + restIndex/8;
	restaurant restBlock[8];

	// Checks if same block is being called before reading

	if (blockNum != oldNum){
		while (!card.readBlock(blockNum, (uint8_t*) restBlock)) {
			Serial.println("Read block failed, trying again.");
		 }
	}

	*restPtr = restBlock[restIndex % 8];

	oldNum = blockNum;


}

// Sets the initial screen:
void drawButton(){
	tft.drawRect(420,0,60,160,TFT_RED);
	tft.drawRect(420, 160, 60, 160, TFT_RED);

	tft.setCursor(450, 50);
	tft.print("Slow");

	tft.setCursor(450, 210);
	tft.print("Fast");

	tft.setTextSize(2);

	tft.setCursor(0,0);
	tft.println("RECENT SLOW RUN:");
	tft.println("Not yet run");
	tft.println();

	tft.println("SLOW RUN AVG:");
	tft.println("Not yet run");
	tft.println();

	tft.println("RECENT FAST RUN:");
	tft.println("Not yet run");
	tft.println();

	tft.println("FAST RUN AVG:");
	tft.println("Not yet run");
}

void loop(){

	TSPoint touch = ts.getPoint(); // (x, y, pressure)
    pinMode(YP, OUTPUT);
    pinMode(XM, OUTPUT);

    if (touch.z >= MINPRESSURE && touch.z <= MAXPRESSURE) {
    	int ptx = map(touch.y, TS_MINX, TS_MAXX, 0, tft.width());
        int pty = map(touch.x, TS_MINY, TS_MAXY, 0, tft.height());

        Serial.println(ptx);
        Serial.println(pty);

        if (ptx <= 60 && pty >= 160){
        	// SLOW READ BUTTON PRESSED

        	Serial.println("Button has been pressed");
        	restaurant rest3;
        	int initialTime = millis();
        	for (int i = 0; i < NUM_RESTAURANTS-1; ++i) {
			    getRestaurant(i, &rest3);
  			}

  			int endTime = millis();

  			// Updates recent slow reading
  			tft.fillRect(0,16,400,16, TFT_BLACK);
  			tft.setCursor(0, 16);
  			tft.print(endTime-initialTime);
  			tft.print(" ms");

  			S_Slow += endTime - initialTime;
  			n_Slow++;

  			int slowAvg = S_Slow/n_Slow;

  			// Updates average with every button click
  			tft.fillRect(0,64,400,16, TFT_BLACK);
  			tft.setCursor(0,64);
  			tft.print(slowAvg);
  			tft.print(" ms");


        }

        else if (ptx <= 60 && pty < 160 ){
        	//FAST READ BUTTON PRESSED

        	Serial.println("Button has been pressed");
        	restaurant rest3;
        	int initialTime = millis();
        	for (int i = 0; i < NUM_RESTAURANTS-1; ++i) {
			    getRestaurantFast(i, &rest3);
  			}

  			int endTime = millis();

  			// Updates most recent fast read
  			tft.fillRect(0,16*7,400,16, TFT_BLACK);
  			tft.setCursor(0, 16*7);
  			tft.print(endTime-initialTime);
  			tft.print(" ms");

  			S_Fast += endTime - initialTime;
  			n_Fast++;

  			int fastAvg = S_Fast/n_Fast;

  			// Updates average with every button press
  			tft.fillRect(0,16*10,400,16, TFT_BLACK);
  			tft.setCursor(0,16*10);
  			tft.print(fastAvg);
  			tft.print(" ms");

        }

    }

}

int main() {
  setup();


  drawButton();

  while (true){
  	loop();
  	delay(5);
  }

  Serial.end();



  return 0;
}