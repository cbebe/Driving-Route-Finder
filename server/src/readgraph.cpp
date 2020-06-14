// ---------------------------------------------------
// Name : Charles Ancheta and Poulomi Ganguly
// CMPUT 275 , Winter 2020
//
// Assignment 2, Part 2: Driving Route Finder (server)
// ---------------------------------------------------

#include "readgraph.h"

// splits string with delimiter and returns a vector of tokens
vector<string> splitStr(string &str, string delim)
{
  vector<string> strvec;
  while (true)
  {
    int pos = str.find(delim);
    // cuts the string if delimiter is found
    if (pos != (int)string::npos)
    {

      string token = str.substr(0, pos); // token
      str = str.substr(pos + 1);         // remainder
      strvec.push_back(token);
    }
    else
    {
      // return once there are no delimiters
      strvec.push_back(str);
      return strvec;
    }
  }
}

// Return the Manhattan distance between the two given points
ll manhattan(const Point &pt1, const Point &pt2)
{
  return llabs(pt1.lat - pt2.lat) + llabs(pt1.lon - pt2.lon);
}

void readGraph(string filename, WDigraph &graph,
               ptMap &points)
{
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
  ifstream file(filename);
  string lineRead;
  vector<string> splitLine;
  getline(file, lineRead);
  while (!file.eof())
  {

    splitLine = splitStr(lineRead, ",");
    // adds a vertex to the graph
    if (splitLine[0] == "V")
    {

      int node = stoi(splitLine[1]);
      // convert floating point coordinates to long integers
      // inserts to vertex Point struct map
      points[node].lat = static_cast<ll>(stod(splitLine[2]) * 100000);
      points[node].lon = static_cast<ll>(stod(splitLine[3]) * 100000);
      graph.addVertex(node);
    }
    else if (splitLine[0] == "E")
    {

      //calculate the cost between the two vertices
      int node1 = stoi(splitLine[1]);
      int node2 = stoi(splitLine[2]);
      ll dist = manhattan(points[node1], points[node2]);

      // it's a directed graph so only add one edge
      graph.addEdge(node1, node2, dist);
    }

    getline(file, lineRead);
  }

  file.close();
}