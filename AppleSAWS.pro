TEMPLATE = subdirs

DEFINES -= ADS_BUILD_STATIC

SUBDIRS = \
	src \
	ads


src.depends = ads

