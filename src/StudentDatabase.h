//
// Created by tgott on 11/13/2026.
//

#ifndef PROJECT3_STUDENTS_H
#define PROJECT3_STUDENTS_H
#include <string>


class StudentDatabase {
    public:
        struct Student {
            string name;
            string ID;
            int residenceID;
            set<string> classCodes;
        }
        bool insert(string name, string id, int residence, vector<string> classes);
        bool remove(int id);
        bool dropClass(int id, string classCode);
        bool replaceClass(int id, string oldCode, string newCode);
        int  removeClass(string classCode);
    private:
        unordered_map<string, Student> students;          // keyed by string ID
        unordered_map<string, set<string>> classLists; // class -> set of string IDs

};

#endif //PROJECT3_STUDENTS_H
