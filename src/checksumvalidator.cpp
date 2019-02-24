#include "checksumvalidator.h"

ChecksumValidator::ChecksumValidator(QObject *parent) :
    QValidator(parent)
{

}

QValidator::State ChecksumValidator::validate(QString& text, int& pos) const
{
    Q_UNUSED(pos);

    text = text.toLower();

    if (text.indexOf(QRegExp("[^0-9a-f]")) != -1)
        return Invalid;

    return mAlgorithms.contains(text.length() * 4) ? Acceptable : Intermediate;
}

QList<QVariant> ChecksumValidator::getValidAlgoritms(const QString& text) const
{
    return mAlgorithms.values(text.length() * 4);
}
