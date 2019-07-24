/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

// Don't define it while compiling this module, or USERS of Qt will
// not be able to link.
#ifdef QT_NO_CAST_FROM_ASCII
#  undef QT_NO_CAST_FROM_ASCII
#endif
#ifdef QT_NO_CAST_TO_ASCII
#  undef QT_NO_CAST_TO_ASCII
#endif
#include "qchar.h"
#include "qdatastream.h"
#include "qtextcodec.h"
#include "qunicodetables_p.h"

#include "utf8proc.h"

QT_BEGIN_NAMESPACE

#ifndef QT_NO_CODEC_FOR_C_STRINGS
#  ifdef QT_NO_TEXTCODEC
#    define QT_NO_CODEC_FOR_C_STRINGS
#  endif
#endif

static inline bool is_ascii_char(uint ucs4)
{
    return ucs4 <= 127;
}

static inline bool is_ascii_number(uint ucs4)
{
    return (ucs4 >= '0' && ucs4 <= '9');
}

static inline bool is_ascii_lower(uint ucs4)
{
    return (ucs4 >= 'a' && ucs4 <= 'z');
}

static inline bool is_ascii_upper(uint ucs4)
{
    return (ucs4 >= 'A' && ucs4 <= 'Z');
}

static inline bool is_ascii_letterornumber(uint ucs4)
{
    return (ucs4 >= 'a' && ucs4 <= 'z') || (ucs4 >= 'A' && ucs4 <= 'Z') || (ucs4 >= '0' && ucs4 <= '9');
}

static inline uint to_ascii_lower(uint ucs4)
{
    switch (ucs4) {
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
        case 'T':
        case 'U':
        case 'V':
        case 'W':
        case 'X':
        case 'Y':
        case 'Z':
            return ucs4 + 32;
        default:
            return ucs4;
    }
    return ucs4;
}

static inline uint to_ascii_upper(uint ucs4)
{
    switch (ucs4) {
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':
            return ucs4 - 32;
        default:
            return ucs4;
    }
    return ucs4;
}

/*!
    \class QLatin1Char
    \brief The QLatin1Char class provides an 8-bit ASCII/Latin-1 character.

    \ingroup string-processing

    This class is only useful to avoid the codec for C strings business
    in the QChar(ch) constructor. You can avoid it by writing
    QChar(ch, 0).

    \sa QChar, QLatin1String, QString
*/

/*!
    \fn const char QLatin1Char::toLatin1() const

    Converts a Latin-1 character to an 8-bit ASCII representation of
    the character.
*/

/*!
    \fn const ushort QLatin1Char::unicode() const

    Converts a Latin-1 character to an 16-bit-encoded Unicode representation
    of the character.
*/

/*!
    \fn QLatin1Char::QLatin1Char(char c)

    Constructs a Latin-1 character for \a c. This constructor should be
    used when the encoding of the input character is known to be Latin-1.
*/

/*!
    \class QChar
    \brief The QChar class provides a 16-bit Unicode character.

    \ingroup string-processing
    \reentrant

    In Qt, Unicode characters are 16-bit entities without any markup
    or structure. This class represents such an entity. It is
    lightweight, so it can be used everywhere. Most compilers treat
    it like a \c{unsigned short}.

    QChar provides a full complement of testing/classification
    functions, converting to and from other formats, converting from
    composed to decomposed Unicode, and trying to compare and
    case-convert if you ask it to.

    The classification functions include functions like those in the
    standard C++ header \<cctype\> (formerly \<ctype.h\>), but
    operating on the full range of Unicode characters. They all
    return true if the character is a certain type of character;
    otherwise they return false. These classification functions are
    isNull() (returns true if the character is '\\0'), isPrint()
    (true if the character is any sort of printable character,
    including whitespace), isPunct() (any sort of punctation),
    isMark() (Unicode Mark), isLetter() (a letter), isNumber() (any
    sort of numeric character, not just 0-9), isLetterOrNumber(), and
    isDigit() (decimal digits). All of these are wrappers around
    category() which return the Unicode-defined category of each
    character.

    QChar also provides direction(), which indicates the "natural"
    writing direction of this character. The joining() function
    indicates how the character joins with its neighbors (needed
    mostly for Arabic) and finally hasMirrored(), which indicates
    whether the character needs to be mirrored when it is printed in
    its "unnatural" writing direction.

    Composed Unicode characters (like \aring) can be converted to
    decomposed Unicode ("a" followed by "ring above") by using
    decomposition().

    In Unicode, comparison is not necessarily possible and case
    conversion is very difficult at best. Unicode, covering the
    "entire" world, also includes most of the world's case and
    sorting problems. operator==() and friends will do comparison
    based purely on the numeric Unicode value (code point) of the
    characters, and toUpper() and toLower() will do case changes when
    the character has a well-defined uppercase/lowercase equivalent.
    For locale-dependent comparisons, use
    QString::localeAwareCompare().

    The conversion functions include unicode() (to a scalar),
    toLatin1() (to scalar, but converts all non-Latin-1 characters to
    0), row() (gives the Unicode row), cell() (gives the Unicode
    cell), digitValue() (gives the integer value of any of the
    numerous digit characters), and a host of constructors.

    QChar provides constructors and cast operators that make it easy
    to convert to and from traditional 8-bit \c{char}s. If you
    defined \c QT_NO_CAST_FROM_ASCII and \c QT_NO_CAST_TO_ASCII, as
    explained in the QString documentation, you will need to
    explicitly call fromAscii() or fromLatin1(), or use QLatin1Char,
    to construct a QChar from an 8-bit \c char, and you will need to
    call toAscii() or toLatin1() to get the 8-bit value back.

    \sa QString, Unicode, QLatin1Char
*/

/*!
    \enum QChar::UnicodeVersion

    Specifies which version of the \l{http://www.unicode.org/}{Unicode standard}
    introduced a certain character.

    \value Unicode_1_1
    \value Unicode_2_0
    \value Unicode_2_1
    \value Unicode_3_0
    \value Unicode_3_1
    \value Unicode_3_2
    \value Unicode_4_0
    \value Unicode_4_1
    \value Unicode_5_0
    \value Unicode_5_1
    \value Unicode_5_2
    \value Unicode_6_0
    \value Unicode_6_1
    \value Unicode_6_2
    \value Unicode_6_3
    \value Unicode_7_0
    \value Unicode_8_0
    \value Unicode_9_0
    \value Unicode_10_0
    \value Unicode_11_0
    \value Unicode_12_0
    \value Unicode_12_1
    \value Unicode_Last Latest supported version
    \value Unicode_Unassigned  The value is not assigned to any character
        in Unicode.

    \sa unicodeVersion()
*/

/*!
    \enum QChar::Category

    This enum maps the Unicode character categories.

    The following characters are normative in Unicode:

    \value Mark_NonSpacing  Unicode class name Mn

    \value Mark_SpacingCombining  Unicode class name Mc

    \value Mark_Enclosing  Unicode class name Me

    \value Number_DecimalDigit  Unicode class name Nd

    \value Number_Letter  Unicode class name Nl

    \value Number_Other  Unicode class name No

    \value Separator_Space  Unicode class name Zs

    \value Separator_Line  Unicode class name Zl

    \value Separator_Paragraph  Unicode class name Zp

    \value Other_Control  Unicode class name Cc

    \value Other_Format  Unicode class name Cf

    \value Other_Surrogate  Unicode class name Cs

    \value Other_PrivateUse  Unicode class name Co

    \value Other_NotAssigned  Unicode class name Cn


    The following categories are informative in Unicode:

    \value Letter_Uppercase  Unicode class name Lu

    \value Letter_Lowercase  Unicode class name Ll

    \value Letter_Titlecase  Unicode class name Lt

    \value Letter_Modifier  Unicode class name Lm

    \value Letter_Other Unicode class name Lo

    \value Punctuation_Connector  Unicode class name Pc

    \value Punctuation_Dash  Unicode class name Pd

    \value Punctuation_Open  Unicode class name Ps

    \value Punctuation_Close  Unicode class name Pe

    \value Punctuation_InitialQuote  Unicode class name Pi

    \value Punctuation_FinalQuote  Unicode class name Pf

    \value Punctuation_Other  Unicode class name Po

    \value Symbol_Math  Unicode class name Sm

    \value Symbol_Currency  Unicode class name Sc

    \value Symbol_Modifier  Unicode class name Sk

    \value Symbol_Other  Unicode class name So

    \sa category()
*/

/*!
    \enum QChar::Direction

    This enum type defines the Unicode direction attributes. See the
    \l{http://www.unicode.org/}{Unicode Standard} for a description
    of the values.

    In order to conform to C/C++ naming conventions "Dir" is prepended
    to the codes used in the Unicode Standard.

    \value DirAL
    \value DirAN
    \value DirB
    \value DirBN
    \value DirCS
    \value DirEN
    \value DirES
    \value DirET
    \value DirL
    \value DirLRE
    \value DirLRO
    \value DirNSM
    \value DirON
    \value DirPDF
    \value DirR
    \value DirRLE
    \value DirRLO
    \value DirS
    \value DirWS
    \value DirLRI
    \value DirRLI
    \value DirFSI
    \value DirPDI

    \sa direction()
*/

/*!
    \enum QChar::Decomposition

    This enum type defines the Unicode decomposition attributes. See
    the \l{http://www.unicode.org/}{Unicode Standard} for a
    description of the values.

    \value NoDecomposition
    \value Circle
    \value Compat
    \value Final
    \value Font
    \value Fraction
    \value Initial
    \value Isolated
    \value Medial
    \value Narrow
    \value NoBreak
    \value Small
    \value Square
    \value Sub
    \value Super
    \value Vertical
    \value Wide

    \sa decomposition()
*/

/*!
    \enum QChar::Joining

    This enum type defines the Unicode joining attributes. See the
    \l{http://www.unicode.org/}{Unicode Standard} for a description
    of the values.

    \value Dual
    \value OtherJoining
    \value Right
    \value Left
    \value Causing
    \value Transparent

    \sa joining()
*/

/*!
    \enum QChar::SpecialCharacter

    \value Null A QChar with this value isNull().
    \value Nbsp Non-breaking space.
    \value ReplacementCharacter The character shown when a font has no glyph
           for a certain codepoint. A special question mark character is often
           used. Codecs use this codepoint when input data cannot be
           represented in Unicode.
    \value ObjectReplacementCharacter Used to represent an object such as an
           image when such objects cannot be presented.
    \value ByteOrderMark
    \value ByteOrderSwapped
    \value ParagraphSeparator
    \value LineSeparator

    \omitvalue null
    \omitvalue replacement
    \omitvalue byteOrderMark
    \omitvalue byteOrderSwapped
    \omitvalue nbsp
*/

/*!
    \fn void QChar::setCell(const uchar cell)
    \internal
*/

/*!
    \fn void QChar::setRow(const uchar row)
    \internal
*/

/*!
    \fn QChar::QChar()

    Constructs a null QChar ('\\0').

    \sa isNull()
*/

/*!
    \fn QChar::QChar(const QLatin1Char ch)

    Constructs a QChar corresponding to ASCII/Latin-1 character \a ch.
*/

/*!
    \fn QChar::QChar(const SpecialCharacter ch)

    Constructs a QChar for the predefined character value \a ch.
*/

/*!
    Constructs a QChar corresponding to ASCII/Latin-1 character \a
    ch.
*/
QChar::QChar(const char ch)
{
#ifndef QT_NO_CODEC_FOR_C_STRINGS
    if (QTextCodec::codecForCStrings())
        // #####
        ucs =  QTextCodec::codecForCStrings()->toUnicode(&ch, 1).at(0).unicode();
    else
#endif
        ucs = uchar(ch);
}

/*!
    Constructs a QChar corresponding to ASCII/Latin-1 character \a ch.
*/
QChar::QChar(const uchar ch)
{
#ifndef QT_NO_CODEC_FOR_C_STRINGS
    if (QTextCodec::codecForCStrings()) {
        // #####
        const char c = char(ch);
        ucs =  QTextCodec::codecForCStrings()->toUnicode(&c, 1).at(0).unicode();
    } else
#endif
        ucs = ch;
}

/*!
    \fn QChar::QChar(const uchar cell, const uchar row)

    Constructs a QChar for Unicode cell \a cell in row \a row.

    \sa cell(), row()
*/

/*!
    \fn QChar::QChar(uconst short code)

    Constructs a QChar for the character with Unicode code point \a
    code.
*/


/*!
    \fn QChar::QChar(const short code)

    Constructs a QChar for the character with Unicode code point \a
    code.
*/


/*!
    \fn QChar::QChar(const uint code)

    Constructs a QChar for the character with Unicode code point \a
    code.
*/


/*!
    \fn QChar::QChar(const int code)

    Constructs a QChar for the character with Unicode code point \a
    code.
*/


/*!
    \fn bool QChar::isNull() const

    Returns true if the character is the Unicode character 0x0000
    ('\\0'); otherwise returns false.
*/

/*!
    \fn uchar QChar::cell() const

    Returns the cell (least significant byte) of the Unicode
    character.

    \sa row()
*/

/*!
    \fn uchar QChar::row() const

    Returns the row (most significant byte) of the Unicode character.

    \sa cell()
*/

/*!
    Returns true if the character is a printable character; otherwise
    returns false. This is any character not of category Cc or Cn.

    Note that this gives no indication of whether the character is
    available in a particular font.
*/
bool QChar::isPrint() const
{
    const utf8proc_category_t category = utf8proc_category(ucs);
    switch (category) {
        case UTF8PROC_CATEGORY_CC:
        case UTF8PROC_CATEGORY_CN:
            return false;
        default:
            return true;
    }
    return true;
}

/*!
    Returns true if the character is a separator character
    (Separator_* categories); otherwise returns false.
*/
bool QChar::isSpace() const
{
    if(ucs >= 9 && ucs <= 13)
        return true;

    const utf8proc_category_t category = utf8proc_category(ucs);
    switch (category) {
        case UTF8PROC_CATEGORY_ZS:
        case UTF8PROC_CATEGORY_ZL:
        case UTF8PROC_CATEGORY_ZP:
            return true;
        default:
            return false;
    }
    return false;
}

/*!
    Returns true if the character is a mark (Mark_* categories);
    otherwise returns false.

    See QChar::Category for more information regarding marks.
*/
bool QChar::isMark() const
{
    const utf8proc_category_t category = utf8proc_category(ucs);
    switch (category) {
        case UTF8PROC_CATEGORY_MN:
        case UTF8PROC_CATEGORY_MC:
        case UTF8PROC_CATEGORY_ME:
            return true;
        default:
            return false;
    }
    return false;
}

/*!
    Returns true if the character is a punctuation mark (Punctuation_*
    categories); otherwise returns false.
*/
bool QChar::isPunct() const
{
    const utf8proc_category_t category = utf8proc_category(ucs);
    switch (category) {
        case UTF8PROC_CATEGORY_PC:
        case UTF8PROC_CATEGORY_PD:
        case UTF8PROC_CATEGORY_PS:
        case UTF8PROC_CATEGORY_PE:
        case UTF8PROC_CATEGORY_PI:
        case UTF8PROC_CATEGORY_PF:
        case UTF8PROC_CATEGORY_PO:
            return true;
        default:
            return false;
    }
    return false;
}

/*!
    Returns true if the character is a letter (Letter_* categories);
    otherwise returns false.
*/
bool QChar::isLetter() const
{
    const utf8proc_category_t category = utf8proc_category(ucs);
    switch (category) {
        case UTF8PROC_CATEGORY_LU:
        case UTF8PROC_CATEGORY_LL:
        case UTF8PROC_CATEGORY_LT:
        case UTF8PROC_CATEGORY_LM:
        case UTF8PROC_CATEGORY_LO:
            return true;
        default:
            return false;
    }
    return false;
}

/*!
    Returns true if the character is a number (Number_* categories,
    not just 0-9); otherwise returns false.

    \sa isDigit()
*/
bool QChar::isNumber() const
{
    if (is_ascii_char(ucs)) {
        return is_ascii_number(ucs);
    }
    const utf8proc_category_t category = utf8proc_category(ucs);
    switch (category) {
        case UTF8PROC_CATEGORY_ND:
        case UTF8PROC_CATEGORY_NL:
        case UTF8PROC_CATEGORY_NO:
            return true;
        default:
            return false;
    }
    return false;
}

/*!
    Returns true if the character is a letter or number (Letter_* or
    Number_* categories); otherwise returns false.
*/
bool QChar::isLetterOrNumber() const
{
    if (is_ascii_char(ucs)) {
        return is_ascii_letterornumber(ucs);
    }
    const utf8proc_category_t category = utf8proc_category(ucs);
    switch (category) {
        // letter
        case UTF8PROC_CATEGORY_LU:
        case UTF8PROC_CATEGORY_LL:
        case UTF8PROC_CATEGORY_LT:
        case UTF8PROC_CATEGORY_LM:
        case UTF8PROC_CATEGORY_LO:
        // number
        case UTF8PROC_CATEGORY_ND:
        case UTF8PROC_CATEGORY_NL:
        case UTF8PROC_CATEGORY_NO:
            return true;
        default:
            return false;
    }
    return false;
}


/*!
    Returns true if the character is a decimal digit
    (Number_DecimalDigit); otherwise returns false.
*/
bool QChar::isDigit() const
{
    if (is_ascii_char(ucs)) {
        return is_ascii_number(ucs);
    }
    const utf8proc_category_t category = utf8proc_category(ucs);
    switch (category) {
        case UTF8PROC_CATEGORY_ND:
            return true;
        default:
            return false;
    }
    return false;
}


/*!
    Returns true if the character is a symbol (Symbol_* categories);
    otherwise returns false.
*/
bool QChar::isSymbol() const
{
    const utf8proc_category_t category = utf8proc_category(ucs);
    switch (category) {
        case UTF8PROC_CATEGORY_SM:
        case UTF8PROC_CATEGORY_SC:
        case UTF8PROC_CATEGORY_SK:
        case UTF8PROC_CATEGORY_SO:
            return true;
        default:
            return false;
    }
    return false;
}

/*!
    \fn bool QChar::isHighSurrogate() const

    Returns true if the QChar is the high part of a utf16 surrogate
    (ie. if its code point is between 0xd800 and 0xdbff, inclusive).
*/

/*!
    \fn bool QChar::isLowSurrogate() const

    Returns true if the QChar is the low part of a utf16 surrogate
    (ie. if its code point is between 0xdc00 and 0xdfff, inclusive).
*/

/*!
    \fn static bool QChar::isHighSurrogate(uint ucs4)
    \since 4.7

    Returns true if the UCS-4-encoded character specified by \a ucs4
    is the high part of a utf16 surrogate
    (ie. if its code point is between 0xd800 and 0xdbff, inclusive).
*/

/*!
    \fn static bool QChar::isLowSurrogate(uint ucs4)
    \since 4.7

    Returns true if the UCS-4-encoded character specified by \a ucs4
    is the low part of a utf16 surrogate
    (ie. if its code point is between 0xdc00 and 0xdfff, inclusive).
*/

/*!
    \fn static bool QChar::requiresSurrogates(uint ucs4)
    \since 4.7

    Returns true if the UCS-4-encoded character specified by \a ucs4
    can be split into the high and low parts of a utf16 surrogate
    (ie. if its code point is greater than or equals to 0x10000).
*/

/*!
    \fn static uint QChar::surrogateToUcs4(ushort high, ushort low)

    Converts a UTF16 surrogate pair with the given \a high and \a low values
    to its UCS-4 code point.
*/

/*!
    \fn static uint QChar::surrogateToUcs4(QChar high, QChar low)

    Converts a utf16 surrogate pair (\a high, \a low) to its ucs4 code point.
*/

/*!
    \fn static ushort QChar::highSurrogate(uint ucs4)

    Returns the high surrogate value of a ucs4 code point.
    The returned result is undefined if \a ucs4 is smaller than 0x10000.
*/

/*!
    \fn static ushort QChar::lowSurrogate(uint ucs4)

    Returns the low surrogate value of a ucs4 code point.
    The returned result is undefined if \a ucs4 is smaller than 0x10000.
*/

/*!
    Returns the numeric value of the digit, or -1 if the character is
    not a digit.
*/
int QChar::digitValue() const
{
    return QUnicodeTables::digitValue(ucs);
}

/*!
    \overload
    Returns the numeric value of the digit, specified by the UCS-2-encoded
    character, \a ucs2, or -1 if the character is not a digit.
*/
int QChar::digitValue(const ushort ucs2)
{
    return QUnicodeTables::digitValue(ucs2);
}

/*!
    \overload
    Returns the numeric value of the digit specified by the UCS-4-encoded
    character, \a ucs4, or -1 if the character is not a digit.
*/
int QChar::digitValue(const uint ucs4)
{
    return QUnicodeTables::digitValue(ucs4);
}

#define QCHAR_CATEGORY(c) \
    const utf8proc_category_t category = utf8proc_category(c); \
    switch (category) { \
        case UTF8PROC_CATEGORY_CN: \
            return QChar::Other_NotAssigned; \
        case UTF8PROC_CATEGORY_LU: \
            return QChar::Letter_Uppercase; \
        case UTF8PROC_CATEGORY_LL: \
            return QChar::Letter_Lowercase; \
        case UTF8PROC_CATEGORY_LT: \
            return QChar::Letter_Titlecase; \
        case UTF8PROC_CATEGORY_LM: \
            return QChar::Letter_Modifier; \
        case UTF8PROC_CATEGORY_LO: \
            return QChar::Letter_Other; \
        case UTF8PROC_CATEGORY_MN: \
            return QChar::Mark_NonSpacing; \
        case UTF8PROC_CATEGORY_MC: \
            return QChar::Mark_SpacingCombining; \
        case UTF8PROC_CATEGORY_ME: \
            return QChar::Mark_Enclosing; \
        case UTF8PROC_CATEGORY_ND: \
            return QChar::Number_DecimalDigit; \
        case UTF8PROC_CATEGORY_NL: \
            return QChar::Number_Letter; \
        case UTF8PROC_CATEGORY_NO: \
            return QChar::Number_Other; \
        case UTF8PROC_CATEGORY_PC: \
            return QChar::Punctuation_Connector; \
        case UTF8PROC_CATEGORY_PD: \
            return QChar::Punctuation_Dash; \
        case UTF8PROC_CATEGORY_PS: \
            return QChar::Punctuation_Open; \
        case UTF8PROC_CATEGORY_PE: \
            return QChar::Punctuation_Close; \
        case UTF8PROC_CATEGORY_PI: \
            return QChar::Punctuation_InitialQuote; \
        case UTF8PROC_CATEGORY_PF: \
            return QChar::Punctuation_FinalQuote; \
        case UTF8PROC_CATEGORY_PO: \
            return QChar::Punctuation_Other; \
        case UTF8PROC_CATEGORY_SM: \
            return QChar::Symbol_Math; \
        case UTF8PROC_CATEGORY_SC: \
            return QChar::Symbol_Currency; \
        case UTF8PROC_CATEGORY_SK: \
            return QChar::Symbol_Modifier; \
        case UTF8PROC_CATEGORY_SO: \
            return QChar::Symbol_Other; \
        case UTF8PROC_CATEGORY_ZS: \
            return QChar::Separator_Space; \
        case UTF8PROC_CATEGORY_ZL: \
            return QChar::Separator_Line; \
        case UTF8PROC_CATEGORY_ZP: \
            return QChar::Separator_Paragraph; \
        case UTF8PROC_CATEGORY_CC: \
            return QChar::Other_Control; \
        case UTF8PROC_CATEGORY_CF: \
            return QChar::Other_Format; \
        case UTF8PROC_CATEGORY_CS: \
            return QChar::Other_Surrogate; \
        case UTF8PROC_CATEGORY_CO: \
            return QChar::Other_PrivateUse; \
    } \
    return QChar::Other_NotAssigned;

/*!
    Returns the character's category.
*/
QChar::Category QChar::category() const
{
    QCHAR_CATEGORY(ucs)
}

/*!
    \overload
    \since 4.3
    Returns the category of the UCS-4-encoded character specified by \a ucs4.
*/
QChar::Category QChar::category(const uint ucs4)
{
    QCHAR_CATEGORY(ucs4)
}

/*!
    \overload
    Returns the category of the UCS-2-encoded character specified by \a ucs2.
*/
QChar::Category QChar::category(const ushort ucs2)
{
    QCHAR_CATEGORY(ucs2)
}

#define QCHAR_DIRECTION(c) \
    const utf8proc_property_t *property = utf8proc_get_property(c); \
    switch (property->bidi_class) { \
        case UTF8PROC_BIDI_CLASS_L: \
            return QChar::DirL; \
        case UTF8PROC_BIDI_CLASS_LRE: \
            return QChar::DirLRE; \
        case UTF8PROC_BIDI_CLASS_LRO: \
            return QChar::DirLRO; \
        case UTF8PROC_BIDI_CLASS_R: \
            return QChar::DirR; \
        case UTF8PROC_BIDI_CLASS_AL: \
            return QChar::DirAL; \
        case UTF8PROC_BIDI_CLASS_RLE: \
            return QChar::DirRLE; \
        case UTF8PROC_BIDI_CLASS_RLO: \
            return QChar::DirRLO; \
        case UTF8PROC_BIDI_CLASS_PDF: \
            return QChar::DirPDF; \
        case UTF8PROC_BIDI_CLASS_EN: \
            return QChar::DirEN; \
        case UTF8PROC_BIDI_CLASS_ES: \
            return QChar::DirES; \
        case UTF8PROC_BIDI_CLASS_ET: \
            return QChar::DirET; \
        case UTF8PROC_BIDI_CLASS_AN: \
            return QChar::DirAN; \
        case UTF8PROC_BIDI_CLASS_CS: \
            return QChar::DirCS; \
        case UTF8PROC_BIDI_CLASS_NSM: \
            return QChar::DirNSM; \
        case UTF8PROC_BIDI_CLASS_BN: \
            return QChar::DirBN; \
        case UTF8PROC_BIDI_CLASS_B: \
            return QChar::DirB; \
        case UTF8PROC_BIDI_CLASS_S: \
            return QChar::DirS; \
        case UTF8PROC_BIDI_CLASS_WS: \
            return QChar::DirWS; \
        case UTF8PROC_BIDI_CLASS_ON: \
            return QChar::DirON; \
        case UTF8PROC_BIDI_CLASS_LRI: \
            return QChar::DirLRI; \
        case UTF8PROC_BIDI_CLASS_RLI: \
            return QChar::DirRLI; \
        case UTF8PROC_BIDI_CLASS_FSI: \
            return QChar::DirFSI; \
        case UTF8PROC_BIDI_CLASS_PDI: \
            return QChar::DirPDI; \
    } \
    return QChar::DirL;

/*!
    Returns the character's direction.
*/
QChar::Direction QChar::direction() const
{
    QCHAR_DIRECTION(ucs)
}

/*!
    \overload
    Returns the direction of the UCS-4-encoded character specified by \a ucs4.
*/
QChar::Direction QChar::direction(const uint ucs4)
{
    QCHAR_DIRECTION(ucs4)
}

/*!
    \overload
    Returns the direction of the UCS-2-encoded character specified by \a ucs2.
*/
QChar::Direction QChar::direction(const ushort ucs2)
{
    QCHAR_DIRECTION(ucs2)
}

/*!
    Returns information about the joining properties of the character
    (needed for certain languages such as Arabic).
*/
QChar::Joining QChar::joining() const
{
    return QUnicodeTables::joining(ucs);
}

/*!
    \overload
    Returns information about the joining properties of the UCS-4-encoded
    character specified by \a ucs4 (needed for certain languages such as
    Arabic).
*/
QChar::Joining QChar::joining(const uint ucs4)
{
    return QUnicodeTables::joining(ucs4);
}

/*!
    \overload
    Returns information about the joining properties of the UCS-2-encoded
    character specified by \a ucs2 (needed for certain languages such as
    Arabic).
*/
QChar::Joining QChar::joining(const ushort ucs2)
{
    return QUnicodeTables::joining(ucs2);
}


/*!
    Returns true if the character should be reversed if the text
    direction is reversed; otherwise returns false.

    Same as (ch.mirroredChar() != ch).

    \sa mirroredChar()
*/
bool QChar::hasMirrored() const
{
    if (is_ascii_char(ucs)) {
        return false;
    }
    const utf8proc_property_t *property = utf8proc_get_property(ucs);
    switch (property->bidi_class) {
        case UTF8PROC_BIDI_CLASS_R:
        case UTF8PROC_BIDI_CLASS_AL:
        case UTF8PROC_BIDI_CLASS_RLE:
        case UTF8PROC_BIDI_CLASS_RLO:
        case UTF8PROC_BIDI_CLASS_RLI:
            return true;
        default:
            return false;
    }
    return false;
}

/*!
    Returns true if the character is a lowercase letter, i.e.
    category() is Letter_Lowercase.

    \sa isUpper(), toLower(), toUpper()
*/
bool QChar::isLower() const
{
    if (is_ascii_char(ucs)) {
        return is_ascii_lower(ucs);
    }
    return category() == Letter_Lowercase;
}

/*!
    \fn bool QChar::isUpper() const

    Returns true if the character is an uppercase letter, i.e.
    category() is Letter_Uppercase.

    \sa isLower(), toUpper(), toLower()
*/
bool QChar::isUpper() const
{
    if (is_ascii_char(ucs)) {
        return is_ascii_upper(ucs);
    }
    return category() == Letter_Uppercase;
}

/*!
    \fn bool QChar::isTitleCase() const
    \since 4.3

    Returns true if the character is a titlecase letter, i.e.
    category() is Letter_Titlecase.

    \sa isLower(), toUpper(), toLower(), toTitleCase()
*/
bool QChar::isTitleCase() const
{
    if (is_ascii_char(ucs)) {
        return false;
    }
    return category() == Letter_Titlecase;
}

/*!
    Returns the mirrored character if this character is a mirrored
    character; otherwise returns the character itself.

    \sa hasMirrored()
*/
QChar QChar::mirroredChar() const
{
    return QUnicodeTables::mirroredChar(ucs);
}

/*!
    \overload
    Returns the mirrored character if the UCS-4-encoded character specified
    by \a ucs4 is a mirrored character; otherwise returns the character itself.

    \sa hasMirrored()
*/
uint QChar::mirroredChar(const uint ucs4)
{
    return QUnicodeTables::mirroredChar(ucs4);
}

/*!
    \overload
    Returns the mirrored character if the UCS-2-encoded character specified
    by \a ucs2 is a mirrored character; otherwise returns the character itself.

    \sa hasMirrored()
*/
ushort QChar::mirroredChar(const ushort ucs2)
{
    return QUnicodeTables::mirroredChar(ucs2);
}

/*!
    Decomposes a character into its parts. Returns an empty string if
    no decomposition exists.
*/
QString QChar::decomposition() const
{
    return decomposition(ucs);
}

/*!
    \overload
    Decomposes the UCS-4-encoded character specified by \a ucs4 into its
    constituent parts. Returns an empty string if no decomposition exists.
*/
QString QChar::decomposition(const uint ucs4)
{
    int buffer[4]; // ### use utf8proc_charwidth?
    int boundclass;
    const ssize_t decresult = utf8proc_decompose_char(ucs4, buffer, sizeof(buffer), UTF8PROC_DECOMPOSE, &boundclass);
    if (Q_UNLIKELY(decresult < 1)) {
        qWarning("QChar::decomposition: %s", utf8proc_errmsg(decresult));
        return QString();
    }
    return QString::fromUtf8(reinterpret_cast<char*>(buffer), decresult);
}

/*!
    Returns the tag defining the composition of the character. Returns
    QChar::Single if no decomposition exists.
*/
QChar::Decomposition QChar::decompositionTag() const
{
    return decompositionTag(ucs);
}

/*!
    \overload
    Returns the tag defining the composition of the UCS-4-encoded character
    specified by \a ucs4. Returns QChar::Single if no decomposition exists.
*/
QChar::Decomposition QChar::decompositionTag(const uint ucs4)
{
    const utf8proc_property_t *property = utf8proc_get_property(ucs4);
    switch (property->decomp_type) {
        case UTF8PROC_DECOMP_TYPE_FONT:
            return QChar::Font;
        case UTF8PROC_DECOMP_TYPE_NOBREAK:
            return QChar::NoBreak;
        case UTF8PROC_DECOMP_TYPE_INITIAL:
            return QChar::Initial;
        case UTF8PROC_DECOMP_TYPE_MEDIAL:
            return QChar::Medial;
        case UTF8PROC_DECOMP_TYPE_FINAL:
            return QChar::Final;
        case UTF8PROC_DECOMP_TYPE_ISOLATED:
            return QChar::Isolated;
        case UTF8PROC_DECOMP_TYPE_CIRCLE:
            return QChar::Circle;
        case UTF8PROC_DECOMP_TYPE_SUPER:
            return QChar::Super;
        case UTF8PROC_DECOMP_TYPE_SUB:
            return QChar::Sub;
        case UTF8PROC_DECOMP_TYPE_VERTICAL:
            return QChar::Vertical;
        case UTF8PROC_DECOMP_TYPE_WIDE:
            return QChar::Wide;
        case UTF8PROC_DECOMP_TYPE_NARROW:
            return QChar::Narrow;
        case UTF8PROC_DECOMP_TYPE_SMALL:
            return QChar::Small;
        case UTF8PROC_DECOMP_TYPE_SQUARE:
            return QChar::Square;
        case UTF8PROC_DECOMP_TYPE_FRACTION:
            return QChar::Fraction;
        case UTF8PROC_DECOMP_TYPE_COMPAT:
            return QChar::Compat;
    }
    return QChar::NoDecomposition;
}

/*!
    Returns the Unicode version that introduced this character.
*/
QChar::UnicodeVersion QChar::unicodeVersion() const
{
    return QUnicodeTables::unicodeVersion(ucs);
}

/*!
    \overload
    Returns the Unicode version that introduced the character specified in
    its UCS-4-encoded form as \a ucs4.
*/
QChar::UnicodeVersion QChar::unicodeVersion(const uint ucs4)
{
    return QUnicodeTables::unicodeVersion(ucs4);
}

/*!
    \overload
    Returns the Unicode version that introduced the character specified in
    its UCS-2-encoded form as \a ucs2.
*/
QChar::UnicodeVersion QChar::unicodeVersion(const ushort ucs2)
{
    return QUnicodeTables::unicodeVersion(ucs2);
}

/*!
    \since 4.8

    Returns the most recent supported Unicode version.
*/
QChar::UnicodeVersion QChar::currentUnicodeVersion()
{
    return QChar::Unicode_Last;
}

/*!
    Returns the lowercase equivalent if the character is uppercase or titlecase;
    otherwise returns the character itself.
*/
QChar QChar::toLower() const
{
    if (is_ascii_char(ucs)) {
        return to_ascii_lower(ucs);
    }
    return utf8proc_tolower(ucs);
}

/*!
    \overload
    Returns the lowercase equivalent of the UCS-4-encoded character specified
    by \a ucs4 if the character is uppercase or titlecase; otherwise returns
    the character itself.
*/
uint QChar::toLower(const uint ucs4)
{
    if (is_ascii_char(ucs4)) {
        return to_ascii_lower(ucs4);
    }
    return utf8proc_tolower(ucs4);
}

/*!
    \overload
    Returns the lowercase equivalent of the UCS-2-encoded character specified
    by \a ucs2 if the character is uppercase or titlecase; otherwise returns
    the character itself.
*/
ushort QChar::toLower(const ushort ucs2)
{
    if (is_ascii_char(ucs2)) {
        return to_ascii_lower(ucs2);
    }
    return utf8proc_tolower(ucs2);
}

/*!
    Returns the uppercase equivalent if the character is lowercase or titlecase;
    otherwise returns the character itself.
*/
QChar QChar::toUpper() const
{
    if (is_ascii_char(ucs)) {
        return to_ascii_upper(ucs);
    }
    return utf8proc_toupper(ucs);
}

/*!
    \overload
    Returns the uppercase equivalent of the UCS-4-encoded character specified
    by \a ucs4 if the character is lowercase or titlecase; otherwise returns
    the character itself.
*/
uint QChar::toUpper(const uint ucs4)
{
    if (is_ascii_char(ucs4)) {
        return to_ascii_upper(ucs4);
    }
    return utf8proc_toupper(ucs4);
}

/*!
    \overload
    Returns the uppercase equivalent of the UCS-2-encoded character specified
    by \a ucs2 if the character is lowercase or titlecase; otherwise returns
    the character itself.
*/
ushort QChar::toUpper(const ushort ucs2)
{
    if (is_ascii_char(ucs2)) {
        return to_ascii_upper(ucs2);
    }
    return utf8proc_toupper(ucs2);
}

/*!
    Returns the title case equivalent if the character is lowercase or uppercase;
    otherwise returns the character itself.
*/
QChar QChar::toTitleCase() const
{
    if (is_ascii_char(ucs)) {
        return to_ascii_upper(ucs);
    }
    return utf8proc_totitle(ucs);
}

/*!
    \overload
    Returns the title case equivalent of the UCS-4-encoded character specified
    by \a ucs4 if the character is lowercase or uppercase; otherwise returns
    the character itself.
*/
uint QChar::toTitleCase(const uint ucs4)
{
    if (is_ascii_char(ucs4)) {
        return to_ascii_upper(ucs4);
    }
    return utf8proc_totitle(ucs4);
}

/*!
    \overload
    Returns the title case equivalent of the UCS-2-encoded character specified
    by \a ucs2 if the character is lowercase or uppercase; otherwise returns
    the character itself.
*/
ushort QChar::toTitleCase(const ushort ucs2)
{
    if (is_ascii_char(ucs2)) {
        return to_ascii_upper(ucs2);
    }
    return utf8proc_totitle(ucs2);
}

/*!
    Returns the case folded equivalent of the character. For most Unicode characters this
    is the same as toLowerCase().
*/
QChar QChar::toCaseFolded() const
{
    if (is_ascii_char(ucs)) {
        return to_ascii_lower(ucs);
    }
    int buffer[4]; // ### use utf8proc_charwidth?
    int boundclass;
    const ssize_t decresult = utf8proc_decompose_char(ucs, buffer, sizeof(buffer), UTF8PROC_CASEFOLD, &boundclass);
    if (Q_UNLIKELY(decresult < 1)) {
        qWarning("QChar::toCaseFolded: %s", utf8proc_errmsg(decresult));
        return QChar(ucs);
    }
    return QChar(*buffer);
}

/*!
    \overload
    Returns the case folded equivalent of the UCS-4-encoded character specified
    by \a ucs4. For most Unicode characters this is the same as toLowerCase().
*/
uint QChar::toCaseFolded(const uint ucs4)
{
    if (is_ascii_char(ucs4)) {
        return to_ascii_lower(ucs4);
    }
    int buffer[4]; // ### use utf8proc_charwidth?
    int boundclass;
    const ssize_t decresult = utf8proc_decompose_char(ucs4, buffer, sizeof(buffer), UTF8PROC_CASEFOLD, &boundclass);
    if (Q_UNLIKELY(decresult < 1)) {
        qWarning("QChar::toCaseFolded: %s", utf8proc_errmsg(decresult));
        return ucs4;
    }
    return uint(*buffer);
}

/*!
    \overload
    Returns the case folded equivalent of the UCS-2-encoded character specified
    by \a ucs2. For most Unicode characters this is the same as toLowerCase().
*/
ushort QChar::toCaseFolded(const ushort ucs2)
{
    if (is_ascii_char(ucs2)) {
        return to_ascii_lower(ucs2);
    }
    int buffer[4]; // ### use utf8proc_charwidth?
    int boundclass;
    const ssize_t decresult = utf8proc_decompose_char(ucs2, buffer, sizeof(buffer), UTF8PROC_CASEFOLD, &boundclass);
    if (Q_UNLIKELY(decresult < 1)) {
        qWarning("QChar::toCaseFolded: %s", utf8proc_errmsg(decresult));
        return ucs2;
    }
    return ushort(*buffer);
}


/*!
    \fn char QChar::latin1() const

    Use toLatin1() instead.
*/

/*!
    \fn char QChar::ascii() const

    Use toAscii() instead.
*/

/*!
    \fn char QChar::toLatin1() const

    Returns the Latin-1 character equivalent to the QChar, or 0. This
    is mainly useful for non-internationalized software.

    \sa toAscii(), unicode(), QTextCodec::codecForCStrings()
*/

/*!
    \fn char QChar::toAscii() const
    Returns the character value of the QChar obtained using the current
    codec used to read C strings, or 0 if the character is not representable
    using this codec. The default codec handles Latin-1 encoded text,
    but this can be changed to assist developers writing source code using
    other encodings.

    The main purpose of this function is to preserve ASCII characters used
    in C strings. This is mainly useful for developers of non-internationalized
    software.

    \sa toLatin1(), unicode(), QTextCodec::codecForCStrings()
*/
char QChar::toAscii() const
{
#ifndef QT_NO_CODEC_FOR_C_STRINGS
    if (QTextCodec::codecForCStrings())
        // #####
        return QTextCodec::codecForCStrings()->fromUnicode(QString(*this)).at(0);
#endif
    return ucs > 0xff ? 0 : char(ucs);
}

/*!
    \fn QChar QChar::fromLatin1(char c)

    Converts the Latin-1 character \a c to its equivalent QChar. This
    is mainly useful for non-internationalized software.

    \sa fromAscii(), unicode(), QTextCodec::codecForCStrings()
*/

/*!
    Converts the ASCII character \a c to its equivalent QChar. This
    is mainly useful for non-internationalized software.

    An alternative is to use QLatin1Char.

    \sa fromLatin1(), unicode(), QTextCodec::codecForCStrings()
*/
QChar QChar::fromAscii(const char c)
{
#ifndef QT_NO_CODEC_FOR_C_STRINGS
    if (QTextCodec::codecForCStrings())
        // #####
        return QTextCodec::codecForCStrings()->toUnicode(&c, 1).at(0).unicode();
#endif
    return QChar(ushort((uchar)c));
}

#ifndef QT_NO_DATASTREAM
/*!
    \relates QChar

    Writes the char \a chr to the stream \a out.

    \sa {Serializing Qt Data Types}
*/
QDataStream &operator<<(QDataStream &out, const QChar &chr)
{
    out << quint16(chr.unicode());
    return out;
}

/*!
    \relates QChar

    Reads a char from the stream \a in into char \a chr.

    \sa {Serializing Qt Data Types}
*/
QDataStream &operator>>(QDataStream &in, QChar &chr)
{
    quint16 u;
    in >> u;
    chr.unicode() = ushort(u);
    return in;
}
#endif // QT_NO_DATASTREAM

/*!
    \fn ushort & QChar::unicode()

    Returns a reference to the numeric Unicode value of the QChar.
*/

/*!
    \fn ushort QChar::unicode() const

    \overload
*/

/*****************************************************************************
  Documentation of QChar related functions
 *****************************************************************************/

/*!
    \fn bool operator==(QChar c1, QChar c2)

    \relates QChar

    Returns true if \a c1 and \a c2 are the same Unicode character;
    otherwise returns false.
*/

/*!
    \fn int operator!=(QChar c1, QChar c2)

    \relates QChar

    Returns true if \a c1 and \a c2 are not the same Unicode
    character; otherwise returns false.
*/

/*!
    \fn int operator<=(QChar c1, QChar c2)

    \relates QChar

    Returns true if the numeric Unicode value of \a c1 is less than
    or equal to that of \a c2; otherwise returns false.
*/

/*!
    \fn int operator>=(QChar c1, QChar c2)

    \relates QChar

    Returns true if the numeric Unicode value of \a c1 is greater than
    or equal to that of \a c2; otherwise returns false.
*/

/*!
    \fn int operator<(QChar c1, QChar c2)

    \relates QChar

    Returns true if the numeric Unicode value of \a c1 is less than
    that of \a c2; otherwise returns false.
*/

/*!
    \fn int operator>(QChar c1, QChar c2)

    \relates QChar

    Returns true if the numeric Unicode value of \a c1 is greater than
    that of \a c2; otherwise returns false.
*/

/*!
    \fn bool QChar::mirrored() const

    Use hasMirrored() instead.
*/

/*!
    \fn QChar QChar::lower() const

    Use toLower() instead.
*/

/*!
    \fn QChar QChar::upper() const

    Use toUpper() instead.
*/

/*!
    \fn bool QChar::networkOrdered()

    See if QSysInfo::ByteOrder == QSysInfo::BigEndian instead.
*/

QT_END_NAMESPACE
