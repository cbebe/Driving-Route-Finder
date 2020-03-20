#include "map_drawing.h"

// all shared variables in the project!
// declared in client.cpp
extern shared_vars shared;

// conversion routines between lat and long and map pixel coordinates
int32_t x_to_longitude(int8_t map_num, int32_t map_x) {
  return map(map_x,
             0, mapdata::map_x_limit[map_num],
             mapdata::map_box[map_num].W, mapdata::map_box[map_num].E);
}

int32_t y_to_latitude(int8_t map_num, int32_t map_y) {
  return map(map_y,
             0, mapdata::map_y_limit[map_num],
             mapdata::map_box[map_num].N, mapdata::map_box[map_num].S);
}


int32_t longitude_to_x(int8_t map_num, int32_t map_lon) {
  return map(map_lon,
             mapdata::map_box[map_num].W, mapdata::map_box[map_num].E,
             0, mapdata::map_x_limit[map_num]);
}

int32_t latitude_to_y(int8_t map_num, int32_t map_lat) {
    return map(map_lat,
               mapdata::map_box[map_num].N, mapdata::map_box[map_num].S,
               0, mapdata::map_y_limit[map_num]);
}

// set initial map and cursor view values
void initialize_display_values() {
  shared.map_number = 2; // initial zoom level 2

  // centre the map on the initial map zoom level
  shared.map_coords = xy_pos(
    mapdata::map_x_limit[shared.map_number]/2 - displayconsts::display_width/2,
    mapdata::map_y_limit[shared.map_number]/2 - displayconsts::display_height/2
  );

  // and then centre the cursor on this view
  centre_cursor();
}

// centres the cursor, assumes the map view is correct
void centre_cursor() {
  shared.cursor_map = xy_pos(
    shared.map_coords.x+displayconsts::display_width/2,
    shared.map_coords.y+displayconsts::display_height/2
  );
}

// constrains the current map coordinates to the actual map dimensions
// useful if you nudge the map view to make sure it does not
// shift to a view outside what is represented in the .lcd file
void constrain_map() {
  int16_t new_x = constrain(
    shared.map_coords.x, 0,
    mapdata::map_x_limit[shared.map_number]-displayconsts::display_width
  );

  int16_t new_y = constrain(
    shared.map_coords.y, 0,
    mapdata::map_y_limit[shared.map_number]-displayconsts::display_height
  );

  shared.map_coords = xy_pos(new_x, new_y);
}

// centre the map around the cursor position (which is assumed correct)
void centre_map() {
  shared.map_coords.x = shared.cursor_map.x - displayconsts::display_width/2;
  shared.map_coords.y = shared.cursor_map.y - displayconsts::display_height/2;

  constrain_map();
}

// get the x/y coordinates of the display of the cursor
// recall cursor positions are, by default, stored in terms of the
// entire current map
xy_pos get_cursor_screen() {
  return xy_pos(
    shared.cursor_map.x - shared.map_coords.x,
    shared.cursor_map.y - shared.map_coords.y
  );
}

// draw the current map view!
void draw_map() {
  lcd_image_draw(
    &mapdata::map_tiles[shared.map_number], shared.tft,
    shared.map_coords.x, shared.map_coords.y,
    0, 0,
    displayconsts::display_width, displayconsts::display_height
  );
}

// erase the position of the current cursor
void erase_cursor() {
  xy_pos cursor_disp = get_cursor_screen();
  lcd_image_draw(
    &mapdata::map_tiles[shared.map_number], shared.tft,
    shared.cursor_map.x - mapdata::dot_radius, shared.cursor_map.y - mapdata::dot_radius,
    cursor_disp.x - mapdata::dot_radius, cursor_disp.y - mapdata::dot_radius,
    2*mapdata::dot_radius+1, 2*mapdata::dot_radius+1
  );
}

// draws the cursor
void draw_cursor() {
  xy_pos cursor_disp = get_cursor_screen();
  shared.tft->fillCircle(cursor_disp.x, cursor_disp.y, mapdata::dot_radius, TFT_RED);
}

// given a pair of x/y shifts, will shift the display over that many
// "half screens"
// example: if shift.x = 1 then the map screen is nudged to the east
// by half of a display so the east edge of the old view is now centred
void shift_map_display(const xy_pos& shift) {
  shared.map_coords.x += shift.x * displayconsts::display_width/2;
  shared.map_coords.y += shift.y * displayconsts::display_height/2;

  constrain_map();
}

// prints a message to the bottom part of the tft screen
void status_message(const char* msg) {
  shared.tft->fillRect(
    0, displayconsts::display_height, displayconsts::display_width,
    24, TFT_GREEN
  );

  shared.tft->setTextColor(TFT_PURPLE);
  shared.tft->setCursor(5, displayconsts::display_height+4);
  shared.tft->setTextSize(2);

  shared.tft->println(msg);
}

// returns the geographic coordinates of the cursor position
lon_lat_32 get_cursor_lonlat() {
  int32_t lon = x_to_longitude(shared.map_number, shared.cursor_map.x);
  int32_t lat = y_to_latitude(shared.map_number, shared.cursor_map.y);
  return lon_lat_32(lon, lat);
}

// constrain the cursor to be within the padded boundary of the map image
void constrain_cursor() {
  shared.cursor_map.x = constrain(
    shared.cursor_map.x,
    displayconsts::padding + 1,
    mapdata::map_x_limit[shared.map_number] - displayconsts::padding - 1
  );

  shared.cursor_map.y = constrain(
    shared.cursor_map.y,
    displayconsts::padding + 1,
    mapdata::map_y_limit[shared.map_number] - displayconsts::padding - 1
  );
}

// zoom function, change map number by "change" (typically -1 or +1)
// centres new view on the cursor, unless the cursor is near the boundary
void zoom_map(int16_t change) {
  // do nothing if we are already zoomed in as far as we can
  int16_t new_map = constrain(shared.map_number + change, 0, 4);

  if (new_map == shared.map_number) {
    // cannot zoom in this direction any further
    return;
  }

  lon_lat_32 geo(
    x_to_longitude(shared.map_number, shared.cursor_map.x),
    y_to_latitude(shared.map_number, shared.cursor_map.y)
  );

  shared.cursor_map = xy_pos(
    longitude_to_x(new_map, geo.lon),
    latitude_to_y(new_map, geo.lat)
  );

  shared.map_number = new_map;

  // Just in case. I don't trust that the scaling will be pixel-perfect
  // so let's make sure the new cursor did not stray into the boundary
  constrain_cursor();

  centre_map();
}

// moves the cursor by the specified number of pixels
// clamps it by the display padding around the map boundary
// shifts the map if it was nudged
// records a map shift by setting shared.redraw_map=1
void move_cursor(const xy_pos& delta) {
  shared.cursor_map.x += delta.x;
  shared.cursor_map.y += delta.y;
  constrain_cursor();

  xy_pos cur_pos = get_cursor_screen();
  xy_pos shift(0, 0);

  if (cur_pos.x < displayconsts::min_x) shift.x = -1;
  else if (cur_pos.x > displayconsts::max_x) shift.x = 1;

  if (cur_pos.y < displayconsts::min_y) shift.y = -1;
  else if (cur_pos.y > displayconsts::max_y) shift.y = 1;

  if (shift.x != 0 || shift.y != 0) {
    shared.redraw_map = 1;
    shift_map_display(shift);
  }
}
