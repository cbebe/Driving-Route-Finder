#ifndef _DIJKSTRA_H_
#define _DIJKSTRA_H_

#include "wdigraph.h"
#include "heap.h"
#include <utility>
#include <unordered_map>

// for brevity
// typedef introduces a synonym (alias)
// for the type specified
typedef std::pair<int, ll> PIL;
typedef std::pair<int,int> PII;

void dijkstra(const WDigraph& graph, int startVertex,
              std::unordered_map<int, PIL>& tree);

#endif
