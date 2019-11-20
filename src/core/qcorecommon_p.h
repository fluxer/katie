#ifndef QCORECOMMON_P_H
#define QCORECOMMON_P_H

#include "qstring.h"

#ifndef QT_NO_COMPRESS
#  include <zlib.h>
#endif

QT_BEGIN_NAMESPACE

#if !defined(QT_NO_THREAD) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && _POSIX_VERSION >= 200112L
// There are two incompatible versions of strerror_r:
// a) the XSI/POSIX.1 version, which returns an int,
//    indicating success or not
// b) the GNU version, which returns a char*, which may or may not
//    be the beginning of the buffer we used
// The GNU libc manpage for strerror_r says you should use the the XSI
// version in portable code. However, it's impossible to do that if
// _GNU_SOURCE is defined so we use C++ overloading to decide what to do
// depending on the return type
static inline QString fromstrerror_helper(int, const QByteArray &buf)
{
    return QString::fromLocal8Bit(buf.constData());
}
static inline QString fromstrerror_helper(const char *str, const QByteArray &)
{
    return QString::fromLocal8Bit(str);
}
#endif

static inline void report_error(int code, const char *where, const char *what)
{
    if (code != 0)
        qWarning("%s: %s failure: %s", where, what, qPrintable(qt_error_string(code)));
}


#ifndef QT_NO_COMPRESS
static inline quint32 qCRC32(const char *data, uint len)
{
    quint32 crc_32 = ::crc32(0, 0, 0);
    return ::crc32(crc_32, reinterpret_cast<const uchar *>(data), len);
}
#endif


static inline uint foldCase(const ushort *ch, const ushort *start)
{
    uint c = *ch;
    if (QChar::isLowSurrogate(c) && ch > start && QChar::isHighSurrogate(*(ch - 1)))
        c = QChar::surrogateToUcs4(*(ch - 1), c);
    return QChar::toCaseFolded(*ch);
}

static inline uint foldCase(const uint ch, uint &last)
{
    uint c = ch;
    if (QChar::isLowSurrogate(c) && QChar::isHighSurrogate(last))
        c = QChar::surrogateToUcs4(last, c);
    last = ch;
    return QChar::toCaseFolded(ch);
}

static inline ushort foldCase(const ushort ch)
{
    return QChar::toCaseFolded(ch);
}

static inline QString timeZone()
{
    tzset();
    return QString::fromLocal8Bit(tzname[1]);
}

QT_END_NAMESPACE

#endif // QCORECOMMON_P_H
