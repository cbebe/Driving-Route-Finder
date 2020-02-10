#include "jcursor.h"
#include "touchs.h"

void Mode0() {
  processTouchScreen(); // for drawing dots
  processJoystick(); // for moving cursor
}