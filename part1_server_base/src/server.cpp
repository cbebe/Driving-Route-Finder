#include "server.h"

using namespace std;

int findClosest(ll lat, ll lon) {

}

void request() {
  ll lat1, lon1, lat2, lon2;
  cin >> lat1 >> lon1 >> lat2 >> lon2;
  int startVertex = findClosest(lat1, lon1);
  int endVertex = findClosest(lat2, lon2);
}

int main() {
  string filename = "edmonton-roads-2.0.1.txt";
  char req;
  WDigraph graph;
  unordered_map<int, Point> points;
  readGraph(filename, graph, points);
  cin >> req;
  while (req != 'R') {
    cin >> req;
  }
  request();
    
  return 0;
}