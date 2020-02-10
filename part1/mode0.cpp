#include "jcursor.h"
#include "touchs.h"

void Mode0() {
  processTouchScreen();
  processJoystick();
}