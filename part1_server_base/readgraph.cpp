#include <fstream>
#include <string>
#include "wdigraph.h"

struct Point {
  long long lat; // latitude of the point
  long long lon; // longitude of the point
};

// splits string with delimiter and returns a vector of tokens 
vector<string> split(string& str, string delim = ",") {
  vector<string> strvec;
  while (true) {
    int pos = str.find(delim);
    // cuts the string if delimiter is found
    if (pos != (int) string::npos) {
      string token = str.substr(0, pos); // token
      str = str.substr(pos + 1); // remainder
      strvec.push_back(token);
    } else {
      strvec.push_back(str);
      return strvec;    
    }
  }
}

long long manhattan(const Point& pt1, const Point& pt2) {
  // Return the Manhattan distance between the two given points
  return llabs(pt1.lat - pt2.lat) + llabs(pt1.lon - pt2.lon);
}

void readGraph(string filename, WDigraph& graph, unordered_map<int, Point> & points) {
  /*
  Read the Edmonton map data from the provided file
  and load it into the given WDigraph object.
  Store vertex coordinates in Point struct and map
  each vertex to its corresponding Point struct.
  PARAMETERS:
  filename: name of the file describing a road network
  graph: an instance of the weighted directed graph (WDigraph) class
  points: a mapping between vertex identifiers and their coordinates
  */
  ifstream file (filename);
  string lineRead;
  Point coords;
  vector<string> splitLine;
  getline(file, lineRead);
  while (!file.eof()) {

    splitLine = split(lineRead);

    if (splitLine[0] == "V") {
      coords.lat = stod(splitLine[2]);
      coords.lon = stod(splitLine[3]);
      int node = stoi(splitLine[1]);
      graph.addVertex(node);
      points.insert({node, coords});
    } else if (splitLine[0] == "E") {
      int node1 = stoi(splitLine[1]);
      int node2 = stoi(splitLine[2]);
      long long dist = manhattan(points[node1], points[node2]);
      graph.addEdge(node1, node2, dist);
      graph.addEdge(node2, node1, dist);
    }

    getline(file, lineRead);
  }
  
  file.close();
}