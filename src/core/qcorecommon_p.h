#ifndef QCORECOMMON_P_H
#define QCORECOMMON_P_H

#include "qstring.h"

#include <time.h>
#include <limits.h>
#include <ctype.h>

QT_BEGIN_NAMESPACE

#define QBYTEARRAY_MAX INT_MAX

// enough space to hold BOM, each char as surrogate pair and terminator
#define QMAXUSTRLEN(X) 4 + (X * 2) + 2

// VLAs are nasty but using them by-passes std::bad_alloc exception for release
// builds (negative size VLAs are a thing, exceptions may even be disabled via
// QT_NO_EXCEPTIONS) and replaces it with assert for debug builds
#define QSTACKARRAY(arraytype, arrayname, arraysize) \
    Q_ASSERT(arraysize >= 1); \
    arraytype arrayname[arraysize]; \
    ::memset(arrayname, 0, arraysize * sizeof(arraytype));

static inline uint foldCase(const ushort *ch, const ushort *start)
{
    uint c = *ch;
    if (QChar::isLowSurrogate(c) && ch > start && QChar::isHighSurrogate(*(ch - 1)))
        c = QChar::surrogateToUcs4(*(ch - 1), c);
    return QChar::toCaseFolded(c);
}

static inline uint foldCase(const uint ch, uint &last)
{
    uint c = ch;
    if (QChar::isLowSurrogate(c) && QChar::isHighSurrogate(last))
        c = QChar::surrogateToUcs4(last, c);
    last = c;
    return QChar::toCaseFolded(c);
}

static inline QString timeZone()
{
    ::tzset();
    return QString::fromLocal8Bit(tzname[1]);
}

// Returns a human readable representation of the first \a len
// characters in \a data.
static inline QByteArray qt_prettyDebug(const char *data, int len, int maxSize)
{
    if (!data) return "(null)";
    QByteArray out;
    for (int i = 0; i < len; ++i) {
        char c = data[i];
        if (isprint(int(uchar(c)))) {
            out += c;
        } else switch (c) {
        case '\n': out += "\\n"; break;
        case '\r': out += "\\r"; break;
        case '\t': out += "\\t"; break;
        default:
            QSTACKARRAY(char, buf, 5);
            qsnprintf(buf, sizeof(buf), "\\%3o", c);
            buf[4] = '\0';
            out += QByteArray(buf);
        }
    }

    if (len < maxSize)
        out += "...";

    return out;
}

QT_END_NAMESPACE

#endif // QCORECOMMON_P_H
