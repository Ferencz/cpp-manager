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
```

---

### **2.2 `commands.md`**
```markdown
# Commands

## Initialize a Project
```bash
cpp-manager init <project-name>
```

## Add a Dependency
```bash
cpp-manager add dependency <dependency-name>
```

## Build the Project
```bash
cpp-manager build
```

## Run Tests
```bash
cpp-manager test
```

## Create a Header
```bash
cpp-manager create header <header-name>
```

## Create a Module
```bash
cpp-manager create module <module-name> [--header]
```

## Delete a Module
```bash
cpp-manager delete module <module-name>
```
```

---

### **2.3 `dependency-management.md`**
```markdown
# Dependency Management

`cpp-manager` supports dependency management using **Conan** or **vcpkg**.

## Using Conan
1. Install Conan:
   ```bash
   pip install conan
   ```

2. Add a dependency:
   ```bash
   cpp-manager add dependency zlib/1.2.11
   ```

## Using vcpkg
1. Install vcpkg:
   ```bash
   git clone https://github.com/microsoft/vcpkg.git
   ./vcpkg/bootstrap-vcpkg.sh
   ```

2. Add a dependency:
   ```bash
   cpp-manager add dependency zlib
   ```
```

---

### **2.4 `faq.md`**
```markdown
# FAQ

## How do I add a dependency?
Use the `add dependency` command:
```bash
cpp-manager add dependency zlib
```

## How do I create a module with a header?
Use the `create module` command with the `--header` flag:
```bash
cpp-manager create module MyClass --header
```

## How do I delete a module?
Use the `delete module` command:
```bash
cpp-manager delete module MyClass
```
```

---

### **2.5 `contributing.md`**
```markdown
# Contributing

We welcome contributions! Here’s how to get started:

1. Fork the repository.
2. Create a new branch (`git checkout -b feature/YourFeature`).
3. Commit your changes (`git commit -m 'Add some feature'`).
4. Push to the branch (`git push origin feature/YourFeature`).
5. Open a pull request.

## Code Style
- Follow the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).
- Use meaningful variable and function names.

## Testing
- Write unit tests for new features.
- Ensure all tests pass before submitting a pull request.
```

---

## **3. License**
Include a `LICENSE` file in the root of your project. For example, if you’re using the MIT License:

```text
MIT License

Copyright (c) [Year] [Your Name]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

## **4. Final Project Structure**
```
cpp-manager/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── ProjectManager.cpp
│   └── ProjectManager.h
├── include/
│   └── ProjectManager.h
├── test/
│   └── ProjectManagerTest.cpp
├── docs/
│   ├── getting-started.md
│   ├── commands.md
│   ├── dependency-management.md
│   ├── faq.md
│   └── contributing.md
├── README.md
└── LICENSE
```

---

This documentation will make your project easy to understand and use. Let me know if you need further assistance! 🚀