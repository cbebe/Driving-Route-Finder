#include "server.h"

using namespace std;

// finds the closest vertex to a given point
int findClosest(Point point, ptMap& points) { 
  // use BinaryHeap because we only care about the minimum distance
  // use long long as key because that's how we'll sort the heap
  BinaryHeap<int,ll> dist;
  for (auto it = points.begin(); it != points.end(); it++) {
    // calculate distance between points and insert in heap
    ll distance = manhattan(point, it->second);
    dist.insert(it->first, distance);
  }
  
  return dist.min().item; // item is the vertex number
}

// processes request from stdin and returns the 
// starting and ending vertices in the map graph
PII request(ptMap& points) {
  Point start, end; 
  cin >> start.lat >> start.lon >> end.lat >> end.lon;
  
  PII vertices;
  vertices.first = findClosest(start, points);
  vertices.second = findClosest(end, points);
  return vertices;
}

int main() {
  // create map graph and point map
  string filename = "edmonton-roads-2.0.1.txt";
  WDigraph graph;
  ptMap points; // stores vertices and its coordinates
  readGraph(filename, graph, points);

  char req;
  cin >> req;
  while (req != 'R') {
    cin >> req;
  }
  PII ver = request(points);
  // this is where i left off
  // prints the starting and ending vertices
  // so all you need to do is run dijkstra on ver.first
  // and find ver.second in the search tree 
  cout << ver.first << ' ' << ver.second << endl;
  return 0;
}