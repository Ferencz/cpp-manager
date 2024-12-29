// src/ProjectManager.cpp
#include "ProjectManager.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <filesystem>
#include <algorithm> // For std::transform
#include <cctype>    // For std::toupper
#include <cstdio>    // For popen and pclose

namespace fs = std::filesystem;

ProjectManager::ProjectManager(const std::string& projectName)
    : projectName(projectName) {}

void ProjectManager::initializeProject() {
    createDirectory(projectName);
    createDirectory(projectName + "/src");
    createDirectory(projectName + "/include");
    createDirectory(projectName + "/test");

    // Set up Python virtual environment in the root folder
    setupPythonVirtualEnv();

    executeCommand("cd " + projectName + " && git init");

    // Generate CMakeLists.txt with Conan support
    std::string cmakeContent = R"(
cmake_minimum_required(VERSION 3.10)
project()" + projectName + R"( VERSION 1.0 LANGUAGES CXX)

# Run Conan to install dependencies
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    message(STATUS "Running Conan to install dependencies...")
    execute_process(
        COMMAND conan install ${CMAKE_SOURCE_DIR} --build=missing
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        RESULT_VARIABLE CONAN_RESULT
    )
    if(NOT CONAN_RESULT EQUAL 0)
        message(FATAL_ERROR "Conan failed to install dependencies.")
    endif()
endif()

# Include Conan-generated files
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable()" + projectName + R"( src/main.cpp)

# Link Conan dependencies
target_link_libraries()" + projectName + R"( ${CONAN_LIBS})
)";
    createFile(projectName + "/CMakeLists.txt", cmakeContent);

    // Create main.cpp
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

void ProjectManager::setupPythonVirtualEnv() {
    // Path to the virtual environment
    std::string venvPath = projectName + "/manager";

    // Check if the virtual environment already exists
    if (fs::exists(venvPath)) {
        std::cout << "Virtual environment already exists at " << venvPath << std::endl;
        return;
    }

    // Create Python virtual environment
    std::string createVenvCommand = "python3 -m venv " + venvPath;
    executeCommandWithOutput(createVenvCommand);

    // Install Conan in the virtual environment
    std::string installConanCommand = venvPath + "/bin/pip install conan"; // Use the virtual environment's pip directly
    executeCommandWithOutput(installConanCommand);

    std::cout << "(" << venvPath << ") virtual environment created and Conan installed successfully " << std::endl;
}

void ProjectManager::addDependency(const std::string& dependency) {
    std::string command = "cd " + projectName + " && source manager/bin/activate && conan install " + dependency + " --build=missing";
    executeCommandWithOutput(command); // Use executeCommandWithOutput for real-time output
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
        this->createHeader(moduleName);
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

void ProjectManager::executeCommandWithOutput(const std::string& command) {
    char buffer[128];
    std::string fullCommand = "/bin/bash -c '" + command + "'"; // Use bash to handle 'source'
    FILE* pipe = popen(fullCommand.c_str(), "r");
    if (!pipe) {
        std::cerr << "Failed to execute command: " << command << std::endl;
        return;
    }

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        std::cout << buffer; // Print the output in real-time
    }

    int status = pclose(pipe);
    if (status != 0) {
        std::cerr << "Command failed with exit code: " << status << std::endl;
    }
}

void ProjectManager::deleteFile(const std::string& path) {
    if (fs::exists(path)) {
        fs::remove(path);
    }
}