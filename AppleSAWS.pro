
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AppleSAWS
TEMPLATE = app

SOURCES += main.cpp \
    diskfile.cxx \
    sector.cxx \
    vtoc.cxx \
    catalogsector.cxx \
    applestring.cxx \
    tracksectorlist.cxx \
    filedescriptiveentry.cxx \
    applesoftfile.cxx \
    genericfile.cxx \
    disassembler.cxx \
    binaryfile.cxx \
    catalogwidget.cxx \
    mainwindow.cxx

HEADERS += \
    diskfile.h \
    sector.h \
    vtoc.h \
    util.h \
    catalogsector.h \
    applestring.h \
    tracksectorlist.h \
    filedescriptiveentry.h \
    applesoftfile.h \
    genericfile.h \
    disassembler.h \
    binaryfile.h \
    catalogwidget.h \
    mainwindow.h

FORMS += \
    catalogwidget.ui \
    mainwindow.ui
