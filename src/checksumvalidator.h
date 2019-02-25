#ifndef CHECKSUMVALIDATOR_H
#define CHECKSUMVALIDATOR_H

#include <QValidator>
#include <QMap>

/*!
 * \brief A validator for checksums
 *
 * The validate() method of this class can tell whether a string can be a checksum based on
 * the characters it contains and its length.
 * Depending on the length of the checksum, the method getValidAlgoritms()
 * can tell from what (known) algorithm it corresponds.
 * The algorithms must be registered using the addAlgorithm() method.
 */
class ChecksumValidator : public QValidator
{
public:
    /*!
     * \brief Constructor
     *
     * Initializes a new path validator instance with given Accepts flags.
     *
     * \param parent The parent of this object
     */
    ChecksumValidator(QObject* parent = nullptr);

    /*!
     * \brief The validator function
     *
     * Validates the given string following checksum rules:
     *  \li Only lower case hexadecimal digits (\c [0-9a-f])
     *  \li Valid length
     * \note Upper-case caracters are automatically changed to lower case.
     *
     * \param text The string to validate (the contents of the line edit).
     * \param pos The position of the cursor.
     * \return \c Acceptable if the given string is valid (right length and valid characters) or empty,
     *         \c Invalid if the given string contains invalid characters (on Windows), and
     *         \c Intermediate otherwise.
     * \sa getValidAlgoritms()
     */
    State validate(QString& text, int& pos) const override;

    /*!
     * \brief Add a hashing algorithm
     *
     * Add an new hash algorithm returning a checksum of given length.
     * \param data Data associated to algorithm.
     * \param numBits Number of bits of checksum.
     * \sa getValidAlgoritms()
     */
    inline void addAlgorithm(const QVariant& data, int numBits) {mAlgorithms.insertMulti(numBits, data);}
    /*!
     * \brief Get valid algoritms
     *
     * Get data for algoriths which can return such a checksum.
     * \warning Only lentgh of checksum is tested, the text should be validated before.
     * \param text The validated string (the contents of the line edit).
     * \return The data corresponding to algorithms which can return such a checksum.
     * \sa addAlgorithm(), validate()
     */
    QList<QVariant> getValidAlgoritms(const QString& text) const;
private:
    QMap<int, QVariant> mAlgorithms; /*!< The map of algorithms: Lengthes of checksums and corresponding algorithm data */
};

#endif // CHECKSUMVALIDATOR_H
