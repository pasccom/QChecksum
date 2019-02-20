TEMPLATE=subdirs

SUBDIRS += src services

lupdate.CONFIG = recursive
lupdate.recurse = src

QMAKE_EXTRA_TARGETS += lupdate
