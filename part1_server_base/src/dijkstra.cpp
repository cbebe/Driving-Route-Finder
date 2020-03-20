// ---------------------------------------------------
// Name : Charles Ancheta and Poulomi Ganguly
// CMPUT 275 , Winter 2020
// 
// Assignment 2, Part 1: Driving Route Finder (server)
// ---------------------------------------------------

#include "dijkstra.h"


// Modified Dijkstra's algorithm that uses BinaryHeap
void dijkstra(const WDigraph& graph, int startVertex,
              std::unordered_map<int, PIL>& tree) {
  // All active fires stored as follows:
  // say an entry is ((v, u), d), then there is a fire that started at u
  // and will burn the u->v edge, reaching v at time d
  BinaryHeap<PII, ll> fires;

  // at time 0, the startVertex burns, we use -1 to indicate there is
  // no "predecessor" of the startVertex
  fires.insert(PII(startVertex,-1), 0);

  // while there is an active fire
  while (fires.size() != 0) {
    // get the minimum of heap
    int v = (fires.min().item).first, u = (fires.min().item).second;
    ll d = fires.min().key;

    // remove this fire
    fires.popMin();

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
      fires.insert(PII(nbr, v), burn);
    }
  }  
}