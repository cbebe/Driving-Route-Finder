#ifndef SERIAL_HANDLING_H
#define SERIAL_HANDLING_H

#include <stdint.h>
#include "core_types.h"

int16_t srv_get_pathlen(LonLat32 start, LonLat32 end);

int16_t srv_get_waypoints(LonLat32* waypoints, 
        int16_t path_len, int16_t max_path_len);

int16_t serial_readline(char *line, uint16_t line_size);

int16_t string_read_field(const char *str, uint16_t str_start, 
    char *field, uint16_t field_size, const char *sep);

int32_t string_get_int(const char *str);

#endif
