/* Copyright 2019 Pascal COMBES <pascom@orange.fr>
 * 
 * This file is part of QChecksum.
 * 
 * QChecksum is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * QChecksum is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with QChecksum. If not, see <http://www.gnu.org/licenses/>
 */

#include "qchecksumdialog.h"

#include <QtWidgets>
#include <QtDebug>

/*!
 * \mainpage
 *
 * \section Description
 * This repository contains the code for a small utiliy program which computes
 * and verifies checksums for files. It also includes services definitions to
 * allow users to compute checksums directly from e.g.  Dolphin or Konqueror context
 * menus and run the program directly from Dolphin and Konqueror context menu.
 *
 * The services are implemented in such a way, that the algorithms can be disabled
 * individually in e.g. Dolphin or Konqueror service configuration.
 *
 * \section Features
 *
 * Here is a list of the current features of the program:
 * \li Allows to compute checksums using the programs in \c /bin
 * \li Allows to compute verify checksums using the programs in \c /bin
 * \li Can be run directly from e.g. Dolphin or Konqueror context menus
 * \li Lightweight version implemented using a service, which allows to
 * compute and verify checksums from e.g. Dolphin or Konqueror context menus
 *
 * \section Installation
 *
 * Compiling the program requires Qt5 and generating the services requires Python 3.
 *
 * The program can either be installed systemwide or in user HOME directory. To
 * select user HOME directory, set \c USER_INSTALL on \c qmake command line.
 * The build system also support the PREFIX option for binary and data installation.
 *
 * \section future Future developments
 *
 * If you have any other feature you will be interested in, please let me know.
 * I will be pleased to develop it if I think it is a must have.
 *
 * If you want to implement extension, also tell me please. Admittedly you
 * can do what you desire with the code (under the [licensing constraints](#license)), but this will avoid double work.
 *
 * \section license Licensing information
 *
 * QChecksum is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QChecksum is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QChecksum. If not, see http://www.gnu.org/licenses/
 */

#define CHECKSUM_LIST(_ALGORITHM_)             \
    _ALGORITHM_(MD5,    md5sum,    128, true)  \
    _ALGORITHM_(SHA1,   sha1sum,   160, false) \
    _ALGORITHM_(SHA224, sha224sum, 224, false) \
    _ALGORITHM_(SHA256, sha256sum, 256, false) \
    _ALGORITHM_(SHA384, sha384sum, 384, false) \
    _ALGORITHM_(SHA512, sha512sum, 512, false) \
    _ALGORITHM_(Blake2, b2sum,     512, false) \

#define __CREATE_BUTTONS(_name_, _program_, _bits_, _deprecated_)                                                                       \
    if ((__anySelected && argParser.isSet(yes ## _name_)) || (!__anySelected && !argParser.isSet(no ## _name_))) {                      \
        __whichProcess.setProgram("which");                                                                                             \
        __whichProcess.setArguments(QStringList() << #_program_);                                                                       \
        __whichProcess.start(QProcess::ReadOnly);                                                                                       \
        if (__whichProcess.waitForFinished(1000)) {                                                                                     \
            if (__whichProcess.exitCode() == 0)                                                                                         \
                dialog.addAlgorithm(#_name_, #_program_, _bits_,                                                                        \
                                    QChecksumDialog::tr("Use <b>%1</b> algorithm %2").arg(#_name_)                                      \
                                                                                     .arg(_deprecated_                                  \
                                                                                          ? QChecksumDialog::tr("(<i>deprecated</i>)")  \
                                                                                          : QString()));                                \
            else                                                                                                                        \
                qWarning() << #_program_ " not found in path";                                                                          \
         } else {                                                                                                                       \
            qWarning() << "which takes too much time to finish";                                                                        \
         }                                                                                                                              \
        __whichProcess.close();                                                                                                         \
    }                                                                                                                                   \

#define CREATE_BUTTONS(_list_)                                                                                                          \
    QProcess __whichProcess(&app);                                                                                                      \
    _list_(__CREATE_BUTTONS)                                                                                                            \

#define __CREATE_OPTIONS(_name_, _program_, _bits_, _deprecated_)                                                                       \
    QCommandLineOption yes ## _name_(QString(#_name_).toLower(), QChecksumDialog::tr("Select %1 algorithm").arg(#_name_));              \
    argParser.addOption(yes ## _name_);                                                                                                 \
    QCommandLineOption no ## _name_(QString("no-" # _name_).toLower(), QChecksumDialog::tr("Disable %1 algorithm").arg(#_name_));       \
    argParser.addOption(no ## _name_);                                                                                                  \

#define CREATE_OPTIONS(_list_)                                                                                                          \
    _list_(__CREATE_OPTIONS)                                                                                                            \

#define __PROCESS_OPTIONS(_name_, _program_, _bits_, _deprecated_)                                                                      \
    if (argParser.isSet(yes ## _name_))                                                                                                 \
        __anySelected = true;                                                                                                           \

#define PROCESS_OPTIONS(_list_)                                                                                                         \
    bool __anySelected = false;                                                                                                         \
    _list_(__PROCESS_OPTIONS)                                                                                                           \

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(true);
    QApplication::setApplicationName("QChecksum");
    QApplication::setApplicationVersion("1.2");

    QTranslator translator;
    if (translator.load(QLocale(), "qchecksum", "_", QApplication::applicationDirPath()) ||
        translator.load(QLocale(), "qchecksum", "_", TRANSLATIONS_INSTALL_DIR))
        app.installTranslator(&translator);
    else
        qDebug() << "Could not load translator";

    QCommandLineParser argParser;
    argParser.setApplicationDescription(QChecksumDialog::tr("Computes and verifies checksums"));
    argParser.addHelpOption();
    argParser.addVersionOption();
    CREATE_OPTIONS(CHECKSUM_LIST)
    argParser.addPositionalArgument("files", QChecksumDialog::tr("Files for which checksums will be computed"));
    argParser.process(app.arguments());
    PROCESS_OPTIONS(CHECKSUM_LIST)

    QChecksumDialog dialog;
    foreach (QString filePath, argParser.positionalArguments())
        dialog.addChecksumFilePath(filePath);
    CREATE_BUTTONS(CHECKSUM_LIST)
    return dialog.exec() - 1;
}
