warning (In cpress.pro)

TEMPLATE = subdirs

SUBDIRS = zlib libhfs nufxlib diskimg

DEFINES += _CRT_NONSTDC_NO_WARNINGS
DEFINES += _CRT_SECURE_NO_WARNINGS

libhfs.depends = zlib

nufxlib.depends = zlib libhfs

diskimg.depends = nufxlib libhfs zlib
