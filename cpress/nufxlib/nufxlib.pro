CONFIG += c++14
#CONFIG += debug_and_release
#TARGET = $$qtLibraryTarget(nufxlib)
TARGET = nufxlib

DEFINES += QT_NO_DEPRECATED_WARNINGS
DEFINES += _CRT_NONSTDC_NO_WARNINGS
DEFINES += _CRT_SECURE_NO_WARNINGS
TEMPLATE = lib

QMAKE_LFLAGS += /NOENTRY


CONFIG += staticlib

SOURCES +=  $$files($$PWD/../ciderpress/nufxlib/*.c)

HEADERS +=   $$files($$PWD/../ciderpress/nufxlib/*.h)
INCLUDEPATH += $$PWD/../ciderpress/nufxlib/
INCLUDEPATH += $$PWD/../ciderpress/zlib/

LIBS +=  vcruntime.lib libcmt.lib  libvcruntime.lib  libucrt.lib

win32:CONFIG(release, debug|release): warning(nufxlib looking in $$OUT_PWD/../zlib/release )
else:win32:CONFIG(debug, debug|release): warning(nufxlib looking in $$OUT_PWD/../zlib/debug )

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../zlib/release/ -lzlib
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../zlib/debug/ -lzlibd
#else:unix: LIBS += -L$$OUT_PWD/../zlib/ -lzlib

win32:CONFIG(release, debug|release): LIBS += $$OUT_PWD/../zlib/release/zlib.dll
else:win32:CONFIG(debug, debug|release): LIBS += $$OUT_PWD/../zlib/debug/zlib.dll
else:unix: LIBS += -L$$OUT_PWD/../zlib/ -lzlib.dll

INCLUDEPATH += $$PWD/../zlib
DEPENDPATH += $$PWD/../zlib
