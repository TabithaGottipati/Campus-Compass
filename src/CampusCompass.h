#pragma once
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
using namespace std;

class CampusCompass {
private:
    // Think about what member variables you need to initialize
    // perhaps some graph representation?
    unordered_map<int, vector<pair<int, int>>> adjList;
    set<pair<int, int>> closedEdges;
public:
    // Think about what helper functions you will need in the algorithm
    CampusCompass(); // constructor
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    bool ParseCommand(const string &command);
};
