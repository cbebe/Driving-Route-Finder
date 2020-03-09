#ifndef READGRAPH_H
#define READPGRAPH_H

#include <fstream>
#include <string>
#include "wdigraph.h"

struct Point {
  long long lat; // latitude of the point
  long long lon; // longitude of the point
};

void readGraph(string filename, WDigraph& graph, unordered_map<int, Point> & points);

#endif