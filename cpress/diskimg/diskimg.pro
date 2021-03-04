CONFIG += c++14
#CONFIG += debug_and_release
#TARGET = $$qtLibraryTarget(diskimg)
TARGET = diskimg
DEFINES += QT_DEPRECATED_WARNINGS

TEMPLATE = lib

CONFIG += dll
DEFINES += DISKIMG_EXPORTS

SOURCES += $$files($$PWD/../ciderpress/diskimg/*.cpp)

HEADERS +=  $$files($$PWD/../ciderpress/diskimg/*.h)


INCLUDEPATH += $$PWD/../ciderpress/nufxlib
INCLUDEPATH += $$PWD/../ciderpress/nufxlib/libhfs


#DEFINES += EXCISE_GPL_CODE

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../nufxlib/release/ -lnufxlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../nufxlib/debug/ -lnufxlib
else:unix: LIBS += -L$$OUT_PWD/../nufxlib/ -lnufxlib

INCLUDEPATH += $$PWD/../ciderpress/nufxlib
DEPENDPATH += $$PWD/../nufxlib


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../zlib/release/ -lzlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../zlib/debug/ -lzlib
else:unix: LIBS += -L$$OUT_PWD/../zlib/ -lzlib

INCLUDEPATH += $$PWD/../zlib
DEPENDPATH += $$PWD/../zlib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libhfs/release/ -llibhfs
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libhfs/debug/ -llibhfs
else:unix: LIBS += -L$$OUT_PWD/../libhfs/ -llibhfs

INCLUDEPATH += $$PWD/../libhfs
DEPENDPATH += $$PWD/../libhfs

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libhfs/release/liblibhfs.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libhfs/debug/liblibhfs.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libhfs/release/libhfs.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libhfs/debug/libhfs.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../libhfs/liblibhfs.a
