TEMPLATE = app     # Make an executable
TARGET = qchecksum # The name of the executable
QT += widgets

HEADERS += qchecksumdialog.h

SOURCES += qchecksumdialog.cpp \
           main.cpp

TRANSLATIONS += qchecksum_en.ts \
                qchecksum_fr.ts

RESOURCES += qchecksum.qrc

OTHER_FILES += .gitignore

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
