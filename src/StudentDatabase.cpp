#include "StudentDatabase.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include<cctype>
using namespace std;

// ─────────────────────────────────────────
// Constructor
// ─────────────────────────────────────────

StudentDatabase::StudentDatabase() {

}

// ─────────────────────────────────────────
// File Loading
// ─────────────────────────────────────────

bool StudentDatabase::loadClasses(const string& filepath) {
    ifstream file(filepath);
    if (!file.is_open()) return false;

    string line;
    getline(file, line); // skip header row

    while (getline(file, line)) {
        if (line.empty()) continue;

        istringstream ss(line);
        string code, locationStr, start, end;

        if (!getline(ss, code, ','))        continue;
        if (!getline(ss, locationStr, ',')) continue;
        if (!getline(ss, start, ','))       continue;
        if (!getline(ss, end, ','))         continue;

        classInfo[code] = {stoi(locationStr), start, end};
    }

    return true;
}

bool StudentDatabase::isValidUFID(const string& id) const {
    if (id.length()!=8) return false;
    for (char c:id) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool StudentDatabase::isValidName(const string& name) const {
    if (name.empty()) return false;
    for (char c:name) {
        if (!isalpha(c)||c==' ') return false;
    }
    return true;
}

bool StudentDatabase::isValidClassCode(const string& code) const {
    if (code.length()!=7) return false;
    for (int i=0;i<code.size();i++) {
        if (i<3&&!isalpha(code[i])) return false;
        if (i>2&&!isdigit(code[i])) return false;
    }
    return true;
}

// ─────────────────────────────────────────
// Private Index Helpers
// ─────────────────────────────────────────

void StudentDatabase::indexAdd(const string& studentId, const string& classCode) {
    // add studentId to classToStudents[classCode]
}

void StudentDatabase::indexRemove(const string& studentId, const string& classCode) {
    // remove studentId from classToStudents[classCode]
    // if classToStudents[classCode] is now empty, erase the entry
}

// ─────────────────────────────────────────
// Public Commands
// ─────────────────────────────────────────

bool StudentDatabase::insert(const string& name,const string& id,int residence,const vector<string>& classes) {
    // validate name
    // validate id format
    // check id is unique
    // check classes size is between 1 and 6
    // validate each class code format
    // check each class code exists in classToLocation
    // if any check fails: print "unsuccessful" and return false

    // build Student object
    // add to students map
    // call indexAdd for each class code
    // print "successful"
}

bool StudentDatabase::remove(const string& id) {
    // check student exists
    // if not: print "unsuccessful" and return false

    // call indexRemove for each of their class codes
    // erase from students map
    // print "successful"
}

bool StudentDatabase::dropClass(const string& id, const string& classCode) {
    // check student exists
    // check student has classCode
    // if either fails: print "unsuccessful" and return false

    // call indexRemove
    // erase classCode from student's classCodes set
    // if student now has 0 classes: erase from students map
    // print "successful"
}

bool StudentDatabase::replaceClass(const string& id,
                                    const string& oldCode,
                                    const string& newCode) {
    // check student exists
    // check student has oldCode
    // check student does NOT already have newCode
    // check newCode exists in classToLocation
    // if any check fails: print "unsuccessful" and return false

    // call indexRemove for oldCode
    // call indexAdd for newCode
    // update student's classCodes set
    // print "successful"
}

int StudentDatabase::removeClass(const string& classCode) {
    // check classCode exists in classToStudents
    // if not: print "unsuccessful" and return 0

    // copy the set of affected student IDs (copy, not reference)
    // for each affected student:
    //     call indexRemove
    //     erase classCode from their classCodes set
    //     if they now have 0 classes: erase from students map
    // print count of affected students
    // return count
}

// ─────────────────────────────────────────
// Public Read Access
// ─────────────────────────────────────────

const StudentDatabase::Student* StudentDatabase::getStudent(const string& id) const {
    // find id in students map
    // return pointer to Student if found, nullptr if not
}

int StudentDatabase::getClassInfo(const string& code) const {
    auto it = classInfo.find(code);
    return it != classInfo.end() ? it->second.locationId : -1;
}

bool StudentDatabase::studentExists(const string& id) const {

}

bool StudentDatabase::classExists(const string& code) const {

}

vector<string> StudentDatabase::getAllClassCodes() const {
    // iterate classToLocation
    // return all keys as a vector
}