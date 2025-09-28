
QT += core gui printsupport
CONFIG += c++20 debug

MOC_DIR = ./.build
UI_DIR = ./.build
OBJECTS_DIR = ./.build

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AppleSAWS
TEMPLATE = app

INCLUDEPATH += src/
INCLUDEPATH += src/relocatablefile
INCLUDEPATH += src/diskfiles
INCLUDEPATH += src/diskfiles/dos33
INCLUDEPATH += src/util
INCLUDEPATH += src/applesoftfile
INCLUDEPATH += src/intbasic
INCLUDEPATH += src/binaryfile
INCLUDEPATH += src/textfile
INCLUDEPATH += src/ui/viewers
INCLUDEPATH += src/imported
INCLUDEPATH += src/internals
INCLUDEPATH += src/ui/diskexplorer/
INCLUDEPATH += src/ui/widgets/
INCLUDEPATH += src/ui
INCLUDEPATH += src/memory
INCLUDEPATH += src/memory/roles


DEFINES += WS_VIDEO

SOURCES += \
    src/intbasic/IntBasicFile.cxx \
    src/main.cxx \
    src/diskfiles/dos33/DiskFile.cxx \
    src/diskfiles/dos33/Sector.cxx \
    src/diskfiles/dos33/Vtoc.cxx \
    src/diskfiles/dos33/CatalogSector.cxx \
    src/diskfiles/dos33/TrackSectorList.cxx \
    src/diskfiles/dos33/FileDescriptiveEntry.cxx \
    src/diskfiles/dos33/GenericFile.cxx \
    src/memory/AttributedMemory.cxx \
    src/memory/MemoryCell.cxx \
    src/memory/MemRole.cxx \
    src/memory/roles/RoleAsmOpcode.cxx \
    src/memory/roles/RoleAsmOperand.cxx \
    src/ui/widgets/startupdialog.cxx \
    src/ui/viewers/intbasicfileviewer.cxx \
    src/ui/widgets/notesdialog.cxx \
    src/util/applestring.cxx \
    src/applesoftfile/ApplesoftFile.cxx \
    src/applesoftfile/ApplesoftToken.cxx \
    src/applesoftfile/ApplesoftFormatter.cxx \
    src/binaryfile/Disassembler.cxx \
    src/binaryfile/BinaryFile.cxx \
    src/textfile/textfile.cxx \
    src/ui/widgets/catalogwidget.cxx \
    src/ui/widgets/hiresviewwidget.cxx \
    src/ui/viewers/applesoftfileviewer.cxx \
    src/ui/viewers/disassemblerviewer.cpp \
    src/ui/viewers/hexdumpviewer.cpp \
    src/ui/viewers/texthexdumpviewer.cpp \
    src/ui/viewers/mazeviewer.cpp \
    src/ui/viewers/charsetviewer.cpp \
    src/relocatablefile/RelocatableFile.cxx \
    src/binaryfile/BinaryFileMetadata.cxx \
    src/util/charset.cpp \
    src/ui/widgets/characterwidget.cpp \
    src/ui/viewers/ApplesoftFileDetailViewer.cxx \
    src/ui/widgets/hexconverter.cpp \
    src/ui/viewers/viewerbase.cpp \
    src/ui/widgets/CharacterSetExplorer.cpp \
    src/ui/widgets/HiresScreenWidget.cpp \
    src/ui/widgets/DisassemblerMetadataDialog.cpp \
    src/binaryfile/EntryPointModel.cxx \
    src/ui/widgets/LocationInfoDialog.cpp \
    src/binaryfile/EntryPoints.cxx \
    src/binaryfile/AssemblerSymbols.cxx \
    src/binaryfile/AssemblerSymbolModel.cxx \
    src/ui/diskexplorer/DiskExplorer.cpp \
    src/ui/diskexplorer/DiskExplorerMapWidget.cpp \
    src/applesoftfile/ApplesoftRetokenizer.cxx \
    src/internals/JumpLineManager.cxx \
    src/ui/widgets/FlowLineTextBrowser.cpp \
    src/util/opcodes.cpp


HEADERS += \
    src/diskfiles/dos33/Diskfile.h \
    src/diskfiles/dos33/Sector.h \
    src/diskfiles/dos33/Vtoc.h \
    src/diskfiles/dos33/CatalogSector.h \
    src/diskfiles/dos33/TrackSectorList.h \
    src/diskfiles/dos33/FileDescriptiveEntry.h \
    src/diskfiles/dos33/GenericFile.h \
    src/intbasic/IntBasicFile.h \
    src/memory/AttributedMemory.h \
    src/memory/MemoryCell.h \
    src/memory/MemRole.h \
    src/memory/roles/RoleAsmOpcode.h \
    src/memory/roles/RoleAsmOperand.h \
    src/ui/widgets/startupdialog.h \
    src/ui/viewers/intbasicfileviewer.h \
    src/ui/widgets/notesdialog.h \
    src/util/opcodes.h \
    src/util/util.h \
    src/util/applestring.h \
    src/applesoftfile/ApplesoftFile.h \
    src/applesoftfile/ApplesoftToken.h \
    src/binaryfile/Disassembler.h \
    src/binaryfile/BinaryFile.h \
    src/textfile/textfile.h \
    src/ui/widgets/catalogwidget.h \
    src/ui/widgets/hiresviewwidget.h \
    src/ui/viewers/applesoftfileviewer.h \
    src/applesoftfile/ApplesoftFormatter.h \
    src/applesoftfile/ApplesoftLine.h  \
    src/ui/viewers/disassemblerviewer.h \
    src/ui/viewers/hexdumpviewer.h \
    src/ui/viewers/texthexdumpviewer.h \
    src/relocatablefile/RelocatableFile.h \
    src/ui/viewers/mazeviewer.h \
    src/binaryfile/BinaryFileMetadata.h \
    src/ui/widgets/characterwidget.h \
    src/util/charset.h \
    src/ui/viewers/charsetviewer.h \
    src/ui/viewers/ApplesoftFileDetailViewer.h \
    src/ui/widgets/hexconverter.h \
    src/ui/widgets/hrcgcontrolsinfo.h \
    src/ui/viewers/viewerbase.h \
    src/ui/viewers/fileviewerinterface.h \
    src/ui/widgets/CharacterSetExplorer.h \
    src/ui/widgets/HiresScreenWidget.h \
    src/ui/widgets/DisassemblerMetadataDialog.h \
    src/binaryfile/EntryPointModel.h \
    src/ui/widgets/LocationInfoDialog.h \
    src/binaryfile/EntryPoints.h \
    src/binaryfile/AssemblerSymbols.h \
    src/binaryfile/AssemblerSymbolModel.h \
    src/binaryfile/MemoryUsageMap.h \
    src/ui/diskexplorer/DiskExplorer.h \
    src/ui/diskexplorer/DiskExplorerMapWidget.h \
    src/applesoftfile/ApplesoftRetokenizer.h \
    src/util/AppleColors.h \
    src/internals/JumpLineManager.h \
    src/ui/widgets/FlowLineTextBrowser.h \
    src/ui/widgets/asciiinfodialog.h

FORMS += \
    src/ui/widgets/catalogwidget.ui \
    src/ui/widgets/startupdialog.ui \
    src/ui/viewers/applesoftfileviewer.ui \
    src/ui/viewers/disassemblerviewer.ui \
    src/ui/viewers/hexdumpviewer.ui \
    src/ui/viewers/intbasicfileviewer.ui \
    src/ui/viewers/texthexdumpviewer.ui \
    src/ui/viewers/ApplesoftFileDetailViewer.ui \
    src/ui/widgets/hexconverter.ui \
    src/ui/widgets/hrcgcontrolsinfo.ui \
    src/ui/viewers/viewerbase.ui \
    src/ui/widgets/CharacterSetExplorer.ui \
    src/ui/widgets/DisassemblerMetadataDialog.ui \
    src/ui/widgets/LocationInfoDialog.ui \
    src/ui/widgets/asciiinfodialog.ui \
    src/ui/widgets/notesdialog.ui

RESOURCES += \
    src/resource/resources.qrc
