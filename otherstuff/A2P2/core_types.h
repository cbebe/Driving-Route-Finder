#ifndef __CORE_TYPES_H
#define __CORE_TYPES_H

// the horrible names are chosen so that one does not mess up
// the initialization order..

// A pair of 32 bit coordinates, we use for accessing longitudes and
// latitudes
struct LonLat32 { 
    int32_t lon;
    int32_t lat;
    // We provide a constructor so that one can use a = Point32(lat,lon);
    LonLat32( int32_t p_lon=0, int32_t p_lat=0 ) 
    : lon(p_lon),lat(p_lat) // initializing the member variables
    {}
};

// A pair of 16 bit coordinates.
struct XY16 { 
    int16_t x;
    int16_t y;
    XY16( int32_t p_x=0, int32_t p_y=0 ) 
    : x(p_x), y(p_y) {}
};

// 16 bit pair of width and height
struct WidthHeight16 {
    int16_t width;
    int16_t height;
    WidthHeight16( int16_t p_width=0, int16_t p_height=0 ) 
    : width(p_width), height(p_height) {}
};

#endif //__CORE_TYPES_H
