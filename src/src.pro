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
