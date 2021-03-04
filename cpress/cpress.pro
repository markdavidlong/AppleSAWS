TEMPLATE = subdirs

SUBDIRS = zlib nufxlib diskimg libhfs


libhfs.depends = zlib
nufxlib.depends = zlib

diskimg.depends = nufxlib libhfs
