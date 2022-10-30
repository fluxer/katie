/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#ifndef QABSTRACTFILEENGINE_H
#define QABSTRACTFILEENGINE_H

#include <QtCore/qfile.h>
#include <QtCore/qdatetime.h>

#ifdef open
#error qabstractfileengine.h must be included before any header file that defines open
#endif

QT_BEGIN_NAMESPACE

class QAbstractFileEnginePrivate;

class Q_CORE_EXPORT QAbstractFileEngine
{
public:
    enum FileName {
        DefaultName,
        BaseName,
        PathName,
        AbsoluteName,
        AbsolutePathName,
        LinkName,
        CanonicalName,
        CanonicalPathName,
        NFileNames
    };
    enum FileOwner {
        OwnerUser,
        OwnerGroup
    };
    enum FileTime {
        CreationTime,
        ModificationTime,
        AccessTime
    };

    ~QAbstractFileEngine();

    virtual bool open(QIODevice::OpenMode openMode); // virtual for QTemporaryFile
    virtual bool close(); // virtual for QTemporaryFile
    bool flush();
    qint64 size() const;
    qint64 pos() const;
    bool seek(qint64 pos);
    bool isSequential() const;
    virtual bool remove(); // virtual for QTemporaryFile
    bool copy(const QString &newName);
    virtual bool rename(const QString &newName); // virtual for QTemporaryFile
    bool link(const QString &newName);
    bool setSize(qint64 size);
    bool isRelativePath() const;
    bool exists() const;
    QFile::Permissions permissions() const;
    bool setPermissions(uint perms);
    QString fileName(FileName file) const;
    uint ownerId(FileOwner) const;
    QString owner(FileOwner) const;
    QDateTime fileTime(FileTime time) const;
    virtual void setFileName(const QString &file); // virtual for QTemporaryFile
    int handle() const;
    uchar *map(qint64 offset, qint64 size);
    bool unmap(uchar *ptr);

    qint64 read(char *data, qint64 maxlen);
    qint64 readLine(char *data, qint64 maxlen);
    qint64 write(const char *data, qint64 len);

    QFile::FileError error() const;
    QString errorString() const;

    enum Extension {
        FastReadLineExtension,
        MapExtension,
        UnMapExtension
    };
    class ExtensionOption
    {};
    class ExtensionReturn
    {};

    class MapExtensionOption : public ExtensionOption {
    public:
        qint64 offset;
        qint64 size;
    };
    class MapExtensionReturn : public ExtensionReturn {
    public:
        uchar *address;
    };

    class UnMapExtensionOption : public ExtensionOption {
    public:
        uchar *address;
    };

    bool extension(Extension extension, const ExtensionOption *option = nullptr, ExtensionReturn *output = nullptr);
    bool supportsExtension(Extension extension) const;

    // Factory
    static QAbstractFileEngine *create(const QString &fileName);

    //FS only!!
    bool open(QIODevice::OpenMode flags, int fd, QFile::FileHandleFlags handleFlags);

protected:
    void setError(QFile::FileError error, const QString &str);

    QAbstractFileEngine();
    QAbstractFileEngine(QAbstractFileEnginePrivate &);

    QAbstractFileEnginePrivate* d_ptr;
private:
    Q_DECLARE_PRIVATE(QAbstractFileEngine)
    Q_DISABLE_COPY(QAbstractFileEngine)

    friend class QFilePrivate;
};

QT_END_NAMESPACE

#endif // QABSTRACTFILEENGINE_H
