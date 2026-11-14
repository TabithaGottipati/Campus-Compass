
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

            adjList[stoi(locationID1)].push_back({stoi(locationID2),stoi(time)});
            adjList[stoi(locationID2)].push_back({stoi(locationID1),stoi(time)});
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
                if (edge.first == y) {
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
    unordered_map<int, int> LocationGraph::dijkstra(int src, const set<int>& targets) const {
        // min heap {distance, node}
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;

        // set all distances to infinity
        for (auto edge : adjList[src]) {}
        // set source distance to 0 and push to queue

        while (!pq.empty()) {
            // pop smallest distance node

            // skip if outdated entry

            // for each neighbor of curr
            // skip if edge is closed
            // calculate new distance
            // if new distance is better, update and push to queue
        }

        // build result map — only include target nodes
        // use -1 for unreachable targets

        return result;
    }
