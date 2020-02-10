#ifndef COORDINATES_H
#define COORDINATES_H

#define JOY_SEL 53

#define TFT_WIDTH 480
#define MAP_WIDTH 420
#define CENTRE_X (MAP_WIDTH/2)

#define TFT_HEIGHT 320
#define MAP_HEIGHT 320
#define CENTRE_Y (MAP_HEIGHT/2)

#define YEG_SIZE 2048
#define MAX_X (YEG_SIZE - MAP_WIDTH/2)
#define MAX_Y (YEG_SIZE - MAP_HEIGHT/2)

#define LAT_NORTH 5361858
#define LAT_SOUTH 5340953
#define LON_WEST -11368652
#define LON_EAST -11333496

#define REST_START_BLOCK 4000000
#define NUM_RESTAURANTS 1066
#define NUM_LINES 21

#define SD_CS     10

struct restaurant {
  int32_t lat;
  int32_t lon;
  uint8_t rating; // from 0 to 10
  char name[55];
};

// global declaration to give access to multiple source files
extern MCUFRIEND_kbv tft; 
// global variables for loading data from sd card
extern uint32_t pastBlockNum;
extern restaurant restBlock[8];
extern Sd2Card card;
extern int currentPatchX, currentPatchY;

void loadAllRestaurants();
void getRestaurantFast(int restIndex, restaurant *restPtr);
void Mode0();
void Mode1();

#endif