# Getting Started

## Prerequisites
- CMake (>= 3.10)
- C++ Compiler (e.g., GCC, Clang, MSVC)
- Conan or vcpkg (for dependency management)

## Installation
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

## Basic Usage
Initialize a new project:
```bash
cpp-manager init MyProject
```