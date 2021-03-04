CONFIG += c++14
#CONFIG += debug_and_release
#TARGET = $$qtLibraryTarget(zlib)
TARGET = zlib
DEFINES += QT_DEPRECATED_WARNINGS
TEMPLATE = lib

CONFIG += staticlib

SOURCES += $$files($$PWD/../ciderpress/zlib/*.c)

HEADERS +=  $$files($$PWD/../ciderpress/zlib/*.h)

INCLUDEPATH +=  $$PWD/../ciderpress/zlib/
