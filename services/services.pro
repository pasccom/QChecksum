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

TEMPLATE    = aux   # Other target
DESTDIR     = gen   # Path to build ouput folder
TARGET      = .     # No default target
INSTALLS    =       # No default install

# Files:
OTHER_FILES += generate_checksum_services.py \
               checksum_services.py \
               services.py \
               doc/index.rst \
               doc/services.rst \
               doc/checksum_services.rst

# Build target:
genServices.depends = generate_checksum_services.py
genServices.commands = (test ! -d "gen" && mkdir "$$DESTDIR");
genServices.commands += (python3 generate_checksum_services.py "$$DESTDIR/");
genServices.CONFIG += no_link

QMAKE_EXTRA_TARGETS += genServices
POST_TARGETDEPS += genServices

# Clean target:
ALGORITMS=$$system(python3 generate_checksum_services.py)
for (algo, ALGORITMS) {
    message(Found $$algo)
    QMAKE_CLEAN += $$DESTDIR/checksum-compute-$${algo}.desktop
    QMAKE_CLEAN += $$DESTDIR/checksum-verify-$${algo}.desktop
}

# Install target:
SERVICE_PATHS=$$system(kf5-config --path services)
SERVICE_PATHS=$$split(SERVICE_PATHS, :)
isEmpty(USER_INSTALL) {
    INSTALL_PATH = $$member(SERVICE_PATHS, 1)
} else {
    INSTALL_PATH = $$member(SERVICE_PATHS, 0)
}

message(INSTALL_PATH=$$INSTALL_PATH)
installServices.path = $$INSTALL_PATH
ALGORITMS=$$system(python3 generate_checksum_services.py)
for (algo, ALGORITMS) {
    message(Found $$algo)
    installServices.files += $$DESTDIR/checksum-compute-$${algo}.desktop
    installServices.files += $$DESTDIR/checksum-verify-$${algo}.desktop
}
installServices.CONFIG += no_check_exist

INSTALLS += installServices
