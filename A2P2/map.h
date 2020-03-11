/*
 Definition of functions and structures for dealing with the map.
 Longitude and latitude to screen coordinate conversions, for instance.
 */

#ifndef MAP_H
#define MAP_H

#include <stdint.h>
#include <image_handling.h>
#include <SD.h>
#include <SPI.h>
#include "core_types.h"

// the number of the current map being displayed
extern uint8_t current_map_num;

// the current association of the map with the screen window
extern uint16_t screen_map_x;
extern uint16_t screen_map_y;

// radius of cursor dot.
extern const uint16_t dot_radius;

// the size of the map display window
extern const uint16_t display_window_width;
extern const uint16_t display_window_height;

// the current position of the cursor on the map
extern uint16_t cursor_map_x;
extern uint16_t cursor_map_y;

// the current approximate lat and lon of the cursor
extern int32_t cursor_lon;
extern int32_t cursor_lat;

extern const uint8_t num_maps;

extern uint16_t map_x_limit[6];
extern uint16_t map_y_limit[6];

typedef struct {
    int32_t N;  // lattitude of NW corner
    int32_t W;  // longitude of NW corner
    int32_t S;  // lattitude of SE corner
    int32_t E;  // longitude of SE corner
} map_box_t;

// a coordinate point for a path
typedef struct {
    int32_t lat;
    int32_t lon;
} coord_t;

extern map_box_t map_box[];

// conversion routines between lat and long and map pixel coordinates
int32_t x_to_longitude(char map_num, int32_t map_x);
int32_t y_to_latitude(char map_num, int32_t map_y);
int32_t longitude_to_x(char map_num, int32_t map_longitude);
int32_t latitude_to_y(char map_num, int32_t map_lattitude);

uint8_t zoom_in();
uint8_t zoom_out();
uint8_t set_zoom();

void initialize_map();
void draw_map_screen();
uint8_t get_cursor_screen_x_y(uint16_t *cursor_screen_x,uint16_t *cursor_screen_y);
void draw_cursor();
void erase_cursor();
void move_window_to(int16_t x, int16_t y);
void move_window(int32_t lon, int32_t lat);
void move_cursor_to(int16_t x, int16_t y);
void move_cursor_by(int16_t dx, int16_t dy);

extern volatile uint8_t shared_new_map_num;

#endif
