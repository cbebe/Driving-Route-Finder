#include "serial_handling.h"

#include <Arduino.h>
#include <errno.h>
#include <assert13.h>
#include "dprintf.h"

/* YOUR TASK:
    replace the srv_get_pathlen and srv_get_waypoints functions
    with the proper implementations. NOTE: I (Aaron) already implemented them
    so you probably don't need to worry about it unless you find a nasty bug
*/

/*
    Gets the length of the path between start and end.

    The function sends a request to the server listening on the serial
    port and waits for the server responding with the number of
    waypoints along the path from start to end.

    Inputs:

    start, end - the start and end point of the requested path

    Returns:

    the length of the path, -1 if an error occurred

*/
int16_t srv_get_pathlen(LonLat32 start, LonLat32 end) {
    size_t buf_size = 32;
    size_t buf_len = 0;
    char buf[buf_size];

    int16_t path_len = 0;

    // start the server communication with a path request
    dprintf("Requesting lat %ld lon %ld to lat %ld lon %ld",
        start.lat, start.lon, end.lat, end.lon);
    char buffer[100];
    //send request to server
    sprintf(buffer,"R %ld %ld %ld %ld",start.lat,start.lon,end.lat,end.lon);
    Serial.println(buffer);

    //read lines until N is starting line
    int start_time = millis();
    int timeout = 1;

    // 10 second time out. Loop until acknowlegment or timeout
    while (true) {
        /* will read lines until first letter is N.
          The buffer may have garbage in it so it will have
          to clear it first before it hits the proper acknowledgment
        */
        serial_readline(buf,buf_size);
        //dprintf(buf);
        if (buf[0] == 'N') {
          timeout = 0;
          break;
        }

        int check = millis();
        if ((check - start_time) > 10000) {
          timeout = 1;
          break;
        }

    }

    if (timeout) {
      //print to serial monitor and diagnostic channel
    //  Serial.println("Error - client timed out after 10 seconds (srv_get_pathlen)");
      return -1;
    }

    sscanf(buf,"N %d\n",&path_len);


    dprintf("Got path_len of %d", path_len);

    // now you will expect to get a N dddddd message back from server
    return path_len;
    }

/*
    Fetch the waypoints from the server corresponding to a previously
    requested path. This function must be called once after a call to
    srv_get_pathlen returns a path of length > 0.

    The server wants to send path_len points, but we can only accept
    max_path_len. So we will continue reading the waypoints past
    max_path_len, but not store them.

    waypoints - an array of [max_path_len] waypoints, each one being
        a LonLat32 coordinate pair.

    path_len - the number of waypoints expected from the server, which
        could be more than the available storage in waypoints.  Excess
        waypoints are discarded by the client, even though the protocol
        continues.

    max_path_len - the maximum mumber of waypoints that can fit int the
        waypoints array

    Returns
        -1 if an error occurred
        >= 0 if ok.
*/

int16_t srv_get_waypoints(LonLat32* waypoints,
    int16_t path_len, int16_t max_path_len) {

    int32_t lat;
    int32_t lon;

    dprintf("Fetching %d way points, keeping at most %d",
        path_len, max_path_len);

    if ( path_len <= 0 || max_path_len < 0 ) {
        dprintf("Bad length %d or max %d", path_len, max_path_len);
        return -1;
        }
    //send acknowledgment to the server
    Serial.println("A");

    size_t buf_size = 32;
    char buf[buf_size];

    // WARNING - server uses lat lon order, client uses lon lat !
    for (int16_t i=0; i < path_len; ++i) {

        //read lines until W is starting line
        int start_time = millis();
        int timeout = 1;
        // 1 second time out. Same structure as timeout in srv_get_pathlen
        while (true) {
          serial_readline(buf,buf_size);
        //  dprintf(buf); //see whats inside the buffer
          if (buf[0] == 'W') {
            timeout = 0;
            break;
          }

          int check = millis();
          if ((check - start_time) >= 1000) {
            timeout = 1;
            break;
          }
        }

        if (timeout) {
          dprintf("Error - client timed out while waiting for acknowledgment (srv_get_waypoints)");
          return -1; //will be used to identify error in client.cpp
        }

        dprintf(buf);
        sscanf(buf,"W %ld %ld\n",&lat,&lon);


        dprintf("Got %ld |%ld|", lat, lon);

        if ( i < max_path_len ) {
            // skip if too many points
            waypoints[i] = LonLat32(lon, lat);
            }
        //send acknowledgment to the server to continue
        Serial.println("A");
      }

      //wait until final 'E' character pops up. send timeout if it takes
      //longer than one second
      int start_time = millis();
      int timeout = 0;
      while (true) {
        serial_readline(buf,buf_size);

        if (buf[0] == 'E') {
          timeout = 0;
          break;
        }
        int check = millis();
        if ((check - start_time) >= 1000) {
          timeout = 1;
          break;
                  }
        }
        if (timeout) {
          return -2; //this will be used in identifying error in client.cpp
        }

    return 0;
    }


/*
    Function to read a single line from the serial buffer up to a
    specified length (length includes the null termination character
    that must be appended onto the string). This function is blocking.
    The newline character sequence is given by CRLF, or "\r\n".

    Arguments:

    buffer - Pointer to a buffer of characters where the string will
        be stored.

    length - The maximum length of the string to be read.

    Preconditions:  None.

    Postconditions: Function will block until a full newline has been
        read, or the maximum length has been reached. Afterwards the new
        string will be stored in the buffer passed to the function.

    Returns: the number of bytes read

*/
int16_t serial_readline(char *line, uint16_t line_size) {
    int bytes_read = 0;    // Number of bytes read from the serial port.
    int start_time = millis();


    // Read until we hit the maximum length, or a newline.
    // One less than the maximum length because we want to add a null terminator.
    while (bytes_read < line_size - 1) {
        while (Serial.available() == 0) {
            // There is no data to be read from the serial port.
            // Wait until data is available.
            //will stay at this stage for at most half a second
            int check = millis();
            if ((check - start_time) > 500) {
              return 0;
            }
        }

        line[bytes_read] = (char) Serial.read();

        // A newline is given by \r or \n, or some combination of both
        // or the read may have failed and returned 0
        if ( line[bytes_read] == '\r' || line[bytes_read] == '\n' ||
             line[bytes_read] == 0 ) {
                // We ran into a newline character!  Overwrite it with \0
                break;    // Break out of this - we are done reading a line.
        } else {
            bytes_read++;
        }
    }

    // Add null termination to the end of our string.
    line[bytes_read] = '\0';
    return bytes_read;
    }

/*
    Function to read a portion of a string into a buffer, up to any
    given separation characters. This will read up to the specified
    length of the character buffer if a separation character is not
    encountered, or until the end of the string which is being copied. A
    starting index of the string being copied can also be specified.

    Arguments:

    str -  The string which is having a portion copied.
    str_start -  The index of the string to start at. (Less than str's length).
    buf -  The buffer to store the copied chunk of the string into.
    buf_len -  The length of the buffer.
    sep -  String containing characters that will be used as separators.

    Preconditions:  Make sure str_start does *NOT* exceed the size of str.

    Postconditions: Stores the resulting string in buf, and returns the
    position where reading was left off at.  The position returned will skip
    separation characters.

*/
int16_t string_read_field(const char *str, uint16_t str_start,
    char *field, uint16_t field_size, const char *sep) {

    // Want to read from the string until we encounter the separator.

    // Character that we are reading from the string.
    uint16_t str_index = str_start;

    while (1) {
        if ( str[str_index] == '\0') {
            str_index++;  // signal off end of str
            break;
            }

        if ( field_size <= 1 ) break;

        if (strchr(sep, str[str_index])) {
            // field finished, skip over the separator character.
            str_index++;
            break;
            }

        // Copy the string character into buffer and move over to next
        *field = str[str_index];
        field++;
        field_size--;
        // Move on to the next character.
        str_index++;
        }

    // Make sure to add NULL termination to our new string.
    *field = '\0';

    // Return the index of where the next token begins.
    return str_index;
    }

/*
    Function to convert a string to an int32_t.

    Arguments:
    str -  The string to convert to an integer.

    Preconditions:  The string should probably represent an integer value.

    Postconditions:
        Will return the equivalent integer. If an error occured the
        blink assert may be triggered, and sometimes zero is just
        returned from this function because EINVAL does not exist for
        the Arduino strtol.

 */

int32_t string_get_int(const char *str) {
    // Attempt to convert the string to an integer using strtol...
    int32_t val = strtol(str, NULL, 10);

    if (val == 0) {
        // Must check errno for possible error.
        if (errno == ERANGE) {
            dprintf("ERROR string_get_int failed with %d on '%s'",
                errno, str);
            assert13(0, errno);
            }
        }

    return val;
    }
