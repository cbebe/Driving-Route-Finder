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
  while (millis() < timeout && Serial.available() == 0) {delay(1);}
  return Serial.available() > 0;
}
// processes line from serial
// returns false if nothing was received
bool processLine(char *buff) {
  int len = 0; char c;
  if (waitOnSerial()) {
    do {
      c = Serial.read();
      // only read 24 characters and drop any excess
      if (len < 24) {
        buff[len] = c;
        len++;
        buff[len] = 0;
      }
    } while ((c != '\n' || c != '\r') && waitOnSerial());
    return true;
  }
  return false;
}

// get number of waypoints from serial
// returns true if successful
bool getNumWayPoints() {
  char buffer[25];
  // restart if not a valid number of waypoints
  if (processLine(buffer) && buffer[0] == 'N') {
    // copy buffer to another char array
    // assumes 500 is the max waypoints
    char num[4]; num[3] = 0;
    for (int i = 0; i < 3; i++) {
      num[i] = buffer[i + 2];
    }
    shared.num_waypoints = atoi(num);
    return true;
  }
  return false;
}


bool processWaypoint(int16_t index) {
  char buff[25], lon[10], lat[10];
  if (processLine(buff) && buff[0] == 'W') {
    // assumes that all numbers would be the same format:
    // 7 digits longitude 8 digits negative latitude
    for (int i = 0; i < 10; i++) {
      lon[i] = buff[i + 2];
      lat[i] = buff[i + 10];
    }
    // add null terminators
    lon[9] = 0; lat[9] = 0;

    // store parsed coordinates to shared variable
    shared.waypoints[index].lon = atoi(lon);
    shared.waypoints[index].lat = atoi(lat);

    // send acknowledge to serial
    Serial.println("A");
    return true;
  }
  return false;
}

uint8_t get_waypoints(const lon_lat_32& start, const lon_lat_32& end) {
  // Currently this does not communicate over the serial port.
  // It just stores a path length of 0. You should make it communicate with
  // the server over Serial using the protocol in the assignment description.

  // TODO: implement the communication protocol from the assignment
  // send request to serial
  sendReq(start, end);
  // return 0 at any point that the communication fails
  if (!getNumWayPoints()) {return 0;}
  // limits the number of waypoints from 0 to 500 (max_waypoints)
  shared.num_waypoints = constrain(shared.num_waypoints, 0, max_waypoints);

  for (int i = 0; i < shared.num_waypoints; i++) {
    if (!processWaypoint(i)) {return 0;}
  }
  // 1 indicates a successful exchange, of course you should only output 1
  // in your final solution if the exchange was indeed successful
  // (otherwise, return 0 if the communication failed)
  return 1;
}
