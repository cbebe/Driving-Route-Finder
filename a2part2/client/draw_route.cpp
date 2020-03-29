#include "draw_route.h"
#include "map_drawing.h"


#define AQUA 0x04FF

extern shared_vars shared;

// draws a route between the two endpoints
void draw_route() {
  xy_pos p1, p2;
  if (shared.num_waypoints >= 1) {
    for (int i = 0; i < shared.num_waypoints - 1; i++){
      // convert to x and y coordinates
      p1.x = longitude_to_x(shared.map_number, shared.waypoints[i].lon) - shared.map_coords.x;
      p1.y = latitude_to_y(shared.map_number, shared.waypoints[i].lat) - shared.map_coords.y;
      p2.x = longitude_to_x(shared.map_number, shared.waypoints[i + 1].lon) - shared.map_coords.x;
      p2.y = latitude_to_y(shared.map_number, shared.waypoints[i + 1].lat) - shared.map_coords.y;
      // draws line between points
      shared.tft->drawLine(p1.x, p1.y, p2.x, p2.y, TFT_BLACK);
    }
  }
}
