TEMPLATE=subdirs

SUBDIRS += src services

OTHER_FILES += .gitignore

lupdate.CONFIG = recursive
lupdate.recurse = src

QMAKE_EXTRA_TARGETS += lupdate
