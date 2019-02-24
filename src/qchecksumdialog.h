#ifndef QCHECKSUMDIALOG_H
#define QCHECKSUMDIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QBoxLayout;
class ChecksumValidator;

/*!
 * \brief Checksum dialog
 *
 * This class implements a checksum computation/validation dialog.
 * It allows the user to:
 *  \li Choose the file for which to compute/validate the checksum
 *  \li Enter the expected checksum to be verified
 *  \li Choose the algorithm to use.
 */
class QChecksumDialog : public QDialog
{
    Q_OBJECT
public:
    /*!
     * \brief Constructor
     *
     * Initializes the UI
     * \param parent The parent widget
     */
    QChecksumDialog(QWidget* parent = nullptr);
    /*!
     * \brief Add a hashing algorithm
     *
     * Add a button for a hashing algorithm
     * \param name The name of the algorithm
     * \param program The program implementing the algorithm
     * \param numBits The number of bits of the checksums computed by the algorithm
     * \param whatsThis What's this message for the button.
     */
    void addAlgorithm(const QString& name, const QString & program, int numBits, const QString& whatsThis = QString());

    /*!
     * \brief Pathes to the files to hash
     *
     * Return the list of the pathes to the files to hash.
     * \return The pathes to the files to hash.
     */
    inline QStringList checksumFilePath(void) const {return mFilePaths;}
    /*!
     * \brief Set an unique path to a file to hash
     *
     * Resets the list of pathes to files to hash and add this path.
     * This will hide the file selector and will enable the checksum input.
     * \param filePath The path to the file to hash.
     * \return \c true on succes, \c false otherwise (if the file does not exist, for instance)
     * \sa addChecksumFilePath(), clearChecksumFilePath(), checksumFilePath()
     */
    bool setChecksumFilePath(const QString& filePath);
    /*!
     * \brief Add a path to a file to hash
     *
     * Add this path to the list of pathes to files to hash.
     * This will hide the file selector and will disable the checksum input
     * (unless the list is empty before).
     * \param filePath The path to the file to hash.
     * \return \c true on succes, \c false otherwise (if the file does not exist, for instance)
     * \sa setChecksumFilePath(), clearChecksumFilePath(), checksumFilePath()
     */
    bool addChecksumFilePath(const QString& filePath);
    /*!
     * \brief Clear the list of pathes to files to hash.
     *
     * Clears the list of pathes to files to hash.
     * This will show th file selector and will enable the checksum input
     * \sa setChecksumFilePath(), addChecksumFilePath(), checksumFilePath()
     */
    void clearChecksumFilePath(void);
private slots:
    /*!
     * \brief Validate QLineEdit
     *
     * Changes the color according to the validity of the QLineEdit contents.
     * If the contents is valid, the default theme color is used,
     * while, if the contents is not valid, the text will be red.
     */
    void validateLineEdit(void);
    /*!
     * \brief Open a QFileDialog
     *
     * Opens a QFileDialog for the user to choose the file to hash.
     */
    void openFileDialog(void);
private:
    QBoxLayout* mButtonsLayout;             /*!< The QLayout for the algorithm buttons.*/
    QLabel* mFileLabel;                     /*!< The QLabel for the file selector */
    QLineEdit* mFileEdit;                   /*!< The QLineEdit for the file selector */
    QPushButton* mFileButton;               /*!< The browse QPushButton of the file selector */
    QLabel* mChecksumLabel;                 /*!< The QLabel for checksum input */
    QLineEdit* mChecksumEdit;               /*!< The QLineEdit for checksum input */
    ChecksumValidator* mChecksumValidator;  /*!< The validator for checksum input */

    QStringList mFilePaths;                 /*!< The list of pathes to files to hash */
};

#endif // QCHECKSUMDIALOG_H
