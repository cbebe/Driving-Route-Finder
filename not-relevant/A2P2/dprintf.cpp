#include "dprintf.h"
#include <Arduino.h>

#ifdef __DPRINTF_ENABLE
/* only generate code if enabled */
static uint8_t dprintf_enable = 1;

uint8_t dprintf_control(uint8_t new_state) {
    /* set the control state for dprintf. 0 - off, 1 - on */

    uint8_t prev_state = dprintf_enable;
    dprintf_enable = new_state;
    return prev_state;
    }

void dprintf(const char* fmt, ... ) {
    /*  
        Generic debug printing function.  

        if dprintf_enable is true, then this function does the following:

        Takes a printf-like argument list an produces a string which is then
        prefixed with "D ", and shipped out on the serial port.  The 
        destination application detects the debug diagnostic message and then
        either ignores it, or displays it, as desired.

        The internal buffer for the message is set at 256.
    */

    if ( ! dprintf_enable ) {
        return;
        }

    // varargs is the mechanism for handling ... arbitrary number of params.
    va_list args;

    size_t buf_size = 256;
    char buf[buf_size];

    va_start(args, fmt);
    vsnprintf(buf, buf_size, fmt, args);
    va_end(args);

    // skip off the diagnostic message
    Serial.print("D ");
    Serial.println(buf);
    }
#endif
