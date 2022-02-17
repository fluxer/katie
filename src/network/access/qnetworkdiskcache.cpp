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

// #define QNETWORKDISKCACHE_DEBUG

#include "qnetworkdiskcache.h"
#include "qnetworkdiskcache_p.h"
#include "qscopedpointer.h"
#include "qfile.h"
#include "qdir.h"
#include "qdatetime.h"
#include "qdiriterator.h"
#include "qurl.h"
#include "qcryptographichash.h"
#include "qbuffer.h"
#include "qdebug.h"
#include "qcorecommon_p.h"
#include "qcore_unix_p.h"

#ifndef QT_NO_NETWORKDISKCACHE

QT_BEGIN_NAMESPACE

#define CACHE_POSTFIX QLatin1String(".cache")

/*!
    Given a URL, generates a unique enough filename with suffix
 */
static QString uniqueFileName(const QUrl &url)
{
    QUrl cleanUrl = url;
    cleanUrl.setPassword(QString());
    cleanUrl.setFragment(QString());

    const QByteArray hash = QCryptographicHash::hash(cleanUrl.toEncoded(), QCryptographicHash::Sha1);
    return hash.toHex() + CACHE_POSTFIX;
}

enum {
    CacheMagic = 0xe8,
    CurrentCacheVersion = 9
};

void QCacheItem::write(QFile *device) const
{
    QDataStream out(device);

    out << qint32(CacheMagic);
    out << qint32(CurrentCacheVersion);
    out << metaData;
}

/*!
    Returns false if the file is invalid cache file, true otherwise
 */
bool QCacheItem::read(QFile *device)
{
    QDataStream in(device);

    qint32 marker;
    qint32 v;
    in >> marker;
    in >> v;
    if (marker != CacheMagic || v != CurrentCacheVersion) {
#if defined(QNETWORKDISKCACHE_DEBUG)
        qDebug() << "QCacheItem::read: invalid marker or cache version" << marker << v;
#endif
        return false;
    }

    in >> metaData;

    // quick and dirty check if metadata's URL field and the file's name are in sync
    QString expectedFilename = uniqueFileName(metaData.url());
    if (!device->fileName().endsWith(expectedFilename)) {
#if defined(QNETWORKDISKCACHE_DEBUG)
        qDebug() << "QCacheItem::read: unexpected filename" << device->fileName();
#endif
        return false;
    }

    return metaData.isValid();
}

/*!
    Generates fully qualified path of cached resource from a URL
 */
QString QNetworkDiskCachePrivate::cacheFileName(const QUrl &url) const
{
    if (!url.isValid() || cacheDirectory.isEmpty()) {
#if defined(QNETWORKDISKCACHE_DEBUG)
        qDebug() << "QNetworkDiskCachePrivate::cacheFileName: invalid URL or cache directory is empty" << url << cacheDirectory;
#endif
        return QString();
    }
    return cacheDirectory + uniqueFileName(url);
}

qint64 QNetworkDiskCachePrivate::cacheSize() const
{
    QDir::Filters filters = QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot;
    QDirIterator it(cacheDirectory, filters, QDirIterator::Subdirectories);

    qint64 total = 0;
    while (it.hasNext()) {
        it.next();
        const QFileInfo info = it.fileInfo();
        const QString fileName = info.fileName();
        if (fileName.endsWith(CACHE_POSTFIX)) {
            total += info.size();
        }
    }
    return total;
}

/*!
    \class QNetworkDiskCache
    \since 4.5
    \inmodule QtNetwork

    \brief The QNetworkDiskCache class provides a very basic disk cache.

    QNetworkDiskCache stores each url in its own file inside of the
    cacheDirectory using QDataStream. Each cache file ends with
    ".cache". Data is written to disk only in insert() and
    updateMetaData().

    Currently you can not share the same cache files with more then
    one disk cache.

    QNetworkDiskCache by default limits the amount of space that the cache will
    use on the system to 50MB.

    Note you have to set the cache directory before it will work.

    A network disk cache can be enabled by:

    \snippet doc/src/snippets/code/src_network_access_qnetworkdiskcache.cpp 0

    When sending requests, to control the preference of when to use the cache
    and when to use the network, consider the following:

    \snippet doc/src/snippets/code/src_network_access_qnetworkdiskcache.cpp 1

    To check whether the response came from the cache or from the network, the
    following can be applied:

    \snippet doc/src/snippets/code/src_network_access_qnetworkdiskcache.cpp 2

    \warning No attempt is made to lock the cache files, if two applications
    or even threads attempt to cache the same URL with the same cache
    directory at the same time the result is undefined.
*/

/*!
    Creates a new disk cache. The \a parent argument is passed to
    QAbstractNetworkCache's constructor.
 */
QNetworkDiskCache::QNetworkDiskCache(QObject *parent)
    : QAbstractNetworkCache(*new QNetworkDiskCachePrivate, parent)
{
    Q_D(QNetworkDiskCache);

    d->currentCacheSize = cacheSize();
#if defined(QNETWORKDISKCACHE_DEBUG)
    qDebug() << "QNetworkDiskCache: cache size is" << d->currentCacheSize;
#endif
}

/*!
    Destroys the cache object.  This does not clear the disk cache.
 */
QNetworkDiskCache::~QNetworkDiskCache()
{
}

/*!
    Returns the location where cached files will be stored.
*/
QString QNetworkDiskCache::cacheDirectory() const
{
    Q_D(const QNetworkDiskCache);
    return d->cacheDirectory;
}

/*!
    Sets the directory where cached files will be stored to \a cacheDir

    QNetworkDiskCache will create this directory if it does not exists.

    Prepared cache items will be stored in the new cache directory when
    they are inserted.

    \sa QStandardPaths::CacheLocation
*/
void QNetworkDiskCache::setCacheDirectory(const QString &cacheDir)
{
#if defined(QNETWORKDISKCACHE_DEBUG)
    qDebug() << "QNetworkDiskCache::setCacheDirectory()" << cacheDir;
#endif
    Q_D(QNetworkDiskCache);
    if (cacheDir.isEmpty()) {
        return;
    }
    d->cacheDirectory = cacheDir;
    QDir dir(d->cacheDirectory);
    d->cacheDirectory = dir.absolutePath();
    if (!d->cacheDirectory.endsWith(QLatin1Char('/'))) {
        d->cacheDirectory += QLatin1Char('/');
    }
    QDir().mkpath(d->cacheDirectory);

    d->currentCacheSize = cacheSize();
}

/*!
    \reimp
*/
qint64 QNetworkDiskCache::cacheSize() const
{
#if defined(QNETWORKDISKCACHE_DEBUG)
    qDebug() << "QNetworkDiskCache::cacheSize()";
#endif
    Q_D(const QNetworkDiskCache);
    return d->currentCacheSize;
}

/*!
    \reimp
*/
QIODevice *QNetworkDiskCache::prepare(const QNetworkCacheMetaData &metaData)
{
#if defined(QNETWORKDISKCACHE_DEBUG)
    qDebug() << "QNetworkDiskCache::prepare()" << metaData.url();
#endif
    Q_D(QNetworkDiskCache);
    if (!metaData.isValid() || !metaData.url().isValid() || !metaData.saveToDisk()) {
        return nullptr;
    }
    if (Q_UNLIKELY(d->cacheDirectory.isEmpty())) {
        qWarning("QNetworkDiskCache::prepare: the cache directory is not set");
        return nullptr;
    }

    foreach (const QNetworkCacheMetaData::RawHeader &header, metaData.rawHeaders()) {
        if (header.first.toLower() == "content-length") {
            const qint64 size = header.second.toInt();
            if (size > (maximumCacheSize() * 3)/4)
                return nullptr;
            break;
        }
    }

    QFile *file = new QFile(d->cacheFileName(metaData.url()), this);
    if (Q_UNLIKELY(!file->open(QFile::ReadWrite))) {
        qWarning("QNetworkDiskCache::prepare: unable to open cache file");
        delete file;
        return nullptr;
    }
    QCacheItem cacheItem;
    cacheItem.metaData = metaData;
    cacheItem.write(file);
    return file;
}

/*!
    \reimp
*/
void QNetworkDiskCache::insert(QIODevice *device)
{
#if defined(QNETWORKDISKCACHE_DEBUG)
    qDebug() << "QNetworkDiskCache::insert()" << device;
#endif
    Q_D(QNetworkDiskCache);

    QFile *file = qobject_cast<QFile*>(device);
    if (Q_UNLIKELY(!file || file->fileName().isEmpty())) {
        qWarning("QNetworkDiskCache::insert: device is not file or filename is empty");
        delete file;
        return;
    }

    d->currentCacheSize = expire();
    if (file->error() == QFile::NoError) {
        d->currentCacheSize += file->size();
    } else {
        file->remove();
    }

#if defined(QNETWORKDISKCACHE_DEBUG)
    qDebug() << "QNetworkDiskCache::insert()" << file->fileName() << file->size() << file->error();
#endif
    delete file;
}

/*!
    \reimp
*/
bool QNetworkDiskCache::remove(const QUrl &url)
{
#if defined(QNETWORKDISKCACHE_DEBUG)
    qDebug() << "QNetworkDiskCache::remove()" << url;
#endif
    Q_D(QNetworkDiskCache);

    const QString fileName = d->cacheFileName(url);
    if (fileName.isEmpty()) {
        return false;
    }
    if (QFile::remove(fileName)) {
        const QStatInfo info(fileName);
        d->currentCacheSize -= info.size();
        return true;
    }
    return false;
}

/*!
    \reimp
*/
QNetworkCacheMetaData QNetworkDiskCache::metaData(const QUrl &url)
{
#if defined(QNETWORKDISKCACHE_DEBUG)
    qDebug() << "QNetworkDiskCache::metaData()" << url;
#endif
    Q_D(QNetworkDiskCache);
    return fileMetaData(d->cacheFileName(url));
}

/*!
    Returns the QNetworkCacheMetaData for the cache file \a fileName.

    If \a fileName is not a cache file QNetworkCacheMetaData will be invalid.
 */
QNetworkCacheMetaData QNetworkDiskCache::fileMetaData(const QString &fileName) const
{
#if defined(QNETWORKDISKCACHE_DEBUG)
    qDebug() << "QNetworkDiskCache::fileMetaData()" << fileName;
#endif

    if (fileName.isEmpty()) {
        return QNetworkCacheMetaData();
    }
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
#if defined(QNETWORKDISKCACHE_DEBUG)
        qDebug() << "QNetworkDiskCache::fileMetaData: could not open" << fileName;
#endif
        return QNetworkCacheMetaData();
    }
    QCacheItem cacheitem;
    if (!cacheitem.read(&file)) {
        // ### this will not update cache size
        QFile::remove(fileName);
        return QNetworkCacheMetaData();
    }
    return cacheitem.metaData;
}

/*!
    \reimp
*/
QIODevice *QNetworkDiskCache::data(const QUrl &url)
{
#if defined(QNETWORKDISKCACHE_DEBUG)
    qDebug() << "QNetworkDiskCache::data()" << url;
#endif
    Q_D(const QNetworkDiskCache);

    const QString fileName = d->cacheFileName(url);
    if (fileName.isEmpty()) {
        return nullptr;
    }
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
#if defined(QNETWORKDISKCACHE_DEBUG)
        qDebug() << "QNetworkDiskCache::data: could not open" << fileName;
#endif
        return nullptr;
    }
    QCacheItem cacheitem;
    if (!cacheitem.read(&file)) {
#if defined(QNETWORKDISKCACHE_DEBUG)
        qDebug() << "QNetworkDiskCache::data: could not read cache" << fileName;
#endif
        remove(url);
        return nullptr;
    }

    QScopedPointer<QBuffer> buffer(new QBuffer());
    buffer->open(QBuffer::ReadWrite);
    buffer->write(file.readAll());
    buffer->seek(0);
    return buffer.take();
}

/*!
    \reimp
*/
void QNetworkDiskCache::updateMetaData(const QNetworkCacheMetaData &metaData)
{
#if defined(QNETWORKDISKCACHE_DEBUG)
    qDebug() << "QNetworkDiskCache::updateMetaData()" << metaData.url();
#endif
    QUrl url = metaData.url();
    QIODevice *oldDevice = data(url);
    if (!oldDevice) {
#if defined(QNETWORKDISKCACHE_DEBUG)
        qDebug() << "QNetworkDiskCache::updateMetaData(), no device!";
#endif
        return;
    }

    QIODevice *newDevice = prepare(metaData);
    if (!newDevice) {
#if defined(QNETWORKDISKCACHE_DEBUG)
        qDebug() << "QNetworkDiskCache::updateMetaData(), no new device!" << url;
#endif
        return;
    }
    QSTACKARRAY(char, data, QT_BUFFSIZE);
    while (!oldDevice->atEnd()) {
        qint64 s = oldDevice->read(data, sizeof(data));
        newDevice->write(data, s);
    }
    delete oldDevice;
    insert(newDevice);
}

/*!
    Returns the current maximum size in bytes for the disk cache.

    \sa setMaximumCacheSize()
 */
qint64 QNetworkDiskCache::maximumCacheSize() const
{
    Q_D(const QNetworkDiskCache);
    return d->maximumCacheSize;
}

/*!
    Sets the maximum size of the disk cache to be \a size in bytes.

    If the new size is smaller then the current cache size then the cache will call expire().

    \sa maximumCacheSize()
 */
void QNetworkDiskCache::setMaximumCacheSize(qint64 size)
{
    Q_D(QNetworkDiskCache);
    d->maximumCacheSize = size;
    d->currentCacheSize = expire();
}

/*!
    Cleans the cache so that its size is under the maximum cache size.
    Returns the current size of the cache.

    When the current size of the cache is greater than the maximumCacheSize()
    older cache files are removed until the total size is less then 90% of
    maximumCacheSize() starting with the oldest ones first using the file
    creation date to determine how old a cache file is.

    Subclasses can reimplement this function to change the order that cache
    files are removed taking into account information in the application
    knows about that QNetworkDiskCache does not, for example the number of times
    a cache is accessed.

    Note: cacheSize() calls expire if the current cache size is unknown.

    \sa maximumCacheSize(), fileMetaData()
 */
qint64 QNetworkDiskCache::expire()
{
    Q_D(QNetworkDiskCache);
    if (d->currentCacheSize < maximumCacheSize()) {
        return d->currentCacheSize;
    }

    if (Q_UNLIKELY(cacheDirectory().isEmpty())) {
        qWarning("QNetworkDiskCache::expire: the cache directory is not set");
        return 0;
    }

    QDir::Filters filters = QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot;
    QDirIterator it(cacheDirectory(), filters, QDirIterator::Subdirectories);

    QMultiMap<QDateTime, QString> cacheItems;
    qint64 totalSize = 0;
    while (it.hasNext()) {
        QString path = it.next();
        QFileInfo info = it.fileInfo();
        QString fileName = info.fileName();
        if (fileName.endsWith(CACHE_POSTFIX)) {
            cacheItems.insert(info.created(), path);
            totalSize += info.size();
        }
    }

    int removedFiles = 0;
    qint64 goal = (maximumCacheSize() * 9) / 10;
    QMultiMap<QDateTime, QString>::const_iterator i = cacheItems.constBegin();
    while (i != cacheItems.constEnd()) {
        if (totalSize < goal)
            break;
        QString name = i.value();
        QFile file(name);
        qint64 size = file.size();
        file.remove();
        totalSize -= size;
        ++removedFiles;
        ++i;
    }
#if defined(QNETWORKDISKCACHE_DEBUG)
    if (removedFiles > 0) {
        qDebug() << "QNetworkDiskCache::expire()"
                << "Removed:" << removedFiles
                << "Kept:" << cacheItems.count() - removedFiles;
    }
#endif
    return totalSize;
}

/*!
    \reimp
*/
void QNetworkDiskCache::clear()
{
#if defined(QNETWORKDISKCACHE_DEBUG)
    qDebug() << "QNetworkDiskCache::clear()";
#endif
    Q_D(QNetworkDiskCache);
    qint64 size = d->maximumCacheSize;
    d->maximumCacheSize = 0;
    d->currentCacheSize = expire();
    d->maximumCacheSize = size;
}

QT_END_NAMESPACE

#endif // QT_NO_NETWORKDISKCACHE

#include "moc_qnetworkdiskcache.h"
