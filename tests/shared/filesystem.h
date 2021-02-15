/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the test suite of the Katie Toolkit.
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
// Helper functions for creating file-system hierarchies and cleaning up.

#ifndef QT_TESTS_SHARED_FILESYSTEM_H_INCLUDED
#define QT_TESTS_SHARED_FILESYSTEM_H_INCLUDED

#include <QString>
#include <QStringList>
#include <QDir>
#include <QFile>

QT_USE_NAMESPACE

struct FileSystem
{
    ~FileSystem()
    {
        Q_FOREACH(QString fileName, createdFiles)
            QFile::remove(fileName);

        Q_FOREACH(QString dirName, createdDirectories)
            currentDir.rmdir(dirName);
    }

    bool createDirectory(const QString &dirName)
    {
        if (currentDir.mkdir(dirName)) {
            createdDirectories.prepend(dirName);
            return true;
        }
        return false;
    }

    bool createFile(const QString &fileName)
    {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            createdFiles << fileName;
            return true;
        }
        return false;
    }

    qint64 createFileWithContent(const QString &fileName)
    {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            createdFiles << fileName;
            return file.write(fileName.toUtf8());
        }
        return -1;
    }

    bool createLink(const QString &destination, const QString &linkName)
    {
        if (QFile::link(destination, linkName)) {
            createdFiles << linkName;
            return true;
        }
        return false;
    }

private:
    QDir currentDir;

    QStringList createdDirectories;
    QStringList createdFiles;
};

#endif // include guard
