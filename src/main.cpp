#include "ProjectManager.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>

void printHelp() {
    std::cout << "Usage: cpp-manager <command> [options]\n"
              << "Commands:\n"
              << "  init <project-name>        Initialize a new C++ project\n"
              << "  build                      Build the project\n"
              << "  test                       Run tests\n"
              << "  create header <name>       Create a header file\n"
              << "  create module <name> [--header] Create a module (with optional header)\n"
              << "  delete module <name>       Delete a module\n"
              << "  src [--list]               Edit or list source files\n"
              << "  help                       Show this help message\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printHelp();
        return 1;
    }

    std::unordered_map<std::string, std::function<void()>> commands;

    commands["init"] = [&]() {
        if (argc == 3) {
            std::string projectName = argv[2];
            ProjectManager manager(projectName);
            manager.initializeProject();
        } else {
            printHelp();
        }
    };

    commands["build"] = [&]() {
        ProjectManager manager(".");
        manager.buildProject();
    };

    commands["test"] = [&]() {
        ProjectManager manager(".");
        manager.runTests();
    };

    commands["create"] = [&]() {
        if (argc >= 4) {
            std::string type = argv[2];
            std::string name = argv[3];
            ProjectManager manager(".");
            if (type == "header") {
                manager.createHeader(name);
            } else if (type == "module") {
                bool createHeader = (argc >= 5 && std::string(argv[4]) == "--header");
                manager.createModule(name, createHeader);
            } else {
                std::cerr << "Invalid create command.\n";
                printHelp();
            }
        } else {
            printHelp();
        }
    };

    commands["delete"] = [&]() {
        if (argc == 4 && std::string(argv[2]) == "module") {
            std::string name = argv[3];
            ProjectManager manager(".");
            manager.deleteModule(name);
        } else {
            printHelp();
        }
    };

    commands["src"] = [&]() {
        std::string subCommand = (argc >= 3) ? argv[2] : "";
        ProjectManager manager(".");
        manager.srcCommand(subCommand);
    };

    commands["help"] = [&]() {
        printHelp();
    };

    std::string command = argv[1];
    if (commands.find(command) != commands.end()) {
        commands[command]();
    } else {
        std::cerr << "Invalid command or arguments.\n";
        printHelp();
        return 1;
    }

    return 0;
}