
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include "LocationGraph.h"
#include <sstream>
#include <fstream>
#include <queue>
#include <algorithm>
using namespace std;
    LocationGraph::LocationGraph(){};
    bool LocationGraph::loadEdges(const string& filepath) {
        ifstream file(filepath);
        if (!file.is_open()) return false;

        string line;
        getline(file, line); // skip header row

        while (getline(file, line)) {
            if (line.empty()) continue;

            istringstream ss(line);
            string locationID1, locationID2, time, name;

            if (!getline(ss, locationID1, ','))        continue;
            if (!getline(ss, locationID2, ',')) continue;
            if (!getline(ss, name, ','))       continue;
            if (!getline(ss, name, ','))       continue;
            if (!getline(ss, time, ','))         continue;

            adjList[stoi(locationID1)].push_back({stoi(time),stoi(locationID2)});
            adjList[stoi(locationID2)].push_back({stoi(time),stoi(locationID1)});
        }

        return true;
    };
    void LocationGraph::toggleEdge(int x, int y) {
        if (x>y) {
            swap(x, y);
        }
        if (closedEdges.count({x,y})) {
            closedEdges.erase({x,y});
        }
        else {
            closedEdges.insert({x,y});
        }
    };
    int LocationGraph::checkEdgeStatus(int x, int y) {
        if (x>y) {
            swap(x, y);
        }

        if (adjList.count(x)) {
            for (auto edge : adjList[x]) {
                if (edge.second == y) {
                    if (closedEdges.count({x,y})) {
                        return 0;
                    }
                    return 1;
                }
            }
        }

        return -1;
    };
    bool LocationGraph::isConnected(int x, int y) {
        if (dijkstra(x).first[y]!=-1) return true;
        return false;
    }
    pair<unordered_map<int, int>,unordered_map<int, int>> LocationGraph::dijkstra(int src){
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
        unordered_map<int, int> distances;
        unordered_map<int, int> previous;
        for (auto i : adjList) {
            distances[i.first] = INT_MAX;
        }
        distances[src] = 0;

        pq.push({0,src});
        while (!pq.empty()) {
            // pop smallest distance node
            auto [currDist,currNode] = pq.top();
            pq.pop();
            if (currDist > distances[currNode] ) {
                continue;
            }
            for (auto edge : adjList[currNode]) {
                int currNeigh = edge.second;
                int currNeighWeight = edge.first;
                if (closedEdges.count({min(currNeigh, currNode), max(currNeigh, currNode)})) continue;
                if (distances[currNeigh] > distances[currNode] + currNeighWeight) {
                    distances[currNeigh] = distances[currNode] + currNeighWeight;
                    previous[currNeigh] = currNode;
                    pq.push({distances[currNode] + currNeighWeight,currNeigh});
                }

            }
        }
        for (auto& [node, dist] : distances) {
            if (dist == INT_MAX) {
                dist = -1;
            }
        }
        return {distances,previous};
    }
vector<int> LocationGraph::getPath(const unordered_map<int,int>& previous,int src,int target){
        vector<int> path;
        int curr = target;

        // walk backwards until we reach source
        while (curr != src) {
            // target is unreachable if it's not in previous
            if (!previous.count(curr)) return {};
            path.push_back(curr);
            curr = previous.at(curr);
        }
        path.push_back(src);

        reverse(path.begin(), path.end());
        return path;
    }
int LocationGraph::primMST(const set<int>& nodes){
        if (nodes.empty()) return 0;

        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
        set<int> visited;
        int totalCost = 0;

        // start from first node in set
        int start = *nodes.begin();
        pq.push({0, start});

        while (!pq.empty()) {
            auto [weight, curr] = pq.top();
            pq.pop();

            // skip if already in tree
            if (visited.count(curr)) continue;
            visited.insert(curr);
            totalCost += weight;

            // look at all neighbors within the subgraph node set
            for (const auto& edge : getNeighborsInSet(curr, nodes)) {
                int neighbor   = edge.second;
                int edgeWeight = edge.first;

                if (!visited.count(neighbor))
                    pq.push({edgeWeight, neighbor});
            }
        }

        return totalCost;
    }
vector<pair<int,int>> LocationGraph::getNeighborsInSet(int node, const set<int>& nodes){
        vector<pair<int,int>> result;

        if (!adjList.count(node)) return result;

        for (const auto& edge : adjList.at(node)) {
            int neighbor = edge.second;
            int weight   = edge.first;
            if (nodes.count(neighbor))
                result.push_back({weight, neighbor});
        }

        return result;
    }
