warning (In diskimg.pro)

CONFIG += c++14
CONFIG += debug_and_release
TARGET = $$qtLibraryTarget(diskimg)
#TARGET = diskimg

TEMPLATE = lib

CONFIG += dll

DEFINES += EXCISE_GPL_CODE
DEFINES += DISKIMG_EXPORTS
DEFINES += QT_NO_DEPRECATED_WARNINGS
DEFINES += _CRT_NONSTDC_NO_WARNINGS
DEFINES += _CRT_SECURE_NO_WARNINGS

SOURCES += $$files($$PWD/../ciderpress/diskimg/*.cpp)

HEADERS +=  $$files($$PWD/../ciderpress/diskimg/*.h)


INCLUDEPATH += $$PWD/../ciderpress/nufxlib
INCLUDEPATH += $$PWD/../ciderpress/nufxlib/libhfs


#DEFINES += EXCISE_GPL_CODE

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../nufxlib/release/ -lnufxlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../nufxlib/debug/ -lnufxlibd
else:unix: LIBS += -L$$OUT_PWD/../nufxlib/ -lnufxlib

INCLUDEPATH += $$PWD/../ciderpress/nufxlib
DEPENDPATH += $$PWD/../nufxlib


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../zlib/release/ -lzlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../zlib/debug/ -lzlibd
else:unix: LIBS += -L$$OUT_PWD/../zlib/ -lzlib

INCLUDEPATH += $$PWD/../zlib
DEPENDPATH += $$PWD/../zlib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libhfs/release/ -llibhfs
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libhfs/debug/ -llibhfsd
else:unix: LIBS += -L$$OUT_PWD/../libhfs/ -llibhfs

INCLUDEPATH += $$PWD/../libhfs
DEPENDPATH += $$PWD/../libhfs

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libhfs/release/liblibhfs.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libhfs/debug/liblibhfs.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libhfs/release/libhfs.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libhfs/debug/libhfs.lib
#else:unix: PRE_TARGETDEPS += $$OUT_PWD/../libhfs/liblibhfs.a
