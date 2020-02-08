#ifndef JCURSOR_H
#define JCURSOR_H

#define JOY_VERT  A9 // should connect A9 to pin VRx
#define JOY_HORIZ A8 // should connect A8 to pin VRy

#define BOUND_HEIGHT 315
#define BOUND_WIDTH 415

#define POS_BUFFER 576
#define NEG_BUFFER 448
#define JOY_DEADZONE 64
#define SPEED_DIV 128
#define CURSOR_SIZE  9
#define CUR_RAD 4
#define CIRC_RAD 3

extern int cursorX, cursorY, currentPatchX, currentPatchY;

void mapInit();
void processJoystick();
void joySetup();

#endif