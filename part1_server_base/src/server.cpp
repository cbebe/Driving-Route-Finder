// ---------------------------------------------------
// Name : Charles Ancheta and Poulomi Ganguly
// CMPUT 275 , Winter 2020
// 
// Assignment 2, Part 1: Driving Route Finder (server)
// ---------------------------------------------------

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
  // finds the starting and ending vertices
  // closest to the given coordinates
  vertices.first = findClosest(start, points);
  vertices.second = findClosest(end, points);
  return vertices;
}

// creates a list of the path's vertices
list<int> createPath(PII endPts, unordered_map<int,PIL> &tree) {
  list<int> path;
  int step = endPts.second; // place the end vertex at the end
  while (step != endPts.first) {
    path.push_front(step);
    // traces the predecessor of the vertex in the path
    step = tree[step].first;
  }
  path.push_front(endPts.first); // place the start vertex in front
  cout << "Cost: " << tree[endPts.second].second << endl;
  return path;
}

// waits for acknowledge from stdin
void waitForAcknowledge() {
  char ack;
  cin >> ack;
  // will not continue until A is received
  while (ack != 'A') {
    cin >> ack;
  }
}


// sends the coordinates of each waypoint of the route
void sendWaypoints(list<int>& path, ptMap& points) {
  // sends the number of waypoints
  cout << "N " << path.size() << endl;
  waitForAcknowledge();

  // sends the coordinates of each waypoint
  for (auto it: path) {
    cout << "W " << points[it].lat << ' ' 
         << points[it].lon << endl;
    waitForAcknowledge();
  }
  // terminate communication with E
  cout << 'E' << endl;
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
  // ver = pair(start vertex, end vertex)
  PII ver = request(points);
  unordered_map<int, PIL> searchTree;
  dijkstra(graph, ver.first, searchTree);
  
  list<int> path = createPath(ver, searchTree);

  sendWaypoints(path, points);

  return 0;
}