%{
#include "QtCore/qstring.h"
QT_USE_NAMESPACE
%}

#undef Q_REQUIRED_RESULT
#define Q_REQUIRED_RESULT

class QString
{
public:
    QString();
    QString(const QChar *unicode, int size = -1);
    QString(const QChar c);
    QString(const int size, const QChar c);
    QString(const QLatin1String &latin1);
    QString(const QString &);
    ~QString();
    QString &operator=(QChar c);
    QString &operator=(const QString &);
    QString &operator=(const QLatin1String &);
#ifdef Q_COMPILER_RVALUE_REFS
    QString &operator=(QString &&other);
#endif
    void swap(QString &other);
    int size() const;
    int count() const;
    int length() const;
    bool isEmpty() const;
    bool isNull() const;
    void resize(int size);

    QString &fill(QChar c, int size = -1);
    void truncate(int pos);
    void chop(int n);

    int capacity() const;
    void reserve(int size);
    void squeeze();

    const QChar *unicode() const;
    QChar *data();
    const QChar *data() const;
    const QChar *constData() const;

    void detach();
    bool isDetached() const;
    bool isSharedWith(const QString &other) const;
    void clear();

    const QChar at(int i) const;
    const QChar operator[](int i) const;
    QCharRef operator[](int i);
    const QChar operator[](uint i) const;
    QCharRef operator[](uint i);

    QString arg(qlonglong a, int fieldwidth=0, int base=10,
                const QChar &fillChar = QLatin1Char(' ')) const Q_REQUIRED_RESULT;
    QString arg(qulonglong a, int fieldwidth=0, int base=10,
                const QChar &fillChar = QLatin1Char(' ')) const Q_REQUIRED_RESULT;
    QString arg(long a, int fieldwidth=0, int base=10,
                const QChar &fillChar = QLatin1Char(' ')) const Q_REQUIRED_RESULT;
    QString arg(ulong a, int fieldwidth=0, int base=10,
                const QChar &fillChar = QLatin1Char(' ')) const Q_REQUIRED_RESULT;
    QString arg(int a, int fieldWidth = 0, int base = 10,
                const QChar &fillChar = QLatin1Char(' ')) const Q_REQUIRED_RESULT;
    QString arg(uint a, int fieldWidth = 0, int base = 10,
                const QChar &fillChar = QLatin1Char(' ')) const Q_REQUIRED_RESULT;
    QString arg(short a, int fieldWidth = 0, int base = 10,
                const QChar &fillChar = QLatin1Char(' ')) const Q_REQUIRED_RESULT;
    QString arg(ushort a, int fieldWidth = 0, int base = 10,
                const QChar &fillChar = QLatin1Char(' ')) const Q_REQUIRED_RESULT;
    QString arg(double a, int fieldWidth = 0, char fmt = 'g', int prec = -1,
                const QChar &fillChar = QLatin1Char(' ')) const Q_REQUIRED_RESULT;
    QString arg(char a, int fieldWidth = 0,
                const QChar &fillChar = QLatin1Char(' ')) const Q_REQUIRED_RESULT;
    QString arg(QChar a, int fieldWidth = 0,
                const QChar &fillChar = QLatin1Char(' ')) const Q_REQUIRED_RESULT;
    QString arg(const QString &a, int fieldWidth = 0,
                const QChar &fillChar = QLatin1Char(' ')) const Q_REQUIRED_RESULT;
    QString arg(const QString &a1, const QString &a2) const Q_REQUIRED_RESULT;
    QString arg(const QString &a1, const QString &a2, const QString &a3) const Q_REQUIRED_RESULT;
    QString arg(const QString &a1, const QString &a2, const QString &a3,
                const QString &a4) const Q_REQUIRED_RESULT;
    QString arg(const QString &a1, const QString &a2, const QString &a3,
                const QString &a4, const QString &a5) const Q_REQUIRED_RESULT;
    QString arg(const QString &a1, const QString &a2, const QString &a3,
                const QString &a4, const QString &a5, const QString &a6) const Q_REQUIRED_RESULT;
    QString arg(const QString &a1, const QString &a2, const QString &a3,
                const QString &a4, const QString &a5, const QString &a6,
                const QString &a7) const Q_REQUIRED_RESULT;
    QString arg(const QString &a1, const QString &a2, const QString &a3,
                const QString &a4, const QString &a5, const QString &a6,
                const QString &a7, const QString &a8) const Q_REQUIRED_RESULT;
    QString arg(const QString &a1, const QString &a2, const QString &a3,
                const QString &a4, const QString &a5, const QString &a6,
                const QString &a7, const QString &a8, const QString &a9) const Q_REQUIRED_RESULT;

    QString    &vsprintf(const char *format, va_list ap)
#if defined(Q_CC_GNU) && !defined(__INSURE__)
        __attribute__ ((format (printf, 2, 0)))
#endif
        ;
    QString    &sprintf(const char *format, ...)
#if defined(Q_CC_GNU) && !defined(__INSURE__)
        __attribute__ ((format (printf, 2, 3)))
#endif
        ;

    int indexOf(QChar c, int from = 0, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int indexOf(const QString &s, int from = 0, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int indexOf(const QLatin1String &s, int from = 0, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int indexOf(const QStringRef &s, int from = 0, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int lastIndexOf(QChar c, int from = -1, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int lastIndexOf(const QString &s, int from = -1, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int lastIndexOf(const QLatin1String &s, int from = -1, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int lastIndexOf(const QStringRef &s, int from = -1, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;

    bool contains(QChar c, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    bool contains(const QString &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    bool contains(const QStringRef &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int count(QChar c, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int count(const QString &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int count(const QStringRef &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;

#ifndef QT_NO_REGEXP
    int indexOf(const QRegExp &, int from = 0) const;
    int lastIndexOf(const QRegExp &, int from = -1) const;
    bool contains(const QRegExp &rx) const;
    int count(const QRegExp &) const;

    int indexOf(QRegExp &, int from = 0) const;
    int lastIndexOf(QRegExp &, int from = -1) const;
    bool contains(QRegExp &rx) const;
#endif

    enum SectionFlags {
        SectionDefault             = 0x00,
        SectionSkipEmpty           = 0x01,
        SectionIncludeLeadingSep   = 0x02,
        SectionIncludeTrailingSep  = 0x04,
        SectionCaseInsensitiveSeps = 0x08
    };

    QString section(QChar sep, int start, int end = -1, SectionFlags flags = SectionDefault) const;
    QString section(const QString &in_sep, int start, int end = -1, SectionFlags flags = SectionDefault) const;
#ifndef QT_NO_REGEXP
    QString section(const QRegExp &reg, int start, int end = -1, SectionFlags flags = SectionDefault) const;
#endif

    QString left(int n) const Q_REQUIRED_RESULT;
    QString right(int n) const Q_REQUIRED_RESULT;
    QString mid(int position, int n = -1) const Q_REQUIRED_RESULT;
    QStringRef leftRef(int n) const Q_REQUIRED_RESULT;
    QStringRef rightRef(int n) const Q_REQUIRED_RESULT;
    QStringRef midRef(int position, int n = -1) const Q_REQUIRED_RESULT;

    bool startsWith(const QString &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    bool startsWith(const QStringRef &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    bool startsWith(const QLatin1String &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    bool startsWith(const QChar &c, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    bool endsWith(const QString &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    bool endsWith(const QStringRef &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    bool endsWith(const QLatin1String &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    bool endsWith(const QChar &c, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;

    QString leftJustified(int width, QChar fill = QLatin1Char(' '), bool trunc = false) const Q_REQUIRED_RESULT;
    QString rightJustified(int width, QChar fill = QLatin1Char(' '), bool trunc = false) const Q_REQUIRED_RESULT;

    QString toLower() const Q_REQUIRED_RESULT;
    QString toUpper() const Q_REQUIRED_RESULT;
    QString toCaseFolded() const Q_REQUIRED_RESULT;

    QString trimmed() const Q_REQUIRED_RESULT;
    QString simplified() const Q_REQUIRED_RESULT;

    QString &insert(int i, QChar c);
    QString &insert(int i, const QChar *uc, int len);
    QString &insert(int i, const QString &s);
    QString &insert(int i, const QLatin1String &s);
    QString &append(QChar c);
    QString &append(const QString &s);
    QString &append(const QStringRef &s);
    QString &append(const QLatin1String &s);
    QString &prepend(QChar c);
    QString &prepend(const QString &s);
    QString &prepend(const QLatin1String &s);

    QString &operator+=(QChar c);
    QString &operator+=(QChar::SpecialCharacter c);
    QString &operator+=(const QString &s);
    QString &operator+=(const QStringRef &s);
    QString &operator+=(const QLatin1String &s);

    QString &remove(int i, int len);
    QString &remove(QChar c, Qt::CaseSensitivity cs = Qt::CaseSensitive);
    QString &remove(const QString &s, Qt::CaseSensitivity cs = Qt::CaseSensitive);
    QString &replace(int i, int len, QChar after);
    QString &replace(int i, int len, const QChar *s, int slen);
    QString &replace(int i, int len, const QString &after);
    QString &replace(QChar before, QChar after, Qt::CaseSensitivity cs = Qt::CaseSensitive);
    QString &replace(const QChar *before, int blen, const QChar *after, int alen, Qt::CaseSensitivity cs = Qt::CaseSensitive);
    QString &replace(const QLatin1String &before, const QLatin1String &after, Qt::CaseSensitivity cs = Qt::CaseSensitive);
    QString &replace(const QLatin1String &before, const QString &after, Qt::CaseSensitivity cs = Qt::CaseSensitive);
    QString &replace(const QString &before, const QLatin1String &after, Qt::CaseSensitivity cs = Qt::CaseSensitive);
    QString &replace(const QString &before, const QString &after,
                     Qt::CaseSensitivity cs = Qt::CaseSensitive);
    QString &replace(QChar c, const QString &after, Qt::CaseSensitivity cs = Qt::CaseSensitive);
    QString &replace(QChar c, const QLatin1String &after, Qt::CaseSensitivity cs = Qt::CaseSensitive);
#ifndef QT_NO_REGEXP
    QString &replace(const QRegExp &rx, const QString &after);
    QString &remove(const QRegExp &rx);
#endif

    enum SplitBehavior { KeepEmptyParts, SkipEmptyParts };

    QStringList split(const QString &sep, SplitBehavior behavior = KeepEmptyParts,
                      Qt::CaseSensitivity cs = Qt::CaseSensitive) const Q_REQUIRED_RESULT;
    QStringList split(const QChar &sep, SplitBehavior behavior = KeepEmptyParts,
                      Qt::CaseSensitivity cs = Qt::CaseSensitive) const Q_REQUIRED_RESULT;
#ifndef QT_NO_REGEXP
    QStringList split(const QRegExp &sep, SplitBehavior behavior = KeepEmptyParts) const Q_REQUIRED_RESULT;
#endif

    enum NormalizationForm {
        NormalizationForm_D,
        NormalizationForm_C,
        NormalizationForm_KD,
        NormalizationForm_KC
    };
    QString normalized(NormalizationForm mode) const Q_REQUIRED_RESULT;
    QString normalized(NormalizationForm mode, QChar::UnicodeVersion version) const Q_REQUIRED_RESULT;

    QString repeated(int times) const;

    const ushort *utf16() const;

    QByteArray toAscii() const Q_REQUIRED_RESULT;
    QByteArray toLatin1() const Q_REQUIRED_RESULT;
    QByteArray toUtf8() const Q_REQUIRED_RESULT;
    QByteArray toLocal8Bit() const Q_REQUIRED_RESULT;
    QVector<uint> toUcs4() const Q_REQUIRED_RESULT;

    static QString fromAscii(const char *, int size = -1);
    static QString fromLatin1(const char *, int size = -1);
    static QString fromUtf8(const char *, int size = -1);
    static QString fromLocal8Bit(const char *, int size = -1);
    static QString fromUtf16(const ushort *, int size = -1);
    static QString fromUcs4(const uint *, int size = -1);
    static QString fromRawData(const QChar *, int size);

    int toWCharArray(wchar_t *array) const;
    static QString fromWCharArray(const wchar_t *, int size = -1);

    QString &setRawData(const QChar *unicode, int size);
    QString &setUnicode(const QChar *unicode, int size);
    QString &setUtf16(const ushort *utf16, int size);

    int compare(const QString &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;

    int compare(const QLatin1String &other, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;

    static int compare(const QString &s1, const QString &s2, Qt::CaseSensitivity cs = Qt::CaseSensitive);

    static int compare(const QString& s1, const QLatin1String &s2,
                              Qt::CaseSensitivity cs = Qt::CaseSensitive);
    static int compare(const QLatin1String& s1, const QString &s2,
                              Qt::CaseSensitivity cs = Qt::CaseSensitive);

    int compare(const QStringRef &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    static int compare(const QString &s1, const QStringRef &s2,
                       Qt::CaseSensitivity = Qt::CaseSensitive);

    int localeAwareCompare(const QString& s) const;
    static int localeAwareCompare(const QString& s1, const QString& s2);

    int localeAwareCompare(const QStringRef &s) const;
    static int localeAwareCompare(const QString& s1, const QStringRef& s2);

    short  toShort(bool *ok=Q_NULLPTR, int base=10) const;
    ushort toUShort(bool *ok=Q_NULLPTR, int base=10) const;
    int toInt(bool *ok=Q_NULLPTR, int base=10) const;
    uint toUInt(bool *ok=Q_NULLPTR, int base=10) const;
    long toLong(bool *ok=Q_NULLPTR, int base=10) const;
    ulong toULong(bool *ok=Q_NULLPTR, int base=10) const;
    qlonglong toLongLong(bool *ok=Q_NULLPTR, int base=10) const;
    qulonglong toULongLong(bool *ok=Q_NULLPTR, int base=10) const;
    float toFloat(bool *ok=Q_NULLPTR) const;
    double toDouble(bool *ok=Q_NULLPTR) const;

    QString &setNum(short, int base=10);
    QString &setNum(ushort, int base=10);
    QString &setNum(int, int base=10);
    QString &setNum(uint, int base=10);
    QString &setNum(long, int base=10);
    QString &setNum(ulong, int base=10);
    QString &setNum(qlonglong, int base=10);
    QString &setNum(qulonglong, int base=10);
    QString &setNum(float, char f='g', int prec=6);
    QString &setNum(double, char f='g', int prec=6);

    static QString number(int, int base=10);
    static QString number(uint, int base=10);
    static QString number(long, int base=10);
    static QString number(ulong, int base=10);
    static QString number(qlonglong, int base=10);
    static QString number(qulonglong, int base=10);
    static QString number(double, char f='g', int prec=6);

    bool operator==(const QString &s) const;
    bool operator<(const QString &s) const;
    bool operator>(const QString &s) const;
    bool operator!=(const QString &s) const;
    bool operator<=(const QString &s) const;
    bool operator>=(const QString &s) const;

    bool operator==(const QLatin1String &s) const;
    bool operator<(const QLatin1String &s) const;
    bool operator>(const QLatin1String &s) const;
    bool operator!=(const QLatin1String &s) const;
    bool operator<=(const QLatin1String &s) const;
    bool operator>=(const QLatin1String &s) const;

    // ASCII compatibility
#ifndef QT_NO_CAST_FROM_ASCII
    QString(const char *ch);
    QString(const QByteArray &a);
    QString &operator=(const char *ch);
    QString &operator=(const QByteArray &a);
    QString &operator=(char c);

    // these are needed, so it compiles with STL support enabled
    QString &prepend(const char *s);
    QString &prepend(const QByteArray &s);
    QString &append(const char *s);
    QString &append(const QByteArray &s);
    QString &operator+=(const char *s);
    QString &operator+=(const QByteArray &s);
    QString &operator+=(char c);

    bool operator==(const char *s) const;
    bool operator!=(const char *s) const;
    bool operator<(const char *s) const;
    bool operator<=(const char *s2) const;
    bool operator>(const char *s2) const;
    bool operator>=(const char *s2) const;

    bool operator==(const QByteArray &s) const;
    bool operator!=(const QByteArray &s) const;
    bool operator<(const QByteArray &s) const;
    bool operator>(const QByteArray &s) const;
    bool operator<=(const QByteArray &s) const;
    bool operator>=(const QByteArray &s) const;
#endif

    typedef QChar *iterator;
    typedef const QChar *const_iterator;
    typedef iterator Iterator;
    typedef const_iterator ConstIterator;
    iterator begin();
    const_iterator begin() const;
    const_iterator constBegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator constEnd() const;

    // STL compatibility
    typedef const QChar & const_reference;
    typedef QChar & reference;
    typedef QChar value_type;
    void push_back(QChar c);
    void push_back(const QString &s);
    void push_front(QChar c);
    void push_front(const QString &s);

    static QString fromStdString(const std::string &s);
    std::string toStdString() const;
#ifndef QT_NO_STL_WCHAR
    static QString fromStdWString(const QStdWString &s);
    QStdWString toStdWString() const;
#endif // QT_NO_STL_WCHAR

    bool isSimpleText() const;
    bool isRightToLeft() const;

    QString(int size, Qt::Initialization);

    typedef Data * DataPtr;
    DataPtr &data_ptr();
};


class QLatin1String
{
public:
    QLatin1String(const char *s);
    QLatin1String &operator=(const QLatin1String &other);

    const char *latin1() const;

    bool operator==(const QString &s) const;
    bool operator!=(const QString &s) const;
    bool operator>(const QString &s) const;
    bool operator<(const QString &s) const;
    bool operator>=(const QString &s) const;
    bool operator<=(const QString &s) const;

    // QT_ASCII_CAST_WARN
    bool operator==(const char *s) const;
    bool operator!=(const char *s) const;
    bool operator<(const char *s) const;
    bool operator>(const char *s) const;
    bool operator<=(const char *s) const;
    bool operator>=(const char *s) const;
};


class QCharRef {
    QCharRef(QString &str, int idx);
public:

    // most QChar operations repeated here

    // all this is not documented: We just say "like QChar" and let it be.
    operator QChar() const;
    QCharRef &operator=(const QChar &c);

    // An operator= for each QChar cast constructors
#ifndef QT_NO_CAST_FROM_ASCII
    QCharRef &operator=(char c);
    QCharRef &operator=(uchar c);
#endif
    QCharRef &operator=(const QCharRef &c);
    QCharRef &operator=(ushort rc);
    QCharRef &operator=(short rc);
    QCharRef &operator=(uint rc);
    QCharRef &operator=(int rc);

    // each function...
    bool isNull() const;
    bool isPrint() const;
    bool isPunct() const;
    bool isSpace() const;
    bool isMark() const;
    bool isLetter() const;
    bool isNumber() const;
    bool isLetterOrNumber() const;
    bool isDigit() const;
    bool isLower() const;
    bool isUpper() const;
    bool isTitleCase() const;

    int digitValue() const;
    QChar toLower() const;
    QChar toUpper() const;
    QChar toTitleCase () const;

    QChar::Category category() const;
    QChar::Direction direction() const;
    QChar::Joining joining() const;
    bool hasMirrored() const;
    QChar mirroredChar() const;
    QString decomposition() const;
    QChar::Decomposition decompositionTag() const;
    uchar combiningClass() const;

    QChar::UnicodeVersion unicodeVersion() const;

    uchar cell() const;
    uchar row() const;
    void setCell(uchar cell);
    void setRow(uchar row);

    char toAscii() const;
    char toLatin1() const;
    ushort unicode() const;
    ushort& unicode();
};

#if !defined(QT_NO_DATASTREAM)
QDataStream &operator<<(QDataStream &, const QString &);
QDataStream &operator>>(QDataStream &, QString &);
#endif

class QStringRef {
public:
    QStringRef();
    QStringRef(const QString *string, int position, int size);
    QStringRef(const QString *string);
    QStringRef(const QStringRef &other);

    ~QStringRef();
    const QString *string() const;
    int position() const;
    int size() const;
    int count() const;
    int length() const;

    QStringRef &operator=(const QStringRef &other);

    int indexOf(const QString &str, int from = 0, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int indexOf(QChar ch, int from = 0, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int indexOf(QLatin1String str, int from = 0, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int indexOf(const QStringRef &str, int from = 0, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int lastIndexOf(const QString &str, int from = -1, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int lastIndexOf(QChar ch, int from = -1, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int lastIndexOf(QLatin1String str, int from = -1, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int lastIndexOf(const QStringRef &str, int from = -1, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;

    bool contains(const QString &str, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    bool contains(QChar ch, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    bool contains(QLatin1String str, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    bool contains(const QStringRef &str, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;

    int count(const QString &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int count(QChar c, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int count(const QStringRef &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;

    bool startsWith(const QString &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    bool startsWith(QLatin1String s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    bool startsWith(QChar c, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    bool startsWith(const QStringRef &c, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;

    bool endsWith(const QString &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    bool endsWith(QLatin1String s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    bool endsWith(QChar c, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    bool endsWith(const QStringRef &c, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;

    QStringRef &operator=(const QString *string);

    const QChar *unicode() const;
    const QChar *data() const;
    const QChar *constData() const;

    QByteArray toAscii() const Q_REQUIRED_RESULT;
    QByteArray toLatin1() const Q_REQUIRED_RESULT;
    QByteArray toUtf8() const Q_REQUIRED_RESULT;
    QByteArray toLocal8Bit() const Q_REQUIRED_RESULT;
    QVector<uint> toUcs4() const Q_REQUIRED_RESULT;

    void clear();
    QString toString() const;
    bool isEmpty() const;
    bool isNull() const;

    QStringRef appendTo(QString *string) const;

    const QChar at(int i) const;

    int compare(const QString &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int compare(const QStringRef &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int compare(QLatin1String s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    static int compare(const QStringRef &s1, const QString &s2,
                       Qt::CaseSensitivity = Qt::CaseSensitive);
    static int compare(const QStringRef &s1, const QStringRef &s2,
                       Qt::CaseSensitivity = Qt::CaseSensitive);
    static int compare(const QStringRef &s1, QLatin1String s2,
                       Qt::CaseSensitivity cs = Qt::CaseSensitive);

    int localeAwareCompare(const QString &s) const;
    int localeAwareCompare(const QStringRef &s) const;
    static int localeAwareCompare(const QStringRef &s1, const QString &s2);
    static int localeAwareCompare(const QStringRef &s1, const QStringRef &s2);
};

// Those are Q_CORE_EXPORT'ed not inline'ed
bool operator==(const QStringRef &s1,const QStringRef &s2);
bool operator==(const QString &s1,const QStringRef &s2);
bool operator==(const QLatin1String &s1, const QStringRef &s2);

bool operator<(const QStringRef &s1,const QStringRef &s2);
