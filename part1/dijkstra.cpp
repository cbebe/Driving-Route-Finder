#include <cassert>
#include "dijkstra.h"
#include "heap.h"

void dijkstra(const WDigraph& graph, int startVertex,
              unordered_map<int, PIL>& tree) {
  BinaryHeap<pair<int, int>, long long> heap;

  // the PLI key in the heap has the distance first and the vertex second, so
  // the min item in the heap will have minimum distance
  heap.insert(pair<int, int>(startVertex, -1), 0);

  while (heap.size() > 0) {
    pair<int, int> top = heap.min().item;
    long long dist = heap.min().key;
    int v = top.first, prev = top.second;

    heap.popMin();

    if (tree.find(v) != tree.end()) {
      continue;
    }

    // Record tree[v] = (d,u) where u is the predecessor of v on the shortest
    // path to v and d is the length of this path
    tree[v] = PIL(prev, dist);

    for (auto iter = graph.neighbours(v); iter != graph.endIterator(v); iter++) {
      int cost = dist + graph.getCost(v, *iter);
      assert(cost >= 0);
      heap.insert(pair<int, int>(*iter, v), cost);
    }
  }
}
