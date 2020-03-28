/*
  Simple demonstration of the SerialPort class.
  Assumes the Arduino is found at /dev/ttyACM0 and that it is
    running simpleclient.cpp

  Requires c++11
*/

/*
  Suggested exercise.
  Have the client and server use timeouts to restart the communication
  if the other end is not responding.

  Randomly have either not respond/restart their protocol from the beginning
  after a long delay.
*/

#include <iostream>
#include <cassert>
#include <string>
#include "serialport.h"

using namespace std;

int main() {
  SerialPort Serial("/dev/ttyACM0");

  string curPhase = "PHASE00\n";
  string nextPhase = "PHASE01\n";
  string line;

  cout << "Server started, you might need to restart your Arduino" << endl;
  cout << "Server is in phase " << curPhase;
  cout << "Waiting for PHASE01 message from Arduino..." << endl;

  // read and ignore lines until we get the message PHASE01
  do {
    line = Serial.readline();
  } while (line != nextPhase);

  // switch to next phase
  curPhase = nextPhase;
  nextPhase = "PHASE02\n";
  cout << "Server is in phase " << curPhase;

  // read the introductory lines until get the message PHASE01
  do {
    line = Serial.readline();
    cout << "Received: " << line; // note '\n' is in the string already
  } while (line != nextPhase);
  cout << "Sending message <Ack\\n>" << endl;
  assert(Serial.writeline("Ack\n"));

  // switch to next phase
  curPhase = nextPhase;

  cout << "Server is in phase " << curPhase;
  cout << "Waiting for client to reply to previous message..." << endl;

  for (int idx = 0; idx <= 10; idx++) {
    line = Serial.readline();
    cout << "Received: " << line;

    if (idx > 10) {
      cout << "Exchange Finished" << endl;
      break;
    }

    string message = to_string(idx); // requires c++11
    cout << "Sending message: <" << message << "\\n>" << endl;
    assert(Serial.writeline(message+"\n"));
  };

  cout << "Finished communications!" << endl;

	return 0;
}
