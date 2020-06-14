// ---------------------------------------------------
// Name : Charles Ancheta and Poulomi Ganguly
// CMPUT 275 , Winter 2020
//
// Assignment 2, Part 2: Driving Route Finder (server)
// ---------------------------------------------------

#include <list>
#include "dijkstra.h"
#include "readgraph.h"
#include "serialport.h"
#include <iostream>
#include <cassert>

using namespace std;

// finds the closest vertex to a given point
int findClosest(Point point, ptMap &points)
{
  // use BinaryHeap because we only care about the minimum distance
  // use long long as key because that's how we'll sort the heap
  BinaryHeap<int, ll> dist;

  for (auto it = points.begin(); it != points.end(); it++)
  {
    // calculate distance between points and insert in heap
    ll distance = manhattan(point, it->second);
    dist.insert(it->first, distance);
  }

  return dist.min().item; // item is the vertex number
}

// processes request from Serial and returns the
// starting and ending vertices in the map graph
PII request(ptMap &points, string req)
{
  Point start, end;
  // splits request message and parses string to numbers
  vector<string> splitLine = splitStr(req, " ");
  start.lat = stoll(splitLine[1]);
  start.lon = stoll(splitLine[2]);
  end.lat = stoll(splitLine[3]);
  end.lon = stoll(splitLine[4]);

  // finds the starting and ending vertices
  // closest to the given coordinates
  PII vertices;
  vertices.first = findClosest(start, points);
  vertices.second = findClosest(end, points);
  return vertices;
}

// creates a list of the path's vertices
list<int> createPath(PII endPts, unordered_map<int, PIL> &tree)
{
  list<int> path;
  int step = endPts.second; // place the end vertex at the end
  while (step != endPts.first)
  {
    path.push_front(step);
    // traces the predecessor of the vertex in the path
    step = tree[step].first;
  }
  path.push_front(endPts.first); // place the start vertex in front
  return path;
}

// waits for acknowledge from Serial
// returns false if no A is received
bool waitForAck(SerialPort &Serial)
{
  // timeout is 1 second
  string ack = Serial.readline(1000);
  return ack[0] == 'A';
}

void sendToSerial(const string &message, SerialPort &Serial)
{
  cout << "Sending: " << message << endl;
  // always include newline char on messages
  assert(Serial.writeline(message + "\n"));
}

// sends the coordinates of each waypoint of the route
void sendWaypoints(const list<int> &path, ptMap &points, SerialPort &Serial)
{

  // send N 0 and restart if there is no path
  if (!path.empty())
  {

    // sends the number of waypoints
    string numWaypoints = "N " + to_string(path.size());
    sendToSerial(numWaypoints, Serial);

    // always wait for acknowledge before continuing
    if (waitForAck(Serial))
    {
      // sends the coordinates of each waypoint
      for (auto it : path)
      {
        // convert coordinates to string and send
        string lat = to_string(points[it].lat);
        string lon = to_string(points[it].lon);
        string wp = "W " + lat + " " + lon;
        sendToSerial(wp, Serial); // send waypoint to serial

        // wait to not overflow buffer
        // return to waiting for request
        // if acknowledgement is not received
        if (!waitForAck(Serial))
          return;
      }
      // terminate communication with E
      sendToSerial("E", Serial);
    }
  }
  else
  {
    cout << "No path found!" << endl;
    sendToSerial("N 0", Serial);
  }
}

int main()
{
  // create map graph and point map
  string filename = "edmonton-roads-2.0.1.txt";
  WDigraph graph;
  ptMap points; // stores vertices and its coordinates
  readGraph(filename, graph, points);
  SerialPort Serial; // assumes port /dev/ttyACM0
  cout << "Starting Route Server..." << endl;
  while (true)
  {
    // will only continue once request is received
    cout << "Waiting for request..." << endl;
    string req = Serial.readline(-1);
    if (req[0] == 'R')
    {
      // ver = pair(start vertex, end vertex)
      PII ver = request(points, req);
      unordered_map<int, PIL> searchTree;
      list<int> path;
      // finds shortest path
      dijkstra(graph, ver.first, searchTree);

      path = createPath(ver, searchTree);
      // send over Serial
      sendWaypoints(path, points, Serial);
    }
  }

  return 0;
}