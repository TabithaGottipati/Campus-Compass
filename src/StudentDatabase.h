//
// Created by tgott on 11/13/2026.
//

#ifndef PROJECT3_STUDENTS_H
#define PROJECT3_STUDENTS_H
#include <string>


class StudentDatabase {
    public:
        struct student {
            string name;
            string ID;
            int residenceID;
            set<string> classCodes;
        };
        bool insert(string name, string id, int residence, vector<string> classes);
        bool remove(string id);
        bool dropClass(string id, string classCode);
        bool replaceClass(string id, string oldCode, string newCode);
        int  removeClass(string classCode);
        bool studentExists(string id, string classCode);
        int getResidenceID(string classCode);
    private:
        std::unordered_map<std::string, Student> students;
        std::unordered_map<std::string, std::set<std::string>> classToStudents;
        std::unordered_map<std::string, int> classToLocation;

        // Validation — private, only StudentDatabase calls these
        bool isValidUFID(const std::string& id) const;
        bool isValidName(const std::string& name) const;
        bool isValidClassCode(const std::string& code) const;

        // Index sync helpers
        void indexAdd(const std::string& studentId, const std::string& classCode);
        void indexRemove(const std::string& studentId, const std::string& classCode);
};

#endif //PROJECT3_STUDENTS_H
