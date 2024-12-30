# Dependency Management

`cpp-manager` supports dependency management using **Conan** or **vcpkg**.

## Using Conan
1. Install Conan:
   ```bash
   pip install conan
   ```

2. Add a dependency:
   ```bash
   cpp-manager add  zlib/1.2.11
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
---
