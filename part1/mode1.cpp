#include <Adafruit_GFX.h>
#include "jcursor.h"
#include "restlist.h"

void Mode1() {
	int prevRest;
  loadAllRestaurants();
	while (digitalRead(JOY_SEL) == HIGH) {
		joySelect(prevRest);
	}
  goToResto();
}