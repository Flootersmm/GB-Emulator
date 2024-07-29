# GB Emulator

## Description

A GB emulator in C, using Dear_ImGUI and GLFW. Built with CMake.

## Requirements

- CMake 3.10 or higher
- A C++11 compatible compiler
- GLFW 3.3 or higher
- Doxygen (optional, for generating documentation)

## Building

1. **Create a build directory and navigate into it**:

   ```sh
   mkdir build
   cd build
   ```

2. **Configure the project with CMake**:

   ```sh
   cmake ..
   ```

3. **Build the project**:

   ```sh
   make
   ```

4. **Run the executable**:
   ```sh
   ./obj/gui path/to/rom
   ```

### Generating Documentation

If you have Doxygen installed, generate docs with:

```sh
make doc_doxygen

```

### Generating .clangd

To generate a .clangd file for correct parsing of headers:

```sh
chmod +x generate_clangd.sh
./generate_clangd.sh
```
