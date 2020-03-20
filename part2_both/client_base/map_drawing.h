#ifndef __MAP_DRAWING_H
#define __MAP_DRAWING_H

#include "consts_and_types.h"

// conversion routines between lat and lon and map pixel coordinates
int32_t x_to_longitude(int8_t map_num, int32_t map_x);
int32_t y_to_latitude(int8_t map_num, int32_t map_y);
int32_t longitude_to_x(int8_t map_num, int32_t map_longitude);
int32_t latitude_to_y(int8_t map_num, int32_t map_lattitude);

// initialize coordinates and initial map
// does not actually draw anything, just initializes values in the extern
// variable "shared"
void initialize_display_values();

// centre the cursor on the current map view
void centre_cursor();

// centre the map around the cursor, clamping to the edges
void centre_map();

// get the display coordinates of the cursor
xy_pos get_cursor_screen();

// draw the map!
void draw_map();

// erase/draw the cursor on the map
void erase_cursor();
void draw_cursor();

// given x/y values, shift the display over that many "units"
// e.g. if shift is (1, 0) will shift over by half of a screen
void shift_map_display(const xy_pos& shift);

// print a message to the bottom of the screen
void status_message(const char* mst);

// return the geographic coordinates of the cursor
lon_lat_32 get_cursor_lonlat();

// zoom function, change map number by "change" (typically -1 or +1)
// centres new view on the cursor, unless the cursor is near the boundary
void zoom_map(int16_t change);

// move the cursor by the given xy coordinates
// returns 1 if the map has to be redrawn, otherwise 0
// will set the "redraw flag" in sharing if the map shifted
// assumes you will redraw the cursor either way
void move_cursor(const xy_pos& delta);

// constrains the cursor to lie within the padding of the current map
// useful for a couple of functions
void constrain_cursor();

#endif
