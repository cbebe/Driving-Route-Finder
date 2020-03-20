#ifndef _READGRAPH_H_
#define _READGRAPH_H_

#include <fstream>
#include <string>
#include "wdigraph.h"

struct Point {
  ll lat; // latitude of the point
  ll lon; // longitude of the point
};

typedef std::unordered_map<int, Point> ptMap;

ll manhattan(const Point& pt1, const Point& pt2);

void readGraph(string filename, WDigraph& graph, ptMap & points);

#endif