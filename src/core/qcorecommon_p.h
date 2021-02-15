#ifndef QCORECOMMON_P_H
#define QCORECOMMON_P_H

#include "qstring.h"

#include <time.h>

QT_BEGIN_NAMESPACE

// enough space to hold BOM, each char as surrogate pair and terminator
#define QMAXUSTRLEN(X) 4 + (X * 2) + 2

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

QT_END_NAMESPACE

#endif // QCORECOMMON_P_H
