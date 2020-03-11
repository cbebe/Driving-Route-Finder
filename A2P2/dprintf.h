#ifndef __DPRINTF_H
#define __DPRINTF_H

#include <stdint.h>
/*
    Set order of next two staements to enable, or completely remove 
    dprintf functionalty and code generation.
*/
#undef __DPRINTF_ENABLE
#define __DPRINTF_ENABLE

#ifndef __DPRINTF_ENABLE
    /* Completely remove dprintf from the code

    these macros will consume all their arguments so that no format string
    is created, and no code is generated.  It is better than an empty
    function declaration like
        void dprintf(const char* fmt, ... ) {}
        uint8_t dprintf_control(uint8_t new_state) {}
    */
    #define dprintf(...)
    #define dprintf_control(...)
#else
    uint8_t dprintf_control(uint8_t new_state);
    void dprintf(const char* fmt, ... );
#endif
#endif
