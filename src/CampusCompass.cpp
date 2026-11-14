#include "CampusCompass.h"
#include <iostream>
#include <sstream>
using namespace std;

CampusCompass::CampusCompass() {}


bool CampusCompass::ParseCSV(const string& edges_filepath, const string& classes_filepath) {
    if (db.loadClasses(classes_filepath)&&graph.loadEdges(edges_filepath)) {
        return true;
    }
    return false;
}

// ─────────────────────────────────────────
// Command Parsing
// ─────────────────────────────────────────

bool CampusCompass::ParseCommand(const string& command) {
    istringstream ss(command);
    string cmd;
    ss >> cmd;

    if (cmd == "insert") {
        // parse quoted name
        // parse id, residence, n
        // parse n class codes
        // validate residence node exists in graph
        // call db.insert()
        // print successful/unsuccessful

    } else if (cmd == "remove") {
        // parse id
        // call db.remove()
        // print successful/unsuccessful

    } else if (cmd == "dropClass") {
        // parse id, classCode
        // call db.dropClass()
        // print successful/unsuccessful

    } else if (cmd == "replaceClass") {
        // parse id, oldCode, newCode
        // call db.replaceClass()
        // print successful/unsuccessful

    } else if (cmd == "removeClass") {
        // parse classCode
        // call db.removeClass()
        // if result == -1: print unsuccessful
        // else print count

    } else if (cmd == "toggleEdgesClosure") {
        // parse n
        // parse n pairs of location ids
        // call graph.toggleEdge() for each pair
        // print successful

    } else if (cmd == "checkEdgeStatus") {
        // parse x, y
        // call graph.checkEdgeStatus()
        // print "open", "closed", or "DNE"

    } else if (cmd == "isConnected") {
        // parse x, y
        // call graph.isConnected()
        // print successful/unsuccessful

    } else if (cmd == "printShortestEdges") {
        // parse id
        // call handlePrintShortest()

    } else if (cmd == "printStudentZone") {
        // parse id
        // call handlePrintZone()

    } else if (cmd == "verifySchedule") {
        // parse id
        // call handleVerifySchedule()

    } else {
        // invalid command
        cout << "unsuccessful\n";
        return false;
    }

    return true;
}

// ─────────────────────────────────────────
// Private Handlers
// ─────────────────────────────────────────

void CampusCompass::PrintShortest(string Id) {
    cout<<"Time For Shortest Edges: "<<db.getStudent(Id)->name<<endl;
    // get student from db
    auto timeToClasses=graph.dijkstra(db.getStudent(Id)->residenceLocationId).first;
    // run dijkstra from student residence
    for (auto classCode : db.getStudent(Id)->classCodes) {

            cout<<classCode<<": "<<timeToClasses[db.getClassLocation(classCode)]<<endl;
    }
}

void CampusCompass::PrintZone(string Id) {
    // get student from db
    auto timeToClasses=graph.dijkstra(db.getStudent(Id)->residenceLocationId).first;

    // run dijkstra from student residence — get distances AND previous map
    // for each reachable class:
    //     walk backwards through previous map to reconstruct path
    //     collect all nodes from all paths into one set
    // build subgraph:
    //     for each node in set:
    //         for each neighbor in adjList:
    //             if neighbor also in set → include edge
    // run primMST on subgraph node set
    // print total cost
}

void CampusCompass::VerifySchedule(string Id) {
    // get student from db
    // get all classes with their start/end times
    // sort classes by start time
    // for each consecutive pair (class1, class2):
    //     calculate time gap in minutes
    //     run dijkstra from class1.locationId
    //     get shortest path to class2.locationId
    //     if gap >= shortest path: print successful
    //     else: print unsuccessful
}