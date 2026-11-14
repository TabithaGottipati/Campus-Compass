#pragma once
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include "LocationGraph.h"
#include "StudentDatabase.h"
using namespace std;

class CampusCompass {
private:
    // Think about what member variables you need to initialize
    // perhaps some graph representation?
    LocationGraph graph;
    StudentDatabase db;
    void PrintShortest(string Id);
    void PrintZone(string Id);
    void VerifySchedule(string Id);
public:
    // Think about what helper functions you will need in the algorithm
    CampusCompass(); // constructor
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    bool ParseCommand(const string &command);

};
