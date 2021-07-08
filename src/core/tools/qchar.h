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

#ifndef QCHAR_H
#define QCHAR_H

#include <QtCore/qglobal.h>


QT_BEGIN_NAMESPACE

class QString;
class QDataStream;

class QLatin1Char
{
public:
    inline explicit QLatin1Char(const char c) : ch(c) {}
    inline char toLatin1() const { return ch; }
    inline ushort unicode() const { return ushort(uchar(ch)); }

private:
    const char ch;
};


class Q_CORE_EXPORT QChar {
public:
    QChar();
#ifndef QT_NO_CAST_FROM_ASCII
    QT_ASCII_CAST_WARN_CONSTRUCTOR QChar(const char c);
    QT_ASCII_CAST_WARN_CONSTRUCTOR QChar(const uchar c);
#endif
    QChar(const QLatin1Char ch);
    QChar(const uchar c, uchar r);
    QChar(const ushort rc);
    QChar(const short rc);
    QChar(const uint rc);
    QChar(const int rc);
    enum SpecialCharacter {
        Null = 0x0000,
        Nbsp = 0x00a0,
        ReplacementCharacter = 0xfffd,
        ObjectReplacementCharacter = 0xfffc,
        ByteOrderMark = 0xfeff,
        ByteOrderSwapped = 0xfffe,
        ParagraphSeparator = 0x2029,
        LineSeparator = 0x2028
    };
    QChar(const SpecialCharacter sc);

    // Unicode information

    enum Category {
        Mark_NonSpacing,          //   Mn
        Mark_SpacingCombining,    //   Mc
        Mark_Enclosing,           //   Me

        Number_DecimalDigit,      //   Nd
        Number_Letter,            //   Nl
        Number_Other,             //   No

        Separator_Space,          //   Zs
        Separator_Line,           //   Zl
        Separator_Paragraph,      //   Zp

        Other_Control,            //   Cc
        Other_Format,             //   Cf
        Other_Surrogate,          //   Cs
        Other_PrivateUse,         //   Co
        Other_NotAssigned,        //   Cn

        Letter_Uppercase,         //   Lu
        Letter_Lowercase,         //   Ll
        Letter_Titlecase,         //   Lt
        Letter_Modifier,          //   Lm
        Letter_Other,             //   Lo

        Punctuation_Connector,    //   Pc
        Punctuation_Dash,         //   Pd
        Punctuation_Open,         //   Ps
        Punctuation_Close,        //   Pe
        Punctuation_InitialQuote, //   Pi
        Punctuation_FinalQuote,   //   Pf
        Punctuation_Other,        //   Po

        Symbol_Math,              //   Sm
        Symbol_Currency,          //   Sc
        Symbol_Modifier,          //   Sk
        Symbol_Other              //   So
    };

    enum Direction {
        DirL,
        DirR,
        DirEN,
        DirES,
        DirET,
        DirAN,
        DirCS,
        DirB,
        DirS,
        DirWS,
        DirON,
        DirLRE,
        DirLRO,
        DirAL,
        DirRLE,
        DirRLO,
        DirPDF,
        DirNSM,
        DirBN,
        DirLRI,
        DirRLI,
        DirFSI,
        DirPDI
    };

    enum Decomposition {
        NoDecomposition,
        Canonical,
        Font,
        NoBreak,
        Initial,
        Medial,
        Final,
        Isolated,
        Circle,
        Super,
        Sub,
        Vertical,
        Wide,
        Narrow,
        Small,
        Square,
        Compat,
        Fraction
    };

    enum Joining {
        OtherJoining,
        Dual,
        Right,
        Left,
        Causing,
        Transparent
    };

    enum UnicodeVersion {
        Unicode_Unassigned,
        Unicode_1_1,
        Unicode_2_0,
        Unicode_2_1,
        Unicode_3_0,
        Unicode_3_1,
        Unicode_3_2,
        Unicode_4_0,
        Unicode_4_1,
        Unicode_5_0,
        Unicode_5_1,
        Unicode_5_2,
        Unicode_6_0,
        Unicode_6_1,
        Unicode_6_2,
        Unicode_6_3,
        Unicode_7_0,
        Unicode_8_0,
        Unicode_9_0,
        Unicode_10_0,
        Unicode_11_0,
        Unicode_12_0,
        Unicode_12_1,
        Unicode_13_0,
        Unicode_14_0,
        Unicode_Last = Unicode_14_0
    };
    // ****** WHEN ADDING FUNCTIONS, CONSIDER ADDING TO QCharRef TOO

    Category category() const;
    Direction direction() const;
    Joining joining() const;
    bool hasMirrored() const;

    QChar mirroredChar() const;
    QString decomposition() const;
    Decomposition decompositionTag() const;

    int digitValue() const;
    QChar toLower() const;
    QChar toUpper() const;
    QChar toTitleCase() const;
    QChar toCaseFolded() const;

    UnicodeVersion unicodeVersion() const;

    char toAscii() const;
    inline char toLatin1() const;
    inline ushort unicode() const { return ucs; }
    inline ushort &unicode() { return ucs; }

    static QChar fromAscii(const char c);
    static QChar fromLatin1(const char c);

    inline bool isNull() const { return ucs == 0; }
    bool isPrint() const;
    bool isPunct() const;
    bool isSpace() const;
    bool isMark() const;
    bool isLetter() const;
    bool isNumber() const;
    bool isLetterOrNumber() const;
    bool isDigit() const;
    bool isSymbol() const;
    bool isLower() const;
    bool isUpper() const;
    bool isTitleCase() const;

    inline bool isHighSurrogate() const {
        return ((ucs & 0xfc00) == 0xd800);
    }
    inline bool isLowSurrogate() const {
        return ((ucs & 0xfc00) == 0xdc00);
    }

    inline uchar cell() const { return uchar(ucs & 0xff); }
    inline uchar row() const { return uchar((ucs>>8)&0xff); }
    inline void setCell(uchar cell);
    inline void setRow(uchar row);

    static inline bool isHighSurrogate(uint ucs4) {
        return ((ucs4 & 0xfffffc00) == 0xd800);
    }
    static inline bool isLowSurrogate(uint ucs4) {
        return ((ucs4 & 0xfffffc00) == 0xdc00);
    }
    static inline bool requiresSurrogates(uint ucs4) {
        return (ucs4 >= 0x10000);
    }
    static inline uint surrogateToUcs4(ushort high, ushort low) {
        return (uint(high)<<10) + low - 0x35fdc00;
    }
    static inline uint surrogateToUcs4(QChar high, QChar low) {
        return (uint(high.ucs)<<10) + low.ucs - 0x35fdc00;
    }
    static inline ushort highSurrogate(uint ucs4) {
        return ushort((ucs4>>10) + 0xd7c0);
    }
    static inline ushort lowSurrogate(uint ucs4) {
        return ushort(ucs4%0x400 + 0xdc00);
    }

    static Category QT_FASTCALL category(const uint ucs4);
    static Category QT_FASTCALL category(const ushort ucs2);
    static Direction QT_FASTCALL direction(const uint ucs4);
    static Direction QT_FASTCALL direction(const ushort ucs2);
    static Joining QT_FASTCALL joining(const uint ucs4);
    static Joining QT_FASTCALL joining(const ushort ucs2);

    static uint QT_FASTCALL mirroredChar(const uint ucs4);
    static ushort QT_FASTCALL mirroredChar(const ushort ucs2);
    static Decomposition QT_FASTCALL decompositionTag(const uint ucs4);

    static int QT_FASTCALL digitValue(const uint ucs4);
    static int QT_FASTCALL digitValue(const ushort ucs2);
    static uint QT_FASTCALL toLower(const uint ucs4);
    static ushort QT_FASTCALL toLower(const ushort ucs2);
    static uint QT_FASTCALL toUpper(const uint ucs4);
    static ushort QT_FASTCALL toUpper(const ushort ucs2);
    static uint QT_FASTCALL toTitleCase(const uint ucs4);
    static ushort QT_FASTCALL toTitleCase(const ushort ucs2);
    static uint QT_FASTCALL toCaseFolded(const uint ucs4);
    static ushort QT_FASTCALL toCaseFolded(const ushort ucs2);

    static UnicodeVersion QT_FASTCALL unicodeVersion(const uint ucs4);
    static UnicodeVersion QT_FASTCALL unicodeVersion(const ushort ucs2);

    static UnicodeVersion QT_FASTCALL currentUnicodeVersion();

    static QString QT_FASTCALL decomposition(const uint ucs4);


private:
#ifdef QT_NO_CAST_FROM_ASCII
    QChar(const char c);
    QChar(const uchar c);
#endif
    ushort ucs;
};

Q_DECLARE_TYPEINFO(QChar, Q_MOVABLE_TYPE);

inline QChar::QChar() : ucs(0) {}

inline char QChar::toLatin1() const { return ucs > 0xff ? '\0' : char(ucs); }
inline QChar QChar::fromLatin1(const char c) { return QChar(ushort(uchar(c))); }

inline QChar::QChar(const uchar c, uchar r) : ucs(ushort((r << 8) | c)){}
inline QChar::QChar(const ushort rc) : ucs(rc){}
inline QChar::QChar(const short rc) : ucs(ushort(rc)){}
inline QChar::QChar(const uint rc) : ucs(ushort(rc & 0xffff)){}
inline QChar::QChar(const int rc) : ucs(ushort(rc & 0xffff)){}
inline QChar::QChar(const SpecialCharacter s) : ucs(ushort(s)) {}
inline QChar::QChar(const QLatin1Char ch) : ucs(ch.unicode()) {}

inline void QChar::setCell(const uchar acell)
{ ucs = ushort((ucs & 0xff00) + acell); }
inline void QChar::setRow(const uchar arow)
{ ucs = ushort((ushort(arow)<<8) + (ucs&0xff)); }

inline bool operator==(const QChar c1, const QChar c2) { return c1.unicode() == c2.unicode(); }
inline bool operator!=(const QChar c1, const QChar c2) { return c1.unicode() != c2.unicode(); }
inline bool operator<=(const QChar c1, const QChar c2) { return c1.unicode() <= c2.unicode(); }
inline bool operator>=(const QChar c1, const QChar c2) { return c1.unicode() >= c2.unicode(); }
inline bool operator<(const QChar c1, const QChar c2) { return c1.unicode() < c2.unicode(); }
inline bool operator>(const QChar c1, const QChar c2) { return c1.unicode() > c2.unicode(); }

#ifndef QT_NO_DATASTREAM
Q_CORE_EXPORT QDataStream &operator<<(QDataStream &, const QChar &);
Q_CORE_EXPORT QDataStream &operator>>(QDataStream &, QChar &);
#endif

QT_END_NAMESPACE


#endif // QCHAR_H
