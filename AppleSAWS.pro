
QT += core gui
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AppleSAWS
TEMPLATE = app

INCLUDEPATH += src/
INCLUDEPATH += src/relocatablefile
INCLUDEPATH += src/diskfiles
INCLUDEPATH += src/diskfiles/dos33
INCLUDEPATH += src/util
INCLUDEPATH += src/applesoftfile
INCLUDEPATH += src/binaryfile
INCLUDEPATH += src/textfile
INCLUDEPATH += src/ui/viewers
INCLUDEPATH += src/imported
INCLUDEPATH += src/internals
INCLUDEPATH += src/ui/widgets/
INCLUDEPATH += src/ui

DEFINES += WS_VIDEO

SOURCES += \
    src/main.cpp \
    src/diskfiles/dos33/diskfile.cxx \
    src/diskfiles/dos33/sector.cxx \
    src/diskfiles/dos33/vtoc.cxx \
    src/diskfiles/dos33/catalogsector.cxx \
    src/diskfiles/dos33/tracksectorlist.cxx \
    src/diskfiles/dos33/filedescriptiveentry.cxx \
    src/diskfiles/dos33/genericfile.cxx \
    src/util/applestring.cxx \
    src/applesoftfile/applesoftfile.cxx \
    src/applesoftfile/applesofttoken.cxx \
    src/applesoftfile/applesoftformatter.cxx \
    src/binaryfile/disassembler.cxx \
    src/binaryfile/binaryfile.cxx \
    src/textfile/textfile.cxx \
    src/ui/catalogwidget.cxx \
    src/ui/mainwindow.cxx \
    src/ui/viewers/hiresviewwidget.cxx \
    src/ui/viewers/applesoftfileviewer.cxx \
    src/ui/viewers/disassemblerviewer.cpp \
    src/ui/viewers/hexdumpviewer.cpp \
    src/ui/viewers/texthexdumpviewer.cpp \
    src/ui/viewers/mazeviewer.cpp \
    src/ui/viewers/charsetviewer.cpp \
    src/internals/memory.cxx \
    src/relocatablefile/relocatablefile.cxx \
    src/binaryfile/binaryfilemetadata.cpp \
    src/util/charset.cpp \
    src/ui/widgets/characterwidget.cpp \
    src/ui/viewers/applesoftfiledetailviewer.cpp


HEADERS += \
    src/diskfiles/dos33/diskfile.h \
    src/diskfiles/dos33/sector.h \
    src/diskfiles/dos33/vtoc.h \
    src/diskfiles/dos33/catalogsector.h \
    src/diskfiles/dos33/tracksectorlist.h \
    src/diskfiles/dos33/filedescriptiveentry.h \
    src/diskfiles/dos33/genericfile.h \
    src/util/util.h \
    src/util/applestring.h \
    src/applesoftfile/applesoftfile.h \
    src/applesoftfile/applesofttoken.h \
    src/binaryfile/disassembler.h \
    src/binaryfile/binaryfile.h \
    src/textfile/textfile.h \
    src/ui/catalogwidget.h \
    src/ui/mainwindow.h \
    src/ui/viewers/hiresviewwidget.h \
    src/ui/viewers/applesoftfileviewer.h \ 
    src/applesoftfile/applesoftformatter.h \
    src/applesoftfile/applesoftline.h  \
    src/internals/memory.h \
    src/ui/viewers/disassemblerviewer.h \
    src/ui/viewers/hexdumpviewer.h \
    src/ui/viewers/texthexdumpviewer.h \
    src/relocatablefile/relocatablefile.h \
    src/ui/viewers/mazeviewer.h \
    src/binaryfile/binaryfilemetadata.h \
    src/ui/widgets/characterwidget.h \
    src/util/charset.h \
    src/ui/viewers/charsetviewer.h \
    src/ui/viewers/applesoftfiledetailviewer.h

FORMS += \
    src/ui/catalogwidget.ui \
    src/ui/mainwindow.ui \
    src/ui/viewers/applesoftfileviewer.ui \
    src/ui/viewers/disassemblerviewer.ui \
    src/ui/viewers/hexdumpviewer.ui \
    src/ui/viewers/texthexdumpviewer.ui \
    src/ui/viewers/applesoftfiledetailviewer.ui
