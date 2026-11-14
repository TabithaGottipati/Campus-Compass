#pragma once
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;
class LocationGraph {
private:
    // Think about what member variables you need to initialize
    // perhaps some graph representation?
    unordered_map<int, vector<pair<int, int>>> adjList;
    set<pair<int, int>> closedEdges;
public:
    // Think about what helper functions you will need in the algorithm
    LocationGraph(); // constructor
    // Right — graph only loads its own data
    bool loadEdges(const string& filepath);
    bool toggleEdge(int x, int y);
    string checkEdgeStatus(int x, int y) const;
    bool isConnected(int x, int y) const;
};
