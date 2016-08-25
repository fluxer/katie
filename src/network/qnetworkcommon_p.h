#ifndef QNETWORKCOMMON_H
#define QNETWORKCOMMON_H

#include "qnetworkaccesscache_p.h"
#include "qnetworkcookie.h"
#include "qnetworkrequest.h"
#include "qnetworksession.h"

Q_DECLARE_METATYPE(QNetworkAccessCache::CacheableObject*)
Q_DECLARE_METATYPE(QSharedPointer<char>)
#ifdef QT_ALLINONE
Q_DECLARE_METATYPE(QNetworkCookie)
Q_DECLARE_METATYPE(QList<QNetworkCookie>)
Q_DECLARE_METATYPE(QNetworkRequest)
Q_DECLARE_METATYPE(QNetworkSession::State)
Q_DECLARE_METATYPE(QNetworkSession::SessionError)
#endif

static void downloadBufferDeleter(char *ptr)
{
    delete[] ptr;
}

/*
   Returns the difference between msecs and elapsed. If msecs is -1,
   however, -1 is returned.
*/
static int qt_timeout_value(int msecs, int elapsed)
{
    if (msecs == -1)
        return -1;

    int timeout = msecs - elapsed;
    return timeout < 0 ? 0 : timeout;
}

#endif // QNETWORKCOMMON_H
