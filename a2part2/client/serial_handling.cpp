// ---------------------------------------------------
// Name : Charles Ancheta and Poulomi Ganguly
// CMPUT 275 , Winter 2020
// 
// Assignment 2, Part 2: Driving Route Finder (client)
// ---------------------------------------------------

#include "serial_handling.h"

extern shared_vars shared;

// prints request to Serial
void send_request(const lon_lat_32& start, const lon_lat_32& end) {
  char msg[40];
  // formats string for message
  sprintf(msg, "R %ld %ld %ld %ld", 
          start.lat, start.lon, end.lat, end.lon);
  Serial.println(msg); 
}

// processes line from serial
// returns false if nothing was received
bool process_line(char *buff, uint32_t timeout) {
  int len = 0; char c;
  uint32_t deadline = millis() + timeout;

  // adapted from simpleclient.cpp to read with timeout
  while (millis() < deadline) {
    if (Serial.available()) {
      c = Serial.read();
      if (c == '\n' || c == '\r') {
        return true;
      } else {
        buff[len] = c;
        len++;
        buff[len] = 0; // null terminator
      }
    }
  }

  //reading failed
  return false;
}

// get number of waypoints from serial
// returns true if successful
bool get_num() {
  char buffer[25];
  // 10 second timeout for number of waypoints
  if (process_line(buffer, 10000)) {
    if (buffer[0] == 'N') {
      // copy buffer to another char array
      char num[4]; num[3] = 0; // null terminator
      // assumes 3 digits max
      for (int i = 0; i < 3; i++) {
        num[i] = buffer[i + 2];
      }
      // convert to int and store in variable struct
      shared.num_waypoints = atoi(num);
      return true;
    }
  }
  return false;
}


bool process_waypoint(int16_t index) {
  char buff[25], lon[10], lat[10];
  // 1 second timeout for every waypoint (1000 ms)
  if (process_line(buff, 1000)) {
    if (buff[0] == 'W') {
      // assumes that all coordinates would be the same format:
      // 7 digits latitude 8 digits negative longitude  
      for (int i = 0; i < 10; i++) {
        lat[i] = buff[i + 2];
        lon[i] = buff[i + 10];
      }
      // add null terminators
      lon[9] = 0; lat[9] = 0;

      // store parsed coordinates to shared variable
      shared.waypoints[index].lon = atol(lon);
      shared.waypoints[index].lat = atol(lat);

      // send acknowledge to serial
      Serial.println("A");
      return true;
    }
  }
  return false;
}

uint8_t get_waypoints(const lon_lat_32& start, const lon_lat_32& end) {
  // send request to serial
  send_request(start, end);
  status_message("Sent Request...");

  // return 0 at any point that the communication fails
  if (!get_num()) {return 0;}

  // let user choose a new route if no path was found 
  if (shared.num_waypoints == 0) {
    status_message("No path found!");
    delay(1000);
    return 1;
  }

  // continue reading the waypoints
  Serial.println("A"); // send acknowledge
  status_message ("Receiving path...");
  
  // loop to read waypoints from Serial
  for (int i = 0; i < shared.num_waypoints; i++) {
    if (!process_waypoint(i)) {return 0;}
  }
  // 1 indicates a successful exchange
  return 1;
}
