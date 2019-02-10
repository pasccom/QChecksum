#include "qchecksumdialog.h"

#include <QtWidgets>
#include <iostream>

QChecksumDialog::QChecksumDialog(QWidget* parent) :
    QDialog(parent)
{
    setWindowTitle(tr("QChecksum"));
    setWindowIcon(QIcon(":/app.png"));

    mLineEdit = new QLineEdit(this);
    mLineEdit->setWhatsThis(tr("Enter here the expected checksum"));

    mLabel = new QLabel(tr("Expected checksum:"), this);
    mLabel->setBuddy(mLineEdit);

    QPushButton* cancelButton = new QPushButton(QIcon::fromTheme("dialog-cancel"), tr("Cancel"), this);
    cancelButton->setMaximumWidth(100);

    mButtonsLayout = new QHBoxLayout;
    mButtonsLayout->setSpacing(2);
    mButtonsLayout->setContentsMargins(2, 2, 2, 2);
    mButtonsLayout->addWidget(cancelButton);

    QBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(2);
    mainLayout->setContentsMargins(2, 2, 2, 2);
    mainLayout->addWidget(mLabel);
    mainLayout->addWidget(mLineEdit);
    mainLayout->addLayout(mButtonsLayout);

    setLayout(mainLayout);
    setFixedHeight(sizeHint().height());

    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void QChecksumDialog::addAlgorithm(const QString& name, const QString& program, const QString& whatsThis)
{
    QPushButton* algoButton = new QPushButton(name, this);
    algoButton->setMaximumWidth(100);
    algoButton->setWhatsThis(whatsThis);
    mButtonsLayout->insertWidget(mButtonsLayout->count() - 1, algoButton);

    connect(algoButton, &QPushButton::clicked, this, [this, program] {
        QProcess checksumProcess(this);
        checksumProcess.setProgram(program);
        checksumProcess.setArguments(mFilePaths);
        checksumProcess.start(QProcess::ReadOnly);
        if (checksumProcess.waitForFinished(30000)) {
            if (mLineEdit->text().isEmpty()) {
                foreach (QString line, QString(checksumProcess.readAllStandardOutput()).split('\n', QString::SkipEmptyParts)) {
                    std::cout << line.section(QRegExp("\\s+"), 1, 1).toStdString() << ": " << line.section(QRegExp("\\s+"), 0, 0).toStdString() << std::endl;
                }
                accept();
            } else {
                QString checksum = QString(checksumProcess.readAllStandardOutput()).section(QRegExp("\\s+"), 0, 0);
                if (QString::compare(checksum, mLineEdit->text(), Qt::CaseInsensitive) == 0)
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
    mLabel->setEnabled(mFilePaths.size() == 1);
    mLineEdit->setEnabled(mFilePaths.size() == 1);
    return true;
}

bool QChecksumDialog::setChecksumFilePath(const QString& filePath)
{
    mFilePaths.clear();
    return addChecksumFilePath(filePath);
}
