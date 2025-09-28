#!/bin/bash
# CMake build script for AppleSAWS

echo "=== AppleSAWS CMake Build Script ==="

# Clean previous builds
echo "Cleaning previous builds..."
rm -rf build build-release

# Configure and build Debug version
echo "Configuring Debug build..."
cmake --preset default
if [ $? -ne 0 ]; then
    echo "CMake configuration failed!"
    exit 1
fi

echo "Building Debug version..."
cmake --build build --parallel $(nproc)
if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

echo "=== Build completed successfully! ==="
echo "Debug executable: ./build/AppleSAWS"
echo ""
echo "To build Release version, run:"
echo "cmake --preset release && cmake --build build-release --parallel $(nproc)"
echo ""
echo "To run the application:"
echo "cd build && ./AppleSAWS"