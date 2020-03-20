#ifndef __DRAW_ROUTE_H
#define __DRAW_ROUTE_H

#include "consts_and_types.h"

/*
   Your job!

   Draw the route to the tft display using the extern variable shared.
   If the number of waypoints is 0, then draw nothing.

   Guidelines:
    - It is OK if the cursor erases the route when it goes over it.
    - Don't worry about making the route "thick", the normal drawLine function
      of the tft display is good enough.
    - Make it a colour that can be easily distinguished from the rest.
*/
void draw_route();

#endif
