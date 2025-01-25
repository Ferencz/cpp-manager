#include "ProjectManager.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <termios.h>
#include <unistd.h>
#include <fstream>
#include <filesystem>

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

void printLLMHelp() {
    std::cout << "LLM Commands:\n"
              << "  load plugin                Load LLM plugin\n";
}

std::string getHiddenInput() {
    termios oldt, newt;
    std::string input;
    char ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while (std::cin.get(ch) && ch != '\n') {
        input += ch;
        std::cout << '*';
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << std::endl;
    return input;
}

void storeApiKey(const std::string& apiKey, const std::string& filePath) {
    std::filesystem::create_directories(std::filesystem::path(filePath).parent_path());
    std::ofstream outFile(filePath);
    if (outFile) {
        outFile << apiKey;
        outFile.close();
        std::cout << "API key stored successfully.\n";
    } else {
        std::cerr << "Failed to store API key.\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printHelp();
        return 1;
    }

    std::unordered_map<std::string, std::function<void()>> commands;
    bool llmLoaded = false;

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
        if (llmLoaded) {
            printLLMHelp();
        }
    };

    commands["load"] = [&]() {
        if (argc == 3 && std::string(argv[2]) == "plugin") {
            std::cout << "Select LLM type (local/remote): ";
            std::string llmType;
            std::cin >> llmType;

            if (llmType == "remote") {
                std::cout << "Enter API key: ";
                std::string apiKey = getHiddenInput();

                std::cout << "Store API key permanently? (yes/no): ";
                std::string storeOption;
                std::cin >> storeOption;

                if (storeOption == "yes") {
                    const char* homeDir = getenv("HOME");
                    if (homeDir) {
                        std::string filePath = std::string(homeDir) + "/.cpp-manager/.llm.key";
                        storeApiKey(apiKey, filePath);
                    } else {
                        std::cerr << "Unable to locate home directory.\n";
                    }
                } else {
                    std::string projectPath = ".";
                    std::string filePath = projectPath + "/.llm.key";
                    storeApiKey(apiKey, filePath);
                }
            } else if (llmType == "local") {
                // Logic to load local LLM
            } else {
                std::cerr << "Invalid LLM type.\n";
                return;
            }

            llmLoaded = true;
            std::cout << "LLM plugin loaded successfully.\n";
        } else {
            printHelp();
        }
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