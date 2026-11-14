#pragma once
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include "LocationGraph.h"
#include <sstream>
#include <fstream>
using namespace std;

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
    bool LocationGraph::toggleEdge(int x, int y) {
        if (x<y) {
            closedEdges.insert({x,y});
            return true;
        }
        if (x>y) {
            closedEdges.insert({y,x});
            return true;
        }
        return false;
    }
    string LocationGraph::checkEdgeStatus(int x, int y) const;
    bool LocationGraph::isConnected(int x, int y) {

    }
};
