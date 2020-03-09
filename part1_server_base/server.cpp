#include <iostream>

using namespace std;

void request() {
  long long lat1, lon1, lat2, lon2;
  cin >> lat1 >> lon1 >> lat2 >> lon2;

}

int main() {
  char req;
  readgraph()
  while (true) {
    cin >> req;
    if (req == 'R') {
      request();    
    }
  }  
  return 0;
}