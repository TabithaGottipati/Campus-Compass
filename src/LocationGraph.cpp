
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include "LocationGraph.h"
#include <sstream>
#include <fstream>
#include <queue>
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
        return true;
    }
    unordered_map<int, int> LocationGraph::dijkstra(int src){
        // min heap {distance, node}
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;

        // set all distances to infinity
        unordered_map<int, int> distances;
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
                    pq.push({distances[currNode] + currNeighWeight,currNeigh});
                }

            }
        }
        return distances;
    }
