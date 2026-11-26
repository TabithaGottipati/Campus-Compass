
#include "CampusCompass.h"
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

CampusCompass::CampusCompass() {}


bool CampusCompass::ParseCSV(const string& edges_filepath, const string& classes_filepath) {
    if (db.loadClasses(classes_filepath) && graph.loadEdges(edges_filepath)) {
        return true;
    }
    return false;
}

// Command parsing

bool CampusCompass::ParseCommand(const string& command) {
    istringstream ss(command);
    string cmd;
    ss >> cmd;

    if (cmd == "insert") {
        string name, id;
        int residence, n;
        vector<string> classes;

        // Parse the quoted student name.
        string token;
        getline(ss, token, '"');
        getline(ss, name, '"');

        if (!(ss >> id >> residence >> n)) {
            cout << "unsuccessful\n";
            return false;
        }

        // Read the specified class codes.
        for (int i = 0; i < n; i++) {
            string code;
            if (!(ss >> code)) {
                cout << "unsuccessful\n";
                return false;
            }
            classes.push_back(code);
        }

        cout << (db.insert(name, id, residence, classes) ? "successful" : "unsuccessful") << "\n";

    } else if (cmd == "remove") {
        string id;
        if (!(ss >> id)) {
            cout << "unsuccessful\n";
            return false;
        }

        cout << (db.remove(id) ? "successful" : "unsuccessful") << "\n";

    } else if (cmd == "dropClass") {
        string id, classCode;
        if (!(ss >> id >> classCode)) {
            cout << "unsuccessful\n";
            return false;
        }

        cout << (db.dropClass(id, classCode) ? "successful" : "unsuccessful") << "\n";

    } else if (cmd == "replaceClass") {
        string id, oldCode, newCode;
        if (!(ss >> id >> oldCode >> newCode)) {
            cout << "unsuccessful\n";
            return false;
        }

        cout << (db.replaceClass(id, oldCode, newCode) ? "successful" : "unsuccessful") << "\n";

    } else if (cmd == "removeClass") {
        string classCode;
        if (!(ss >> classCode)) {
            cout << "unsuccessful\n";
            return false;
        }

        int result = db.removeClass(classCode);
        if (result == -1) {
            cout << "unsuccessful\n";
        } else {
            cout << result << "\n";
        }

    } else if (cmd == "toggleEdgesClosure") {
        int n;
        if (!(ss >> n)) {
            cout << "unsuccessful\n";
            return false;
        }

        // Read and toggle each specified edge.
        for (int i = 0; i < n; i++) {
            int x, y;
            if (!(ss >> x >> y)) {
                cout << "unsuccessful\n";
                return false;
            }
            graph.toggleEdge(x, y);
        }

        cout << "successful\n";

    } else if (cmd == "checkEdgeStatus") {
        int x, y;
        if (!(ss >> x >> y)) {
            cout << "unsuccessful\n";
            return false;
        }

        int status = graph.checkEdgeStatus(x, y);
        if (status == 1) {
            cout << "open\n";
        } else if (status == 0) {
            cout << "closed\n";
        } else {
            cout << "DNE\n";
        }

    } else if (cmd == "isConnected") {
        int x, y;
        if (!(ss >> x >> y)) {
            cout << "unsuccessful\n";
            return false;
        }

        cout << (graph.isConnected(x, y) ? "successful" : "unsuccessful") << "\n";

    } else if (cmd == "printShortestEdges") {
        string id;
        if (!(ss >> id)) {
            return false;
        }

        PrintShortest(id);

    } else if (cmd == "printStudentZone") {
        string id;
        if (!(ss >> id)) {
            cout << "unsuccessful\n";
            return false;
        }

        PrintZone(id);

    } else if (cmd == "verifySchedule") {
        string id;
        if (!(ss >> id)) {
            cout << "unsuccessful\n";
            return false;
        }

        VerifySchedule(id);

    } else {
        cout << "unsuccessful\n";
        return false;
    }

    return true;
}

// Private handlers

void CampusCompass::PrintShortest(string id) {
    cout << "Time For Shortest Edges: " << db.getStudent(id)->name << endl;

    // Calculate shortest travel times from the student's residence.
    auto timeToClasses = graph.dijkstra(db.getStudent(id)->residenceLocationId).first;

    for (auto classCode : db.getStudent(id)->classCodes) {
        cout << classCode << ": " << timeToClasses[db.getClassLocation(classCode)] << endl;
    }
}

void CampusCompass::PrintZone(string id) {
    const StudentDatabase::Student* student = db.getStudent(id);

    auto [timeToClasses, previous] = graph.dijkstra(student->residenceLocationId);

    set<int> requiredNodes;
    for (const auto& code : student->classCodes) {
        int loc = db.getClassLocation(code);
        if (loc == -1 || timeToClasses[loc] == -1) {
            continue;
        }

        // Collect all nodes along the path from the residence to the class.
        vector<int> path = graph.getPath(previous, student->residenceLocationId, loc);
        for (int node : path) {
            requiredNodes.insert(node);
        }
    }

    int cost = graph.primMST(requiredNodes);
    cout << "Student Zone Cost For " << student->name << ": " << cost << "\n";
}

int toMinutes(const string& time) {
    int hours = stoi(time.substr(0, 2));
    int minutes = stoi(time.substr(3, 2));

    return hours * 60 + minutes;
}

void CampusCompass::VerifySchedule(string id) {
    const StudentDatabase::Student* student = db.getStudent(id);

    // A schedule with only one class does not require travel verification.
    if (student->classCodes.size() == 1) {
        cout << "unsuccessful\n";
        return;
    }

    // Store each class as [start time, end time, location ID, class code].
    vector<tuple<int, int, int, string>> classes;
    for (const auto& code : student->classCodes) {
        const StudentDatabase::ClassInfo* info = db.getClassInfo(code);
        if (!info) {
            continue;
        }

        classes.push_back({
            toMinutes(info->startTime),
            toMinutes(info->endTime),
            info->locationId,
            code
        });
    }

    // Sort classes by their start times.
    sort(classes.begin(), classes.end());

    cout << "Schedule Check for " << student->name << ":\n";

    // Verify travel time between each consecutive pair of classes.
    for (size_t i = 0; i < classes.size() - 1; i++) {
        auto [start1, end1, loc1, code1] = classes[i];
        auto [start2, end2, loc2, code2] = classes[i + 1];

        int gap = start2 - end1;

        // Calculate the shortest travel time between the two class locations.
        auto [distances, previous] = graph.dijkstra(loc1);
        int travelTime = distances.count(loc2) ? distances[loc2] : -1;

        string result = (travelTime != -1 && gap >= travelTime)
            ? "successful"
            : "unsuccessful";

        cout << code1 << " - " << code2 << ": " << result << "\n";
    }
}
