#pragma once
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;
class LocationGraph {
public:
    LocationGraph();
    bool loadEdges(const std::string& filepath);

    void toggleEdge(int x, int y);
    int checkEdgeStatus(int x, int y);  // 1 open, 0 closed, -1 DNE
    bool isConnected(int x, int y);

    pair<unordered_map<int, int>,unordered_map<int, int>> dijkstra(int src);
    int primMST(const std::set<int>& nodes) const;

private:
    std::unordered_map<int, std::vector<std::pair<int, int>>> adjList;
    std::set<std::pair<int, int>> closedEdges;

};