TEMPLATE = subdirs

DEFINES -= ADS_BUILD_STATIC

SUBDIRS = \
	cpress \
    src \
    ads


src.depends = ads cpress

