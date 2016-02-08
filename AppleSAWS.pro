
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AppleSAWS
TEMPLATE = app

INCLUDEPATH += src/
INCLUDEPATH += src/diskfiles
INCLUDEPATH += src/diskfiles/dos33
INCLUDEPATH += src/util
INCLUDEPATH += src/applesoftfile
INCLUDEPATH += src/binaryfile
INCLUDEPATH += src/ui
INCLUDEPATH += src/ui/viewers
INCLUDEPATH += src/imported
INCLUDEPATH += src/internals

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
    src/binaryfile/disassembler.cxx \
    src/binaryfile/binaryfile.cxx \
    src/ui/catalogwidget.cxx \
    src/ui/mainwindow.cxx \
    src/ui/viewers/hiresviewwidget.cxx \
    src/ui/viewers/applesoftfileviewer.cxx \ 
    src/applesoftfile/applesoftformatter.cxx \
    src/applesoftfile/applesoftline.cpp  \
    src/internals/memory.cxx \
    src/ui/viewers/disassemblerviewer.cpp

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
    src/ui/catalogwidget.h \
    src/ui/mainwindow.h \
    src/ui/viewers/hiresviewwidget.h \
    src/ui/viewers/applesoftfileviewer.h \ 
    src/applesoftfile/applesoftformatter.h \
    src/applesoftfile/applesoftline.h  \
    src/internals/memory.h \
    src/ui/viewers/disassemblerviewer.h

FORMS += \
    src/ui/catalogwidget.ui \
    src/ui/mainwindow.ui \
    src/ui/viewers/applesoftfileviewer.ui \
    src/ui/viewers/disassemblerviewer.ui
