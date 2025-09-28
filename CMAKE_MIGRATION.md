# AppleSAWS: QMake to CMake Migration Guide

## Overview
This guide documents the conversion of AppleSAWS from QMake (.pro) to CMake build system while maintaining your existing directory structure and build environment.

## Migration Benefits
- **Modern Build System**: CMake is more widely supported and actively developed
- **Better IDE Integration**: Enhanced support in VS Code, CLion, Qt Creator, and other IDEs
- **Cross-Platform**: More robust cross-platform building
- **Dependency Management**: Better integration with package managers (vcpkg, Conan)
- **Modern C++ Support**: Better support for C++20 features and modules

## Files Added/Modified

### New Files Created:
- `CMakeLists.txt` - Main CMake configuration
- `CMakePresets.json` - Build presets for different configurations
- `build.sh` - Convenience build script
- `CMAKE_MIGRATION.md` - This guide

### Modified Files:
- `.gitignore` - Added CMake build directories and files

### Preserved Files:
- `AppleSAWS.pro` - Kept for reference/backup (can be removed later)
- All source files and directory structure - unchanged
- `.vscode/` configuration - unchanged

## Build Comparison

### Old QMake Workflow:
```bash
qmake6
make -j$(nproc)
./AppleSAWS
```

### New CMake Workflow:
```bash
# Using presets (recommended)
cmake --preset default
cmake --build build --parallel $(nproc)
./build/AppleSAWS

# Or using the convenience script
./build.sh
```

### Alternative CMake Workflow:
```bash
# Manual configuration
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
./AppleSAWS
```

## Configuration Equivalents

| QMake (.pro) | CMake (CMakeLists.txt) | Description |
|--------------|------------------------|-------------|
| `CONFIG += c++20 debug` | `set(CMAKE_CXX_STANDARD 20)` + Debug preset | C++20 standard |
| `QT += core gui widgets printsupport` | `find_package(Qt6 REQUIRED COMPONENTS ...)` | Qt modules |
| `INCLUDEPATH += src/...` | `include_directories(...)` | Include paths |
| `SOURCES += src/...` | `set(SOURCES ...)` | Source files |
| `HEADERS += src/...` | `set(HEADERS ...)` | Header files |
| `FORMS += src/...` | `set(UI_FILES ...)` | UI forms |
| `RESOURCES += src/...` | `set(QRC_FILES ...)` | Resource files |
| `MOC_DIR = ./.build` | `set(CMAKE_AUTOMOC_OUTPUT_DIR ...)` | MOC output |
| `DEFINES += WS_VIDEO` | `add_definitions(-DWS_VIDEO)` | Preprocessor defines |

## Build Directories

### QMake Structure:
```
AppleSAWS/
├── .build/          # Generated MOC/UI files
├── AppleSAWS        # Executable in root
├── Makefile         # Generated makefile
└── qrc_resources.cpp # Generated resource file
```

### CMake Structure:
```
AppleSAWS/
├── build/           # Debug build directory
│   ├── AppleSAWS    # Debug executable
│   └── .build/      # Generated MOC/UI files
├── build-release/   # Release build directory (when built)
│   └── AppleSAWS    # Release executable
└── CMakeCache.txt   # CMake configuration cache
```

## VS Code Integration

Your existing `.vscode/` configuration should work with CMake. The CMake Tools extension will automatically detect the CMakeLists.txt and provide:
- Automatic configuration detection
- Build target selection
- Debug configuration
- IntelliSense integration

## Build Presets

The `CMakePresets.json` file provides two presets:

### Debug Preset (`default`):
- Build type: Debug
- Output: `build/AppleSAWS`
- Generates compile_commands.json for IntelliSense

### Release Preset:
- Build type: Release  
- Output: `build-release/AppleSAWS`
- Optimized executable

## Testing the Migration

1. **Backup current setup** (optional):
   ```bash
   git add -A && git commit -m "Backup before CMake migration"
   ```

2. **Test CMake build**:
   ```bash
   ./build.sh
   ```

3. **Compare executables**:
   ```bash
   # Build with QMake
   make clean && qmake6 && make -j$(nproc)
   ls -la AppleSAWS
   
   # Build with CMake  
   ./build.sh
   ls -la build/AppleSAWS
   ```

4. **Test functionality**:
   ```bash
   # Test QMake version
   ./AppleSAWS
   
   # Test CMake version
   ./build/AppleSAWS
   ```

## Troubleshooting

### Common Issues:

1. **Qt6 not found**:
   ```bash
   sudo apt update && sudo apt install qt6-base-dev qt6-tools-dev
   ```

2. **Missing CMake version**:
   ```bash
   cmake --version  # Should be 3.20+
   sudo apt install cmake  # or upgrade if needed
   ```

3. **Build fails with missing files**:
   - Check that all source files in `SOURCES` list exist
   - Verify file extensions match actual files (.cxx vs .cpp)

4. **MOC/UI generation issues**:
   - Ensure `CMAKE_AUTOMOC=ON` and `CMAKE_AUTOUIC=ON`
   - Check that Qt6 components are properly linked

## Rollback Plan

If you need to revert to QMake:
1. The original `AppleSAWS.pro` file is preserved
2. Simply use: `qmake6 && make`
3. Remove CMake files: `rm -rf build build-release CMakeCache.txt`

## Maintenance

### Adding New Files:
1. **Source files**: Add to `SOURCES` list in CMakeLists.txt
2. **Headers**: Add to `HEADERS` list (optional, for IDE organization)
3. **UI files**: Add to `UI_FILES` list
4. **Resources**: Add to `QRC_FILES` list

### Configuration Changes:
- Edit `CMakeLists.txt` for build configuration changes
- Edit `CMakePresets.json` for preset modifications
- Use `cmake --build build --clean-first` for clean rebuilds

## Performance Notes

- CMake builds should be comparable in speed to QMake
- Parallel builds: `cmake --build build --parallel $(nproc)`
- Incremental builds are typically faster with CMake
- First-time configuration may be slower due to dependency detection

## Next Steps

Once comfortable with CMake:
1. Remove `AppleSAWS.pro` and `Makefile`
2. Consider modern CMake features (target-based configuration)
3. Explore CMake package management integration
4. Set up CI/CD with CMake support