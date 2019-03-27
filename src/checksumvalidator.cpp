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
