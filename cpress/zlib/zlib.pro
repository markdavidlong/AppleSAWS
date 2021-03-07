CONFIG += c++14
#CONFIG += debug_and_release
#TARGET = $$qtLibraryTarget(zlib)
TARGET = zlib
TEMPLATE = lib

DEFINES += QT_NO_DEPRECATED_WARNINGS
DEFINES += _CRT_NONSTDC_NO_WARNINGS
DEFINES += _CRT_SECURE_NO_WARNINGS

CONFIG += staticlib

SOURCES += $$files($$PWD/../ciderpress/zlib/*.c)

HEADERS +=  $$files($$PWD/../ciderpress/zlib/*.h)

INCLUDEPATH +=  $$PWD/../ciderpress/zlib/
