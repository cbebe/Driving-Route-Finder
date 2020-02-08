#ifndef COORDINATES_H
#define COORDINATES_H

#define JOY_VERT  A9 // should connect A9 to pin VRx
#define JOY_HORIZ A8 // should connect A8 to pin VRy
#define JOY_SEL 53

#define MAP_WIDTH 420
#define MAP_HEIGHT 320
#define YEG_SIZE 2048
#define BOUND_HEIGHT 315
#define BOUND_WIDTH 415

#define POS_BUFFER 576
#define NEG_BUFFER 448
#define JOY_DEADZONE 64
#define SPEED_DIV 128
#define CURSOR_SIZE  9
#define C_RAD 4

#define LAT_NORTH 5361858
#define LAT_SOUTH 5340953
#define LON_WEST -11368652
#define LON_EAST -11333496

#define REST_START_BLOCK 4000000
#define NUM_RESTAURANTS 1066
#define NUM_LINES 21

#define SD_CS     10
#define DISPLAY_WIDTH  480
#define DISPLAY_HEIGHT 320

struct restaurant {
  int32_t lat;
  int32_t lon;
  uint8_t rating; // from 0 to 10
  char name[55];
};

struct RestDist {
  uint16_t index;
  uint16_t dist;
};

// global declaration to give access to multiple source files
extern MCUFRIEND_kbv tft; 
// global variables for cursor and map positions 
extern int cursorX, cursorY, currentPatchX, currentPatchY;
// global variables for loading data from sd card
extern uint32_t pastBlockNum;
extern restaurant restBlock[8];
extern Sd2Card card;
extern RestDist rest_dist[NUM_RESTAURANTS];

void joySetup();
void mapInit();
void Mode0();
void Mode1();
void getRestaurantFast(int restIndex, restaurant *restPtr);

#endif