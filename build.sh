#!/bin/bash
# CMake build script for AppleSAWS

show_usage() {
    echo "Usage: $0 [target]"
    echo ""
    echo "Targets:"
    echo "  debug     - Configure and build Debug version (default)"
    echo "  release   - Configure and build Release version"
    echo "  both      - Build both Debug and Release versions"
    echo "  clean     - Clean build directories"
    echo "  help      - Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0           # Build debug version"
    echo "  $0 debug     # Build debug version"
    echo "  $0 release   # Build release version"
    echo "  $0 both      # Build both versions"
    echo "  $0 clean     # Clean build directories"
}

clean_build() {
    echo "=== Cleaning build directories ==="
    if [ -d "build" ] || [ -d "build-release" ]; then
        echo "Removing build directories..."
        rm -rf build build-release
        echo "Build directories cleaned."
    else
        echo "No build directories to clean."
    fi
}

build_debug() {
    echo "=== Building Debug Version ==="
    echo "Configuring Debug build..."
    cmake --preset default
    if [ $? -ne 0 ]; then
        echo "CMake Debug configuration failed!"
        return 1
    fi

    echo "Building Debug version..."
    cmake --build build --parallel $(nproc)
    if [ $? -ne 0 ]; then
        echo "Debug build failed!"
        return 1
    fi

    echo "Debug build completed successfully!"
    echo "Executable: ./build/AppleSAWS"
    return 0
}

build_release() {
    echo "=== Building Release Version ==="
    echo "Configuring Release build..."
    cmake --preset release
    if [ $? -ne 0 ]; then
        echo "CMake Release configuration failed!"
        return 1
    fi

    echo "Building Release version..."
    cmake --build build-release --parallel $(nproc)
    if [ $? -ne 0 ]; then
        echo "Release build failed!"
        return 1
    fi

    echo "Release build completed successfully!"
    echo "Executable: ./build-release/AppleSAWS"
    return 0
}

# Main script logic
TARGET=${1:-debug}

case "$TARGET" in
    debug)
        build_debug
        EXIT_CODE=$?
        ;;
    release)
        build_release
        EXIT_CODE=$?
        ;;
    both)
        echo "=== Building Both Debug and Release Versions ==="
        build_debug
        DEBUG_RESULT=$?
        
        build_release
        RELEASE_RESULT=$?
        
        if [ $DEBUG_RESULT -eq 0 ] && [ $RELEASE_RESULT -eq 0 ]; then
            echo "=== Both builds completed successfully! ==="
            echo "Debug executable: ./build/AppleSAWS"
            echo "Release executable: ./build-release/AppleSAWS"
            EXIT_CODE=0
        else
            echo "=== One or more builds failed ==="
            EXIT_CODE=1
        fi
        ;;
    clean)
        clean_build
        EXIT_CODE=0
        ;;
    help|--help|-h)
        show_usage
        EXIT_CODE=0
        ;;
    *)
        echo "Error: Unknown target '$TARGET'"
        echo ""
        show_usage
        EXIT_CODE=1
        ;;
esac

if [ $EXIT_CODE -eq 0 ] && [ "$TARGET" != "clean" ] && [ "$TARGET" != "help" ] && [ "$TARGET" != "--help" ] && [ "$TARGET" != "-h" ]; then
    echo ""
    echo "To run the application:"
    if [ "$TARGET" = "debug" ] || [ "$TARGET" = "both" ]; then
        echo "  cd build && ./AppleSAWS"
    fi
    if [ "$TARGET" = "release" ] || [ "$TARGET" = "both" ]; then
        echo "  cd build-release && ./AppleSAWS"
    fi
fi

exit $EXIT_CODE