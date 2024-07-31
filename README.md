# GB Emulator

A GB emulator in C, using Dear_ImGUI and GLFW. Built with Make.

## Requirements

- GNU Make
- A C++11 compatible compiler
- GLFW 3.3 or higher
- Doxygen (optional, for generating documentation)

## Building

2. **Clean previous build artifacts**:

   ```sh
   make clean
   ```

3. **Build the project with Make**:

   ```sh
   make gui
   ```

4. **Run the executable**:
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

### Current progress:

Failing cpu_instrs 1, 2, 8, 11.
