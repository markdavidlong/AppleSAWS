CONFIG += c++14
#CONFIG += debug_and_release
#TARGET = $$qtLibraryTarget(zlib)
TARGET = libhfs
DEFINES += QT_DEPRECATED_WARNINGS
TEMPLATE = lib

CONFIG += staticlib

SOURCES += $$files($$PWD/../ciderpress/diskimg/libhfs/*.c)

HEADERS +=  $$files($$PWD/../ciderpress/diskimg/libhfs/*.h)

INCLUDEPATH +=  $$PWD/../ciderpress/diskimg/libhfs/
