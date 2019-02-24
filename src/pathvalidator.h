#ifndef PATHVALIDATOR_H
#define PATHVALIDATOR_H

#include <QValidator>

class PathValidator : public QValidator
{
public:
    /*!
     * \enum Accept
     * \brief Accetable input
     *
     * Each of the values of this enum describes an acceptable input.
     * \sa Accepts
     */
    enum Accept {
        AcceptsNone = 0x0,          /*!< No accepts flags */
        AcceptsFiles = 0x1,         /*!< Accepts a file path */
        AcceptsDirectories = 0x2,   /*!< Accepts a folder path */
        RequireReadable = 0x10,     /*!< Require the path to be readable */
        RequireWritable = 0x20,     /*!< Require the path to be writable */
        RequireExecutable = 0x40,   /*!< Require the path to be executable */
        AcceptEmpty = 0x100,        /*!< Accepts an empty string */
        AcceptNew = 0x200,          /*!< Accepts a non-existing path */
    };

    /*!
     * \typedef Accepts
     * \brief Acceptable input flags
     *
     * An OR-ed combination of the flags in emum Accept.
     */
    Q_DECLARE_FLAGS(Accepts, Accept);
#ifdef DOXYGEN
    typedef QFlags<Accept> Accepts;
#endif

    /*!
     * \brief Constructor
     *
     * Initializes a new path validator instance with given Accepts flags.
     *
     * \param accepts Conditions which must be satisfied for an input to be \c Acceptable.
     * \param parent The parent of this object
     */
    PathValidator(Accepts accepts = AcceptsNone, QObject* parent = nullptr);
    /*!
     * \brief The validator function
     *
     * Validates the given string with the current rules
     *  \li If contents if empty and AcceptEmpty is set, the path is validated
     *  \li Expand the macro variables
     *  \li If path does not exist, the path is validated depending on AcceptNew
     *  \li Calls validateName(), validateType(), validatePermissions() in order and AND them.
     *
     * \param text The string to validate (the contents of the line edit).
     * \param pos The position of the cursor.
     * \return \c Acceptable if the given string is valid,
     *         \c Invalid if the given string contains invalid characters (on Windows), and
     *         \c Intermediate otherwise.
     * \sa validateName(), validateType(), validatePermissions()
     */
    State validate(QString& text, int& pos) const override;

    /*!
     * \brief Set required extensions
     *
     * The selected file must have one extension in this set (if this set contains one).
     * \note Extensions must be given without any leading character
     * (for executable files on windows, give <tt>"exe"</tt>).
     * \param extensions The valid extensions for the chosen file.
     * \sa addRequiredExtension(), addRequiredExtensions(), clearRequiredExtensions(), requiredExtensions()
     */
    inline void setRequiredExtensions(const QStringList& extensions) {mRequiredExtensions = extensions;}
    /*!
     * \brief Clear required extensions
     *
     * Removes all required extensions currently set.
     * This let the user choose a file with any extension.
     * \sa setRequiredExtensions(), addRequiredExtension(), addRequiredExtensions(), requiredExtensions()
     */
    inline void clearRequiredExtensions(void) {mRequiredExtensions.clear();}
    /*!
     * \brief Add a required extension
     *
     * The given extension is added to the list of required extensions.
     * \note Extension must be given without any leading character
     * (for executable files on windows, give <tt>"exe"</tt>).
     * \param extension The extension to add to the valid extensions set.
     * \sa addRequiredExtensions(), setRequiredExtensions(), clearRequiredExtensions(), requiredExtensions()
     */
    inline void addRequiredExtension(const QString& extension) {mRequiredExtensions << extension;}
    /*!
     * \brief Add required extensionq
     *
     * The given extension list is merged with the list of required extensions.
     * \note Extensions must be given without any leading character
     * (for executable files on windows, give <tt>"exe"</tt>).
     * \param extensions The list of extensions to add to the valid extensions set.
     * \sa addRequiredExtension(), setRequiredExtensions(), clearRequiredExtensions(), requiredExtensions()
     */
    inline void addRequiredExtensions(const QString& extensions) {mRequiredExtensions.append(extensions);}
    /*!
     * \brief The list of required extensions
     *
     * Returns the list of valid extensions, this line edit will accept.
     * \return The list of valid extensions.
     * \sa setRequiredExtensions(), addRequiredExtensions(), addRequiredExtensions(), clearRequiredExtensions(), requiredExtensions()
     */
    inline QStringList requiredExtensions(void) const {return mRequiredExtensions;}

    /*!
     * \brief Accect files
     *
     * This function sets whether file paths are accepted by the validator.
     * \param accept Whether file paths are accepted.
     * \sa acceptFiles(), setAcceptDirectories()
     */
    inline void setAcceptFiles(bool accept) {manageAcceptFlags(AcceptsFiles, accept);}
    /*!
     * \brief Does it accept files?
     *
     * This function tells whether the validator accepts file paths or not.
     * \return Whether the validator accepts file paths
     * \sa setAcceptFiles(), acceptDirectories()
     */
    inline bool acceptFiles(void) const {return mAccepted & AcceptsFiles;}
    /*!
     * \brief Accect folders
     *
     * This function sets whether folder paths are accepted by the validator.
     * \param accept Whether folder paths are accepted.
     * \sa acceptDirectories(), setAcceptFiles()
     */
    inline void setAcceptDirectories(bool accept) {manageAcceptFlags(AcceptsDirectories, accept);}
    /*!
     * \brief Does it accept folders?
     *
     * This function tells whether the validator accepts folder paths or not.
     * \return Whether the validator accepts folder paths
     * \sa setAcceptDirectories(), acceptFiles()
     */
    inline bool acceptDirectories(void) const {return mAccepted & AcceptsDirectories;}

    /*!
     * \brief Requires paths to be readable
     *
     * This function sets the validator requirements concerning path readability.
     * \param require Whether readable paths are required by the validator.
     * \sa setRequireWritable(), setRequireExecutable(), requiredPermissions()
     */
    inline void setRequireReadable(bool require) {manageAcceptFlags(RequireReadable, require);}
    /*!
     * \brief Requires paths to be writable
     *
     * This function sets the validator requirements concerning path writability.
     * \param require Whether writable paths are required by the validator.
     * \sa setRequireReadable(), setRequireExecutable(), requiredPermissions()
     */
    inline void setRequireWritable(bool require) {manageAcceptFlags(RequireWritable, require);}
    /*!
     * \brief Requires paths to be executable
     *
     * This function sets the validator requirements concerning path executability.
     * \param require Whether executable paths are required by the validator.
     * \sa setRequireReadable(), setRequireWritable(), requiredPermissions()
     */
    inline void setRequireExecutable(bool require) {manageAcceptFlags(RequireExecutable, require);}
    /*!
     * \brief Gives permission requirements
     *
     * This function returns the permission requirement of the validator.
     * \return The permission requirements of the validator
     * \sa setRequireReadable(), setRequireWritable(), setRequireExecutable()
     */
    inline Accepts requiredPermissions(void) const {return mAccepted & Accepts(RequireReadable | RequireWritable | RequireExecutable);}

    /*!
     * \brief Accepts empty contents
     *
     * This function set whether empty paths are accepted by the validator.
     * \param accept Whether the validator accepts empty paths
     * \sa acceptEmpty()
     */
    inline void setAcceptEmpty(bool accept) {manageAcceptFlags(AcceptEmpty, accept);}
    /*!
     * \brief Does it accept empty paths?
     *
     * This function says whether empty paths are accepted by the validator.
     * \return Whether the validator accepts empty paths.
     * \sa setAcceptEmpty()
     */
    inline bool acceptEmpty(void) const {return mAccepted & AcceptEmpty;}
    /*!
     * \brief Accepts new paths
     *
     * This function set whether non-existing paths are accepted by the validator.
     * \param accept Whether the validator accepts non-existing paths
     * \sa acceptNew()
     */
    inline void setAcceptNew(bool accept) {manageAcceptFlags(AcceptNew, accept);}
    /*!
     * \brief Does it accept new paths?
     *
     * This function says whether non-existing paths are accepted by the validator.
     * \return Whether the validator accepts non-existing paths.
     * \sa setAcceptNew()
     */
    inline bool acceptNew(void) const {return mAccepted & AcceptNew;}

private:
    /*!
     * \brief Check name and extension of the given path.
     *
     * Check name and extension of the given path:
     *  \li If requiredExtensions() list is empty, the path is valid
     *  \li Othewise, the path is valid only if it ends with a dot followed by one of the extensions.
     *
     * \param text The path to validate.
     * \param pos The position of the cursor.
     * \return \c true if the given string is valid \c false otherwise.
     * \sa validate()
     */
    bool validateName(QString& text, int& pos) const;
    /*!
     * \brief Check the type of the given path.
     *
     * Check that the type of the given path corresponds to an acceptable type
     * (\see acceptFiles(), acceptDirectories())
     * \param text The path to validate.
     * \param pos The position of the cursor.
     * \return \c true if the given string is valid \c false otherwise.
     * \sa validate()
     */
    bool validateType(QString& text, int& pos) const;
    /*!
     * \brief Check permissions of the given path.
     *
     * Check that the requirements on the permission (\see requiredPermissions())
     * of the given path are enforced.
     * \param text The path to validate.
     * \param pos The position of the cursor.
     * \return \c true if the given string is valid \c false otherwise.
     * \sa validate()
     */
    bool validatePermissions(QString& text, int& pos) const;

    /*!
     * \brief Manage accept flags
     *
     * Helper function for the helper flag management.
     * \param flag The accept flag to change.
     * \param enable Whether to enable or disable it.
     */
    void manageAcceptFlags(Accept flag, bool enable);

    QStringList mRequiredExtensions;        /*!< The internal list of required extensions */
    Accepts mAccepted;                      /*!< Accept flags */
};

#endif // PATHVALIDATOR_H
