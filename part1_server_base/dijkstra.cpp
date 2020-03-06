#include "dijkstra.h"
#include "heap.h"

void dijkstra(const WDigraph& graph, int startVertex,
              std::unordered_map<int, PIL>& tree) {
  // All active fires stored as follows:
  // say an entry is (v, (u, d)), then there is a fire that started at u
  // and will burn the u->v edge, reaching v at time d
  BinaryHeap<PIPIL> fires;

  // at time 0, the startVertex burns, we use -1 to indicate there is
  // no "predecessor" of the startVertex
  fires.push_back(PIPIL(startVertex, PIL(-1, 0)));

  // while there is an active fire
  while (!fires.empty()) {
    // find the fire that reaches its endpoint "v" earliest,
    // represented as an iterator into the list
    auto earliestFire = fires.begin();
    for (auto iter = fires.begin(); iter != fires.end(); ++iter) {
      if (iter->second.second < earliestFire->second.second) {
        earliestFire = iter;
      }
    }

    // to reduce notation in the code below, this u,v,d agrees with
    // the intuition presented in the comment when PIPIL is typedef'ed
    int v = earliestFire->first, u = earliestFire->second.first, d = earliestFire->second.second;

    // remove this fire
    fires.erase(earliestFire);

    // if u is already "burned", there nothing to do
    if (tree.find(v) != tree.end()) {
      continue;
    }

    // declare that v is "burned" at time d with a fire that spawned from u
    tree[v] = PIL(u, d);

    // now start fires from all edges exiting vertex v
    for (auto iter = graph.neighbours(v); iter != graph.endIterator(v); iter++) {
      int nbr = *iter;

      // the fire starts at v at time d and will reach nbr
      // at time d + (length of v->nbr edge)
      int burn = d + graph.getCost(v, nbr);
      fires.push_back(PIPIL(nbr, PIL(v, burn)));
    }
  }  
  
}