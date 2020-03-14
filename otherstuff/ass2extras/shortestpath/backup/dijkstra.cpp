#include <iostream>
#include <list>
#include <utility> // for pair()
// pair is a simple container of two heterogeneous objects
// which can be accessed by dot operator followed by 
// first or second keyword

#include <unordered_map>
#include "wdigraph.h"

using namespace std;

// for brevity
// typedef introduces a synonym (alias) 
// for the type specified
typedef pair<int, long long> PIL;

// again, for brevity
// used to store a vertex v and its predecessor pair (u,d) on the search
// where u is the node prior to v on a path to v of cost d
// eg. PIPIL x;
// x.first is "v", x.second.first is "u" and x.second.second is "d" from this
typedef pair<int, PIL> PIPIL;

void dijkstra(const WDigraph& graph, int startVertex, 
    unordered_map<int, PIL>& searchTree) {

    // All active fires stored as follows:
    // say an entry is (v, (u, d)), then there is a fire that started at u
    // and will burn the u->v edge, reaching v at time d
    list<PIPIL> fires;

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
        if (searchTree.find(v) != searchTree.end()) {
            continue;
        }

        // declare that v is "burned" at time d with a fire that spawned from u
        searchTree[v] = PIL(u, d);

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

int main() {
    WDigraph graph;

    int startVertex, endVertex;

    // reading in the graph
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < n; ++i) {
        int label;
        cin >> label;
        graph.addVertex(label);
    }
    for (int i = 0; i < m; ++i) {
        int u, v;
        long long c;
        cin >> u >> v >> c;
        graph.addEdge(u, v, c);
    }
    cin >> startVertex >> endVertex;


    // run the search
    // searchTree[v] will be the pair (u,d) of the node u prior to v
    // in a shortest path to v and d will be the shortest path distance to v
    unordered_map<int, PIL> searchTree;
    dijkstra(graph, startVertex, searchTree);

    // read off a path
    list<int> path;
    if (searchTree.find(endVertex) == searchTree.end()) {
      cout << "Vertex " << endVertex << " not reachable from " << startVertex << endl;
    }
    else {
      int stepping = endVertex;
      while (stepping != startVertex) {
        path.push_front(stepping);

        // crawl up the search tree one step
        stepping = searchTree[stepping].first;
      }
      path.push_front(startVertex);

      cout << "Cost of cheapest path from " << startVertex << " to "
           << endVertex << " is: " << searchTree[endVertex].second << endl;
      cout << "Path:";
      for (auto it : path) {
        cout << ' ' << it;
      }
      cout << endl;
    }

    return 0;
}