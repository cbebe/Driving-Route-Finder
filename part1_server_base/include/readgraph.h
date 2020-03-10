#ifndef _READGRAPH_H_
#define _READGRAPH_H_

#include <fstream>
#include <string>
#include "wdigraph.h"

struct Point {
  long long lat; // latitude of the point
  long long lon; // longitude of the point
};

long long manhattan(const Point& pt1, const Point& pt2);

void readGraph(string filename, WDigraph& graph, unordered_map<int, Point> & points);

#endif