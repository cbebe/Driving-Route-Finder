/*
  VERY IMPORTANT
  the struct shared_vars contains all of the variables that are
  "global" to the project across multiple files.

  In particular, you are mostly concerned with
   - shared.num_waypoints: the number of waypoints on the path
   - shared.waypoints[]: the lat/lon pairs of these waypoints
   - max_waypoints (a global const, not in the shared_vars struct):
     the maximum number of waypoints that can be stored in the
     shared.waypoints[] array
*/

#ifndef __CONSTS_AND_TYPES_H
#define __CONSTS_AND_TYPES_H

#include <Arduino.h>
#include <MCUFRIEND_kbv.h>
#include "lcd_image.h"

// the name emphasizes the fact that lon is the first field
struct lon_lat_32 {
  int32_t lon, lat;
  lon_lat_32(int32_t lon1 = 0, int32_t lat1 = 0) : lon(lon1), lat(lat1) {}
};

// a handy pair to store two signed 16-bit integers
// used frequently in many places
struct xy_pos {
  int16_t x, y;
  xy_pos(int16_t x1 = 0, int16_t y1 = 0) : x(x1), y(y1) {}
};

// maximum number of waypoints that can be stored
// Do not increase! You might run out of memory.
const int16_t max_waypoints = 500;

// all the variables shared between various files of the project
struct shared_vars {
  // the upper-left corner of the current .lcd being displayed
  xy_pos map_coords;

  // the index of the current zoom level being displayed
  int8_t map_number;

  // location of the cursor in pixels on the display
  // (so between 0 and the width/height of the current map in pixels)
  xy_pos cursor_map;

  // the tft display
  MCUFRIEND_kbv* tft;

  // joystick calibration data
  xy_pos joy_centre;

  // stores most recent reading of the buttons
  // 1 means pushed, 0 means not pushed
  uint8_t zoom_in_pushed, zoom_out_pushed, joy_button_pushed;

  // a flag indicating something changed that should cause us to redraw
  // everything (map, cursor, and the waypoints once you do that part)
  uint8_t redraw_map;

  int16_t num_waypoints;
  lon_lat_32 waypoints[max_waypoints];
};

// collect all pins in a namespace
namespace clientpins {
  // Pins for sd card
  const uint8_t sd_cs = 10;

  // Pins and for the zoom in and out buttons.
  const uint8_t zoom_in_pin = 45;
  const uint8_t zoom_out_pin = 47;

  // analog pins for joystick axes
  const uint8_t joy_x_pin = A8;
  const uint8_t joy_y_pin = A9;

  // joystick button pin
  const uint8_t joy_button_pin = 53;
}

// constants related to the display
namespace displayconsts {
  // physical dimensions of the tft display
  const int16_t tft_width = 480;
  const int16_t tft_height = 320;

  // dimensions of the bottom "display" that has the FROM? or TO? prompt
  const int16_t msg_width = tft_width;
  const int16_t msg_height = 24;

  // the display window size for the map of edmonton
  const int16_t display_width = tft_width;
  const int16_t display_height = tft_height - msg_height;

  // amount of "padding" around the display, the cursor should
  // not move into the padding (will cause a map view nudge
  // unless we are near the boundary of the map itself)
  const int16_t padding = 5;


  // the minimum and maximum display coordinates we allow the
  // cursor to occupy, anything outside this should nudge the screen
  const int16_t min_x = padding;
  const int16_t max_x = display_width - padding;
  const int16_t min_y = padding;
  const int16_t max_y = display_height - padding;
};

// struct to store the geographic coordinates of the corners of some map
struct map_box_t {
    int32_t N;  // lattitude of NW corner
    int32_t W;  // longitude of NW corner
    int32_t S;  // lattitude of SE corner
    int32_t E;  // longitude of SE corner
};

// mostly all constants related to the .lcd maps
namespace mapdata {
  // maybe a bit out of place here, but it is related
  const int16_t dot_radius = 3;

  // the x and y coordinate limits for each map (in pixels)
  const int16_t map_x_limit[6] = { 511, 1023, 2047, 4095, 8191, 16383};
  const int16_t map_y_limit[6] = { 511, 1023, 2047, 4095, 8191, 16383};

  // data for the various edmonton images:
  // file name, pixel_height, pixel_width
  const lcd_image_t map_tiles[] = {
    { "yeg-1.lcd",  512, 512, },
    { "yeg-2.lcd",  1024, 1024, },
    { "yeg-3.lcd",  2048, 2048, },
    { "yeg-4.lcd",  4096, 4096, },
    { "yeg-5.lcd",  8192, 8192, },
    { "yeg-6.lcd",  16384, 16384, },
  };

  // geographic coordinates of the corners of each map
  const map_box_t map_box[] = {
    { // map 0 zoom 11
      (int32_t)     5364463,    //   53.6446378248565
      (int32_t)   -11373047,    // -113.73046875
      (int32_t)     5343572,    //   53.4357192066942
      (int32_t)   -11337891,    // -113.37890625
    },
    { // map 1 zoom 12
      (int32_t)     5364464,    //   53.6446378248565
      (int32_t)   -11373047,    // -113.73046875
      (int32_t)     5343572,    //   53.4357192066942
      (int32_t)   -11337891,    // -113.37890625
    },
    { // map 2 zoom 13
      (int32_t)     5361858,    //   53.6185793648952
      (int32_t)   -11368652,    // -113.6865234375
      (int32_t)     5340953,    //   53.4095318530864 S
      (int32_t)   -11333496,    // -113.3349609375 E
    },
    { // map 3 zoom 14
      (int32_t)     5360554,    //   53.605544099238
      (int32_t)   -11368652,    // -113.6865234375
      (int32_t)     5339643,    //   53.396432127096
      (int32_t)   -11333496,    // -113.3349609375
    },
    { // map 4 zoom 15
      (int32_t)     5360554,    //   53.605544099238
      (int32_t)   -11367554,    // -113.675537109375
      (int32_t)     5339643,    //   53.396432127096
      (int32_t)   -11332397,    // -113.323974609375
    },
    { // map 5 zoom 16
      (int32_t)     5360228,    //   53.6022846540113
      (int32_t)   -11367554,    // -113.675537109375
      (int32_t)     5339316,    //   53.3931565653804
      (int32_t)   -11332397,    // -113.323974609375
    },
  };
}

#endif
