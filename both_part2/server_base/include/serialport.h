/*
  Serial port class specifically set up for communication with Arduino Mega2560
  in the Ubuntu VM used in CMPUT 275

  Created by Zachary Friggstad, 2019
  Ideas adapted from https://www.cmrr.umn.edu/~strupp/serial.html#config
*/


#ifndef _SERIALPORT_H_
#define _SERIALPORT_H_

#include <string>
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>

using namespace std;

class SerialPort {
public:
  // on my Mac, it shows up as /dev/tty.usbmodem1411
  SerialPort(const char *portName = "/dev/ttyACM0");
  ~SerialPort();

  // Read bytes from the serial port until '\n' which is also included in the returned string.
  // NOTE, the carriage return '\r' is ignored completely
  // Returns "" if it times out, i.e. '\n' is not read within the "timeout" milliseconds
  // If timeout <= 0, then no timeout will be used: it will wait indefinitely until '\n' is read.
  string readline(int timeout = 0);

  // Writes the string with no additional '\n'.
  // Returns success if all characters were written.
  bool writeline(const string& line);

private:
  int fd;
  termios oldtio, newtio;
};

#endif
