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
#include "pathvalidator.h"
#include "checksumvalidator.h"

#include <QtWidgets>
#include <iostream>

QChecksumDialog::QChecksumDialog(QWidget* parent) :
    QDialog(parent)
{
    setWindowTitle(tr("QChecksum"));
    setWindowIcon(QIcon(":/app.png"));

    mFileEdit = new QLineEdit(this);
    mFileEdit->setValidator(new PathValidator(PathValidator::AcceptsFiles, mFileEdit));
    mFileEdit->setWhatsThis(tr("Enter here the path to the file to hash"));

    mFileLabel = new QLabel(tr("File to hash:"), this);
    mFileLabel->setBuddy(mFileEdit);

    mFileButton = new QPushButton(QIcon::fromTheme("folder"), QString(), this);
    mFileButton->setWhatsThis(tr("Open file browser to choose file to hash"));

    mChecksumEdit = new QLineEdit(this);
    mChecksumEdit->setWhatsThis(tr("Enter here the expected checksum"));

    mChecksumValidator = new ChecksumValidator(mChecksumEdit);
    mChecksumEdit->setValidator(mChecksumValidator);

    mChecksumLabel = new QLabel(tr("Expected checksum:"), this);
    mChecksumLabel->setBuddy(mChecksumEdit);

    QPushButton* cancelButton = new QPushButton(QIcon::fromTheme("dialog-cancel"), tr("Cancel"), this);
    cancelButton->setMaximumWidth(100);

    mButtonsLayout = new QHBoxLayout;
    mButtonsLayout->setSpacing(2);
    mButtonsLayout->setContentsMargins(2, 2, 2, 2);
    mButtonsLayout->addWidget(cancelButton);

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->setSpacing(2);
    mainLayout->setContentsMargins(2, 2, 2, 2);
    mainLayout->addWidget(mFileLabel, 0, 0);
    mainLayout->addWidget(mFileEdit, 0, 1);
    mainLayout->addWidget(mFileButton, 0, 2);
    mainLayout->addWidget(mChecksumLabel, 1, 0);
    mainLayout->addWidget(mChecksumEdit, 1, 1, 1, 2);
    mainLayout->addLayout(mButtonsLayout, 2, 0, 1, 3);

    setLayout(mainLayout);
    setFixedHeight(sizeHint().height());

    connect(mFileEdit, &QLineEdit::textChanged, this, &QChecksumDialog::validateLineEdit);
    connect(mChecksumEdit, &QLineEdit::textChanged, this, &QChecksumDialog::validateLineEdit);
    connect(mFileButton, &QPushButton::clicked, this, &QChecksumDialog::openFileDialog);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void QChecksumDialog::validateLineEdit(void)
{
    QLineEdit* lineEdit = qobject_cast<QLineEdit*>(sender());
    if (lineEdit == nullptr)
        return;

    if (lineEdit->hasAcceptableInput()) {
        lineEdit->setPalette(QPalette());
    } else {
        QPalette palette = lineEdit->palette();
        palette.setColor(QPalette::Text, Qt::red);
        lineEdit->setPalette(palette);
    }
}

void QChecksumDialog::openFileDialog(void)
{
    QString dir;
    if (QFileInfo(mFileEdit->text()).isDir())
        dir = mFileEdit->text();
    else if (QFileInfo(mFileEdit->text()).dir().exists())
        dir = QFileInfo(mFileEdit->text()).dir().absolutePath();

    dir = QFileDialog::getOpenFileName(this, tr("Choose file to hash"), dir);
    if (!dir.isNull())
        mFileEdit->setText(dir);
}

void QChecksumDialog::addAlgorithm(const QString& name, const QString& program, int numBits, const QString& whatsThis)
{
    QPushButton* algoButton = new QPushButton(name, this);
    algoButton->setMaximumWidth(100);
    algoButton->setWhatsThis(whatsThis);
    mButtonsLayout->insertWidget(mButtonsLayout->count() - 1, algoButton);

    mChecksumValidator->addAlgorithm(name, numBits);

    connect(mChecksumEdit, &QLineEdit::textChanged, this, [this, algoButton, name] (const QString& text) {
        algoButton->setEnabled(text.isEmpty() || mChecksumValidator->getValidAlgoritms(text).contains(name));
    });

    connect(algoButton, &QPushButton::clicked, this, [this, program] {
        QProcess checksumProcess(this);
        checksumProcess.setProgram(program);

        if (!mFilePaths.isEmpty())
            checksumProcess.setArguments(mFilePaths);
        else if (!mFileEdit->text().isEmpty() && QFileInfo(mFileEdit->text()).isFile())
            checksumProcess.setArguments(QStringList() << mFileEdit->text());
        else
            return;

        checksumProcess.start(QProcess::ReadOnly);
        if (checksumProcess.waitForFinished(30000)) {
            if (mChecksumEdit->text().isEmpty()) {
                foreach (QString line, QString(checksumProcess.readAllStandardOutput()).split('\n', QString::SkipEmptyParts)) {
                    std::cout << line.section(QRegExp("\\s+"), 1, 1).toStdString() << ": " << line.section(QRegExp("\\s+"), 0, 0).toStdString() << std::endl;
                }
                accept();
            } else {
                QString checksum = QString(checksumProcess.readAllStandardOutput()).section(QRegExp("\\s+"), 0, 0);
                if (QString::compare(checksum, mChecksumEdit->text(), Qt::CaseInsensitive) == 0)
                    accept();
                else
                    done(2);
            }
        } else {
            qWarning() << "Checksum process error:" << checksumProcess.error() << checksumProcess.errorString();
            reject();
        }
    });
}

bool QChecksumDialog::addChecksumFilePath(const QString& filePath)
{
    if (!QFileInfo(filePath).isFile())
        return false;

    mFilePaths.append(filePath);

    mChecksumLabel->setEnabled(mFilePaths.size() <= 1);
    mChecksumEdit->setEnabled(mFilePaths.size() <= 1);

    mFileLabel->hide();
    mFileEdit->hide();
    mFileButton->hide();
    setFixedHeight(sizeHint().height());

    return true;
}

bool QChecksumDialog::setChecksumFilePath(const QString& filePath)
{
    mFilePaths.clear();
    return addChecksumFilePath(filePath);
}

void QChecksumDialog::clearChecksumFilePath(void)
{
    mFilePaths.clear();

    mFileLabel->show();
    mFileEdit->show();
    mFileButton->show();
    setFixedHeight(sizeHint().height());
}
