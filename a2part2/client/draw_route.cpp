#include "draw_route.h"
#include "map_drawing.h"


#define AQUA 0x04FF

extern shared_vars shared;

void draw_route() {
  // implement this!
  // If there is at least 1 way point
  if (shared.num_waypoints >= 1) {
    char msg[100];
    sprintf(msg, "%d %d", shared.map_coords.x, shared.map_coords.y);    
    status_message(msg);
    delay(1000);
    for (int i = 0; i < shared.num_waypoints - 1; i++){
      int32_t x0 = longitude_to_x(shared.map_number, shared.waypoints[i].lon);
      int32_t x1 = longitude_to_x(shared.map_number, shared.waypoints[i+1].lon);
      int32_t y0 = latitude_to_y(shared.map_number, shared.waypoints[i].lat);
      int32_t y1 = latitude_to_y(shared.map_number, shared.waypoints[i+1].lat);
      sprintf(msg, "%ld %ld %ld %ld", x0, x1, y0, y1);
      status_message(msg);
      delay(500);
      // If the line to be plotted is in the scope of the zoom level
      shared.tft->drawLine(x0,y0,x1,y1,AQUA); 
    }
  }
}
