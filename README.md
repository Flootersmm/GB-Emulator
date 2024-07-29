# GB Emulator

## Description

A GB emulator in C, using Dear_ImGUI and GLFW. Built with CMake.

## Requirements

- GNU Make
- A C++11 compatible compiler
- GLFW 3.3 or higher
- Doxygen (optional, for generating documentation)

## Building

1. **Clean previous build artifacts**:

```sh
make clean
```

2. **Build the project with Make**:

   ```sh
   make gui
   ```

3. **Run the executable**:
   ```sh
   ./gui path/to/rom
   ```

### Generating Documentation

If you have Doxygen installed, generate ./docs with:

```sh
make doxygen

```

### Generating .clangd

To generate a .clangd file for correct parsing of headers:

```sh
chmod +x generate_clangd.sh
./generate_clangd.sh
```
