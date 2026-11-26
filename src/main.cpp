#include <iostream>

#include "CampusCompass.h"

using namespace std;

int main() {
    // Initialize the main project object.
    CampusCompass compass;

    // Load graph and class data.
    compass.ParseCSV("data/edges.csv", "data/classes.csv");

    // Read and process the specified number of commands.
    int no_of_lines;
    string command;

    cin >> no_of_lines;
    cin.ignore(); // Ignore the newline left by cin.

    for (int i = 0; i < no_of_lines; i++) {
        getline(cin, command);
        compass.ParseCommand(command);
    }

    return 0;
}