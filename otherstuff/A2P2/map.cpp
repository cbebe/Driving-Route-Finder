#include <Arduino.h>
#include <Adafruit_GFX.h>        // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include "lcd_image.h"
#include "map.h"

// #define DEBUG

/*
Module to handle the display of map, cursor, and path information
on the screen.

It assumes that all of the map information has been placed on the
SD card as .lcd 16 bit rgb files, and that the tile dimensions
and lat-long positions match that of the data structures below.

All coordinates are in usual graphics corrdinates, that is, the
top left is (0,0) and the bottom right is (SCREEN_WIDTH-1,SCREEN_HEIGHT-1)

At any given time, the map image shown on the display is a window onto
a bigger map tile.  The top-left position of the display corrsponds
to position (screen_map_x, screen_map_y) in the map tile.

The cursor can be anywhere on the map, so it may not be visible in the
current display window.  It is visible if
screen_map_x < 
*/

extern Adafruit_ST7735 tft;

// the number of the current map being displayed
uint8_t current_map_num;

// the size of the map display window
const uint16_t display_window_width = 128;
const uint16_t display_window_height = 160;

// the current association of the map with the display window
uint16_t screen_map_x;
uint16_t screen_map_y;

// radius of cursor dot.
const uint16_t dot_radius = 2;

// the current position of the cursor on the map
uint16_t cursor_map_x;
uint16_t cursor_map_y;

// the current approximate lat and lon of the cursor
int32_t cursor_lon;
int32_t cursor_lat;

const uint8_t num_maps = 6;

/* 
Map limits: a pixel position on map i must lie in the interval
0 <= x <= map_x_limit[i]
0 <= y <= map_y_limit[i]
*/

uint16_t map_x_limit[6] = { 511, 1023, 2047, 4095, 8191, 16383};
uint16_t map_y_limit[6] = { 511, 1023, 2047, 4095, 8191, 16383};

lcd_image_t map_tiles[] = {
    { "yeg-1.lcd",  512, 512, },
    { "yeg-2.lcd",  1024, 1024, },
    { "yeg-3.lcd",  2048, 2048, },
    { "yeg-4.lcd",  4096, 4096, },
    { "yeg-5.lcd",  8192, 8192, },
    { "yeg-6.lcd",  16384, 16384, },
};

map_box_t map_box[] = {
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



// conversion routines between lat and long and map pixel coordinates
int32_t x_to_longitude(char map_num, int32_t map_x) {
    return map(map_x
        , (int32_t) 0, (int32_t) map_x_limit[map_num]
        , (int32_t) map_box[map_num].W, (int32_t) map_box[map_num].E);
    }

int32_t y_to_latitude(char map_num, int32_t map_y) {
    return map(map_y
        , (int32_t) 0, (int32_t) map_y_limit[map_num]
        , (int32_t) map_box[map_num].N, (int32_t) map_box[map_num].S);
    }

int32_t longitude_to_x(char map_num, int32_t map_longitude) {
    return map(map_longitude
        , (int32_t) map_box[map_num].W, (int32_t) map_box[map_num].E
        , (int32_t) 0, (int32_t) map_x_limit[map_num]);
    }

int32_t latitude_to_y(char map_num, int32_t map_latitude) {
    return map(map_latitude
        , (int32_t) map_box[map_num].N, (int32_t) map_box[map_num].S
        , (int32_t) 0, (int32_t) map_y_limit[map_num]);
    }

// zoom in and out routines that are used in interrupt handlers to
// change the shared version of the map_num.  In order to maintain consistent
// updates, the shared value should only be sampled at well-defined points.

volatile uint8_t shared_new_map_num = 2;

void initialize_map() {
    cursor_lon = 0;
    cursor_lat = 0;
    cursor_map_x = 0;
    cursor_map_y = 0;
    screen_map_x = 0;
    screen_map_y = 0;

    // set the actual map number from the shared version
    // this should be atomic and thus can be done outside a critical section
    current_map_num = shared_new_map_num;
}

uint8_t zoom_in() {
    if (shared_new_map_num < num_maps-1) { 
        shared_new_map_num++; 
    }
    return shared_new_map_num;
}

uint8_t zoom_out() {
    if (shared_new_map_num > 0) { 
        shared_new_map_num--; 
    }
    return shared_new_map_num;
}

uint8_t set_zoom() {
    // set the actual map number from the shared version
    // this should be atomic and thus can be done outside a critical section
    current_map_num = shared_new_map_num;

    // At this point all of the cursor and map information is invalidated
    // except for the cursor lat and long, which is the only thing that
    // survives zooming.  So set the map cursor position on the new map to
    // roughly correspond to lat and lon position that was determined by the
    // previous map.

    cursor_map_x = longitude_to_x(current_map_num, cursor_lon);
    cursor_map_y = latitude_to_y(current_map_num, cursor_lat);

    return current_map_num;
}
        

void draw_map_screen() {
#ifdef DEBUG
    // Want to display a small message saying that we are redrawing the map!
    tft.fillRect(78, 148, 50, 12, GREEN);

    tft.setTextSize(1);
    tft.setTextColor(MAGENTA);
    tft.setCursor(80, 150);
    tft.setTextSize(1);

    tft.println("DRAWING...");
#endif

    lcd_image_draw(&map_tiles[current_map_num], &tft
        , screen_map_x, screen_map_y
        , 0, 0, 128, 160);
    
}


uint8_t is_cursor_visible() {
    uint8_t r = 
        screen_map_x < cursor_map_x &&
            cursor_map_x < screen_map_x + display_window_width &&
                screen_map_y < cursor_map_y &&
                    cursor_map_y < screen_map_y + display_window_height; 

#ifdef IGNORE
    if (!r) {
        Serial.print("cv: ");
        Serial.print(screen_map_x);
        Serial.print(" ");
        Serial.print(screen_map_y);
        Serial.print(" ");
        Serial.print(cursor_map_x);
        Serial.print(" ");
        Serial.print(cursor_map_y);
        Serial.println();
    }
#endif

    return r;
}

uint8_t get_cursor_screen_x_y( uint16_t *cursor_screen_x
                             , uint16_t *cursor_screen_y) {
    if (is_cursor_visible) {
        *cursor_screen_x = cursor_map_x - screen_map_x;
        *cursor_screen_y = cursor_map_y - screen_map_y;
        return 1;
    }

    // not visible
    return 0;
}

void draw_cursor() {
    // the current position of the cursor on the screen, if visible
    uint16_t cursor_screen_x;
    uint16_t cursor_screen_y;
    if (get_cursor_screen_x_y(&cursor_screen_x, &cursor_screen_y)) {
        cursor_screen_x = cursor_map_x - screen_map_x;
        cursor_screen_y = cursor_map_y - screen_map_y;
        tft.fillCircle(cursor_screen_x, cursor_screen_y, dot_radius, RED);
    }
}

void erase_cursor() {
    uint16_t cursor_screen_x;
    uint16_t cursor_screen_y;
    if (get_cursor_screen_x_y(&cursor_screen_x, &cursor_screen_y)) {
        // Redraw the map on top of the current cursor position
        lcd_image_draw(&map_tiles[current_map_num], &tft,
        cursor_map_x - dot_radius,
        cursor_map_y - dot_radius,
        cursor_screen_x - dot_radius,
        cursor_screen_y - dot_radius,
        2 * dot_radius + 1,
        2 * dot_radius + 1);
    }
}

void move_window(int32_t lon, int32_t lat) {
    // Shift the current window to have lon and lat in top left corner

    screen_map_x = longitude_to_x(current_map_num, lon);
    screen_map_y = latitude_to_y(current_map_num, lat);

    move_window_to(screen_map_x, screen_map_y);
}

void move_window_to(int16_t x, int16_t y) {
    // Shift the current window to have x y in top left corner
    // cursor does not move

    // constrain move
    screen_map_x = constrain(
        x, 0, map_x_limit[current_map_num] - display_window_width);
    screen_map_y = constrain(
        y, 0, map_y_limit[current_map_num] - display_window_height);
}

void move_cursor_by(int16_t dx, int16_t dy) {
    move_cursor_to(cursor_map_x + dx, cursor_map_y + dy);
}

void move_cursor_to(int16_t x, int16_t y) {
    // move cursor on map
    cursor_map_x = constrain(x, 0, map_x_limit[current_map_num]);
    cursor_map_y = constrain(y, 0, map_y_limit[current_map_num]);

    // then update cursor lat and long - we need this because that is the
    // only thing that will survive zooming.

    cursor_lon = x_to_longitude(current_map_num, cursor_map_x);
    cursor_lat = y_to_latitude(current_map_num, cursor_map_y);
}

