#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <sstream>
#include "CampusCompass.h"
using namespace std;

// ─────────────────────────────────────────
// Helper
// ─────────────────────────────────────────

static string runCommands(const string& input) {
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
// Test 1: At least 5 incorrect commands
// ─────────────────────────────────────────

TEST_CASE("Invalid Commands", "[invalid]") {
    // name contains digit
    string r1 = runCommands(R"(1
insert "A11y" 45679999 1 1 COP3530
)");
    REQUIRE(r1 == "unsuccessful\n");

    // UFID too short
    string r2 = runCommands(R"(1
insert "Alex" 1234 1 1 COP3530
)");
    REQUIRE(r2 == "unsuccessful\n");

    // UFID too long
    string r3 = runCommands(R"(1
insert "Alex" 123456789 1 1 COP3530
)");
    REQUIRE(r3 == "unsuccessful\n");

    // too many classes (more than 6)
    string r4 = runCommands(R"(1
insert "Alex" 11111111 1 7 COP3530 CDA3101 MAC2311 EEL3701 COP3502 COP3503 COP3504
)");
    REQUIRE(r4 == "unsuccessful\n");

    // class code lowercase — invalid format
    string r5 = runCommands(R"(1
insert "Alex" 22222222 1 1 cop3530
)");
    REQUIRE(r5 == "unsuccessful\n");

    // 0 classes
    string r6 = runCommands(R"(1
insert "Alex" 33333333 1 0 COP3530
)");
    REQUIRE(r6 == "unsuccessful\n");

    // misspelled command
    string r7 = runCommands(R"(1
insrt "Alex" 44444444 1 1 COP3530
)");
    REQUIRE(r7 == "unsuccessful\n");

    // duplicate UFID
    string r8 = runCommands(R"(2
insert "Alex" 45679999 1 1 COP3530
insert "Bob" 45679999 1 1 MAC2311
)");
    REQUIRE(r8 == "successful\nunsuccessful\n");
}

// ─────────────────────────────────────────
// Test 2: At least 3 edge cases
// ─────────────────────────────────────────

TEST_CASE("Edge Cases", "[edge]") {
    // remove student that doesn't exist
    string r1 = runCommands(R"(1
remove 99999999
)");
    REQUIRE(r1 == "unsuccessful\n");

    // dropClass on class student doesn't have
    string r2 = runCommands(R"(2
insert "Josh Smith" 12345678 1 1 COP3530
dropClass 12345678 MAC2311
)");
    REQUIRE(r2 == "successful\nunsuccessful\n");

    // dropClass last class — student should be automatically removed
    // subsequent remove should fail since student no longer exists
    string r3 = runCommands(R"(3
insert "Josh Smith" 12345678 1 1 COP3530
dropClass 12345678 COP3530
remove 12345678
)");
    REQUIRE(r3 == "successful\nsuccessful\nunsuccessful\n");

    // removeClass on class with no students enrolled
    string r4 = runCommands(R"(1
removeClass COP3530
)");
    REQUIRE(r4 == "unsuccessful\n");

    // replaceClass with class student already has
    string r5 = runCommands(R"(2
insert "Josh Smith" 12345678 1 2 COP3530 MAC2311
replaceClass 12345678 COP3530 MAC2311
)");
    REQUIRE(r5 == "successful\nunsuccessful\n");
}

// ─────────────────────────────────────────
// Test 3: dropClass, removeClass, remove, replaceClass
// ─────────────────────────────────────────

TEST_CASE("Core Commands", "[commands]") {
    // remove
    string r1 = runCommands(R"(3
insert "Josh Smith" 12345678 1 1 COP3530
remove 12345678
remove 12345678
)");
    REQUIRE(r1 == "successful\nsuccessful\nunsuccessful\n");

    // dropClass
    string r2 = runCommands(R"(3
insert "Josh Smith" 12345678 1 2 COP3530 MAC2311
dropClass 12345678 COP3530
dropClass 12345678 COP3530
)");
    REQUIRE(r2 == "successful\nsuccessful\nunsuccessful\n");

    // replaceClass — success then fail (old code gone)
    string r3 = runCommands(R"(3
insert "Josh Smith" 12345678 1 2 COP3530 MAC2311
replaceClass 12345678 COP3530 CDA3101
replaceClass 12345678 COP3530 EEL3701
)");
    REQUIRE(r3 == "successful\nsuccessful\nunsuccessful\n");

    // removeClass — removes from multiple students
    string r4 = runCommands(R"(6
insert "Student A" 10000001 1 1 COP3502
insert "Student B" 10000002 1 1 COP3502
insert "Student C" 10000003 1 2 COP3502 MAC2311
dropClass 10000001 COP3502
remove 10000001
removeClass COP3502
)");
    REQUIRE(r4 == "successful\nsuccessful\nsuccessful\nsuccessful\nunsuccessful\n2\n");
}

// ─────────────────────────────────────────
// Test 4: printShortestEdges with toggled edges
// MAC2311 is at node 18
// shortest path from node 21: 21->12->13->15->18, distance 11
// only edges into node 18: 15-18, 17-18, 18-25, 18-34
// closing all 4 makes node 18 unreachable
// ─────────────────────────────────────────

TEST_CASE("printShortestEdges with toggled edges", "[shortest]") {
    CampusCompass compass;
    compass.ParseCSV("data/edges.csv", "data/classes.csv");

    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());

    // student at node 21, MAC2311 is at node 18
    compass.ParseCommand("insert \"Brian\" 35459999 21 1 MAC2311");

    // before toggling — MAC2311 reachable, distance 11
    compass.ParseCommand("printShortestEdges 35459999");
    string before = output.str();
    output.str("");
    REQUIRE(before.find("MAC2311: 11") != string::npos);

    // close all 4 edges connected to node 18 (MAC2311)
    // verified via dijkstra: 15-18, 17-18, 18-25, 18-34
    compass.ParseCommand("toggleEdgesClosure 4 15 18 17 18 18 25 18 34");

    // after toggling — MAC2311 unreachable
    compass.ParseCommand("printShortestEdges 35459999");
    string after = output.str();

    cout.rdbuf(old);

    REQUIRE(after.find("MAC2311: -1") != string::npos);
}