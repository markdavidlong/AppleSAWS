cmake_minimum_required(VERSION 3.20)
project(AppleSAWS VERSION 1.0.0 LANGUAGES CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Enable debug information and disable optimizations for Debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
endif()

# Find required Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui PrintSupport)

# Automatically handle Qt's MOC, UIC, and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Set custom build directory for generated files
set(CMAKE_AUTOMOC_OUTPUT_DIR ${CMAKE_BINARY_DIR}/.build)
set(CMAKE_AUTOUIC_OUTPUT_DIR ${CMAKE_BINARY_DIR}/.build)
set(CMAKE_AUTORCC_OUTPUT_DIR ${CMAKE_BINARY_DIR}/.build)

# Add compile definitions
add_definitions(-DWS_VIDEO)

# Include directories (equivalent to INCLUDEPATH in .pro file)
include_directories(
    src/
    src/relocatablefile
    src/diskfiles
    src/diskfiles/dos33
    src/util
    src/applesoftfile
    src/intbasic
    src/binaryfile
    src/textfile
    src/ui/viewers
    src/imported
    src/internals
    src/ui/diskexplorer
    src/ui/widgets
    src/ui
    src/memory
    src/memory/roles
)

# Source files
set(SOURCES
    src/intbasic/IntBasicFile.cxx
    src/main.cxx
    src/diskfiles/dos33/DiskFile.cxx
    src/diskfiles/dos33/Sector.cxx
    src/diskfiles/dos33/Vtoc.cxx
    src/diskfiles/dos33/CatalogSector.cxx
    src/diskfiles/dos33/TrackSectorList.cxx
    src/diskfiles/dos33/FileDescriptiveEntry.cxx
    src/diskfiles/dos33/GenericFile.cxx
    src/memory/AttributedMemory.cxx
    src/memory/MemoryCell.cxx
    src/memory/MemRole.cxx
    src/memory/roles/RoleAsmOpcode.cxx
    src/memory/roles/RoleAsmOperand.cxx
    src/ui/widgets/StartupDialog.cxx
    src/ui/viewers/IntBasicFileViewer.cxx
    src/ui/widgets/NotesDialog.cxx
    src/util/AppleString.cxx
    src/applesoftfile/ApplesoftFile.cxx
    src/applesoftfile/ApplesoftToken.cxx
    src/applesoftfile/ApplesoftFormatter.cxx
    src/binaryfile/Disassembler.cxx
    src/binaryfile/DisassembledItem.cxx
    src/binaryfile/MemoryUsageMap.cxx
    src/binaryfile/BinaryFile.cxx
    src/textfile/TextFile.cxx
    src/ui/widgets/CatalogWidget.cxx
    src/ui/viewers/HiresViewer.cxx
    src/ui/viewers/ApplesoftFileViewer.cxx
    src/ui/viewers/DisassemblerViewer.cxx
    src/ui/viewers/HexDumpViewer.cxx
    src/ui/viewers/TextHexDumpViewer.cxx
    src/ui/viewers/MazeViewer.cxx
    src/ui/viewers/CharSetViewer.cxx
    src/relocatablefile/RelocatableFile.cxx
    src/binaryfile/BinaryFileMetadata.cxx
    src/util/CharSet.cxx
    src/ui/widgets/CharacterWidget.cxx
    src/ui/viewers/ApplesoftFileDetailViewer.cxx
    src/ui/widgets/HexConverter.cxx
    src/ui/viewers/ViewerBase.cxx
    src/ui/widgets/CharacterSetExplorer.cxx
    src/ui/widgets/HiresScreenWidget.cxx
    src/ui/widgets/DisassemblerMetadataDialog.cxx
    src/binaryfile/EntryPointModel.cxx
    src/ui/widgets/LocationInfoDialog.cxx
    src/binaryfile/EntryPoints.cxx
    src/binaryfile/AssemblerSymbols.cxx
    src/binaryfile/AssemblerSymbolModel.cxx
    src/ui/diskexplorer/DiskExplorer.cxx
    src/ui/diskexplorer/DiskExplorerMapWidget.cxx
    src/ui/diskexplorer/DEButton.cxx
    src/applesoftfile/ApplesoftRetokenizer.cxx
    src/binaryfile/JumpLine.cxx
    src/binaryfile/JumpLines.cxx
    src/binaryfile/JumpLineManager.cxx
    src/ui/widgets/FlowLineTextBrowser.cxx
    src/util/OpCodes.cxx
)

# Header files (for IDE organization, not strictly necessary for CMake)
set(HEADERS
    src/diskfiles/dos33/DiskFile.h
    src/diskfiles/dos33/Sector.h
    src/diskfiles/dos33/Vtoc.h
    src/diskfiles/dos33/CatalogSector.h
    src/diskfiles/dos33/TrackSectorList.h
    src/diskfiles/dos33/FileDescriptiveEntry.h
    src/diskfiles/dos33/GenericFile.h
    src/intbasic/IntBasicFile.h
    src/memory/AttributedMemory.h
    src/memory/MemoryCell.h
    src/memory/MemRole.h
    src/memory/roles/RoleAsmOpcode.h
    src/memory/roles/RoleAsmOperand.h
    src/ui/widgets/StartupDialog.h
    src/ui/viewers/IntBasicFileViewer.h
    src/ui/widgets/NotesDialog.h
    src/util/OpCodes.h
    src/util/Util.h
    src/util/AppleString.h
    src/applesoftfile/ApplesoftFile.h
    src/applesoftfile/ApplesoftToken.h
    src/binaryfile/Disassembler.h
    src/binaryfile/DisassembledItem.h
    src/binaryfile/MemoryUsageMap.h
    src/binaryfile/BinaryFile.h
    src/textfile/TextFile.h
    src/ui/widgets/CatalogWidget.h
    src/ui/viewers/HiresViewer.h
    src/ui/viewers/ApplesoftFileViewer.h
    src/applesoftfile/ApplesoftFormatter.h
    src/applesoftfile/ApplesoftLine.h
    src/ui/viewers/DisassemblerViewer.h
    src/ui/viewers/HexDumpViewer.h
    src/ui/viewers/TextHexDumpViewer.h
    src/relocatablefile/RelocatableFile.h
    src/ui/viewers/MazeViewer.h
    src/binaryfile/BinaryFileMetadata.h
    src/ui/widgets/CharacterWidget.h
    src/util/CharSet.h
    src/ui/viewers/CharSetViewer.h
    src/ui/viewers/ApplesoftFileDetailViewer.h
    src/ui/widgets/HexConverter.h
    src/ui/widgets/HRCGControlsInfo.h
    src/ui/viewers/ViewerBase.h
    src/ui/viewers/FileViewerInterface.h
    src/ui/widgets/CharacterSetExplorer.h
    src/ui/widgets/HiresScreenWidget.h
    src/ui/widgets/DisassemblerMetadataDialog.h
    src/binaryfile/EntryPointModel.h
    src/ui/widgets/LocationInfoDialog.h
    src/binaryfile/EntryPoints.h
    src/binaryfile/AssemblerSymbols.h
    src/binaryfile/AssemblerSymbolModel.h
    src/binaryfile/MemoryUsageMap.h
    src/ui/diskexplorer/DiskExplorer.h
    src/ui/diskexplorer/DiskExplorerMapWidget.h
    src/ui/diskexplorer/DEButton.h
    src/ui/diskexplorer/DiskExplorerTypes.h
    src/applesoftfile/ApplesoftRetokenizer.h
    src/util/AppleColors.h
    src/binaryfile/JumpLine.h
    src/binaryfile/JumpLines.h
    src/binaryfile/JumpLineManager.h
    src/ui/widgets/FlowLineTextBrowser.h
    src/ui/widgets/AsciiInfoDialog.h
)

# UI files (Qt Designer forms)
set(UI_FILES
    src/ui/widgets/CatalogWidget.ui
    src/ui/widgets/StartupDialog.ui
    src/ui/viewers/ApplesoftFileViewer.ui
    src/ui/viewers/DisassemblerViewer.ui
    src/ui/viewers/HexDumpViewer.ui
    src/ui/viewers/IntBasicFileViewer.ui
    src/ui/viewers/TextHexDumpViewer.ui
    src/ui/viewers/ApplesoftFileDetailViewer.ui
    src/ui/widgets/HexConverter.ui
    src/ui/widgets/HRCGControlsInfo.ui
    src/ui/viewers/ViewerBase.ui
    src/ui/widgets/CharacterSetExplorer.ui
    src/ui/widgets/DisassemblerMetadataDialog.ui
    src/ui/widgets/LocationInfoDialog.ui
    src/ui/widgets/AsciiInfoDialog.ui
    src/ui/widgets/NotesDialog.ui
)

# Resource files
set(QRC_FILES
    src/resource/resources.qrc
)

# Create the executable
add_executable(AppleSAWS
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
    ${QRC_FILES}
)

# Link Qt libraries
target_link_libraries(AppleSAWS
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::PrintSupport
)

# Set output directory to match your current setup
set_target_properties(AppleSAWS PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Optional: Set properties for different build types
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(AppleSAWS PRIVATE DEBUG_BUILD)
endif()

# Optional: Install target (customize as needed)
install(TARGETS AppleSAWS
    RUNTIME DESTINATION bin
)

# Optional: Create a custom target for cleaning generated files
add_custom_target(clean-cmake-files
    COMMAND ${CMAKE_COMMAND} -P clean-cmake-files.cmake
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Cleaning CMake generated files"
)

# Optional: Print configuration info
message(STATUS "AppleSAWS Configuration:")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Qt6 version: ${Qt6_VERSION}")