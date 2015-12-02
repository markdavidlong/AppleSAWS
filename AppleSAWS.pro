
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AppleSAWS
TEMPLATE = app

INCLUDEPATH += src/

SOURCES += src/main.cpp \
    src/diskfile.cxx \
    src/sector.cxx \
    src/vtoc.cxx \
    src/catalogsector.cxx \
    src/applestring.cxx \
    src/tracksectorlist.cxx \
    src/filedescriptiveentry.cxx \
    src/applesoftfile.cxx \
    src/genericfile.cxx \
    src/disassembler.cxx \
    src/binaryfile.cxx \
    src/catalogwidget.cxx \
    src/mainwindow.cxx \
    src/hiresviewwidget.cxx

HEADERS += \
    src/diskfile.h \
    src/sector.h \
    src/vtoc.h \
    src/util.h \
    src/catalogsector.h \
    src/applestring.h \
    src/tracksectorlist.h \
    src/filedescriptiveentry.h \
    src/applesoftfile.h \
    src/genericfile.h \
    src/disassembler.h \
    src/binaryfile.h \
    src/catalogwidget.h \
    src/mainwindow.h \
    src/hiresviewwidget.h

FORMS += \
    src/catalogwidget.ui \
    src/mainwindow.ui
