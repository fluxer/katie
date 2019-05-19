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

QT_BEGIN_NAMESPACE

static void downloadBufferDeleter(char *ptr)
{
    delete[] ptr;
}

QT_END_NAMESPACE

#endif // QNETWORKCOMMON_H
