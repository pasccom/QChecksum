#ifndef QCHECKSUMDIALOG_H
#define QCHECKSUMDIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QBoxLayout;

class QChecksumDialog : public QDialog
{
    Q_OBJECT
public:
    QChecksumDialog(QWidget* parent = nullptr);
    void addAlgorithm(const QString& name, const QString & program, const QString& whatsThis = QString());

    inline QStringList checksumFilePath(void) const {return mFilePaths;}
    bool setChecksumFilePath(const QString& filePath);
    bool addChecksumFilePath(const QString& filePath);
    void clearChecksumFilePath(void);
private:
    QBoxLayout* mButtonsLayout;
    QLabel* mFileLabel;
    QLineEdit* mFileEdit;
    QPushButton* mFileButton;
    QLabel* mChecksumLabel;
    QLineEdit* mChecksumEdit;

    QStringList mFilePaths;
};

#endif // QCHECKSUMDIALOG_H
