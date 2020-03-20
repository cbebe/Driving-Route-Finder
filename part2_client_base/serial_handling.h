#ifndef __SERIAL_HANDLING_H
#define __SERIAL_HANDLING_H

#include "consts_and_types.h"
#include "map_drawing.h"

/*
   Your job!

   Print the request with the given endpoints and read the replies
   from the server. Store the number of waypoints and the waypoints themselves
   in the appropriate fields of the extern variable shared

   Returns 1 if successful, otherwise 0.

   You can feel free to modify this as you see fit. If you want to break
   the steps into multiple functions, then go ahead.

   If the number of waypoints is > max_waypoints (from consts_and_types.h),
   print some message using status_message() indicating the path is too long,
   delay for 5 seconds, and then resume having treated this like there is no
   path.
*/
uint8_t get_waypoints(const lon_lat_32& start, const lon_lat_32& end);

#endif
