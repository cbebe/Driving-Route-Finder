#include "SerialPort.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

SerialPort::SerialPort(const char *portName) {
  fd = open(portName, O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1) {
    printf("Error opening device %s\n", portName);
    exit(-1);
  }

  fcntl(fd, F_SETFL, FNDELAY);

  // save old termio settings
  tcgetattr(fd, &oldtio);

  // zero out all fields of the newtio struct
  bzero(&newtio, sizeof(newtio));

  newtio.c_cflag = B9600 | CRTSCTS | CS8 | CLOCAL | CREAD;
  newtio.c_iflag = IGNCR; // carriage returns ('\r') are completely ignored when reading
  newtio.c_oflag = 0;
  newtio.c_lflag = ICANON;

  tcsetattr(fd, TCSADRAIN, &newtio);
}

SerialPort::~SerialPort() {
  // restore old termio settings before closing
  tcsetattr(fd, TCSANOW, &oldtio);

  close(fd);
}

string SerialPort::readline(int timeout) {
  string line = "";
  int avail;
  char c;

  clock_t start = clock();

  // loop until '\n' is read
  do {
    // loop until a character is available, or timeout
    do {
      clock_t cur = clock();
      if (timeout > 0 && (cur - start) > CLOCKS_PER_SEC / 1000) {
        // timeout!
        return "";
      }

      // get number of bytes available
      ioctl(fd, FIONREAD, &avail);
    } while (avail == 0);

    // read a character
    if (read(fd, &c, 1) == 0) {
      continue; // probably doesn't happen
    }
    line += c;
  } while (c != '\n');

  return line;
}

bool SerialPort::writeline(const string& line) {
  return write(fd, line.c_str(), line.length());
}
