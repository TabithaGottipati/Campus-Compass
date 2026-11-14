#pragma once
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
using namespace std;
class LocationGraph {
public:
    LocationGraph();
    bool loadEdges(const std::string& filepath);

    // Graph commands
    void toggleEdge(int x, int y);
    int checkEdgeStatus(int x, int y);  // 1 open, 0 closed, -1 DNE
    bool isConnected(int x, int y);

    // Algorithms
    std::pair<std::unordered_map<int,int>, std::unordered_map<int,int>> dijkstra(int src);
    int primMST(const std::set<int>& nodes) const;

    // Path reconstruction — used by printStudentZone to collect all nodes
    std::vector<int> getPath(const std::unordered_map<int,int>& previous,int src,int target);
    vector<pair<int,int>> getNeighborsInSet(int node, const set<int>& nodes
private:
    std::unordered_map<int, std::vector<std::pair<int, int>>> adjList; // {weight, neighborId}
    std::set<std::pair<int, int>> closedEdges;
};