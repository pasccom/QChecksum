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

TEMPLATE = app     # Make an executable
TARGET = qchecksum # The name of the executable
QT += widgets      # Uses Qt Widgets

# Files:
HEADERS += qchecksumdialog.h     \
           pathvalidator.h       \
           checksumvalidator.h   \

SOURCES += qchecksumdialog.cpp   \
           pathvalidator.cpp     \
           main.cpp              \
           checksumvalidator.cpp \

TRANSLATIONS += qchecksum_en.ts  \
                qchecksum_fr.ts  \

RESOURCES += qchecksum.qrc

OTHER_FILES += qchecksum.desktop

# The directory where to put MOC-generated files :
MOC_DIR = ./.moc

# Cheating dirs :
win32 {
    OBJECTS_DIR = ./.obj_win
    CONFIG(debug, debug|release):DESTDIR = ./debug
    else:DESTDIR = ./release
}
unix {
    OBJECTS_DIR = ./.obj_unix
    CONFIG(debug, debug|release):DESTDIR = ./debug
    else:DESTDIR = ./release
}

# Translation files update (not handled by Qt)
isEmpty(QMAKE_LUPDATE) {
    win32:QMAKE_LUPDATE = $$[QT_INSTALL_BINS]/lupdate.exe
    else:QMAKE_LUPDATE = $$[QT_INSTALL_BINS]/lupdate
}

lupdate.depends += $$_PRO_FILE_
lupdate.depends += $$SOURCES
lupdate.depends += $$HEADERS
lupdate.commands = $$QMAKE_LUPDATE \"$$_PRO_FILE_\"

QMAKE_EXTRA_TARGETS += lupdate

# Translation files generation (not handled by Qt)
isEmpty(QMAKE_LRELEASE) {
    win32:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
}

lrelease.input = TRANSLATIONS
lrelease.output = ./$$DESTDIR/${QMAKE_FILE_BASE}.qm
lrelease.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN} -qm ./$$DESTDIR/${QMAKE_FILE_BASE}.qm
lrelease.CONFIG += no_link

QMAKE_EXTRA_COMPILERS += lrelease
POST_TARGETDEPS += compiler_lrelease_make_all

# Target installation:
isEmpty(PREFIX) {
    isEmpty(USER_INSTALL) {
        BIN_DIR="/usr/bin"
        DATA_DIR="/usr/share"
    } else {
        BIN_DIR="$$(HOME)/bin"
        DATA_DIR="$$(HOME)/.local/share"
    }
} else {
    BIN_DIR="$$PREFIX/bin"
    DATA_DIR="$$PREFIX/share"
}

target.files=$$DESTDIR/$$TARGET
target.path="$$BIN_DIR"

# Translation files installation:
translations.files=$$DESTDIR/*.qm
translations.path="$$DATA_DIR/$$TARGET"
DEFINES += "TRANSLATIONS_INSTALL_DIR=\\\"$${translations.path}\\\""

# Service file installation:
SERVICE_PATHS=$$system(kf5-config --path services)
SERVICE_PATHS=$$split(SERVICE_PATHS, :)
isEmpty(USER_INSTALL) {
    INSTALL_SERVICE_PATH = $$member(SERVICE_PATHS, 1)
} else {
    INSTALL_SERVICE_PATH = $$member(SERVICE_PATHS, 0)
}

message(INSTALL_SERVICE_PATH=$$INSTALL_SERVICE_PATH)
installService.path = $$INSTALL_SERVICE_PATH
installService.files = qchecksum.desktop

# Installation:
INSTALLS += target installService translations
