#pragma once
#include <set>
#include <string>
#include <vector>
#include <unordered_map>

class StudentDatabase {
public:

    struct Student {
        std::string name;
        std::string studentId;
        int residenceLocationId;
        std::set<std::string> classCodes;
    };

    struct ClassInfo {
        int locationId;
        std::string startTime;
        std::string endTime;
    };

    StudentDatabase();

    bool loadClasses(const std::string& filepath);

    bool insert(const std::string& name,
                const std::string& id,
                int residence,
                const std::vector<std::string>& classes);

    bool remove(const std::string& id);

    bool dropClass(const std::string& id,
                   const std::string& classCode);

    bool replaceClass(const std::string& id,
                      const std::string& oldCode,
                      const std::string& newCode);

    int removeClass(const std::string& classCode);

    const Student* getStudent(const std::string& id) const;
    const ClassInfo* getClassInfo(const std::string& code) const;
    int getClassLocation(const std::string& code) const;
    bool studentExists(const std::string& id) const;
    std::vector<std::string> getAllClassCodes() const;

private:

    // ─────────────────────────────────────────
    // Data
    // ─────────────────────────────────────────

    std::unordered_map<std::string, Student> students;
    std::unordered_map<std::string, std::set<std::string>> classToStudents;
    std::unordered_map<std::string, ClassInfo> classInfo;

    // ─────────────────────────────────────────
    // Validation
    // ─────────────────────----------------------------------------------------------------────────
    bool isValidUFID(const std::string& id) const;
    bool isValidName(const std::string& name) const;
    bool isValidClassCode(const std::string& code) const;
};