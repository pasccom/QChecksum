TEMPLATE=subdirs # Recursive project

# Directories and files:
SUBDIRS += src services

OTHER_FILES += .gitignore README.md LICENSE

# Translations update target:
lupdate.CONFIG = recursive
lupdate.recurse = src

QMAKE_EXTRA_TARGETS += lupdate
