#include "CampusCompass.h"
#include <iostream>
#include <sstream>
#include <algorithm>
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
        string name, id;
        int residence, n;
        vector<string> classes;

        // parse quoted name
        string token;
        getline(ss, token, '"');
        getline(ss, name, '"');

        if (!(ss >> id >> residence >> n)) { cout << "unsuccessful\n"; return false; }
        for (int i = 0; i < n; i++) {
            string code;
            if (!(ss >> code)) { cout << "unsuccessful\n"; return false; }
            classes.push_back(code);
        }
        // check n matches actual number of classes parsed
        if ((int)classes.size() != n) { cout << "unsuccessful\n"; return false; }

        cout << (db.insert(name, id, residence, classes) ? "successful" : "unsuccessful") << "\n";

    } else if (cmd == "remove") {
        string id;
        if (!(ss >> id)) { cout << "unsuccessful\n"; return false; }
        cout << (db.remove(id) ? "successful" : "unsuccessful") << "\n";

    } else if (cmd == "dropClass") {
        string id, classCode;
        if (!(ss >> id >> classCode)) { cout << "unsuccessful\n"; return false; }
        cout << (db.dropClass(id, classCode) ? "successful" : "unsuccessful") << "\n";

    } else if (cmd == "replaceClass") {
        string id, oldCode, newCode;
        if (!(ss >> id >> oldCode >> newCode)) { cout << "unsuccessful\n"; return false; }
        cout << (db.replaceClass(id, oldCode, newCode) ? "successful" : "unsuccessful") << "\n";

    } else if (cmd == "removeClass") {
        string classCode;
        if (!(ss >> classCode)) { cout << "unsuccessful\n"; return false; }
        int result = db.removeClass(classCode);
        if (result == -1) cout << "unsuccessful\n";
        else cout << result << "\n";

    } else if (cmd == "toggleEdgesClosure") {
        int n;
        if (!(ss >> n)) { cout << "unsuccessful\n"; return false; }
        for (int i = 0; i < n; i++) {
            int x, y;
            if (!(ss >> x >> y)) { cout << "unsuccessful\n"; return false; }
            graph.toggleEdge(x, y);
        }
        cout << "successful\n";

    } else if (cmd == "checkEdgeStatus") {
        int x, y;
        if (!(ss >> x >> y)) { cout << "unsuccessful\n"; return false; }
        int status = graph.checkEdgeStatus(x, y);
        if (status == 1)       cout << "open\n";
        else if (status == 0)  cout << "closed\n";
        else                   cout << "DNE\n";

    } else if (cmd == "isConnected") {
        int x, y;
        if (!(ss >> x >> y)) { cout << "unsuccessful\n"; return false; }
        cout << (graph.isConnected(x, y) ? "successful" : "unsuccessful") << "\n";

    } else if (cmd == "printShortestEdges") {
        string id;
        if (!(ss >> id)) { cout << "unsuccessful\n"; return false; }
        PrintShortest(id);

    } else if (cmd == "printStudentZone") {
        string id;
        if (!(ss >> id)) { cout << "unsuccessful\n"; return false; }
        PrintZone(id);

    } else if (cmd == "verifySchedule") {
        string id;
        if (!(ss >> id)) { cout << "unsuccessful\n"; return false; }
        VerifySchedule(id);

    } else {
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
    const StudentDatabase::Student* student = db.getStudent(Id);

    auto [timeToClasses, previous] = graph.dijkstra(student->residenceLocationId);

    set<int> requiredNodes;
    for (const auto& code : student->classCodes) {
        int loc = db.getClassLocation(code);
        if (loc == -1 || timeToClasses[loc] == -1) continue;

        // walk path from residence to this class and collect all nodes
        vector<int> path = graph.getPath(previous, student->residenceLocationId, loc);
        for (int node : path)
            requiredNodes.insert(node);
    }

    int cost = graph.primMST(requiredNodes);
    cout << "Student Zone Cost For " << student->name << ": " << cost << "\n";
}
int toMinutes(const string& time) {
    int hours   = stoi(time.substr(0, 2));
    int minutes = stoi(time.substr(3, 2));
    return hours * 60 + minutes;
}
void CampusCompass::VerifySchedule(string Id) {
    const StudentDatabase::Student* student = db.getStudent(Id);

    // if only 1 class, print unsuccessful
    if (student->classCodes.size() == 1) {
        cout << "unsuccessful\n";
        return;
    }

    // build list of {startTime, endTime, locationId, classCode}
    vector<tuple<int,int,int,string>> classes;
    for (const auto& code : student->classCodes) {
        const StudentDatabase::ClassInfo* info = db.getClassInfo(code);
        if (!info) continue;
        classes.push_back({toMinutes(info->startTime),
                           toMinutes(info->endTime),
                           info->locationId,
                           code});
    }

    // sort by start time
    sort(classes.begin(), classes.end());

    cout << "Schedule Check for " << student->name << ":\n";

    // check each consecutive pair
    for (size_t i = 0; i < classes.size() - 1; i++) {
        auto [start1, end1, loc1, code1] = classes[i];
        auto [start2, end2, loc2, code2] = classes[i+1];

        int gap = start2 - end1;

        // run dijkstra from class1 location to class2 location
        auto [distances, previous] = graph.dijkstra(loc1);
        int travelTime = distances.count(loc2) ? distances[loc2] : -1;

        string result = (travelTime != -1 && gap >= travelTime)
                        ? "successful" : "unsuccessful";

        cout << code1 << " - " << code2 << ": " << result << "\n";
    }
}