// src/main.cpp
#include "ProjectManager.h"
#include <iostream>
#include <string>

void printHelp() {
    std::cout << "Usage: cpp-manager <command> [options]\n"
              << "Commands:\n"
              << "  init <project-name>        Initialize a new C++ project\n"
              << "  add <dependency>           Add a dependency\n"
              << "  build                      Build the project\n"
              << "  test                       Run tests\n"
              << "  create header <name>       Create a header file\n"
              << "  create module <name> [--header] Create a module (with optional header)\n"
              << "  delete module <name>       Delete a module\n"
              << "  help                       Show this help message\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printHelp();
        return 1;
    }

    std::string command = argv[1];

    if (command == "init" && argc == 3) {
        std::string projectName = argv[2];
        ProjectManager manager(projectName);
        manager.initializeProject();
    } else if (command == "add" && argc == 3) {
        std::string dependency = argv[2];
        ProjectManager manager(".");
        manager.addDependency(dependency);
    } else if (command == "build") {
        ProjectManager manager(".");
        manager.buildProject();
    } else if (command == "test") {
        ProjectManager manager(".");
        manager.runTests();
    } else if (command == "create" && argc >= 4) {
        std::string subCommand = argv[2];
        std::string name = argv[3];
        ProjectManager manager(".");

        if (subCommand == "header") {
            manager.createHeader(name);
        } else if (subCommand == "module") {
            bool createHeader = (argc >= 5 && std::string(argv[4]) == "--header");
            manager.createModule(name, createHeader);
        } else {
            std::cerr << "Invalid create command.\n";
            printHelp();
            return 1;
        }
    } else if (command == "delete" && argc == 4 && std::string(argv[2]) == "module") {
        std::string name = argv[3];
        ProjectManager manager(".");
        manager.deleteModule(name);
    } else if (command == "help") {
        printHelp();
    } else {
        std::cerr << "Invalid command or arguments.\n";
        printHelp();
        return 1;
    }

    return 0;
}