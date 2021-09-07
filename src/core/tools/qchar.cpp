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

#include <unicode/uchar.h>
#include <unicode/unorm2.h>

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
    \value Unicode_13_0
    \value Unicode_14_0
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
    \value Canonical
    \value Font
    \value NoBreak
    \value Initial
    \value Medial
    \value Final
    \value Isolated
    \value Circle
    \value Super
    \value Sub
    \value Vertical
    \value Wide
    \value Narrow
    \value Small
    \value Square
    \value Compat
    \value Fraction

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
    return u_isprint(ucs);
}

/*!
    Returns true if the character is a separator character
    (Separator_* categories); otherwise returns false.
*/
bool QChar::isSpace() const
{
    if(ucs >= 9 && ucs <= 13)
        return true;
    const int8_t category = u_charType(ucs);
    switch (category) {
        case U_SPACE_SEPARATOR:
        case U_LINE_SEPARATOR:
        case U_PARAGRAPH_SEPARATOR:
            return true;
        default:
            // special characters check
            return u_isblank(ucs);
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
    const int8_t category = u_charType(ucs);
    switch (category) {
        case U_NON_SPACING_MARK:
        case U_COMBINING_SPACING_MARK:
        case U_ENCLOSING_MARK:
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
    return u_ispunct(ucs);
}

/*!
    Returns true if the character is a letter (Letter_* categories);
    otherwise returns false.
*/
bool QChar::isLetter() const
{
    return u_isalpha(ucs);
}

/*!
    Returns true if the character is a number (Number_* categories,
    not just 0-9); otherwise returns false.

    \sa isDigit()
*/
bool QChar::isNumber() const
{
    if (is_ascii_number(ucs)) {
        return true;
    }
    return u_isxdigit(ucs);
}

/*!
    Returns true if the character is a letter or number (Letter_* or
    Number_* categories); otherwise returns false.
*/
bool QChar::isLetterOrNumber() const
{
    if ((ucs >= 'a' && ucs <= 'z') || (ucs >= 'A' && ucs <= 'Z') || (ucs >= '0' && ucs <= '9')) {
        return true;
    }
    const int8_t category = u_charType(ucs);
    switch (category) {
        case U_UPPERCASE_LETTER:
        case U_LOWERCASE_LETTER:
        case U_TITLECASE_LETTER:
        case U_MODIFIER_LETTER:
        case U_OTHER_LETTER:
        case U_DECIMAL_DIGIT_NUMBER:
        case U_LETTER_NUMBER:
        case U_OTHER_NUMBER:
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
    if (is_ascii_number(ucs)) {
        return true;
    }
    return u_isdigit(ucs);
}


/*!
    Returns true if the character is a symbol (Symbol_* categories);
    otherwise returns false.
*/
bool QChar::isSymbol() const
{
    const int8_t category = u_charType(ucs);
    switch (category) {
        case U_MATH_SYMBOL:
        case U_CURRENCY_SYMBOL:
        case U_MODIFIER_SYMBOL:
        case U_OTHER_SYMBOL:
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
    return QChar::digitValue(ucs);
}

/*!
    \overload
    Returns the numeric value of the digit, specified by the UCS-2-encoded
    character, \a ucs2, or -1 if the character is not a digit.
*/
int QChar::digitValue(const ushort ucs2)
{
    return QChar::digitValue(uint(ucs2));
}

/*!
    \overload
    Returns the numeric value of the digit specified by the UCS-4-encoded
    character, \a ucs4, or -1 if the character is not a digit.
*/
int QChar::digitValue(const uint ucs4)
{
    return u_digit(ucs4, 10);
}

/*!
    Returns the character's category.
*/
QChar::Category QChar::category() const
{
    return QChar::category(uint(ucs));
}


/*!
    \overload
    Returns the category of the UCS-2-encoded character specified by \a ucs2.
*/
QChar::Category QChar::category(const ushort ucs2)
{
    return QChar::category(uint(ucs2));
}

/*!
    \overload
    \since 4.3
    Returns the category of the UCS-4-encoded character specified by \a ucs4.
*/
QChar::Category QChar::category(const uint ucs4)
{
    const int8_t category = u_charType(ucs4);
    switch (category) {
        case U_GENERAL_OTHER_TYPES:
            return QChar::Other_NotAssigned;
        case U_UPPERCASE_LETTER:
            return QChar::Letter_Uppercase;
        case U_LOWERCASE_LETTER:
            return QChar::Letter_Lowercase;
        case U_TITLECASE_LETTER:
            return QChar::Letter_Titlecase;
        case U_MODIFIER_LETTER:
            return QChar::Letter_Modifier;
        case U_OTHER_LETTER:
            return QChar::Letter_Other;
        case U_NON_SPACING_MARK:
            return QChar::Mark_NonSpacing;
        case U_COMBINING_SPACING_MARK:
            return QChar::Mark_SpacingCombining;
        case U_ENCLOSING_MARK:
            return QChar::Mark_Enclosing;
        case U_DECIMAL_DIGIT_NUMBER:
            return QChar::Number_DecimalDigit;
        case U_LETTER_NUMBER:
            return QChar::Number_Letter;
        case U_OTHER_NUMBER:
            return QChar::Number_Other;
        case U_CONNECTOR_PUNCTUATION:
            return QChar::Punctuation_Connector;
        case U_DASH_PUNCTUATION:
            return QChar::Punctuation_Dash;
        case U_START_PUNCTUATION:
            return QChar::Punctuation_Open;
        case U_END_PUNCTUATION:
            return QChar::Punctuation_Close;
        case U_INITIAL_PUNCTUATION:
            return QChar::Punctuation_InitialQuote;
        case U_FINAL_PUNCTUATION:
            return QChar::Punctuation_FinalQuote;
        case U_OTHER_PUNCTUATION:
            return QChar::Punctuation_Other;
        case U_MATH_SYMBOL:
            return QChar::Symbol_Math;
        case U_CURRENCY_SYMBOL:
            return QChar::Symbol_Currency;
        case U_MODIFIER_SYMBOL:
            return QChar::Symbol_Modifier;
        case U_OTHER_SYMBOL:
            return QChar::Symbol_Other;
        case U_SPACE_SEPARATOR:
            return QChar::Separator_Space;
        case U_LINE_SEPARATOR:
            return QChar::Separator_Line;
        case U_PARAGRAPH_SEPARATOR:
            return QChar::Separator_Paragraph;
        case U_CONTROL_CHAR:
            return QChar::Other_Control;
        case U_FORMAT_CHAR:
            return QChar::Other_Format;
        case U_SURROGATE:
            return QChar::Other_Surrogate;
        case U_PRIVATE_USE_CHAR:
            return QChar::Other_PrivateUse;
    }
    return QChar::Other_NotAssigned;
}

/*!
    Returns the character's direction.
*/
QChar::Direction QChar::direction() const
{
    return QChar::direction(uint(ucs));
}

/*!
    \overload
    Returns the direction of the UCS-2-encoded character specified by \a ucs2.
*/
QChar::Direction QChar::direction(const ushort ucs2)
{
    return QChar::direction(uint(ucs2));
}

/*!
    \overload
    Returns the direction of the UCS-4-encoded character specified by \a ucs4.
*/
QChar::Direction QChar::direction(const uint ucs4)
{
    const UCharDirection direction = u_charDirection(ucs4);
    switch (direction) {
        case U_LEFT_TO_RIGHT:
            return QChar::DirL;
        case U_LEFT_TO_RIGHT_EMBEDDING:
            return QChar::DirLRE;
        case U_LEFT_TO_RIGHT_OVERRIDE:
            return QChar::DirLRO;
        case U_RIGHT_TO_LEFT:
            return QChar::DirR;
        case U_RIGHT_TO_LEFT_ARABIC:
            return QChar::DirAL;
        case U_RIGHT_TO_LEFT_EMBEDDING:
            return QChar::DirRLE;
        case U_RIGHT_TO_LEFT_OVERRIDE:
            return QChar::DirRLO;
        case U_POP_DIRECTIONAL_FORMAT:
            return QChar::DirPDF;
        case U_EUROPEAN_NUMBER:
            return QChar::DirEN;
        case U_EUROPEAN_NUMBER_SEPARATOR:
            return QChar::DirES;
        case U_EUROPEAN_NUMBER_TERMINATOR:
            return QChar::DirET;
        case U_ARABIC_NUMBER:
            return QChar::DirAN;
        case U_COMMON_NUMBER_SEPARATOR:
            return QChar::DirCS;
        case U_DIR_NON_SPACING_MARK:
            return QChar::DirNSM;
        case U_BOUNDARY_NEUTRAL:
            return QChar::DirBN;
        case U_BLOCK_SEPARATOR:
            return QChar::DirB;
        case U_SEGMENT_SEPARATOR:
            return QChar::DirS;
        case U_WHITE_SPACE_NEUTRAL:
            return QChar::DirWS;
        case U_OTHER_NEUTRAL:
            return QChar::DirON;
        case U_LEFT_TO_RIGHT_ISOLATE:
            return QChar::DirLRI;
        case U_RIGHT_TO_LEFT_ISOLATE:
            return QChar::DirRLI;
        case U_FIRST_STRONG_ISOLATE:
            return QChar::DirFSI;
        case U_POP_DIRECTIONAL_ISOLATE:
            return QChar::DirPDI;
        // just to silence compiler warning
#ifndef U_HIDE_DEPRECATED_API
        case U_CHAR_DIRECTION_COUNT:
            return QChar::DirL;
#endif
    }
    return QChar::DirL;
}

/*!
    Returns information about the joining properties of the character
    (needed for certain languages such as Arabic).
*/
QChar::Joining QChar::joining() const
{
    return QChar::joining(uint(ucs));
}

/*!
    \overload
    Returns information about the joining properties of the UCS-2-encoded
    character specified by \a ucs2 (needed for certain languages such as
    Arabic).
*/
QChar::Joining QChar::joining(const ushort ucs2)
{
    return QChar::joining(uint(ucs2));
}

/*!
    \overload
    Returns information about the joining properties of the UCS-4-encoded
    character specified by \a ucs4 (needed for certain languages such as
    Arabic).
*/
QChar::Joining QChar::joining(const uint ucs4)
{
    const int32_t property = u_getIntPropertyValue(ucs4, UCHAR_JOINING_TYPE);
    switch (property) {
        case U_JT_JOIN_CAUSING:
            return QChar::Causing;
        case U_JT_DUAL_JOINING:
            return QChar::Dual;
        case U_JT_LEFT_JOINING:
            return QChar::Left;
        case U_JT_RIGHT_JOINING:
            return QChar::Right;
        case U_JT_TRANSPARENT:
            return QChar::Transparent;
        case U_JT_NON_JOINING:
            // for compatibility
            return QChar::OtherJoining;
        // just to silence compiler warning
#ifndef U_HIDE_DEPRECATED_API
        case U_JT_COUNT:
            return QChar::OtherJoining;
#endif
    }
    return QChar::OtherJoining;
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
    return u_isMirrored(ucs);
}

/*!
    Returns true if the character is a lowercase letter, i.e.
    category() is Letter_Lowercase.

    \sa isUpper(), toLower(), toUpper()
*/
bool QChar::isLower() const
{
    if (ucs >= 'a' && ucs <= 'z') {
        return true;
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
    if (ucs >= 'A' && ucs <= 'Z') {
        return true;
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
    return QChar::mirroredChar(ucs);
}

/*!
    \overload
    Returns the mirrored character if the UCS-2-encoded character specified
    by \a ucs2 is a mirrored character; otherwise returns the character itself.

    \sa hasMirrored()
*/
ushort QChar::mirroredChar(const ushort ucs2)
{
    return QChar::mirroredChar(uint(ucs2));
}

/*!
    \overload
    Returns the mirrored character if the UCS-4-encoded character specified
    by \a ucs4 is a mirrored character; otherwise returns the character itself.

    \sa hasMirrored()
*/
uint QChar::mirroredChar(const uint ucs4)
{
    return u_charMirror(ucs4);
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
    UErrorCode errorcode = U_ZERO_ERROR;
    const UNormalizer2 *normalizer = unorm2_getNFKDInstance(&errorcode);
    if (Q_UNLIKELY(U_FAILURE(errorcode))) {
        qWarning("QChar::decomposition: unorm2_getNFKDInstance() failed %s", u_errorName(errorcode));
        return QString();
    }

    errorcode = U_ZERO_ERROR;
    static const int maxlength = 4;
    UChar result[maxlength];
    const int decresult = unorm2_getDecomposition(normalizer, ucs4, result, maxlength, &errorcode);
    if (Q_UNLIKELY(U_FAILURE(errorcode))) {
        qWarning("QChar::decomposition: unorm2_getDecomposition() failed %s", u_errorName(errorcode));
        return QString();
    } else if (Q_UNLIKELY(decresult < 1)) {
        // no decomposition value
        return QString();
    }

    return QString(reinterpret_cast<QChar*>(result), decresult);
}

/*!
    Returns the tag defining the composition of the character. Returns
    QChar::NoDecomposition if no decomposition exists.
*/
QChar::Decomposition QChar::decompositionTag() const
{
    return decompositionTag(ucs);
}

/*!
    \overload
    Returns the tag defining the composition of the UCS-4-encoded character
    specified by \a ucs4. Returns QChar::NoDecomposition if no decomposition exists.
*/
QChar::Decomposition QChar::decompositionTag(const uint ucs4)
{
    const int32_t property = u_getIntPropertyValue(ucs4, UCHAR_DECOMPOSITION_TYPE);
    switch (property) {
        case U_DT_CANONICAL:
            return QChar::Canonical;
        case U_DT_FONT:
            return QChar::Font;
        case U_DT_NOBREAK:
            return QChar::NoBreak;
        case U_DT_INITIAL:
            return QChar::Initial;
        case U_DT_MEDIAL:
            return QChar::Medial;
        case U_DT_FINAL:
            return QChar::Final;
        case U_DT_ISOLATED:
            return QChar::Isolated;
        case U_DT_CIRCLE:
            return QChar::Circle;
        case U_DT_SUPER:
            return QChar::Super;
        case U_DT_SUB:
            return QChar::Sub;
        case U_DT_VERTICAL:
            return QChar::Vertical;
        case U_DT_WIDE:
            return QChar::Wide;
        case U_DT_NARROW:
            return QChar::Narrow;
        case U_DT_SMALL:
            return QChar::Small;
        case U_DT_SQUARE:
            return QChar::Square;
        case U_DT_FRACTION:
            return QChar::Fraction;
        case U_DT_COMPAT:
            return QChar::Compat;
        case U_DT_NONE:
            return QChar::NoDecomposition;
        // just to silence compiler warning
#ifndef U_HIDE_DEPRECATED_API
        case U_DT_COUNT:
            return QChar::NoDecomposition;
#endif
    }
    return QChar::NoDecomposition;
}

/*!
    Returns the Unicode version that introduced this character.
*/
QChar::UnicodeVersion QChar::unicodeVersion() const
{
    return QChar::unicodeVersion(ucs);
}

/*!
    \overload
    Returns the Unicode version that introduced the character specified in
    its UCS-2-encoded form as \a ucs2.
*/
QChar::UnicodeVersion QChar::unicodeVersion(const ushort ucs2)
{
    return QChar::unicodeVersion(uint(ucs2));
}

/*!
    \overload
    Returns the Unicode version that introduced the character specified in
    its UCS-4-encoded form as \a ucs4.
*/
QChar::UnicodeVersion QChar::unicodeVersion(const uint ucs4)
{
    Q_ASSERT(U_MAX_VERSION_LENGTH == 4);

    UVersionInfo info;
    u_charAge(ucs4, info);

    // for reference:
    // https://www.unicode.org/versions/enumeratedversions.html
    if (info[0] == 1 && info[1] == 1) {
        return QChar::Unicode_1_1;
    } else if (info[0] == 2 && info[1] == 0) {
        return QChar::Unicode_2_0;
    } else if (info[0] == 2 && info[1] == 1) {
        return QChar::Unicode_2_1;
    } else if (info[0] == 3 && info[1] == 0) {
        return QChar::Unicode_3_0;
    } else if (info[0] == 3 && info[1] == 1) {
        return QChar::Unicode_3_1;
    } else if (info[0] == 3 && info[1] == 2) {
        return QChar::Unicode_3_2;
    } else if (info[0] == 4 && info[1] == 0) {
        return QChar::Unicode_4_0;
    } else if (info[0] == 4 && info[1] == 1) {
        return QChar::Unicode_4_1;
    } else if (info[0] == 5 && info[1] == 0) {
        return QChar::Unicode_5_0;
    } else if (info[0] == 5 && info[1] == 1) {
        return QChar::Unicode_5_1;
    } else if (info[0] == 5 && info[1] == 2) {
        return QChar::Unicode_5_2;
    } else if (info[0] == 6 && info[1] == 0) {
        return QChar::Unicode_6_0;
    } else if (info[0] == 6 && info[1] == 1) {
        return QChar::Unicode_6_1;
    } else if (info[0] == 6 && info[1] == 2) {
        return QChar::Unicode_6_2;
    } else if (info[0] == 6 && info[1] == 3) {
        return QChar::Unicode_6_3;
    } else if (info[0] == 7 && info[1] == 0) {
        return QChar::Unicode_7_0;
    } else if (info[0] == 8 && info[1] == 0) {
        return QChar::Unicode_8_0;
    } else if (info[0] == 9 && info[1] == 0) {
        return QChar::Unicode_9_0;
    } else if (info[0] == 10 && info[1] == 0) {
        return QChar::Unicode_10_0;
    } else if (info[0] == 11 && info[1] == 0) {
        return QChar::Unicode_11_0;
    } else if (info[0] == 12 && info[1] == 0) {
        return QChar::Unicode_12_0;
    } else if (info[0] == 12 && info[1] == 1) {
        return QChar::Unicode_12_1;
    } else if (info[0] == 13 && info[1] == 0) {
        return QChar::Unicode_13_0;
    } else if (info[0] == 14 && info[1] == 0) {
        return QChar::Unicode_14_0;
    }
    return QChar::Unicode_Unassigned;
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
    return QChar::toLower(ucs);
}

/*!
    \overload
    Returns the lowercase equivalent of the UCS-2-encoded character specified
    by \a ucs2 if the character is uppercase or titlecase; otherwise returns
    the character itself.
*/
ushort QChar::toLower(const ushort ucs2)
{
    return QChar::toLower(uint(ucs2));
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
    return u_tolower(ucs4);
}

/*!
    Returns the uppercase equivalent if the character is lowercase or titlecase;
    otherwise returns the character itself.
*/
QChar QChar::toUpper() const
{
    return QChar::toUpper(ucs);
}

/*!
    \overload
    Returns the uppercase equivalent of the UCS-2-encoded character specified
    by \a ucs2 if the character is lowercase or titlecase; otherwise returns
    the character itself.
*/
ushort QChar::toUpper(const ushort ucs2)
{
    return QChar::toUpper(uint(ucs2));
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
    return u_toupper(ucs4);
}

/*!
    Returns the title case equivalent if the character is lowercase or uppercase;
    otherwise returns the character itself.
*/
QChar QChar::toTitleCase() const
{
    return QChar::toTitleCase(ucs);
}

/*!
    \overload
    Returns the title case equivalent of the UCS-2-encoded character specified
    by \a ucs2 if the character is lowercase or uppercase; otherwise returns
    the character itself.
*/
ushort QChar::toTitleCase(const ushort ucs2)
{
    return QChar::toTitleCase(uint(ucs2));
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
    return u_totitle(ucs4);
}

/*!
    Returns the case folded equivalent of the character. For most Unicode characters this
    is the same as toLowerCase().
*/
QChar QChar::toCaseFolded() const
{
    return QChar::toCaseFolded(ucs);
}

/*!
    \overload
    Returns the case folded equivalent of the UCS-2-encoded character specified
    by \a ucs2. For most Unicode characters this is the same as toLowerCase().
*/
ushort QChar::toCaseFolded(const ushort ucs2)
{
    return QChar::toCaseFolded(uint(ucs2));
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
    return u_foldCase(ucs4, U_FOLD_CASE_DEFAULT);
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
    return QChar(ushort(uchar(c)));
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

QT_END_NAMESPACE
