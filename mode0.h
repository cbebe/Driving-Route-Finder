#ifndef _MODE_ZERO_H
#define _MODE_ZERO_H

#define SD_CS     10
#define JOY_VERT  A9 // should connect A9 to pin VRx
#define JOY_HORIZ A8 // should connect A8 to pin VRy
#define JOY_SEL 53

#define DISPLAY_WIDTH  480
#define DISPLAY_HEIGHT 320
#define MAP_WIDTH 420
#define MAP_HEIGHT 320
#define YEG_SIZE 2048
#define BOUND_HEIGHT 315
#define BOUND_WIDTH 415

#define POS_BUFFER 576
#define NEG_BUFFER 448
#define JOY_DEADZONE
#define CURSOR_SIZE  9
#define C_RAD 4

#define LAT_NORTH 5361858
#define LAT_SOUTH 5340953
#define LON_WEST -11368652
#define LON_EAST -11333496

#define REST_START_BLOCK 4000000
#define NUM_RESTAURANTS 1066
#define NUM_LINES 21

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


void joySetup();
void mapInit();
void Mode0();
void Mode1();

#endif
