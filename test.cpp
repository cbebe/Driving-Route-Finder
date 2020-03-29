#include <iostream>
#include <string>
using namespace std;

void printTest() {
  int x = 12, y = 24;
  char c[50];
  sprintf(c, "W %d %d\n", x, y);
  cout << c;
}

// discovered that anything that is not
// int is dropped
void atoiTest() {
  cout << atoi("N 12\n") << endl;
}

void numTest() {
  char *buffer = "N -500\n";
  if (buffer[0] != 'N') {return;}
  char num[5]; num[4] = 0;
  for (int i = 0; i < 4; i++) {
    num[i] = buffer[i + 2];
  }
  cout << atoi(num) << endl;
  char *num1 = "500-";
  cout << atoi(num1) << endl;
}

void processWaypoint() {
  char lon[10], lat[10];
  char buff[] = "W 5365488 -11333914";
  // assumes that all numbers would be the same format:
  // 7 digits longitude 8 digits negative latitude
  for (int i = 0; i < 10; i++) {
    lon[i] = buff[i + 2];
    lat[i] = buff[i + 10];
  }
  lon[9] = 0; lat[9] = 0;
  cout << atoi(lon) << endl;
  cout << atoi(lat) << endl;
}

int main() {
  processWaypoint();
  string req = "R 1 2";
  if (req[0] == 'R') {
    cout << "it works?\n";
  }
  return 0;
}