# Copyright 2019 Pascal COMBES <pascom@orange.fr>
# 
# This file is part of QChecksum.
# 
# QChecksum is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# QChecksum is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with QChecksum. If not, see <http://www.gnu.org/licenses/>

TEMPLATE=subdirs # Recursive project

# Directories and files:
SUBDIRS += src services

OTHER_FILES += .gitignore README.md LICENSE

# Tarball target:
system(git status) {
    TARBALL_TAG=$$system(git describe --tags)
    TARBALL_TAG=$$split(TARBALL_TAG, -)
    TARBALL_TAG=$$member(TARBALL_TAG, 0)
    TARBALL_TAG=$$str_member($$TARBALL_TAG, 1, -1)
    message("TARBALL_TAG=$$TARBALL_TAG")
    TARBALL_BASENAME="QChecksum-$${TARBALL_TAG}"
} else {
    TARBALL_BASENAME="QChecksum"
}

DOLLAR="$"
DOUBLE_DOLLAR="$${DOLLAR}$${DOLLAR}"

tarball.commands =  "rm -f ./$${TARBALL_BASENAME}.tar.gz;"
tarball.commands += "mkdir $${TARBALL_BASENAME};"
tarball.commands += "for FILE in `git ls-files`; do"
tarball.commands += "    mkdir -p $${TARBALL_BASENAME}/`dirname $$DOUBLE_DOLLAR{FILE}`;"
tarball.commands += "    cp $$DOUBLE_DOLLAR{FILE} $${TARBALL_BASENAME}/$$DOUBLE_DOLLAR{FILE};"
tarball.commands += "done;"
tarball.commands += "tar -czf ./$${TARBALL_BASENAME}.tar.gz ./$${TARBALL_BASENAME};"
tarball.commands += "rm -Rf ./$${TARBALL_BASENAME}/"

# Translations update target:
lupdate.CONFIG = recursive
lupdate.recurse = src

QMAKE_EXTRA_TARGETS += lupdate tarball
