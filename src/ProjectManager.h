// include/ProjectManager.h
#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <string>
#include <vector>

class ProjectManager {
public:
    ProjectManager(const std::string& projectName);

    void initializeProject();
    void addDependency(const std::string& dependency);
    void buildProject();
    void runTests();

    void createHeader(const std::string& headerName);
    void createModule(const std::string& moduleName, bool createHeader = false);
    void deleteModule(const std::string& moduleName);
    void srcCommand(const std::string& subCommand = "");

private:
    std::string projectName;
    std::vector<std::string> dependencies;

    void createDirectory(const std::string& path);
    void createFile(const std::string& path, const std::string& content);
    void executeCommand(const std::string& command);
    void executeCommandWithOutput(const std::string& command); // New method
    void deleteFile(const std::string& path);

    void setupPythonVirtualEnv();
    bool promptToInstallPackageManager();

    void listSourceFiles();
    void editSourceFile(const std::string& fileName);
    std::string createClassOrStructPrompt(const std::string& type, const std::string& name);
    std::string createFunctionPrompt(const std::string &name, bool nested = false);
};

#endif // PROJECTMANAGER_H