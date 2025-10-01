# AI Project Hints for AppleSAWS

This file contains project-specific information to help AI assistants work effectively with this codebase.

## Build System & Commands

### Primary Build System: CMake
- **Quick build**: `./build.sh` (preferred method)
- **Alternative**: `cmake --build build --parallel 16`
- **Clean build**: `rm -rf build && ./build.sh`
- **Build directory**: `build/`
- **Binary output**: `build/AppleSAWS` (in build directory)

### Important: Do NOT use `make` commands
- No Makefile in project root
- Always use CMake build commands

## Application Type & Testing

### GUI Application - NO Command Line Interface
- **Application type**: Qt6 GUI application
- **Cannot test with**: `--help`, `--version`, or any CLI flags
- **Test method**: Launch GUI (`build/AppleSAWS`) or run specific tests
- **Entry point**: `main.cpp` launches Qt GUI

## Development Standards & Preferences

### C++ Standard: C++20
- Use modern C++20 features extensively
- Prefer `[[nodiscard]]` attributes
- Use `constexpr` and `noexcept` where appropriate
- Explicit constructors for single-parameter constructors
- Use `enum class` instead of plain enums
- Modern member initialization and Rule of Five
- Use `overload` when appropriate
- Check for method hiding in subclasses

### Qt Version: Qt6.8.3
- **Signal/Slot connections**: Use modern function pointer syntax
  ```cpp
  // Preferred (modern):
  connect(button, &QPushButton::clicked, this, &MyClass::onButtonClicked);
  
  // Avoid (legacy):
  connect(button, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
  ```
- Use `unique_ptr` for UI objects where appropriate
- Remember that QObject classes cannot be contained in smart pointers, as they maintain their own lifetimes
- Proper Qt object lifetime management

### Code Organization Principles
- **Const correctness**: Extensive use of const methods and parameters
- **Type safety**: Strong typing, avoid implicit conversions
- **Performance**: Move semantics, efficient containers
- **Documentation**: Comprehensive Doxygen-style comments
- **File organization**: Keep domain-specific code in appropriate directories

### Include Order Preference
1. Own header file (for .cxx files)
2. Project headers (related functionality)
3. Project utility headers
4. Qt headers
5. Standard library headers

## Project Structure

### Key Directories
- `src/diskfiles/dos33/` - DOS 3.3 disk format handling
- `src/ui/viewers/` - File content viewer widgets
- `src/ui/widgets/` - Reusable UI components
- `src/ui/diskexplorer/` - Disk exploration interface
- `src/applesoftfile/` - Applesoft BASIC file handling
- `src/binaryfile/` - Binary file analysis and disassembly
- `src/util/` - Generic utilities (keep DOS-specific code out)

### Recent Structural Improvements
- TSPair structure moved from `src/util/Util.h` to `src/diskfiles/dos33/TSPair.h`
- Signal/slot connections modernized throughout codebase
- UI components fully modernized with C++20 patterns

### File Organization Rules
- Keep domain-specific structures in their appropriate directories
- Generic utilities in `src/util/`, specific functionality elsewhere
- Headers should include only what they need
- Implementation files should include their own header first

## Common Mistakes to Avoid

### Build System
- ❌ Don't try `make` commands (no Makefile)
- ❌ Don't assume GNU build tools
- ✅ Always use `./build.sh` or CMake commands

### Application Testing
- ❌ Don't try `build/AppleSAWS --help` (GUI app, no CLI)
- ❌ Don't expect command-line output for testing
- ✅ Build success indicates code correctness
- ✅ Can launch GUI for functional testing (`build/AppleSAWS`)

### Code Style
- ❌ Don't use old-style Qt SIGNAL/SLOT macros
- ❌ Don't put domain-specific code in generic util headers
- ✅ Use modern C++20 and Qt6 patterns consistently
- ✅ Maintain comprehensive documentation
- ✅ Implementation files use a .cxx extension
- ✅ Header files use a .h extension

## Development Context

### Project Purpose
AppleSAWS is a tool for analyzing Apple II disk images and files, providing viewers for various file formats including Applesoft BASIC, binary files, and disk structures.

### Recent Modernization Work
- Comprehensive C++20 modernization completed
- UI components fully updated with modern Qt6 patterns
- Signal/slot connections modernized throughout
- Code organization improvements (TSPair extraction)
- Extensive const correctness and type safety improvements

### Current Focus Areas
- Continued code organization and modernization
- Performance improvements
- Enhanced type safety
- Better separation of concerns

---

**Note**: This file should be referenced by AI assistants working on this project to avoid repeating common mistakes and to maintain consistency with established patterns and preferences.