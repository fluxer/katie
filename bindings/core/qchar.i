%{
#include "QtCore/qchar.h"
QT_USE_NAMESPACE
%}

#undef QT_FASTCALL
#define QT_FASTCALL

class QLatin1Char
{
public:
    QLatin1Char(const char c);
    char toLatin1() const;
    ushort unicode() const;
};


class QChar {
public:
    QChar();
#ifndef QT_NO_CAST_FROM_ASCII
    QChar(const char c);
    QChar(const uchar c);
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

    enum Category
    {
        NoCategory,    // ### Qt 5: replace with Other_NotAssigned

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
        Symbol_Other             //   So
    };

    enum Direction
    {
        DirL, DirR, DirEN, DirES, DirET, DirAN, DirCS, DirB, DirS, DirWS, DirON,
        DirLRE, DirLRO, DirAL, DirRLE, DirRLO, DirPDF, DirNSM, DirBN
    };

    enum Decomposition
    {
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

    enum Joining
    {
        OtherJoining, Dual, Right, Center
    };

    enum CombiningClass
    {
        Combining_BelowLeftAttached       = 200,
        Combining_BelowAttached           = 202,
        Combining_BelowRightAttached      = 204,
        Combining_LeftAttached            = 208,
        Combining_RightAttached           = 210,
        Combining_AboveLeftAttached       = 212,
        Combining_AboveAttached           = 214,
        Combining_AboveRightAttached      = 216,

        Combining_BelowLeft               = 218,
        Combining_Below                   = 220,
        Combining_BelowRight              = 222,
        Combining_Left                    = 224,
        Combining_Right                   = 226,
        Combining_AboveLeft               = 228,
        Combining_Above                   = 230,
        Combining_AboveRight              = 232,

        Combining_DoubleBelow             = 233,
        Combining_DoubleAbove             = 234,
        Combining_IotaSubscript           = 240
    };

    enum UnicodeVersion {
        Unicode_Unassigned,    // ### Qt 5: assign with some constantly big value
        Unicode_1_1,
        Unicode_2_0,
        Unicode_2_1_2,
        Unicode_3_0,
        Unicode_3_1,
        Unicode_3_2,
        Unicode_4_0,
        Unicode_4_1,
        Unicode_5_0
    };
    // ****** WHEN ADDING FUNCTIONS, CONSIDER ADDING TO QCharRef TOO

    Category category() const;
    Direction direction() const;
    Joining joining() const;
    bool hasMirrored() const;
    unsigned char combiningClass() const;

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
    char toLatin1() const;
    ushort unicode() const;
    ushort &unicode();

    static QChar fromAscii(const char c);
    static QChar fromLatin1(const char c);

    bool isNull() const;
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

    bool isHighSurrogate() const;
    bool isLowSurrogate() const;

    uchar cell() const;
    uchar row() const;
    void setCell(uchar cell);
    void setRow(uchar row);

    static bool isHighSurrogate(uint ucs4);
    static bool isLowSurrogate(uint ucs4);
    static bool requiresSurrogates(uint ucs4);
    static uint surrogateToUcs4(ushort high, ushort low);
    static uint surrogateToUcs4(QChar high, QChar low);
    static ushort highSurrogate(uint ucs4);
    static ushort lowSurrogate(uint ucs4);

    static Category QT_FASTCALL category(const uint ucs4);
    static Category QT_FASTCALL category(const ushort ucs2);
    static Direction QT_FASTCALL direction(const uint ucs4);
    static Direction QT_FASTCALL direction(const ushort ucs2);
    static Joining QT_FASTCALL joining(const uint ucs4);
    static Joining QT_FASTCALL joining(const ushort ucs2);
    static unsigned char QT_FASTCALL combiningClass(const uint ucs4);
    static unsigned char QT_FASTCALL combiningClass(const ushort ucs2);

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

    static QString QT_FASTCALL decomposition(uint ucs4);


private:
#ifdef QT_NO_CAST_FROM_ASCII
    QChar(const char c);
    QChar(const uchar c);
#endif
};

#ifndef QT_NO_DATASTREAM
QDataStream &operator<<(QDataStream &, const QChar &);
QDataStream &operator>>(QDataStream &, QChar &);
#endif
