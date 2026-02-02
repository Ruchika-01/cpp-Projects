#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

/* ---------------- DATA MODEL ---------------- */

struct Task {
    std::string name;
    int duration;
    std::vector<std::string> dependents;
};

/* ---------------- STORAGE ---------------- */

// Fast lookup by task name
std::unordered_map<std::string, Task> taskTable;

// Preserve insertion order from file
std::vector<std::string> insertionOrder;

/* ---------------- TOKENIZER ---------------- */
// Responsibility: split a line into tokens

std::vector<std::string> tokenize(const std::string& line) {
    std::stringstream ss(line);
    std::vector<std::string> tokens;
    std::string token;

    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

/* ---------------- PARSER ---------------- */
// Responsibility: validate structure + build task

void parseTokens(const std::vector<std::string>& tokens) {
    if (tokens.size() < 3 || tokens[0] != "TASK") {
        std::cout << "Invalid format\n";
        return;
    }

    Task task;
    task.name = tokens[1];
    task.duration = std::stoi(tokens[2]);

    // Handle dependencies
    if (tokens.size() > 3 && tokens[3] == "->") {
        for (size_t i = 4; i < tokens.size(); i++) {
            task.dependents.push_back(tokens[i]);
        }
    }

    // Preserve insertion order once
    if (taskTable.find(task.name) == taskTable.end()) {
        insertionOrder.push_back(task.name);
    }

    taskTable[task.name] = task;
}

/* ---------------- LINE PROCESSOR ---------------- */
// Responsibility: glue tokenizer + parser

void parseLine(const std::string& line) {
    std::vector<std::string> tokens = tokenize(line);
    parseTokens(tokens);
}

/* ---------------- FILE LOADER ---------------- */

void loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Could not open file\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            parseLine(line);
        }
    }
    file.close();
}

/* ---------------- OUTPUT ---------------- */

void printTasks() {
    for (const std::string& name : insertionOrder) {
        const Task& task = taskTable[name];

        std::cout << "Task " << task.name
                  << " | Duration: " << task.duration
                  << " | Dependents: ";

        for (const std::string& dep : task.dependents) {
            std::cout << dep << " ";
        }
        std::cout << "\n";
    }
}

/* ---------------- MAIN ---------------- */

int main() {
    loadConfig("tasks.cfg");

    std::cout << "Parsed Task Graph\n";
    std::cout << "------------------\n";
    printTasks();

    return 0;
}