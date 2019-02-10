#include "qchecksumdialog.h"

#include <QtWidgets>
#include <QtDebug>

#define CHECKSUM_LIST(_ALGORITHM_)        \
    _ALGORITHM_(MD5,    md5sum,    true)  \
    _ALGORITHM_(SHA1,   sha1sum,   false) \
    _ALGORITHM_(SHA224, sha224sum, false) \
    _ALGORITHM_(SHA256, sha256sum, false) \
    _ALGORITHM_(SHA384, sha384sum, false) \
    _ALGORITHM_(SHA512, sha512sum, false) \
    _ALGORITHM_(Blake2, b2sum,     false) \

#define __CREATE_BUTTONS(_name_, _program_, _deprecated_)                                                                               \
    if ((__anySelected && argParser.isSet(yes ## _name_)) || (!__anySelected && !argParser.isSet(no ## _name_))) {                      \
        __whichProcess.setProgram("which");                                                                                             \
        __whichProcess.setArguments(QStringList() << #_program_);                                                                       \
        __whichProcess.start(QProcess::ReadOnly);                                                                                       \
        if (__whichProcess.waitForFinished(1000)) {                                                                                     \
            if (__whichProcess.exitCode() == 0)                                                                                         \
                dialog.addAlgorithm(#_name_, #_program_,                                                                                \
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

#define __CREATE_OPTIONS(_name_, _program_, _deprecated_)                                                                               \
    QCommandLineOption yes ## _name_(QString(#_name_).toLower(), QChecksumDialog::tr("Select %1 algorithm").arg(#_name_));              \
    argParser.addOption(yes ## _name_);                                                                                                 \
    QCommandLineOption no ## _name_(QString("no-" # _name_).toLower(), QChecksumDialog::tr("Disable %1 algorithm").arg(#_name_));       \
    argParser.addOption(no ## _name_);                                                                                                  \

#define CREATE_OPTIONS(_list_)                                                                                                          \
    _list_(__CREATE_OPTIONS)                                                                                                            \

#define __PROCESS_OPTIONS(_name_, _program_, _deprecated_)                                                                              \
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
    QApplication::setApplicationVersion("1.0.0");

    QTranslator translator;
    if (translator.load(QLocale(), "qchecksum", "_"))
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

    // return app.exec();
}
