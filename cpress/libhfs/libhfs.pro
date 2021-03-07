CONFIG += c++14
#CONFIG += debug_and_release
#TARGET = $$qtLibraryTarget(zlib)
TARGET = libhfs
TEMPLATE = lib
DEFINES += QT_NO_DEPRECATED_WARNINGS
DEFINES += _CRT_NONSTDC_NO_WARNINGS
DEFINES += _CRT_SECURE_NO_WARNINGS
CONFIG += staticlib

SOURCES += $$files($$PWD/../ciderpress/diskimg/libhfs/*.c)

HEADERS +=  $$files($$PWD/../ciderpress/diskimg/libhfs/*.h)

INCLUDEPATH +=  $$PWD/../ciderpress/diskimg/libhfs/
