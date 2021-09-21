/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qplatformdefs.h"
#include "qtextcodec.h"
#include "qtextcodec_p.h"

#ifndef QT_NO_TEXTCODEC

#include "qlist.h"
#include "qfile.h"
#include "qvarlengtharray.h"
#include "qstringlist.h"
#include "qlocale.h"
#include "qlocale_tools_p.h"
#include "qmutex.h"
#include "qhash.h"
#include "qicucodec_p.h"

#include <stdlib.h>
#include <ctype.h>
#include <locale.h>
#include <langinfo.h>
#include <unicode/uclean.h>

// enabling this is not exception safe!
// #define Q_DEBUG_TEXTCODEC

QT_BEGIN_NAMESPACE

static inline bool nameMatch(const QByteArray &name, const QByteArray &name2)
{
    if (ucnv_compareNames(name.constData(), name2.constData()) == 0) {
        return true;
    }
    UErrorCode error = U_ZERO_ERROR;
    const char *iana = ucnv_getStandardName(name.constData(), "IANA", &error);
    if (Q_UNLIKELY(U_FAILURE(error) || !iana)) {
        return false;
    }
    return (ucnv_compareNames(iana, name2.constData()) == 0);
}

static QTextCodec *localeMapper = nullptr;
QTextCodec *QTextCodec::cftr = nullptr;
#ifndef QT_NO_THREAD
Q_GLOBAL_STATIC(QMutex, textCodecsMutex)
#endif

QTextCodec* qt_localeMapper()
{
    return localeMapper;
}

class QTextCodecCleanup : public QList<QTextCodec*>
{
public:
    ~QTextCodecCleanup();
};

/*
    Deletes all the created codecs. This destructor is called just
    before exiting to delete any QTextCodec objects that may be lying
    around.
*/
QTextCodecCleanup::~QTextCodecCleanup()
{
    while (begin() != end()) {
        delete *begin();
    }
    localeMapper = nullptr;
#ifndef QT_NO_DEBUG
    u_cleanup();
#endif
}
Q_GLOBAL_STATIC(QTextCodecCleanup, qGlobalQTextCodec)

static QTextCodec *checkForCodec(const QByteArray &name) {
    QTextCodec *c = QTextCodec::codecForName(name);
    if (!c) {
        const int index = name.indexOf('@');
        if (index != -1) {
            c = QTextCodec::codecForName(name.left(index));
        }
    }
    return c;
}

static void setupLocaleMapper()
{
    Q_ASSERT(!localeMapper);

    // Get the first non-empty value from $LC_ALL, $LC_CTYPE, and $LANG
    // environment variables.
    QByteArray lang = qgetenv("LC_ALL");
    if (lang.isEmpty()) {
        lang = qgetenv("LC_CTYPE");
    }
    if (lang.isEmpty()) {
        lang = qgetenv("LANG");
    }

    const int indexOfDot = lang.indexOf('.');
    if (indexOfDot != -1) {
        localeMapper = checkForCodec(lang.mid(indexOfDot + 1));
    }

    if (!localeMapper && !lang.isEmpty()) {
        localeMapper = checkForCodec(lang);
    }

    // Fallback to implementation-defined default locale
    if (!localeMapper) {
        const QByteArray charset = ::nl_langinfo(CODESET); // deep copy
        localeMapper = QTextCodec::codecForName(charset);
    }

    // nl_langinfo() is documented to return empty string only if its argument
    // is not valid
    if (!localeMapper) {
        qFatal("Could not detect codec for locale");
    }
}

/*!
    \enum QTextCodec::ConversionFlag

    \value DefaultConversion  No flag is set.
    \value ConvertInvalidToNull  If this flag is set, each invalid input
                                 character is output as a null character.
    \value IgnoreHeader  Ignore any Unicode byte-order mark and don't generate any.
*/

/*!
    \fn QTextCodec::ConverterState::ConverterState(ConversionFlags flags)

    Constructs a ConverterState object initialized with the given \a flags.
*/

/*!
    Destroys the ConverterState object.
*/
QTextCodec::ConverterState::~ConverterState()
{
    if (d) {
        ucnv_close(static_cast<UConverter*>(d));
    }
}

QTextCodec::ConverterState::ConverterState(const QTextCodec::ConverterState &other)
{
    QTextCodec::ConverterState::operator=(other);
}

QTextCodec::ConverterState& QTextCodec::ConverterState::operator=(const QTextCodec::ConverterState &other)
{
    flags = other.flags;
    invalidChars = other.invalidChars;
    if (d) {
        ucnv_close(static_cast<UConverter*>(d));
        d = nullptr;
    }
    if (other.d) {
        UErrorCode error = U_ZERO_ERROR;
        d = ucnv_safeClone(static_cast<UConverter*>(other.d), nullptr, nullptr, &error);
        if (Q_UNLIKELY(U_FAILURE(error))) {
#ifdef Q_DEBUG_TEXTCODEC
            qWarning("ConverterState: ucnv_safeClone() failed %s", u_errorName(error));
#endif
            d = nullptr;
        }
    }
    return *this;
}

/*!
    \class QTextCodec
    \brief The QTextCodec class provides conversions between text encodings.
    \reentrant
    \ingroup i18n

    Qt uses Unicode to store, draw and manipulate strings. In many
    situations you may wish to deal with data that uses a different
    encoding. For example, most Japanese documents are still stored
    in Shift-JIS or ISO 2022-JP, while Russian users often have their
    documents in KOI8-R or Windows-1251.

    Qt provides a set of QTextCodec classes to help with converting
    non-Unicode formats to and from Unicode. You can also create your
    own codec classes.

    Some of the supported encodings are:

    \list
    \o Adobe-Standard-Encoding
    \o ANSI_X3.4-1968
    \o Big5
    \o Big5-HKSCS
    \o BOCU-1
    \o CESU-8
    \o EUC-KR
    \o Extended_UNIX_Code_Packed_Format_for_Japanese
    \o gb18030
    \o GB2312
    \o GB_2312-80
    \o GBK
    \o hp-roman8
    \o HZ-GB-2312
    \o IBM00858
    \o IBM01140
    \o IBM01141
    \o IBM01142
    \o IBM01143
    \o IBM01144
    \o IBM01145
    \o IBM01146
    \o IBM01147
    \o IBM01148
    \o IBM01149
    \o IBM037
    \o IBM1026
    \o IBM1047
    \o IBM273
    \o IBM277
    \o IBM278
    \o IBM280
    \o IBM284
    \o IBM285
    \o IBM290
    \o IBM297
    \o IBM420
    \o IBM424
    \o IBM437
    \o IBM500
    \o IBM775
    \o IBM850
    \o IBM851
    \o IBM852
    \o IBM855
    \o IBM857
    \o IBM860
    \o IBM861
    \o IBM862
    \o IBM863
    \o IBM864
    \o IBM865
    \o IBM866
    \o IBM868
    \o IBM869
    \o IBM870
    \o IBM871
    \o IBM918
    \o IBM-Thai
    \o ISO-2022-CN
    \o ISO-2022-CN-EXT
    \o ISO-2022-JP
    \o ISO-2022-JP-2
    \o ISO-2022-KR
    \o ISO-8859-10
    \o ISO_8859-1:1987
    \o ISO-8859-13
    \o ISO-8859-14
    \o ISO-8859-15
    \o ISO_8859-2:1987
    \o ISO_8859-3:1988
    \o ISO_8859-4:1988
    \o ISO_8859-5:1988
    \o ISO_8859-6:1987
    \o ISO_8859-7:1987
    \o ISO_8859-8:1988
    \o ISO_8859-9:1989
    \o JIS_Encoding
    \o KOI8-R
    \o KOI8-U
    \o KS_C_5601-1987
    \o macintosh
    \o SCSU
    \o Shift_JIS
    \o TIS-620
    \o UTF-16
    \o UTF-16BE
    \o UTF-16LE
    \o UTF-32
    \o UTF-32BE
    \o UTF-32LE
    \o UTF-7
    \o UTF-8
    \o windows-1250
    \o windows-1251
    \o windows-1252
    \o windows-1253
    \o windows-1254
    \o windows-1255
    \o windows-1256
    \o windows-1257
    \o windows-1258

    \o System Any of the above depending on the system locale
    \endlist

    QTextCodecs can be used as follows to convert some locally encoded
    string to Unicode. Suppose you have some string encoded in Russian
    KOI8-R encoding, and want to convert it to Unicode. The simple way
    to do it is like this:

    \snippet doc/src/snippets/code/src_corelib_codecs_qtextcodec.cpp 0

    After this, \c string holds the text converted to Unicode.
    Converting a string from Unicode to the local encoding is just as
    easy:

    \snippet doc/src/snippets/code/src_corelib_codecs_qtextcodec.cpp 1

    To read or write files in various encodings, use QTextStream and
    its \l{QTextStream::setCodec()}{setCodec()} function. See the
    \l{tools/codecs}{Codecs} example for an application of QTextCodec
    to file I/O.

    Some care must be taken when trying to convert the data in chunks,
    for example, when receiving it over a network. In such cases it is
    possible that a multi-byte character will be split over two
    chunks. At best this might result in the loss of a character and
    at worst cause the entire conversion to fail.

    The approach to use in these situations is to create a QTextDecoder
    object for the codec and use this QTextDecoder for the whole
    decoding process, as shown below:

    \snippet doc/src/snippets/code/src_corelib_codecs_qtextcodec.cpp 2

    The QTextDecoder object maintains state between chunks and therefore
    works correctly even if a multi-byte character is split between
    chunks.

    \section1 Creating Your Own Codec Class

    Support for new text encodings can be added to Qt by creating
    QTextCodec subclasses.

    The pure virtual functions describe the encoder to the system and
    the coder is used as required in the different text file formats
    supported by QTextStream, and under X11, for the locale-specific
    character input and output.

    To add support for another encoding to Qt, make a subclass of
    QTextCodec and implement the functions listed in the table below.

    \table
    \header \o Function \o Description

    \row \o name()
         \o Returns the official name for the encoding. If the
            encoding is listed in the
            \l{IANA character-sets encoding file}, the name
            should be the preferred MIME name for the encoding.

    \row \o aliases()
         \o Returns a list of alternative names for the encoding.
            QTextCodec provides a default implementation that returns
            an empty list. For example, "ISO-8859-1" has "latin1",
            "CP819", "IBM819", and "iso-ir-100" as aliases.

    \row \o mibEnum()
         \o Return the MIB enum for the encoding if it is listed in
            the \l{IANA character-sets encoding file}.

    \row \o convertToUnicode()
         \o Converts an 8-bit character string to Unicode.

    \row \o convertFromUnicode()
         \o Converts a Unicode string to an 8-bit character string.
    \endtable

    You may find it more convenient to make your codec class
    available as a plugin; see \l{How to Create Qt Plugins} for
    details.

    \sa QTextStream, QTextDecoder, QTextEncoder, {Codecs Example}
*/

/*!
    Constructs a QTextCodec, and gives it the highest precedence. The
    QTextCodec should always be constructed on the heap (i.e. with \c
    new). Qt takes ownership and will delete it when the application
    terminates.
*/
QTextCodec::QTextCodec()
{
#ifndef QT_NO_THREAD
    QMutexLocker locker(textCodecsMutex());
#endif
    qGlobalQTextCodec()->append(this);
}


/*!
    \nonreentrant

    Destroys the QTextCodec. Note that you should not delete codecs
    yourself: once created they become Qt's responsibility.
*/
QTextCodec::~QTextCodec()
{
#ifndef QT_NO_THREAD
    QMutexLocker locker(textCodecsMutex());
#endif
    qGlobalQTextCodec()->removeAll(this);
}

/*!
    \fn QTextCodec *QTextCodec::codecForName(const char *name)

    Searches all installed QTextCodec objects and returns the one
    which best matches \a name; the match is case-insensitive. Returns
    0 if no codec matching the name \a name could be found.
*/

/*!
    Searches all installed QTextCodec objects and returns the one
    which best matches \a name; the match is case-insensitive. Returns
    0 if no codec matching the name \a name could be found.
*/
QTextCodec *QTextCodec::codecForName(const QByteArray &name)
{
    if (name.isEmpty())
        return nullptr;

    if (name == "System")
        return QTextCodec::codecForLocale();

    {
#ifndef QT_NO_THREAD
        QMutexLocker locker(textCodecsMutex());
#endif
        for (int i = 0; i < qGlobalQTextCodec()->size(); ++i) {
            QTextCodec *cursor = qGlobalQTextCodec()->at(i);
            if (nameMatch(name, cursor->name())) {
                return cursor;
            }
            foreach (const QByteArray &alias, cursor->aliases()) {
                if (nameMatch(name, alias)) {
                    return cursor;
                }
            }
        }
    }

    foreach(const QByteArray &codec, QIcuCodec::allCodecs()) {
        if (nameMatch(name, codec)) {
            return new QIcuCodec(name);
        }
    }

    return nullptr;
}


/*!
    Returns the QTextCodec which matches the \link
    QTextCodec::mibEnum() MIBenum\endlink \a mib.
*/
QTextCodec* QTextCodec::codecForMib(int mib)
{
    {
#ifndef QT_NO_THREAD
        QMutexLocker locker(textCodecsMutex());
#endif
        for (int i = 0; i < qGlobalQTextCodec()->size(); ++i) {
            QTextCodec *cursor = qGlobalQTextCodec()->at(i);
            if (cursor->mibEnum() == mib) {
                return cursor;
            }
        }
    }

    foreach(const int codec, QIcuCodec::allMibs()) {
        if (mib == codec) {
            return new QIcuCodec(mib);
        }
    }

    return nullptr;
}

/*!
    Returns the list of all available codecs, by name. Call
    QTextCodec::codecForName() to obtain the QTextCodec for the name.

    The list may contain many mentions of the same codec
    if the codec has aliases.

    \sa availableMibs(), name(), aliases()
*/
QList<QByteArray> QTextCodec::availableCodecs()
{
    static const QList<QByteArray> codecs = QList<QByteArray>()
        << "System"
        << QIcuCodec::allCodecs();
    return codecs;
}

/*!
    Returns the list of MIBs for all available codecs. Call
    QTextCodec::codecForMib() to obtain the QTextCodec for the MIB.

    \sa availableCodecs(), mibEnum()
*/
QList<int> QTextCodec::availableMibs()
{
    static const QList<int> codecs = QIcuCodec::allMibs();
    return codecs;
}

/*!
    Set the codec to \a c; this will be returned by
    codecForLocale(). If \a c is a null pointer, the codec is reset to
    the default.

    This might be needed for some applications that want to use their
    own mechanism for setting the locale.

    \sa codecForLocale()
*/
void QTextCodec::setCodecForLocale(QTextCodec *c)
{
    localeMapper = c;
    if (!localeMapper)
        setupLocaleMapper();
}

/*!
    Returns a pointer to the codec most suitable for this locale.
*/

QTextCodec* QTextCodec::codecForLocale()
{
    if (!localeMapper)
        setupLocaleMapper();

    return localeMapper;
}


/*!
    \fn QByteArray QTextCodec::name() const

    QTextCodec subclasses must reimplement this function. It returns
    the name of the encoding supported by the subclass.

    If the codec is registered as a character set in the
    \l{IANA character-sets encoding file} this method should
    return the preferred mime name for the codec if defined,
    otherwise its name.
*/

/*!
    \fn int QTextCodec::mibEnum() const

    Subclasses of QTextCodec must reimplement this function. It
    returns the MIBenum (see \l{IANA character-sets encoding file}
    for more information). It is important that each QTextCodec
    subclass returns the correct unique value for this function.
*/

/*!
  Subclasses can return a number of aliases for the codec in question.

  Standard aliases for codecs can be found in the
  \l{IANA character-sets encoding file}.
*/
QList<QByteArray> QTextCodec::aliases() const
{
    return QList<QByteArray>();
}

/*!
    \fn QString QTextCodec::convertToUnicode(const char *chars, int len,
                                             ConverterState *state) const

    QTextCodec subclasses must reimplement this function.

    Converts the first \a len characters of \a chars from the
    encoding of the subclass to Unicode, and returns the result in a
    QString.

    \a state can be 0, in which case the conversion is stateless and
    default conversion rules should be used. If state is not 0, the
    codec should save the state after the conversion in \a state, and
    adjust the invalidChars members of the struct.
*/

/*!
    \fn QByteArray QTextCodec::convertFromUnicode(const QChar *input, int number,
                                                  ConverterState *state) const

    QTextCodec subclasses must reimplement this function.

    Converts the first \a number of characters from the \a input array
    from Unicode to the encoding of the subclass, and returns the result
    in a QByteArray.

    \a state can be 0 in which case the conversion is stateless and
    default conversion rules should be used. If state is not 0, the
    codec should save the state after the conversion in \a state, and
    adjust the invalidChars members of the struct.
*/

/*!
    Creates a QTextDecoder with a specified \a flags to decode chunks
    of \c{char *} data to create chunks of Unicode data.

    The caller is responsible for deleting the returned object.

    \since 4.7
*/
QTextDecoder* QTextCodec::makeDecoder(QTextCodec::ConversionFlags flags) const
{
    return new QTextDecoder(this, flags);
}

/*!
    Creates a QTextEncoder with a specified \a flags to encode chunks
    of Unicode data as \c{char *} data.

    The caller is responsible for deleting the returned object.

    \since 4.7
*/
QTextEncoder* QTextCodec::makeEncoder(QTextCodec::ConversionFlags flags) const
{
    return new QTextEncoder(this, flags);
}

/*!
    \fn QByteArray QTextCodec::fromUnicode(const QChar *input, int number,
                                           ConverterState *state) const

    Converts the first \a number of characters from the \a input array
    from Unicode to the encoding of this codec, and returns the result
    in a QByteArray.

    The \a state of the convertor used is updated.
*/

/*!
    Converts \a str from Unicode to the encoding of this codec, and
    returns the result in a QByteArray.
*/
QByteArray QTextCodec::fromUnicode(const QString& str) const
{
    return convertFromUnicode(str.constData(), str.length(), nullptr);
}

/*!
    \fn QString QTextCodec::toUnicode(const char *input, int size,
                                      ConverterState *state) const

    Converts the first \a size characters from the \a input from the
    encoding of this codec to Unicode, and returns the result in a
    QString.

    The \a state of the convertor used is updated.
*/

/*!
    Converts \a a from the encoding of this codec to Unicode, and
    returns the result in a QString.
*/
QString QTextCodec::toUnicode(const QByteArray& a) const
{
    return convertToUnicode(a.constData(), a.length(), nullptr);
}

/*!
    Returns true if the Unicode character \a ch can be fully encoded
    with this codec; otherwise returns false.
*/
bool QTextCodec::canEncode(QChar ch) const
{
    ConverterState state;
    state.flags = ConvertInvalidToNull;
    convertFromUnicode(&ch, 1, &state);
    return (state.invalidChars == 0);
}

/*!
    \overload

    \a s contains the string being tested for encode-ability.
*/
bool QTextCodec::canEncode(const QString& s) const
{
    ConverterState state;
    state.flags = ConvertInvalidToNull;
    convertFromUnicode(s.constData(), s.length(), &state);
    return (state.invalidChars == 0);
}


/*!
    \overload

    \a chars contains the source characters.
*/
QString QTextCodec::toUnicode(const char *chars) const
{
    return convertToUnicode(chars, qstrlen(chars), nullptr);
}


/*!
    \class QTextEncoder
    \brief The QTextEncoder class provides a state-based encoder.
    \reentrant
    \ingroup i18n

    A text encoder converts text from Unicode into an encoded text format
    using a specific codec.

    The encoder converts Unicode into another format, remembering any
    state that is required between calls.

    \sa QTextCodec::makeEncoder(), QTextDecoder
*/

/*!
    \fn QTextEncoder::QTextEncoder(const QTextCodec *codec)

    Constructs a text encoder for the given \a codec.
*/

/*!
    Constructs a text encoder for the given \a codec and conversion \a flags.

    \since 4.7
*/
QTextEncoder::QTextEncoder(const QTextCodec *codec, QTextCodec::ConversionFlags flags)
    : c(codec)
{
    state.flags = flags;
}

/*!
    Destroys the encoder.
*/
QTextEncoder::~QTextEncoder()
{
}

/*! \internal
    \since 4.5
    Determines whether the eecoder encountered a failure while decoding the input. If
    an error was encountered, the produced result is undefined, and gets converted as according
    to the conversion flags.
 */
bool QTextEncoder::hasFailure() const
{
    return state.invalidChars != 0;
}

/*!
    Converts the Unicode string \a str into an encoded QByteArray.
*/
QByteArray QTextEncoder::fromUnicode(const QString& str)
{
    return c->fromUnicode(str.constData(), str.length(), &state);
}

/*!
    \overload

    Converts \a len characters (not bytes) from \a uc, and returns the
    result in a QByteArray.
*/
QByteArray QTextEncoder::fromUnicode(const QChar *uc, int len)
{
    return c->fromUnicode(uc, len, &state);
}


/*!
    \class QTextDecoder
    \brief The QTextDecoder class provides a state-based decoder.
    \reentrant
    \ingroup i18n

    A text decoder converts text from an encoded text format into Unicode
    using a specific codec.

    The decoder converts text in this format into Unicode, remembering any
    state that is required between calls.

    \sa QTextCodec::makeDecoder(), QTextEncoder
*/

/*!
    \fn QTextDecoder::QTextDecoder(const QTextCodec *codec)

    Constructs a text decoder for the given \a codec.
*/

/*!
    Constructs a text decoder for the given \a codec and conversion \a flags.

    \since 4.7
*/

QTextDecoder::QTextDecoder(const QTextCodec *codec, QTextCodec::ConversionFlags flags)
    : c(codec)
{
    state.flags = flags;
}

/*!
    Destroys the decoder.
*/
QTextDecoder::~QTextDecoder()
{
}

/*!
    \fn QString QTextDecoder::toUnicode(const char *chars, int len)

    Converts the first \a len bytes in \a chars to Unicode, returning
    the result.

    If not all characters are used (e.g. if only part of a multi-byte
    encoding is at the end of the characters), the decoder remembers
    enough state to continue with the next call to this function.
*/
QString QTextDecoder::toUnicode(const char *chars, int len)
{
    return c->toUnicode(chars, len, &state);
}


/*! \overload

    The converted string is returned in \a target.
 */
void QTextDecoder::toUnicode(QString *target, const char *chars, int len)
{
    Q_ASSERT(target);
    *target = c->toUnicode(chars, len, &state);
}


/*!
    \overload

    Converts the bytes in the byte array specified by \a ba to Unicode
    and returns the result.
*/
QString QTextDecoder::toUnicode(const QByteArray &ba)
{
    return c->toUnicode(ba.constData(), ba.length(), &state);
}


/*!
    \fn QTextCodec* QTextCodec::codecForTr()

    Returns the codec used by QObject::tr() on its argument. If this
    function returns 0 (the default), tr() assumes Latin-1.

    \sa setCodecForTr()
*/

/*!
    \fn void QTextCodec::setCodecForTr(QTextCodec *c)
    \nonreentrant

    Sets the codec used by QObject::tr() on its argument to \a c. If
    \a c is 0 (the default), tr() assumes Latin-1.

    If the literal quoted text in the program is not in the Latin-1
    encoding, this function can be used to set the appropriate
    encoding. For example, software developed by Korean programmers
    might use eucKR for all the text in the program, in which case the
    main() function might look like this:

    \snippet doc/src/snippets/code/src_corelib_codecs_qtextcodec.cpp 3

    Note that this is not the way to select the encoding that the \e
    user has chosen. For example, to convert an application containing
    literal English strings to Korean, all that is needed is for the
    English strings to be passed through tr() and for translation
    files to be loaded. For details of internationalization, see
    \l{Internationalization with Qt}.

    \sa codecForTr(), setCodecForCStrings()
*/


/*!
    \fn QTextCodec* QTextCodec::codecForCStrings()

    Returns the codec used by QString to convert to and from \c{const
    char *} and QByteArrays. If this function returns 0 (the default),
    QString assumes Latin-1.

    \sa setCodecForCStrings()
*/

/*!
    \fn void QTextCodec::setCodecForCStrings(QTextCodec *codec)
    \nonreentrant

    Sets the codec used by QString to convert to and from \c{const
    char *} and QByteArrays. If the \a codec is 0 (the default),
    QString assumes Latin-1.

    \warning Some codecs do not preserve the characters in the ASCII
    range (0x00 to 0x7F). For example, the Japanese Shift-JIS
    encoding maps the backslash character (0x5A) to the Yen
    character. To avoid undesirable side-effects, we recommend
    avoiding such codecs with setCodecsForCString().

    \sa codecForCStrings(), setCodecForTr()
*/

/*!
    \since 4.4

    Tries to detect the encoding of the provided snippet of HTML in
    the given byte array, \a ba, by checking the BOM (Byte Order Mark)
    and the content-type meta header and returns a QTextCodec instance
    that is capable of decoding the html to unicode.  If the codec
    cannot be detected from the content provided, \a defaultCodec is
    returned.

    \sa codecForText(), codecForUtfText()
*/
QTextCodec *QTextCodec::codecForHtml(const QByteArray &ba, QTextCodec *defaultCodec)
{
    // determine charset
    QTextCodec *c = QTextCodec::codecForUtfText(ba, nullptr);
    if (!c) {
        QByteArray header = ba.left(512).toLower();
        int pos = header.indexOf("http-equiv=");
        if (pos != -1) {
            pos = header.lastIndexOf("meta ", pos);
            if (pos != -1) {
                pos = header.indexOf("charset=", pos) + qstrlen("charset=");
                if (pos != -1) {
                    int pos2 = header.indexOf('\"', pos+1);
                    QByteArray cs = header.mid(pos, pos2-pos);
                    //            qDebug("found charset: %s", cs.data());
                    return QTextCodec::codecForName(cs);
                }
            }
        }
    }
    return QTextCodec::codecForText(ba, defaultCodec);;
}

/*!
    \overload

    Tries to detect the encoding of the provided snippet of HTML in
    the given byte array, \a ba, by checking the BOM (Byte Order Mark)
    and the content-type meta header and returns a QTextCodec instance
    that is capable of decoding the html to unicode. If the codec cannot
    be detected, this overload returns a Latin-1 QTextCodec.

    \sa codecForText(), codecForUtfText()
*/
QTextCodec *QTextCodec::codecForHtml(const QByteArray &ba)
{
    return codecForHtml(ba, QTextCodec::codecForMib(/*Latin 1*/ 4));
}

/*!
    \since 4.6

    Tries to detect the encoding of the provided snippet \a ba by
    using the BOM (Byte Order Mark) and returns a QTextCodec instance
    that is capable of decoding the text to unicode. If the codec
    cannot be detected from the content provided, \a defaultCodec is
    returned.

    \sa codecForText(), codecForHtml()
*/
QTextCodec *QTextCodec::codecForUtfText(const QByteArray &ba, QTextCodec *defaultCodec)
{
    return QIcuCodec::codecForUtf(ba, defaultCodec);
}

/*!
    \overload

    Tries to detect the encoding of the provided snippet \a ba by
    using the BOM (Byte Order Mark) and returns a QTextCodec instance
    that is capable of decoding the text to unicode. If the codec
    cannot be detected, this overload returns a Latin-1 QTextCodec.

    \sa codecForText(), codecForHtml()
*/
QTextCodec *QTextCodec::codecForUtfText(const QByteArray &ba)
{
    return codecForUtfText(ba, QTextCodec::codecForMib(/*Latin 1*/ 4));
}


/*!
    \since 4.9

    Tries to detect the encoding of the provided snippet \a ba and
    returns a QTextCodec instance that is capable of decoding the text
    to unicode. If the codec cannot be detected from the content
    provided, \a defaultCodec is returned. The results can not be
    guaranteed to always be correct.

    \sa codecForUtfText(), codecForHtml()
*/
QTextCodec *QTextCodec::codecForText(const QByteArray &ba, QTextCodec *defaultCodec)
{
    return QIcuCodec::codecForData(ba, defaultCodec);
}

/*!
    \overload

    Tries to detect the encoding of the provided snippet \a ba by
    returns a QTextCodec instance that is capable of decoding the
    text to unicode. If the codec cannot be detected, this overload
    returns a Latin-1 QTextCodec. The results can not be guaranteed
    to always be correct.

    \sa codecForUtfText(), codecForHtml()
*/
QTextCodec *QTextCodec::codecForText(const QByteArray &ba)
{
    return codecForText(ba, QTextCodec::codecForMib(/*Latin 1*/ 4));
}

/*! \internal
    \since 4.3
    Determines whether the decoder encountered a failure while decoding the input. If
    an error was encountered, the produced result is undefined, and gets converted as according
    to the conversion flags.
 */
bool QTextDecoder::hasFailure() const
{
    return state.invalidChars != 0;
}

/*!
    \fn QTextCodec *QTextCodec::codecForContent(const char *str, int size)

    This functionality is no longer provided by Qt. This
    compatibility function always returns a null pointer.
*/

/*!
    \fn QTextCodec *QTextCodec::codecForName(const char *hint, int accuracy)

    Use the codecForName(const QByteArray &) overload instead.
*/

/*!
    \fn QTextCodec *QTextCodec::codecForIndex(int i)

    Use availableCodecs() or availableMibs() instead and iterate
    through the resulting list.
*/


/*!
    \fn QByteArray QTextCodec::mimeName() const

    Use name() instead.
*/

QT_END_NAMESPACE

#endif // QT_NO_TEXTCODEC
