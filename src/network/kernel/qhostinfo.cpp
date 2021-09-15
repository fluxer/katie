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
    \brief The QHostInfo class provides static functions for host name lookups.

    \reentrant
    \inmodule QtNetwork
    \ingroup network

    QHostInfo uses the lookup mechanisms provided by the operating
    system to find the IP address(es) associated with a host name,
    or the host name associated with an IP address.
    The class provides two static convenience functions: one that
    works asynchronously and emits a signal once the host is found,
    and one that blocks and returns a QHostInfo object.

    To look up a host's IP addresses asynchronously, call lookupHost(),
    which takes the host name or IP address, a receiver object, and a slot
    signature as arguments and returns an ID. You can abort the
    lookup by calling abortHostLookup() with the lookup ID.

    Example:

    \snippet doc/src/snippets/code/src_network_kernel_qhostinfo.cpp 0


    The slot is invoked when the results are ready. The results are
    stored in a QHostInfo object. Call
    addresses() to get the list of IP addresses for the host, and
    hostName() to get the host name that was looked up.

    If the lookup failed, error() returns the type of error that
    occurred. errorString() gives a human-readable description of the
    lookup error.

    If you want a blocking lookup, use the QHostInfo::fromName() function:

    \snippet doc/src/snippets/code/src_network_kernel_qhostinfo.cpp 1

    QHostInfo supports Internationalized Domain Names (IDNs) through the
    IDNA and Punycode standards.

    To retrieve the name of the local host, use the static
    QHostInfo::localHostName() function.

    \note Since 4.6.3 QHostInfo is using a small internal 60 second DNS cache
    for performance improvements.
    \note Since 4.9.0 QHostInfo is not using multiple threads for DNS lookup.

    \sa QAbstractSocket, {http://www.rfc-editor.org/rfc/rfc3492.txt}{RFC 3492}
*/

static QAtomicInt theIdCounter(1);

/*!
    Looks up the IP address(es) associated with host name \a name, and
    returns an ID for the lookup. When the result of the lookup is
    ready, the slot or signal \a member in \a receiver is called with
    a QHostInfo argument. The QHostInfo object can then be inspected
    to get the results of the lookup.

    The lookup is performed by a single function call, for example:

    \snippet doc/src/snippets/code/src_network_kernel_qhostinfo.cpp 2

    The implementation of the slot prints basic information about the
    addresses returned by the lookup, or reports an error if it failed:

    \snippet doc/src/snippets/code/src_network_kernel_qhostinfo.cpp 3

    If you pass a literal IP address to \a name instead of a host name,
    QHostInfo will search for the domain name for the IP (i.e., QHostInfo will
    perform a \e reverse lookup). On success, the resulting QHostInfo will
    contain both the resolved domain name and IP addresses for the host
    name. Example:

    \snippet doc/src/snippets/code/src_network_kernel_qhostinfo.cpp 4

    \note There is no guarantee on the order the signals will be emitted
    if you start multiple requests with lookupHost().

    \sa abortHostLookup(), addresses(), error(), fromName()
*/
int QHostInfo::lookupHost(const QString &name, QObject *receiver,
                          const char *member)
{
#ifdef QHOSTINFO_DEBUG
    qDebug("QHostInfo::lookupHost(\"%s\", %p, %s)",
           name.toLatin1().constData(), receiver, member ? member + 1 : 0);
#endif

    qRegisterMetaType<QHostInfo>("QHostInfo");

    if (!receiver)
        return -1;

    int id = theIdCounter.fetchAndAddRelaxed(1); // generate unique ID

    if (name.isEmpty()) {
        QHostInfo info(id);
        info.setError(QHostInfo::HostNotFound);
        info.setErrorString(QCoreApplication::translate("QHostInfo", "No host name given"));
        QHostInfoResult result;
        QObject::connect(&result, SIGNAL(resultsReady(QHostInfo)), receiver, member);
        result.emitResultsReady(info);
        return id;
    }

    QHostInfo info = QHostInfo::fromName(name);
    info.setLookupId(id);
    QHostInfoResult result;
    QObject::connect(&result, SIGNAL(resultsReady(QHostInfo)), receiver, member);
    result.emitResultsReady(info);

    return id;
}

/*!
    Aborts the host lookup with the ID \a id, as returned by lookupHost().

    \sa lookupHost(), lookupId()
*/
void QHostInfo::abortHostLookup(int id)
{
    // nothing to do
    Q_UNUSED(id);
}

/*!
    Looks up the IP address(es) for the given host \a name. The
    function blocks during the lookup which means that execution of
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

    QHostInfo info = QHostInfoAgent::fromName(name);
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

    \sa error(), setError()
*/

/*!
    Constructs an empty host info object with lookup ID \a id.

    \sa lookupId()
*/
QHostInfo::QHostInfo(int id)
    : d(new QHostInfoPrivate)
{
    d->lookupId = id;
}

/*!
    Constructs a copy of \a other.
*/
QHostInfo::QHostInfo(const QHostInfo &other)
    : d(new QHostInfoPrivate(*other.d.data()))
{
}

/*!
    Assigns the data of the \a other object to this host info object,
    and returns a reference to it.
*/
QHostInfo &QHostInfo::operator=(const QHostInfo &other)
{
    *d.data() = *other.d.data();
    return *this;
}

/*!
    Destroys the host info object.
*/
QHostInfo::~QHostInfo()
{
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
    Sets the list of addresses in this QHostInfo to \a addresses.

    \sa addresses()
*/
void QHostInfo::setAddresses(const QList<QHostAddress> &addresses)
{
    d->addrs = addresses;
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
    Sets the host name of this QHostInfo to \a hostName.

    \sa hostName()
*/
void QHostInfo::setHostName(const QString &hostName)
{
    d->hostName = hostName;
}

/*!
    Returns the type of error that occurred if the host name lookup
    failed; otherwise returns NoError.

    \sa setError(), errorString()
*/
QHostInfo::HostInfoError QHostInfo::error() const
{
    return d->err;
}

/*!
    Sets the error type of this QHostInfo to \a error.

    \sa error(), errorString()
*/
void QHostInfo::setError(HostInfoError error)
{
    d->err = error;
}

/*!
    Returns the ID of this lookup.

    \sa setLookupId(), abortHostLookup(), hostName()
*/
int QHostInfo::lookupId() const
{
    return d->lookupId;
}

/*!
    Sets the ID of this lookup to \a id.

    \sa lookupId(), lookupHost()
*/
void QHostInfo::setLookupId(int id)
{
    d->lookupId = id;
}

/*!
    If the lookup failed, this function returns a human readable
    description of the error; otherwise "Unknown error" is returned.

    \sa setErrorString(), error()
*/
QString QHostInfo::errorString() const
{
    return d->errorStr;
}

/*!
    Sets the human readable description of the error that occurred to \a str
    if the lookup failed.

    \sa errorString(), setError()
*/
void QHostInfo::setErrorString(const QString &str)
{
    d->errorStr = str;
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

// this function is currently only used for the auto tests
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


#include "moc_qhostinfo_p.h"
