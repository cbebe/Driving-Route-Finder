// ---------------------------------------------------
// Name : Charles Ancheta and Poulomi Ganguly
// CMPUT 275 , Winter 2020
// 
// Assignment 2, Part 1: Driving Route Finder (server)
// ---------------------------------------------------

#include <list>
#include "dijkstra.h"
#include "readgraph.h"
#include "serialport.h"
#include <iostream>
#include <cassert>

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

// processes request from Serial and returns the 
// starting and ending vertices in the map graph
PII request(ptMap& points, SerialPort& Serial) {
  // waits for R from Serial
  string req = Serial.readline(1000);
  while (req[0] != 'R') {
    req = Serial.readline(1000);
  }
  req.pop_back();
  Point start, end;
  // splits request message and parses string to numbers
  vector<string> splitLine = splitStr(req, " ");
  start.lat = stoll(splitLine[1]); 
  start.lon = stoll(splitLine[2]);
  end.lat = stoll(splitLine[3]); 
  end.lon = stoll(splitLine[4]);
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
  return path;
}

// waits for acknowledge from Serial
// returns false if no A is received
bool waitForAck(SerialPort& Serial) {
  return Serial.readline(1000)[0] == 'A'; 
}


// sends the coordinates of each waypoint of the route
void sendWaypoints(const list<int>& path, ptMap& points, SerialPort& Serial) {
  // sends the number of waypoints
  string waypoints = "N "+ to_string(path.size()) + "\n";
  assert(Serial.writeline(waypoints));
  // always wait for acknowledge before continuing
  if (waitForAck(Serial)) {
    // sends the coordinates of each waypoint
    for (auto it: path) {
      // convert coordinates to string and send
      string lat = to_string(points[it].lat);
      string lon = to_string(points[it].lon);
      assert(Serial.writeline("W " + lat + " " + lon + "\n"));
      
      // wait to not overflow buffer
      // return to waiting for request
      // if acknowledgement is not received
      if (!waitForAck(Serial)) {return;}   
    }
    // terminate communication with E
    Serial.writeline("E\n");
  }
}

int main() {
  // create map graph and point map
  string filename = "edmonton-roads-2.0.1.txt";
  WDigraph graph;
  ptMap points; // stores vertices and its coordinates
  readGraph(filename, graph, points);
  SerialPort Serial; // assumes port /dev/ttyACM0
  while (true) {
    // ver = pair(start vertex, end vertex)
    PII ver = request(points, Serial);
    unordered_map<int, PIL> searchTree;
    // finds shortest path
    dijkstra(graph, ver.first, searchTree);
    
    list<int> path = createPath(ver, searchTree);

    // send over Serial
    sendWaypoints(path, points, Serial);
  }

  return 0;
}