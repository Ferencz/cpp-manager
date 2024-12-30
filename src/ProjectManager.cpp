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

    // Prompt to install the package manager
    if (promptToInstallPackageManager()) {
        // Set up Python virtual environment in the root folder
        setupPythonVirtualEnv();
        // Create conanfile.txt
        std::string conanfileContent = R"(
[requires]

[generators]
CMakeToolchain
)";
        createFile(projectName + "/conanfile.txt", conanfileContent);
    }

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

bool ProjectManager::promptToInstallPackageManager() {
    std::string response;
    std::cout << "Do you want to install the package manager (Conan)? [y/N]: ";
    std::getline(std::cin, response);

    // Default to 'N' if the response is empty
    if (response.empty() || (response[0] != 'y' && response[0] != 'Y')) {
        std::cout << "Skipping package manager installation." << std::endl;
        return false;
    }

    std::cout << "Installing Conan..." << std::endl;
    return true;
}

void ProjectManager::setupPythonVirtualEnv() {
    // Path to the virtual environment in the project root folder
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
    std::string installConanCommand = venvPath + "/bin/pip install conan";
    executeCommandWithOutput(installConanCommand);

    std::cout << "Python virtual environment created and Conan installed in " << venvPath << std::endl;
}

void ProjectManager::addDependency(const std::string& dependency) {
    // Update conanfile.txt with the new dependency
    std::string conanfilePath = projectName + "/conanfile.txt";
    std::ifstream conanfileIn(conanfilePath);
    std::string conanfileContent((std::istreambuf_iterator<char>(conanfileIn)), std::istreambuf_iterator<char>());
    conanfileIn.close();

    // Add the dependency to the [requires] section
    size_t requiresPos = conanfileContent.find("[requires]");
    if (requiresPos != std::string::npos) {
        size_t insertPos = conanfileContent.find("\n", requiresPos) + 1;
        conanfileContent.insert(insertPos, dependency + "\n");
    }

    // Write the updated conanfile.txt
    std::ofstream conanfileOut(conanfilePath);
    conanfileOut << conanfileContent;
    conanfileOut.close();

    // Install the dependency using Conan
    std::string installCommand = "cd " + projectName + " && source manager/bin/activate && conan install . --build=missing";
    executeCommandWithOutput(installCommand);

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

void ProjectManager::srcCommand(const std::string& subCommand) {
    if (subCommand == "--list") {
        listSourceFiles();
    } else {
        std::string fileName;
        std::cout << "Enter the file name (e.g., query.cpp): ";
        std::getline(std::cin, fileName);

        if (fileName.empty()) {
            std::cerr << "File name cannot be empty." << std::endl;
            return;
        }

        editSourceFile(fileName);
    }
}

void ProjectManager::listSourceFiles() {
    std::string srcPath = projectName + "/src";
    std::cout << "Source files in " << srcPath << ":" << std::endl;

    for (const auto& entry : fs::directory_iterator(srcPath)) {
        if (entry.path().extension() == ".cpp") {
            std::cout << "  " << entry.path().filename().string() << std::endl;
        }
    }
}

void ProjectManager::editSourceFile(const std::string& fileName) {
    std::string filePath = projectName + "/src/" + fileName;

     // Check if the file is being created for the first time
    bool isNewFile = !fs::exists(filePath);

    // Create the file if it doesn't exist
    if (isNewFile) {
        std::ofstream file(filePath);
        file << "#include <iostream>\n\n"; // Include iostream for new files
        file.close();
        std::cout << "Created file: " << filePath << std::endl;
    }

    std::cout << "Editing file: (" << fileName << ")" << std::endl;

    while (true) {
        std::string input;
        std::cout << "Enter class/struct/function [name] (or 'quit' to exit): ";
        std::getline(std::cin, input);

        if (input == "quit") {
            break;
        }

        if (input.find("class") == 0 || input.find("struct") == 0) {
            std::string type = input.substr(0, input.find(' '));
            std::string name = input.substr(input.find(' ') + 1);
            std::string code = createClassOrStructPrompt(type, name);
            std::ofstream file(filePath, std::ios::app);
            file << code << std::endl;
            file.close();
        } else if (input.find("function") == 0) {
            std::string name = input.substr(input.find(' ') + 1);
            std::string code = createFunctionPrompt(name);
            std::ofstream file(filePath, std::ios::app);
            file << code << std::endl;
            file.close();
        } else {
            std::cerr << "Invalid input. Use 'class', 'struct', or 'function'." << std::endl;
        }
    }
}

std::string ProjectManager::createFunctionPrompt(const std::string& name, bool nested) {
    std::string description;
    std::cout << "(" << name << ")Enter function description: ";
    std::getline(std::cin, description);

    std::string returnType;
    std::cout << "(" << name << ")Enter return type: ";
    std::getline(std::cin, returnType);

    std::vector<std::pair<std::string, std::string>> parameters;
    while (true) {
        std::string param;
        std::cout << "(" << name << ") Enter parameter (name type) or press Enter to finish: ";
        std::getline(std::cin, param);

        if (param.empty()) {
            break;
        }

        size_t space = param.find(' ');
        if (space == std::string::npos) {
            std::cerr << "Invalid parameter format. Use 'name type'." << std::endl;
            continue;
        }

        std::string paramName = param.substr(0, space);
        std::string paramType = param.substr(space + 1);
        parameters.push_back({paramName, paramType});
    }

    std::string code = returnType + " " + name + "(";
    for (size_t i = 0; i < parameters.size(); ++i) {
        code += parameters[i].second + " " + parameters[i].first;
        if (i < parameters.size() - 1) {
            code += ", ";
        }
    }
    code += ") {\n";
    code += nested ?"\t// " + description + "\n" : "// " + description + "\n";
    code += nested ? "\t    // Write your code\n" :"    // Write your code\n";
    code += nested ? "\t}\n" : "}\n";

    return code;
}

std::string ProjectManager::createClassOrStructPrompt(const std::string& type, const std::string& name) {
    std::string description;
    std::cout << "(" << name << ") Enter " << type << " description: ";
    std::getline(std::cin, description);

    std::vector<std::pair<std::string, std::string>> attributes;
    while (true) {
        std::string attr;
        std::cout << "(" << name << ") Enter attribute (name type) or press Enter to finish: ";
        std::getline(std::cin, attr);

        if (attr.empty()) {
            break;
        }

        size_t space = attr.find(' ');
        if (space == std::string::npos) {
            std::cerr << "Invalid attribute format. Use 'name type'." << std::endl;
            continue;
        }

        std::string attrName = attr.substr(0, space);
        std::string attrType = attr.substr(space + 1);
        attributes.push_back({attrName, attrType});
    }

    std::string code = type + " " + name + " {\n";
    code += "// " + description + "\n";
    code += "public:\n";
    for (const auto& attr : attributes) {
        code += "    " + attr.second + " " + attr.first + ";\n";
    }

     // Add methods
    while (true) {
        std::string methodName;
        std::cout << "(" << name << ") Add method (name) or press Enter to finish: ";
        std::getline(std::cin, methodName);

        if (methodName.empty()) {
            break;
        }

        std::string methodCode = createFunctionPrompt(methodName, true);
        code += "\t";
        code += methodCode;
    }
    code += "};\n";

    return code;
}