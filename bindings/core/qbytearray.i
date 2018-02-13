%{
#include "QtCore/qbytearray.h"
QT_USE_NAMESPACE
%}

char *qstrdup(const char *);

uint qstrlen(const char *str);
uint qstrnlen(const char *str, uint maxlen);

char *qstrcpy(char *dst, const char *src);
char *qstrncpy(char *dst, const char *src, uint len);

int qstrcmp(const char *str1, const char *str2);
int qstrcmp(const QByteArray &str1, const QByteArray &str2);
int qstrcmp(const QByteArray &str1, const char *str2);
static int qstrcmp(const char *str1, const QByteArray &str2);
int qstrncmp(const char *str1, const char *str2, uint len);
int qstricmp(const char *, const char *);
int qstrnicmp(const char *, const char *, uint len);

// implemented in qvsnprintf.cpp
int qvsnprintf(char *str, size_t n, const char *fmt, va_list ap);
int qsnprintf(char *str, size_t n, const char *fmt, ...);


// qChecksum: Internet checksum
quint16 qChecksum(const char *s, uint len);

class QByteArray
{
public:
    QByteArray();
    QByteArray(const char *);
    QByteArray(const char *, int size);
    QByteArray(int size, char c);
    QByteArray(int size, Qt::Initialization);
    QByteArray(const QByteArray &);
    ~QByteArray();

    QByteArray &operator=(const QByteArray &);
    QByteArray &operator=(const char *str);
#ifdef Q_COMPILER_RVALUE_REFS
    QByteArray &operator=(QByteArray &&other);
#endif

    void swap(QByteArray &other);

    int size() const;
    bool isEmpty() const;
    void resize(int size);

    QByteArray &fill(char c, int size = -1);

    int capacity() const;
    void reserve(int size);
    void squeeze();

#ifndef QT_NO_CAST_FROM_BYTEARRAY
    operator const char *() const;
#endif
    char *data();
    const char *data() const;
    const char *constData() const;
    void detach();
    bool isDetached() const;
    bool isSharedWith(const QByteArray &other) const;
    void clear();

    char at(int i) const;
    char operator[](int i) const;
    char operator[](uint i) const;
    QByteRef operator[](int i);
    QByteRef operator[](uint i);

    int indexOf(char c, int from = 0) const;
    int indexOf(const char *c, const int from = 0) const;
    int indexOf(const QByteArray &a, const int from = 0) const;
    int lastIndexOf(char c, const int from = -1) const;
    int lastIndexOf(const char *c, const int from = -1) const;
    int lastIndexOf(const QByteArray &a, const int from = -1) const;

    bool contains(char c) const;
    bool contains(const char *a) const;
    bool contains(const QByteArray &a) const;
    int count(char c) const;
    int count(const char *a) const;
    int count(const QByteArray &a) const;

    QByteArray left(int len) const;
    QByteArray right(int len) const;
    QByteArray mid(int index, int len = -1) const;

    bool startsWith(const QByteArray &a) const;
    bool startsWith(char c) const;
    bool startsWith(const char *c) const;

    bool endsWith(const QByteArray &a) const;
    bool endsWith(char c) const;
    bool endsWith(const char *c) const;

    void truncate(int pos);
    void chop(int n);

    QByteArray toLower() const;
    QByteArray toUpper() const;

    QByteArray trimmed() const;
    QByteArray simplified() const;
    QByteArray leftJustified(int width, char fill = ' ', bool truncate = false) const;
    QByteArray rightJustified(int width, char fill = ' ', bool truncate = false) const;


    QByteArray &prepend(char c);
    QByteArray &prepend(const char *s);
    QByteArray &prepend(const char *s, int len);
    QByteArray &prepend(const QByteArray &a);
    QByteArray &append(char c);
    QByteArray &append(const char *s);
    QByteArray &append(const char *s, int len);
    QByteArray &append(const QByteArray &a);
    QByteArray &insert(int i, char c);
    QByteArray &insert(int i, const char *s);
    QByteArray &insert(int i, const char *s, int len);
    QByteArray &insert(int i, const QByteArray &a);
    QByteArray &remove(int index, int len);
    QByteArray &replace(int index, int len, const char *s);
    QByteArray &replace(int index, int len, const char *s, int alen);
    QByteArray &replace(int index, int len, const QByteArray &s);
    QByteArray &replace(char before, const char *after);
    QByteArray &replace(char before, const QByteArray &after);
    QByteArray &replace(const char *before, const char *after);
    QByteArray &replace(const char *before, int bsize, const char *after, int asize);
    QByteArray &replace(const QByteArray &before, const QByteArray &after);
    QByteArray &replace(const QByteArray &before, const char *after);
    QByteArray &replace(const char *before, const QByteArray &after);
    QByteArray &replace(char before, char after);
    QByteArray &operator+=(char c);
    QByteArray &operator+=(const char *s);
    QByteArray &operator+=(const QByteArray &a);

    QList<QByteArray> split(char sep) const;

    QByteArray repeated(const int times) const;

#ifndef QT_NO_CAST_TO_ASCII
    QByteArray &append(const QString &s);
    QByteArray &insert(const int i, const QString &s);
    QByteArray &replace(const QString &before, const char *after);
    QByteArray &replace(char c, const QString &after);
    QByteArray &replace(const QString &before, const QByteArray &after);

    QByteArray &operator+=(const QString &s);
    int indexOf(const QString &s, const int from = 0) const;
    int lastIndexOf(const QString &s, int const from = -1) const;
#endif
#ifndef QT_NO_CAST_FROM_ASCII
    bool operator==(const QString &s2) const;
    bool operator!=(const QString &s2) const;
    bool operator<(const QString &s2) const;
    bool operator>(const QString &s2) const;
    bool operator<=(const QString &s2) const;
    bool operator>=(const QString &s2) const;
#endif

    short toShort(bool *ok = Q_NULLPTR, int base = 10) const;
    ushort toUShort(bool *ok = Q_NULLPTR, int base = 10) const;
    int toInt(bool *ok = Q_NULLPTR, int base = 10) const;
    uint toUInt(bool *ok = Q_NULLPTR, int base = 10) const;
    long toLong(bool *ok = Q_NULLPTR, int base = 10) const;
    ulong toULong(bool *ok = Q_NULLPTR, int base = 10) const;
    qlonglong toLongLong(bool *ok = Q_NULLPTR, int base = 10) const;
    qulonglong toULongLong(bool *ok = Q_NULLPTR, int base = 10) const;
    float toFloat(bool *ok = Q_NULLPTR) const;
    double toDouble(bool *ok = Q_NULLPTR) const;
    QByteArray toBase64() const;
    QByteArray toHex() const;
    QByteArray toPercentEncoding(const QByteArray &exclude = QByteArray(),
                                 const QByteArray &include = QByteArray(),
                                 char percent = '%') const;

    QByteArray &setNum(short, int base = 10);
    QByteArray &setNum(ushort, int base = 10);
    QByteArray &setNum(int, int base = 10);
    QByteArray &setNum(uint, int base = 10);
    QByteArray &setNum(qlonglong, int base = 10);
    QByteArray &setNum(qulonglong, int base = 10);
    QByteArray &setNum(float, char f = 'g', int prec = 6);
    QByteArray &setNum(double, char f = 'g', int prec = 6);
    QByteArray &setRawData(const char *a, uint n); // ### Qt 5: use an int

    static QByteArray number(int, int base = 10);
    static QByteArray number(uint, int base = 10);
    static QByteArray number(qlonglong, int base = 10);
    static QByteArray number(qulonglong, int base = 10);
    static QByteArray number(double, char f = 'g', int prec = 6);
    static QByteArray fromRawData(const char *, int size);
    static QByteArray fromBase64(const QByteArray &base64);
    static QByteArray fromHex(const QByteArray &hexEncoded);
    static QByteArray fromPercentEncoding(const QByteArray &pctEncoded, char percent = '%');

    // stl compatibility
    void push_back(char c);
    void push_back(const char *c);
    void push_back(const QByteArray &a);
    void push_front(char c);
    void push_front(const char *c);
    void push_front(const QByteArray &a);

    static QByteArray fromStdString(const std::string &s);
    std::string toStdString() const;

    int count() const;
    int length() const;
    bool isNull() const;

public:
    typedef Data * DataPtr;
    DataPtr &data_ptr();
};

#if !defined(QT_NO_DATASTREAM)
QDataStream &operator<<(QDataStream &, const QByteArray &);
QDataStream &operator>>(QDataStream &, QByteArray &);
#endif

#ifndef QT_NO_COMPRESS
QByteArray qCompress(const uchar* data, int nbytes, int compressionLevel = -1);
QByteArray qUncompress(const uchar* data, int nbytes);
QByteArray qCompress(const QByteArray& data, int compressionLevel = -1);
QByteArray qUncompress(const QByteArray& data);
#endif
