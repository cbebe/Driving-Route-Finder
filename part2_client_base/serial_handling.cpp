#include "serial_handling.h"

extern shared_vars shared;

uint8_t get_waypoints(const lon_lat_32& start, const lon_lat_32& end) {
  // TODO: implement the communication protocol from the assignment
  enum {sending, receiving} state = receiving; 
  char serialmsg[]
  while (state = receiving) {
    // Send request to Serial
    while (!Serial.available()) {
      
      Serial.write('R'); 
      Serial.write(' '); int32_to_Serial(start.lon); 
      Serial.write(' '); int32_to_Serial(start.lat);
      Serial.write(' '); int32_to_Serial(end.lat);
      Serial.write(' '); int32_to_Serial(end.lat);
      Serial.write('\n');
      // don't overflow the serial!
      delay(500);
    }
    processSerial();
    int16_t num_waypoints = char_to_int32(buffer, 1, 1); 
    shared.num_waypoints = num;
  }
  

  




  // 1 indicates a successful exchange, of course you should only output 1
  // in your final solution if the exchange was indeed successful
  // (otherwise, return 0 if the communication failed)
  return 1;
}
