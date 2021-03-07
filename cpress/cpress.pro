TEMPLATE = subdirs

SUBDIRS = zlib nufxlib diskimg libhfs

DEFINES += _CRT_NONSTDC_NO_WARNINGS
DEFINES += _CRT_SECURE_NO_WARNINGS

libhfs.depends = zlib
nufxlib.depends = zlib

diskimg.depends = nufxlib libhfs
