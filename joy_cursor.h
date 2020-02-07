#ifndef _JOY_CURSOR_H
#define _JOY_CURSOR_H

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

#define JOY_CENTER   512
#define JOY_DEADZONE 64
#define CURSOR_SIZE  9
#define C_RAD 4

#define NUM_LINES 21

void processJoystick();
void processAnalog(int aVal, char dir);
void joySetup();
void mapInit();

#endif
