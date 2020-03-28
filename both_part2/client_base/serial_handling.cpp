#include "serial_handling.h"

extern shared_vars shared;

// prints request to Serial
void sendReq(const lon_lat_32& start, const lon_lat_32& end) {
  // Request format: R slon slat elon elat
  Serial.print("R "); 
  Serial.print(start.lon); Serial.print(" ");
  Serial.print(start.lat); Serial.print(" ");
  Serial.print(end.lon); Serial.print(" ");
  Serial.println(end.lat);
}

// timeout function based on cmput274 assignment 2
bool waitOnSerial() {
  uint32_t timeout = millis() + 1000;
  while ((millis() < timeout && Serial.available() == 0) {delay(1);}
  return Serial.available() > 0;
}
// processes line from serial
// returns false if nothing was received
bool processLine(char *buff) {
  int len = 0;
  if (waitOnSerial()) {
    do {
      c = Serial.read();
      // only read 24 characters and drop any excess
      if (len < 24) {
        buff[len] = c;
        len++
        buff[len] = 0;
      }
    } while ((c != '\n' || c != '\r') && waitOnSerial())
    return true;
  }
  return false;
}

bool getNumWayPoints(char *buffer) {
  // restart if not a valid number of waypoints
  if (buffer[0] != 'N') {return false;}
  
  return true;
}

uint8_t get_waypoints(const lon_lat_32& start, const lon_lat_32& end) {
  // Currently this does not communicate over the serial port.
  // It just stores a path length of 0. You should make it communicate with
  // the server over Serial using the protocol in the assignment description.

  // TODO: implement the communication protocol from the assignment

  // for now, nothing is stored
  shared.num_waypoints = 0;
  sendReq(start, end);
  // resend request when it times out (1 second)
  if (!waitOnSerial()) {return 0;}
  char buffer[25];
  // return 0 at any point that the communication fails
  if (!processLine(buffer)) {return 0;}
  if (!getNumWayPoints(buffer)) {return 0;}
  // 1 indicates a successful exchange, of course you should only output 1
  // in your final solution if the exchange was indeed successful
  // (otherwise, return 0 if the communication failed)
  return 1;
}
