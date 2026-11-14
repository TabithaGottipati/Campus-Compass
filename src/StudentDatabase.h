#pragma once
#include <set>
#include <string>
#include <vector>
#include <unordered_map>

class StudentDatabase {
public:

    // ─────────────────────────────────────────
    // Nested Struct
    // ─────────────────────────────────────────

    struct Student {
        std::string name;
        std::string studentId;
        int residenceLocationId;
        std::set<std::string> classCodes;
    };

    // ─────────────────────────────────────────
    // Constructor
    // ─────────────────────────────────────────

    StudentDatabase();

    // ─────────────────────────────────────────
    // File Loading
    // ─────────────────────────────────────────

    bool loadClasses(const std::string& filepath);

    // ─────────────────────────────────────────
    // Commands
    // ─────────────────────────────────────────

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

    // ─────────────────────────────────────────
    // Read Access for CampusCompass
    // ─────────────────────────────────────────

    const Student* getStudent(const std::string& id) const;
    int getClassLocation(const std::string& code) const;
    bool studentExists(const std::string& id) const;
    bool classExists(const std::string& code) const;
    std::vector<std::string> getAllClassCodes() const;

private:

    // ─────────────────────────────────────────
    // Data
    // ─────────────────────────────────────────

    std::unordered_map<std::string, Student> students;
    std::unordered_map<std::string, std::set<std::string>> classToStudents;
    std::unordered_map<std::string, int> classToLocation;

    // ─────────────────────────────────────────
    // Validation
    // ─────────────────────────────────────────

    bool isValidUFID(const std::string& id) const;
    bool isValidName(const std::string& name) const;
    bool isValidClassCode(const std::string& code) const;

    // ─────────────────────────────────────────
    // Index Helpers
    // ─────────────────────────────────────────

    void indexAdd(const std::string& studentId, const std::string& classCode);
    void indexRemove(const std::string& studentId, const std::string& classCode);
};