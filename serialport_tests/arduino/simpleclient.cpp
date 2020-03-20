#include <Arduino.h>

/*
  Test read and write to arduino through minicom.

  The program, running on the Arduino, reads data on the serial 0 port and
  echos it back on the same port. The program also demonstrates how to
  detect new lines.

  You can test this program with serial-mon or serialtest.cpp

  To quit from serial-mon, use CTRL-a x
*/

// max size of buffer, including null terminator
const uint16_t buf_size = 256;
// current number of chars in buffer, not counting null terminator
uint16_t buf_len = 0;

// input buffer
char* buffer = (char *) malloc(buf_size);

void setup() {
  // 115200 can be used with Python, but not with serial-mon
  Serial.begin(9600);

  Serial.println("PHASE01");
  Serial.println("Client side is waiting for input.");
  Serial.println("PHASE02");

  // set up buffer as empty string
  buf_len = 0;
  buffer[buf_len] = 0;
}

void process_line() {
  // print what's in the buffer back to server
  Serial.print("Got: ");
  Serial.println(buffer);

  // clear the buffer
  buf_len = 0;
  buffer[buf_len] = 0;
}

void loop() {
  char in_char;

  if (Serial.available()) {
      // read the incoming byte:
      char in_char = Serial.read();

      // if end of line is received, waiting for line is done:
      if (in_char == '\n' || in_char == '\r') {
          // now we process the buffer
          process_line();
          }
      else {
          // add character to buffer, provided that we don't overflow.
          // drop any excess characters.
          if ( buf_len < buf_size-1 ) {
              buffer[buf_len] = in_char;
              buf_len++;
              buffer[buf_len] = 0;
          }
        }
    }
  }
