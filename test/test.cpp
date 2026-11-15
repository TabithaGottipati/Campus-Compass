#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <sstream>
#include "CampusCompass.h"
using namespace std;

// helper to capture cout output
string runCommands(const string& input) {
    CampusCompass compass;
    compass.ParseCSV("data/edges.csv", "data/classes.csv");

    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());

    istringstream ss(input);
    int n;
    ss >> n;
    ss.ignore();
    string line;
    for (int i = 0; i < n; i++) {
        getline(ss, line);
        compass.ParseCommand(line);
    }

    cout.rdbuf(old);
    return output.str();
}

// ─────────────────────────────────────────
// Test 1: At least 5 invalid commands
// ─────────────────────────────────────────
TEST_CASE("Invalid Commands", "[invalid]") {
    string result = runCommands(R"(8
insert "B4d Name" 12345678 20 1 COP3530
insert "Good Name" 1234 20 1 COP3530
insert "Good Name" 123456789 20 1 COP3530
insert "Good Name" 11111111 20 7 COP3530 CDA3101 MAC2311 EEL3701 COP3502 COP3503 COP3504
insert "Good Name" 22222222 20 1 cop3530
insert "Good Name" 33333333 20 0 COP3530
invalidCommand 123
insert "Good Name" 44444444 20 1 COP353
)");
    REQUIRE(result == "unsuccessful\nunsuccessful\nunsuccessful\nunsuccessful\nunsuccessful\nunsuccessful\nunsuccessful\nunsuccessful\n");
}

// ─────────────────────────────────────────
// Test 2: At least 3 edge cases
// ─────────────────────────────────────────
TEST_CASE("Edge Cases", "[edge]") {
    // removing student that doesn't exist
    string result1 = runCommands(R"(1
remove 99999999
)");
    REQUIRE(result1 == "unsuccessful\n");

    // dropClass on class student doesn't have
    string result2 = runCommands(R"(2
insert "Josh Smith" 12345678 20 1 COP3530
dropClass 12345678 MAC2311
)");
    REQUIRE(result2 == "successful\nunsuccessful\n");

    // duplicate student ID
    string result3 = runCommands(R"(2
insert "Josh Smith" 12345678 20 1 COP3530
insert "Jane Smith" 12345678 20 1 MAC2311
)");
    REQUIRE(result3 == "successful\nunsuccessful\n");

    // dropClass last class — student should be removed
    string result4 = runCommands(R"(3
insert "Josh Smith" 12345678 20 1 COP3530
dropClass 12345678 COP3530
remove 12345678
)");
    REQUIRE(result4 == "successful\nsuccessful\nunsuccessful\n");
}

// ─────────────────────────────────────────
// Test 3: dropClass, removeClass, remove, replaceClass
// ─────────────────────────────────────────
TEST_CASE("Core Commands", "[commands]") {
    // dropClass
    string result1 = runCommands(R"(3
insert "Josh Smith" 12345678 20 2 COP3530 MAC2311
dropClass 12345678 COP3530
dropClass 12345678 COP3530
)");
    REQUIRE(result1 == "successful\nsuccessful\nunsuccessful\n");

    // remove
    string result2 = runCommands(R"(3
insert "Josh Smith" 12345678 20 1 COP3530
remove 12345678
remove 12345678
)");
    REQUIRE(result2 == "successful\nsuccessful\nunsuccessful\n");

    // replaceClass
    string result3 = runCommands(R"(4
insert "Josh Smith" 12345678 20 2 COP3530 MAC2311
replaceClass 12345678 COP3530 CDA3101
replaceClass 12345678 COP3530 EEL3701
replaceClass 12345678 CDA3101 MAC2311
)");
    REQUIRE(result3 == "successful\nsuccessful\nunsuccessful\nunsuccessful\n");

    // removeClass
    string result4 = runCommands(R"(6
insert "Student A" 10000001 1 1 COP3502
insert "Student B" 10000002 1 1 COP3502
insert "Student C" 10000003 1 2 COP3502 MAC2311
dropClass 10000001 COP3502
remove 10000001
removeClass COP3502
)");
    REQUIRE(result4 == "successful\nsuccessful\nsuccessful\nsuccessful\nunsuccessful\n2\n");
}

TEST_CASE("printShortestEdges with toggled edges", "[shortest]") {
    CampusCompass compass;
    compass.ParseCSV("data/edges.csv", "data/classes.csv");

    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());

    // student at node 21, MAC2311 is at node 18
    // shortest path: 21 -> 12 -> 13 -> 15 -> 18, distance 11
    compass.ParseCommand("insert \"Brian\" 35459999 21 1 MAC2311");

    // before toggling — should be reachable, distance 11
    compass.ParseCommand("printShortestEdges 35459999");
    string before = output.str();
    output.str("");
    REQUIRE(before.find("MAC2311: 11") != string::npos);

    // close all 4 edges connected to node 18 (MAC2311)
    // 15-18, 17-18, 18-25, 18-34
    compass.ParseCommand("toggleEdgesClosure 4 15 18 17 18 18 25 18 34");

    // after toggling — should be unreachable
    compass.ParseCommand("printShortestEdges 35459999");
    string after = output.str();
    cout.rdbuf(old);

    REQUIRE(after.find("MAC2311: -1") != string::npos);
}