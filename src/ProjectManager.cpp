#include "ProjectManager.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <filesystem>
#include <algorithm> // For std::transform
#include <cctype>    // For std::toupper

namespace fs = std::filesystem;

ProjectManager::ProjectManager(const std::string& projectName)
    : projectName(projectName) {}

void ProjectManager::initializeProject() {
    createDirectory(projectName);
    createDirectory(projectName + "/src");
    createDirectory(projectName + "/include");
    createDirectory(projectName + "/test");

    executeCommand("cd " + projectName + " && git init");

    std::string cmakeContent = R"(
cmake_minimum_required(VERSION 3.10)
project()" + projectName + R"( VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable()" + projectName + R"( src/main.cpp)
)";
    createFile(projectName + "/CMakeLists.txt", cmakeContent);

    std::string mainContent = R"(
#include <iostream>

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
)";
    createFile(projectName + "/src/main.cpp", mainContent);

    std::cout << "Project initialized successfully!" << std::endl;
}

void ProjectManager::addDependency(const std::string& dependency) {
    dependencies.push_back(dependency);
    std::cout << "Added dependency: " << dependency << std::endl;
}

void ProjectManager::buildProject() {
    executeCommand("cd " + projectName + " && mkdir -p build && cd build && cmake .. && make");
    std::cout << "Project built successfully!" << std::endl;
}

void ProjectManager::runTests() {
    executeCommand("cd " + projectName + "/build && ctest");
    std::cout << "Tests executed successfully!" << std::endl;
}

void ProjectManager::createHeader(const std::string& headerName) {
    std::string headerPath = projectName + "/include/" + headerName + ".h";
    std::string headerGuard = headerName;
    std::transform(headerGuard.begin(), headerGuard.end(), headerGuard.begin(), [](unsigned char c) {
        return std::toupper(c);
    });
    headerGuard += "_H";

    std::string headerContent = R"(
#ifndef )" + headerGuard + R"(
#define )" + headerGuard + R"(

// Your code here

#endif // )" + headerGuard + R"(
)";
    createFile(headerPath, headerContent);
    std::cout << "Header file created: " << headerPath << std::endl;
}

void ProjectManager::createModule(const std::string& moduleName, bool createHeader) {
    std::string cppPath = projectName + "/src/" + moduleName + ".cpp";
    std::string cppContent = R"(
#include ")" + (createHeader ? (moduleName + ".h") : "") + R"("

// Your code here
)";
    createFile(cppPath, cppContent);
    std::cout << "Module file created: " << cppPath << std::endl;

    if (createHeader) {
        this->createHeader(moduleName); // Fixed: Use 'this->' to call the member function
    }
}

void ProjectManager::deleteModule(const std::string& moduleName) {
    std::string cppPath = projectName + "/src/" + moduleName + ".cpp";
    std::string headerPath = projectName + "/include/" + moduleName + ".h";

    deleteFile(cppPath);
    deleteFile(headerPath);

    std::cout << "Module deleted: " << moduleName << std::endl;
}

void ProjectManager::createDirectory(const std::string& path) {
    fs::create_directories(path);
}

void ProjectManager::createFile(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    file << content;
    file.close();
}

void ProjectManager::executeCommand(const std::string& command) {
    std::system(command.c_str());
}

void ProjectManager::deleteFile(const std::string& path) {
    if (fs::exists(path)) {
        fs::remove(path);
    }
}