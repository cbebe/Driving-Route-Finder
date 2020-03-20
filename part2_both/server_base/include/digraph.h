// CHANGE SO NEIGHBOURS RETURNS AN ITERATOR INTO ITS SET

#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

/*
  Represents a graph using an adjacency list representation.
  Vertices are assumed to be integers.
*/
class Digraph {
public:
  // No constructor or destructor are necessary this time.
  // A new instance will be an empty graph with no nodes.

  // add a vertex, does nothing if it already exists
  void addVertex(int v);

  // adds an edge, creating the vertices if they do not exist
  // if the edge already existed, does nothing
  void addEdge(int u, int v);

  // returns true if and only if v is a vertex in the graph
  bool isVertex(int v);

  // returns true if and only if (u,v) is an edge in the graph
  // will certainly return false if neither vertex is in the graph
  bool isEdge(int u, int v);

  // returns a const iterator to the neighbours of v
  unordered_set<int>::const_iterator neighbours(int v) const;

  // returns a const iterator to the end of v's neighour set
  unordered_set<int>::const_iterator endIterator(int v) const;

  // return the number of outgoing neighbours of v
  int numNeighbours(int v);

  // returns the number of nodes
  int size();

  // return a vector with all vertices
  vector<int> vertices();

private:
  unordered_map<int, unordered_set<int>> nbrs;
};

#endif
