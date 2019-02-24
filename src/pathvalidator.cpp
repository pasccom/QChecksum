#include "pathvalidator.h"

#include <QtCore>

PathValidator::PathValidator(Accepts accepts, QObject* parent) :
    QValidator(parent)
{
    mAccepted = accepts;
}

QValidator::State PathValidator::validate(QString& text, int& pos) const
{
#ifdef Q_OS_WIN
    QString invalid = "<>:\"|?*"
#else
    QString invalid;
#endif

    if ((mAccepted & AcceptEmpty) && text.isEmpty())
       return Acceptable;

    for (int c = 0; c< invalid.length(); c++)
        if (text.contains(invalid[c], Qt::CaseSensitive))
            return Invalid;

    if ((mAccepted & AcceptNew) && !QFileInfo::exists(text))
        return validateName(text, pos) ? Acceptable : Intermediate;

   if (!QFileInfo::exists(text))
       return Intermediate;

   return (validateName(text, pos) && validateType(text, pos) && validatePermissions(text, pos)) ? Acceptable : Intermediate;
}

void PathValidator::manageAcceptFlags(Accept flag, bool enable)
{
    if (enable)
        mAccepted |= flag;
    else
        mAccepted &= ~Accepts(flag);
}

bool PathValidator::validateName(QString& text, int& pos) const
{
    Q_UNUSED(pos);

    if (mRequiredExtensions.isEmpty())
        return true;

    foreach (QString ext, mRequiredExtensions) {
        if (text.endsWith(QLatin1Char('.') + ext))
            return true;
    }

    return false;
}

bool PathValidator::validateType(QString& text, int& pos) const
{
    Q_UNUSED(pos);

    QFileInfo info(text);

    if (info.isFile() && !(mAccepted & AcceptsFiles))
        return false;

    if (info.isDir() && !(mAccepted & AcceptsDirectories))
        return false;

    return true;
}

bool PathValidator::validatePermissions(QString& text, int& pos) const
{
    Q_UNUSED(pos);

    QFileInfo info(text);

    if ((mAccepted & RequireReadable) && !info.isReadable())
        return false;
    if ((mAccepted & RequireWritable) && !info.isWritable())
        return false;
    if ((mAccepted & RequireExecutable) && !info.isExecutable())
        return false;

    return true;
}


