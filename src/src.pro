
QT += core gui printsupport
CONFIG += c++11
CONFIG += debug_and_release

MOC_DIR = ./.build
UI_DIR = ./.build
OBJECTS_DIR = ./.build


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AppleSAWS
TEMPLATE = app

INCLUDEPATH += ./
INCLUDEPATH += ./relocatablefile
INCLUDEPATH += ./diskfiles
INCLUDEPATH += ./diskfiles/dos33
INCLUDEPATH += ./util
INCLUDEPATH += ./applesoftfile
INCLUDEPATH += ./intbasic
INCLUDEPATH += ./binaryfile
INCLUDEPATH += ./textfile
INCLUDEPATH += ./ui/viewers
INCLUDEPATH += ./imported
INCLUDEPATH += ./internals
INCLUDEPATH += ./ui/diskexplorer/
INCLUDEPATH += ./ui/widgets/
INCLUDEPATH += ./ui
INCLUDEPATH += ./memory
INCLUDEPATH += ./memory/roles
INCLUDEPATH += ./sequence
INCLUDEPATH += ./ui/central
INCLUDEPATH += ./diskfiles/diskstore

INCLUDEPATH += ../ads/src/

INCLUDEPATH += ../cpress/ciderpress/diskimg/


DEFINES += WS_VIDEO

SOURCES += \
    ./diskfiles/dos33/dos33disktreeview.cpp \
    ./intbasic/IntBasicFile.cxx \
    ./main.cpp \
    ./diskfiles/dos33/dos33diskimage.cxx \
    ./diskfiles/dos33/sector.cxx \
    ./diskfiles/dos33/vtoc.cxx \
    ./diskfiles/dos33/catalogsector.cxx \
    ./diskfiles/dos33/tracksectorlist.cxx \
    ./diskfiles/dos33/filedescriptiveentry.cxx \
    ./diskfiles/dos33/genericfile.cxx \
    ./diskfiles/tspair.cpp \
    ./diskfiles/dos33/dos33imagemodel.cpp \
    ./diskfiles/dos33/dos33treeitem.cpp \
    ./memory/attributedmemory.cpp \
    ./memory/memorycell.cpp \
    ./memory/memrole.cpp \
    ./memory/roles/role_asm_opcode.cpp \
    ./memory/roles/role_asm_operand.cpp \
    ./sequence/sequenceevent.cpp \
    ./sequence/sequenceeventlist.cpp \
    ./sequence/sequenceeventlistwidget.cpp \
    ./sequence/sequenceoutputview.cpp \
    ./sequence/sequencetool.cpp \
    ./sequence/sequencetoolbox.cpp \
    ./sequence/sequenceviewer.cpp \
    ./sequence/textblockuserdata.cpp \
    ./ui/central/centralappwindow.cpp \
    ./ui/central/mainapptoolbar.cpp \
    ./ui/diskexplorer/catalogsectorview.cpp \
    ./ui/diskexplorer/tslistview.cpp \
    ./ui/diskexplorer/viewwidgetstack.cpp \
    ./ui/diskexplorer/vtocview.cpp \
    ./ui/startupdialog.cpp \
    ./ui/viewers/intbasicfileviewer.cxx \
    ./ui/widgets/notesdialog.cpp \
    ./util/applestring.cxx \
    ./applesoftfile/applesoftfile.cxx \
    ./applesoftfile/applesofttoken.cxx \
    ./applesoftfile/applesoftformatter.cxx \
    ./binaryfile/disassembler.cxx \
    ./binaryfile/binaryfile.cxx \
    ./textfile/textfile.cxx \
    ./ui/catalogwidget.cxx \
    ./ui/viewers/hiresviewwidget.cxx \
    ./ui/viewers/applesoftfileviewer.cxx \
    ./ui/viewers/disassemblerviewer.cpp \
    ./ui/viewers/hexdumpviewer.cpp \
    ./ui/viewers/texthexdumpviewer.cpp \
    ./ui/viewers/mazeviewer.cpp \
    ./ui/viewers/charsetviewer.cpp \
    ./relocatablefile/relocatablefile.cxx \
    ./binaryfile/binaryfilemetadata.cpp \
    ./util/charset.cpp \
    ./ui/widgets/characterwidget.cpp \
    ./ui/viewers/applesoftfiledetailviewer.cpp \
    ./ui/widgets/hexconverter.cpp \
    ./ui/viewers/viewerbase.cpp \
    ./ui/widgets/CharacterSetExplorer.cpp \
    ./ui/widgets/HiresScreenWidget.cpp \
    ./ui/widgets/DisassemblerMetadataDialog.cpp \
    ./binaryfile/EntryPointModel.cpp \
    ./ui/widgets/LocationInfoDialog.cpp \
    ./binaryfile/EntryPoints.cpp \
    ./binaryfile/AssemblerSymbols.cpp \
    ./binaryfile/AssemblerSymbolModel.cpp \
    ./ui/diskexplorer/DiskExplorer.cpp \
    ./ui/diskexplorer/DiskExplorerMapWidget.cpp \
    ./applesoftfile/ApplesoftRetokenizer.cpp \
    ./internals/JumpLineManager.cpp \
    ./ui/widgets/FlowLineTextBrowser.cpp \
    ./util/opcodes.cpp \
    diskfiles/cpressdiskfs.cpp \
    diskfiles/cpressdiskimage.cpp \
    diskfiles/cpressfile.cpp \
    diskfiles/diskstore/asdiskdata.cpp \
    diskfiles/diskstore/asdiskimporter.cpp \
    diskfiles/diskstore/asdiskstore.cpp


HEADERS += \
    ./diskfiles/dos33/dos33diskimage.h \
    ./diskfiles/dos33/dos33disktreeview.h \
    ./diskfiles/dos33/sector.h \
    ./diskfiles/dos33/vtoc.h \
    ./diskfiles/dos33/catalogsector.h \
    ./diskfiles/dos33/tracksectorlist.h \
    ./diskfiles/dos33/filedescriptiveentry.h \
    ./diskfiles/dos33/genericfile.h \
    ./diskfiles/tspair.h \
    ./diskfiles/dos33/dos33imagemodel.h\
    ./diskfiles/dos33/dos33treeitem.h\
    ./intbasic/IntBasicFile.h \
    ./memory/attributedmemory.h \
    ./memory/memorycell.h \
    ./memory/memrole.h \
    ./memory/roles/role_asm_opcode.h \
    ./memory/roles/role_asm_operand.h \
    ./sequence/sequenceevent.h \
    ./sequence/sequenceeventlist.h \
    ./sequence/sequenceeventlistwidget.h \
    ./sequence/sequenceoutputview.h \
    ./sequence/sequencetool.h \
    ./sequence/sequencetoolbox.h \
    ./sequence/sequenceviewer.h \
    ./sequence/textblockuserdata.h \
    ./ui/central/centralappwindow.h \
    ./ui/central/mainapptoolbar.h \
    ./ui/diskexplorer/catalogsectorview.h \
    ./ui/diskexplorer/tslistview.h \
    ./ui/diskexplorer/viewwidgetstack.h \
    ./ui/diskexplorer/vtocview.h \
    ./ui/startupdialog.h \
    ./ui/viewers/intbasicfileviewer.h \
    ./ui/widgets/notesdialog.h \
    ./util/opcodes.h \
    ./util/util.h \
    ./util/applestring.h \
    ./applesoftfile/applesoftfile.h \
    ./applesoftfile/applesofttoken.h \
    ./binaryfile/disassembler.h \
    ./binaryfile/binaryfile.h \
    ./textfile/textfile.h \
    ./ui/catalogwidget.h \
    ./ui/viewers/hiresviewwidget.h \
    ./ui/viewers/applesoftfileviewer.h \
    ./applesoftfile/applesoftformatter.h \
    ./applesoftfile/applesoftline.h  \
    ./ui/viewers/disassemblerviewer.h \
    ./ui/viewers/hexdumpviewer.h \
    ./ui/viewers/texthexdumpviewer.h \
    ./relocatablefile/relocatablefile.h \
    ./ui/viewers/mazeviewer.h \
    ./binaryfile/binaryfilemetadata.h \
    ./ui/widgets/characterwidget.h \
    ./util/charset.h \
    ./ui/viewers/charsetviewer.h \
    ./ui/viewers/applesoftfiledetailviewer.h \
    ./ui/widgets/hexconverter.h \
    ./ui/widgets/hrcgcontrolsinfo.h \
    ./ui/viewers/viewerbase.h \
    ./ui/viewers/fileviewerinterface.h \
    ./ui/widgets/CharacterSetExplorer.h \
    ./ui/widgets/HiresScreenWidget.h \
    ./ui/widgets/DisassemblerMetadataDialog.h \
    ./binaryfile/EntryPointModel.h \
    ./ui/widgets/LocationInfoDialog.h \
    ./binaryfile/EntryPoints.h \
    ./binaryfile/AssemblerSymbols.h \
    ./binaryfile/AssemblerSymbolModel.h \
    ./binaryfile/MemoryUsageMap.h \
    ./ui/diskexplorer/DiskExplorer.h \
    ./ui/diskexplorer/DiskExplorerMapWidget.h \
    ./applesoftfile/ApplesoftRetokenizer.h \
    ./util/AppleColors.h \
    ./internals/JumpLineManager.h \
    ./ui/widgets/FlowLineTextBrowser.h \
    ./ui/widgets/asciiinfodialog.h \
    diskfiles/cpressdiskfs.h \
    diskfiles/cpressdiskimage.h \
    diskfiles/cpressfile.h \
    diskfiles/diskstore/asdiskdata.h \
    diskfiles/diskstore/asdiskimporter.h \
    diskfiles/diskstore/asdiskstore.h \
    util/referencelist.h

FORMS += \
    ./sequence/sequenceviewer.ui \
    ./ui/catalogwidget.ui \
    ./ui/diskexplorer/catalogsectorview.ui \
    ./ui/diskexplorer/vtocview.ui \
    ./ui/startupdialog.ui \
    ./ui/viewers/applesoftfileviewer.ui \
    ./ui/viewers/disassemblerviewer.ui \
    ./ui/viewers/hexdumpviewer.ui \
    ./ui/viewers/intbasicfileviewer.ui \
    ./ui/viewers/texthexdumpviewer.ui \
    ./ui/viewers/applesoftfiledetailviewer.ui \
    ./ui/widgets/hexconverter.ui \
    ./ui/widgets/hrcgcontrolsinfo.ui \
    ./ui/viewers/viewerbase.ui \
    ./ui/widgets/CharacterSetExplorer.ui \
    ./ui/widgets/DisassemblerMetadataDialog.ui \
    ./ui/widgets/LocationInfoDialog.ui \
    ./ui/widgets/asciiinfodialog.ui \
    ./ui/widgets/notesdialog.ui

RESOURCES += \
    ./resource/resources.qrc \
    ./qdarkstyle/style.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ads/lib/ -lqtadvanceddocking
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ads/lib/ -lqtadvanceddockingd
else:unix: LIBS += -L$$OUT_PWD/../ads/src/ -lqtadvanceddockingd

INCLUDEPATH += $$PWD/../ads/src
DEPENDPATH += $$PWD/../ads/src

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../cpress/zlib/release/ -lzlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../cpress/zlib/debug/ -lzlibd
else:unix: LIBS += -L$$OUT_PWD/../cpress/zlib/ -lzlib

INCLUDEPATH += $$PWD/../cpress/zlib
DEPENDPATH += $$PWD/../cpress/zlib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../cpress/nufxlib/release/ -lnufxlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../cpress/nufxlib/debug/ -lnufxlibd
else:unix: LIBS += -L$$OUT_PWD/../cpress/nufxlib/ -lnufxlib

INCLUDEPATH += $$PWD/../cpress/nufxlib
DEPENDPATH += $$PWD/../cpress/nufxlib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../cpress/diskimg/release/ -ldiskimg
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../cpress/diskimg/debug/ -ldiskimgd
else:unix: LIBS += -L$$OUT_PWD/../cpress/diskimg/ -ldiskimg

INCLUDEPATH += $$PWD/../cpress/diskimg
DEPENDPATH += $$PWD/../cpress/diskimg
