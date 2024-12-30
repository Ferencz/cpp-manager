# cpp-manager

`cpp-manager` is a command-line tool for managing C++ projects. It automates project initialization, dependency management, compilation, and testing.

## Features
- **Project Initialization:** Create a new C++ project with a standard directory structure.
- **Dependency Management:** Add and manage dependencies using Conan or vcpkg.
- **Compilation:** Automate the build process using CMake.
- **Testing:** Run unit tests using Google Test or Catch2.
- **Module Management:** Create and delete headers and source files.

## Installation

### Prerequisites
- CMake (>= 3.10)
- C++ Compiler (e.g., GCC, Clang, MSVC)
- Conan or vcpkg (for dependency management)

### Build Instructions
1. Clone the repository:
   ```bash
   git clone https://github.com/Ferencz/cpp-manager.git
   cd cpp-manager
   ```

2. Build the project:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

3. Install the binary (optional):
   ```bash
   sudo cmake --install .
   ```

## Usage

### Initialize a Project
```bash
cpp-manager init MyProject
```

### Add a Dependency
```bash
cpp-manager add zlib/1.2.11
```

### Build the Project
```bash
cpp-manager build
```

### Run Tests
```bash
cpp-manager test
```

### Create a Header
```bash
cpp-manager create header MyClass
```

### Create a Module
```bash
cpp-manager create module MyClass --header
```

### Delete a Module
```bash
cpp-manager delete module MyClass
```

### List Modules
```bash
cpp-manager src --list
```

### Scaffold a Module
```bash
cpp-manager src
```

## Contributing
Contributions are welcome! Please follow these steps:
1. Fork the repository.
2. Create a new branch (`git checkout -b feature/YourFeature`).
3. Commit your changes (`git commit -m 'Add some feature'`).
4. Push to the branch (`git push origin feature/YourFeature`).
5. Open a pull request.

## Acknowledgments
- Optional [Conan](https://conan.io/) for dependency management.
- Optional [vcpkg](https://vcpkg.io/) for dependency management.
- [CMake](https://cmake.org/) for build automation.
