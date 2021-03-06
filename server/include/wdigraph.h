#ifndef _WEIGHTED_GRAPH_H_
#define _WEIGHTED_GRAPH_H_

#include <unordered_map>
#include "digraph.h"

typedef long long ll;

/*
  Represents a weighted graph using
  an adjacency list representation.
  Vertex identifiers are integers.
  Weights are of type long long.
*/

class WDigraph : public Digraph
{
public:
  // No constructor or destructor are necessary this time.
  // A new instance will be an empty graph with no nodes.

  // returns the cost/weight of an edge
  // if it does not exist, returns error
  ll getCost(int u, int v) const
  {
    // uses .at because there is no const operator[]
    // for unordered maps
    return cost.at(u).at(v);
  }

  // adds a weighted edge
  // if the edge already existed, does nothing
  void addEdge(int u, int v, ll w)
  {
    // use Digraph's addEdge method
    Digraph::addEdge(u, v);
    cost[u][v] = w;
  }

private:
  std::unordered_map<int, std::unordered_map<int, ll>> cost;
};

#endif
