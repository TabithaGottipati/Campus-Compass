
#include "StudentDatabase.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <cctype>

using namespace std;

// Constructor

StudentDatabase::StudentDatabase() {}

// File loading

bool StudentDatabase::loadClasses(const string& filepath) {
    ifstream file(filepath);
    if (!file.is_open()) {
        return false;
    }

    string line;
    getline(file, line); // Skip the header row.

    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        istringstream ss(line);
        string code, locationStr, start, end;

        if (!getline(ss, code, ',')) {
            continue;
        }
        if (!getline(ss, locationStr, ',')) {
            continue;
        }
        if (!getline(ss, start, ',')) {
            continue;
        }
        if (!getline(ss, end, ',')) {
            continue;
        }

        classInfo[code] = {stoi(locationStr), start, end};
    }

    return true;
}


bool StudentDatabase::isValidUFID(const string& id) const {
    if (id.length() != 8) {
        return false;
    }

    for (char c : id) {
        if (!isdigit(c)) {
            return false;
        }
    }

    if (students.find(id) != students.end()) {
        return false;
    }

    return true;
}


bool StudentDatabase::isValidName(const string& name) const {
    if (name.empty()) {
        return false;
    }

    bool hasLetter = false;
    for (char c : name) {
        if (!isalpha(c) && c != ' ') {
            return false;
        }
        if (isalpha(c)) {
            hasLetter = true;
        }
    }

    return hasLetter;
}


bool StudentDatabase::isValidClassCode(const string& code) const {
    if (code.length() != 7) {
        return false;
    }

    for (size_t i = 0; i < code.size(); i++) {
        if (i < 3 && !isupper(code[i])) {
            return false;
        }
        if (i > 2 && !isdigit(code[i])) {
            return false;
        }
    }

    return true;
}


bool StudentDatabase::insert(const string& name, const string& id, int residence, const vector<string>& classes) {
    // Validate the student name and ID.
    Student student;
    if (!isValidName(name)) {
        return false;
    }
    if (!isValidUFID(id)) {
        return false;
    }

    student.name = name;
    student.studentId = id;
    student.residenceLocationId = residence;

    // Validate and store the student's class codes.
    if (classes.empty() || classes.size() > 6) {
        return false;
    }

    for (auto i : classes) {
        if (!isValidClassCode(i)) {
            return false;
        }
    }
    for (auto i : classes) {
        student.classCodes.insert(i);
        classToStudents[i].insert(id);
    }

    if (students.find(id) != students.end()) {
        return false;
    }

    students[id] = student;
    return true;
}


bool StudentDatabase::remove(const string& id) {
    // Ensure the student exists before removing their class associations.
    if (students.find(id) == students.end()) {
        return false;
    }

    for (auto i : students[id].classCodes) {
        classToStudents[i].erase(id);
    }

    students.erase(id);
    return true;
}


bool StudentDatabase::dropClass(const string& id, const string& classCode) {
    if (students.find(id) == students.end()) {
        return false;
    }
    if (!students[id].classCodes.count(classCode)) {
        return false;
    }

    students[id].classCodes.erase(classCode);
    classToStudents[classCode].erase(id);

    // Remove the student if they no longer have any classes.
    if (students[id].classCodes.empty()) {
        students.erase(id);
    }

    return true;
}


bool StudentDatabase::replaceClass(const string& id, const string& oldCode, const string& newCode) {
    // Validate the student, existing class, and replacement class.
    if (students.find(id) == students.end()) {
        return false;
    }
    if (!students[id].classCodes.count(oldCode)) {
        return false;
    }
    if (students[id].classCodes.count(newCode)) {
        return false;
    }
    if (!classInfo.count(newCode)) {
        return false;
    }

    students[id].classCodes.insert(newCode);
    classToStudents[newCode].insert(id);
    dropClass(id, oldCode);

    return true;
}


int StudentDatabase::removeClass(const string& classCode) {
    if (!isValidClassCode(classCode) || !classInfo.count(classCode)) {
        return -1;
    }
    // Ensure the class has an associated student set.
    if (!classToStudents.count(classCode) || classToStudents[classCode].empty()) {
        return -1;
    }


    int count = 0;
    for (auto i : classToStudents[classCode]) {
        count++;
        students[i].classCodes.erase(classCode);

        // Remove students who no longer have any classes.
        if (students[i].classCodes.empty()) {
            students.erase(i);
        }
    }

    classToStudents.erase(classCode);
    return count;
}


const StudentDatabase::Student* StudentDatabase::getStudent(const string& id) const {
    auto it = students.find(id);
    return it != students.end() ? &it->second : nullptr;
}


const StudentDatabase::ClassInfo* StudentDatabase::getClassInfo(const string& code) const {
    auto it = classInfo.find(code);
    return it != classInfo.end() ? &it->second : nullptr;
}


int StudentDatabase::getClassLocation(const string& code) const {
    auto it = classInfo.find(code);
    return it != classInfo.end() ? it->second.locationId : -1;
}


bool StudentDatabase::studentExists(const string& id) const {
    return students.count(id) > 0;
}


vector<string> StudentDatabase::getAllClassCodes() const {
    vector<string> codes;

    for (const auto& [code, info] : classInfo) {
        codes.push_back(code);
    }

    return codes;
}
