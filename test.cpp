#include <iostream>
#include <string>
#include <vector>
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

vector<string> splitStr(string& str, string delim) {
  vector<string> strvec;
  while (true) {
    int pos = str.find(delim);
    // cuts the string if delimiter is found
    if (pos != (int) string::npos) {

      string token = str.substr(0, pos); // token
      str = str.substr(pos + 1); // remainder
      strvec.push_back(token);

    } else {
      // return once there are no delimiters
      strvec.push_back(str);
      return strvec;
    }
  }
}

// processes request from Serial and returns the 
// starting and ending vertices in the map graph
void request() {
  // waits for R from Serial
  string req = "R 123123 123123 123123 123123";

  struct Point {long long lat; long long lon;};
  Point start, end;
  // splits request message and parses string to numbers
  vector<string> splitLine = splitStr(req, " ");
  start.lat = stoll(splitLine[1]); 
  start.lon = stoll(splitLine[2]);
  end.lat = stoll(splitLine[3]); 
  end.lon = stoll(splitLine[4]);
  cout << start.lat << ' ' << start.lon << ' ' 
       << end.lat << ' ' << end.lon << endl;
}

int main() {
  request();  
  return 0;
}