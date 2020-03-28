#include "digraph.h"

using namespace std;

void Digraph::addVertex(int v) {
  // If it already exists, does nothing.
  // Otherwise, adds v with an empty adjacency set.
  nbrs[v];
}

void Digraph::addEdge(int u, int v) {
  addVertex(v);
  nbrs[u].insert(v); // will also add u to nbrs if it was not there already
}


bool Digraph::isVertex(int v) {
  return nbrs.find(v) != nbrs.end();
}

bool Digraph::isEdge(int u, int v) {
  // check that u is in the keys and that it's associated set contains v
  return nbrs.find(u) != nbrs.end() && nbrs[u].find(v) != nbrs[u].end();
}

unordered_set<int>::const_iterator Digraph::neighbours(int v) const {
  // this will crash the program if v is not a vertex
  return nbrs.find(v)->second.begin();
}

unordered_set<int>::const_iterator Digraph::endIterator(int v) const {
  // this will crash the program if v is not a vertex
  return nbrs.find(v)->second.end();
}

int Digraph::numNeighbours(int v) {
  // this will crash the program if v is not a vertex
  return nbrs.find(v)->second.size();
}

int Digraph::size() {
  return nbrs.size();
}

vector<int> Digraph::vertices() {
  vector<int> v;
  for (unordered_map<int, unordered_set<int>>::const_iterator it = nbrs.begin();
    it != nbrs.end(); it++) {
      v.push_back(it->first);
    }
  return v;
}
