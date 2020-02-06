#ifndef _JOY_CURSOR_H
#define _JOY_CURSOR_H

#define SD_CS     10
#define JOY_VERT  A9 // should connect A9 to pin VRx
#define JOY_HORIZ A8 // should connect A8 to pin VRy


#define DISPLAY_WIDTH  480
#define DISPLAY_HEIGHT 320
#define MAP_WIDTH 420
#define MAP_HEIGHT 320
#define YEG_SIZE 2048

#define JOY_CENTER   512
#define JOY_DEADZONE 64
#define CURSOR_SIZE  9
#define C_RAD 4

void processJoystick();
void joySetup();

#endif
