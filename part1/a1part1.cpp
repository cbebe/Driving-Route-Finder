#include <Arduino.h>
#include <MCUFRIEND_kbv.h>
#include <SPI.h>
#include <SD.h>

#include "jcursor.h"
#include "touchs.h"
#include "coordinates.h"
#include "restlist.h"

int main() {
  setup();
  while (1) {
    // Mode 0
    processTouchScreen(); // for drawing dots
    processJoystick(); // for moving cursor

    // switches to Mode 1 when joystick is pressed
    if (digitalRead(JOY_SEL) == LOW) {

      int prevRest;
      loadAllRestaurants();
      while (digitalRead(JOY_SEL) == HIGH) {
		    joySelect(prevRest);
	    }
      goToResto();

      mapInit(); // reinitializes map after exiting Mode 1
    }
  }
  Serial.end();
  return 0;
}