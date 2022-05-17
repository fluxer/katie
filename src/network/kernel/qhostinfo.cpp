/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtNetwork module of the Katie Toolkit.
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

#include "qhostinfo.h"
#include "qhostinfo_p.h"
#include "qcoreapplication.h"
#include "qmetaobject.h"
#include "qstringlist.h"

QT_BEGIN_NAMESPACE

// #define QHOSTINFO_DEBUG

Q_GLOBAL_STATIC(QHostInfoCache, globalHostInfoCache)

/*!
    \class QHostInfo
    \brief The QHostInfo class provides static methods for host name lookups.

    \reentrant
    \inmodule QtNetwork
    \ingroup network

    QHostInfo uses the lookup mechanisms provided by the operating
    system to find the IP address(es) associated with a host name,
    or the host name associated with an IP address. The class
    provides static method that blocks and returns a QHostInfo object.

    Call addresses() to get the list of IP addresses for the host, and
    hostName() to get the host name that was looked up.

    If the lookup failed, error() returns the type of error that
    occurred. errorString() gives a human-readable description of the
    lookup error.

    If you want a non-blocking lookup, use the QHostInfo::fromName()
    in a thread.

    QHostInfo supports Internationalized Domain Names (IDNs) through the
    IDNA and Punycode standards.

    To retrieve the name of the local host, use the static
    QHostInfo::localHostName() method.

    \note Since 4.6.3 QHostInfo is using a small internal 60 second DNS cache
    for performance improvements.
    \note Since 4.9.0 QHostInfo is not using multiple threads for DNS lookup.

    \sa QAbstractSocket, {http://www.rfc-editor.org/rfc/rfc3492.txt}{RFC 3492}
*/

/*!
    Looks up the IP address(es) for the given host \a name. The
    method blocks during the lookup which means that execution of
    the program is suspended until the results of the lookup are
    ready. Returns the result of the lookup in a QHostInfo object.

    If you pass a literal IP address to \a name instead of a host name,
    QHostInfo will search for the domain name for the IP (i.e., QHostInfo will
    perform a \e reverse lookup). On success, the returned QHostInfo will
    contain both the resolved domain name and IP addresses for the host name.

    \sa lookupHost()
*/
QHostInfo QHostInfo::fromName(const QString &name)
{
#ifdef QHOSTINFO_DEBUG
    qDebug("QHostInfo::fromName(\"%s\")",name.toLatin1().constData());
#endif

    QHostInfoCache* cache = globalHostInfoCache();
    if (cache && cache->isEnabled()) {
        bool valid = false;
        QHostInfo info = cache->get(name, &valid);
        if (valid) {
            return info;
        }
    }

    QHostInfo info = QHostInfoPrivate::fromName(name);
    if (cache && cache->isEnabled()) {
        cache->put(name, info);
    }
    return info;
}

/*!
    \enum QHostInfo::HostInfoError

    This enum describes the various errors that can occur when trying
    to resolve a host name.

    \value NoError The lookup was successful.
    \value HostNotFound No IP addresses were found for the host.
    \value UnknownError An unknown error occurred.

    \sa error(), errorString()
*/

/*!
    Constructs an empty host info object.

    \sa lookupId()
*/
QHostInfo::QHostInfo()
    : d(new QHostInfoPrivate())
{
}

/*!
    Constructs a copy of \a other.
*/
QHostInfo::QHostInfo(const QHostInfo &other)
    : d(new QHostInfoPrivate())
{
    d->err = other.d->err;
    d->errorStr = other.d->errorStr;
    d->addrs = other.d->addrs;
    d->hostName = other.d->hostName;
}

/*!
    Assigns the data of the \a other object to this host info object,
    and returns a reference to it.
*/
QHostInfo &QHostInfo::operator=(const QHostInfo &other)
{
    d->err = other.d->err;
    d->errorStr = other.d->errorStr;
    d->addrs = other.d->addrs;
    d->hostName = other.d->hostName;
    return *this;
}

/*!
    Destroys the host info object.
*/
QHostInfo::~QHostInfo()
{
    delete d;
}

/*!
    Returns the list of IP addresses associated with hostName(). This
    list may be empty.

    Example:

    \snippet doc/src/snippets/code/src_network_kernel_qhostinfo.cpp 5

    \sa hostName(), error()
*/
QList<QHostAddress> QHostInfo::addresses() const
{
    return d->addrs;
}

/*!
    Returns the name of the host whose IP addresses were looked up.

    \sa localHostName()
*/
QString QHostInfo::hostName() const
{
    return d->hostName;
}

/*!
    Returns the type of error that occurred if the host name lookup
    failed; otherwise returns NoError.

    \sa errorString()
*/
QHostInfo::HostInfoError QHostInfo::error() const
{
    return d->err;
}

/*!
    If the lookup failed, this method returns a human readable
    description of the error; otherwise "Unknown error" is returned.

    \sa error()
*/
QString QHostInfo::errorString() const
{
    return d->errorStr;
}

/*!
    \fn QString QHostInfo::localHostName()

    Returns the host name of this machine.

    \sa hostName()
*/

/*!
    \fn QString QHostInfo::localDomainName()

    Returns the DNS domain of this machine.

    Note: DNS domains are not related to domain names found in
    Windows networks.

    \sa hostName()
*/

void qt_qhostinfo_clear_cache()
{
    QHostInfoCache* cache = globalHostInfoCache();
    if (cache) {
        cache->clear();
    }
}

void Q_AUTOTEST_EXPORT qt_qhostinfo_enable_cache(bool e)
{
    QHostInfoCache* cache = globalHostInfoCache();
    if (cache) {
        cache->setEnabled(e);
    }
}

// cache for 60 seconds
// cache 128 items
QHostInfoCache::QHostInfoCache() : max_age(60), enabled(true), cache(128)
{
#ifdef QT_QHOSTINFO_CACHE_DISABLED_BY_DEFAULT
    enabled = false;
#endif
}

bool QHostInfoCache::isEnabled() const
{
    return enabled;
}

// this method is currently only used for the auto tests
// and not usable by public API
void QHostInfoCache::setEnabled(bool e)
{
    enabled = e;
}


QHostInfo QHostInfoCache::get(const QString &name, bool *valid) const
{
    *valid = false;
    QHostInfoCacheElement *element = cache.object(name);
    if (element && element->age.elapsed() < max_age*1000) {
        *valid = true;
        return element->info;
    }

    return QHostInfo();
}

void QHostInfoCache::put(const QString &name, const QHostInfo &info)
{
    // if the lookup failed, don't cache
    if (info.error() != QHostInfo::NoError)
        return;

    QHostInfoCacheElement* element = new QHostInfoCacheElement();
    element->info = info;
    element->age.restart();

    QMutexLocker locker(&this->mutex);
    cache.insert(name, element); // cache will take ownership
}

void QHostInfoCache::clear()
{
    QMutexLocker locker(&this->mutex);
    cache.clear();
}

QT_END_NAMESPACE
