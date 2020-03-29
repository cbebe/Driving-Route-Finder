#include "draw_route.h"
#include "map_drawing.h"


#define AQUA 0x04FF

extern shared_vars shared;

void draw_route() {
  // implement this!
  
  if (shared.num_waypoints >= 1 ) {
    // If there is at least 1 way point

    status_message ("Receiving path...");

    for (int i = 0; i < shared.num_waypoints - 1; i++){
      int32_t x0 = longitude_to_x(shared.map_number, shared.waypoints[i].lon) - shared.map_coords.x;
      int32_t x1 = longitude_to_x(shared.map_number, shared.waypoints[i+1].lon) - shared.map_coords.x;
      int32_t y0 = latitude_to_y(shared.map_number, shared.waypoints[i].lat) - shared.map_coords.y;
      int32_t y1 = latitude_to_y(shared.map_number, shared.waypoints[i+1].lat) - shared.map_coords.y;

      
      if ( x0 >= 0 && x0 <= 320 && y0 >= 0 && y0 <= 480
        && x1 >= 0 && x1 <= 320 && y1 >= 0 && y1 <= 480 ){
        // If the line to be plotted is in the scope of the zoom level
        shared.tft -> drawLine(x0,y0,x1,y1,AQUA);
      }
    }
  }

  else {
    status_message( "No path found. ");
  }
}
