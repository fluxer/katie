#ifndef QCORECOMMON_P_H
#define QCORECOMMON_P_H

#include "qstring.h"

#ifndef QT_NO_COMPRESS
#  include <zlib.h>
#endif

QT_BEGIN_NAMESPACE

// enough space to hold BOM, each char as surrogate pair and terminator
#define QMAXUSTRLEN(X) 4 + (X * 2) + 2

// There are two incompatible versions of strerror_r:
// a) the XSI/POSIX.1 version, which returns an int,
//    indicating success or not
// b) the GNU version, which returns a char*, which may or may not
//    be the beginning of the buffer we used
// The GNU libc manpage for strerror_r says you should use the the XSI
// version in portable code.
static inline QString fromstrerror_helper(const int errorcode)
{
#if !defined(QT_NO_THREAD) && defined(QT_HAVE_STRERROR_R)
    char errbuf[1024];
    ::memset(errbuf, '\0', sizeof(errbuf));
    if (Q_LIKELY(::strerror_r(errorcode, errbuf, sizeof(errbuf)))) {
        return QString::fromLocal8Bit(errbuf, sizeof(errbuf));
    }
    return QString();
#else
    return QString::fromLocal8Bit(::strerror(errorcode));
#endif
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
    tzset();
    return QString::fromLocal8Bit(tzname[1]);
}

QT_END_NAMESPACE

#endif // QCORECOMMON_P_H
