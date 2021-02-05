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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qfsfileengine_iterator_p.h"
#include "qfileinfo_p.h"
#include "qvariant.h"

#ifndef QT_NO_FSFILEENGINE
#ifndef QT_NO_FILESYSTEMITERATOR

QT_BEGIN_NAMESPACE

QFSFileEngineIterator::QFSFileEngineIterator(QDir::Filters filters, const QStringList &filterNames)
    : QAbstractFileEngineIterator(filters, filterNames)
    , nativeIterator(Q_NULLPTR)
    , done(false)
{
}

QFSFileEngineIterator::~QFSFileEngineIterator()
{
    delete nativeIterator;
}

bool QFSFileEngineIterator::hasNext() const
{
    if (!done && !nativeIterator) {
        nativeIterator = new QFileSystemIterator(QFileSystemEntry(path()));
        advance();
    }

    return !done;
}

QString QFSFileEngineIterator::next()
{
    if (!hasNext())
        return QString();

    advance();
    return currentFilePath();
}

void QFSFileEngineIterator::advance() const
{
    currentInfo = nextInfo;

    QFileSystemEntry entry;
    QFileSystemMetaData data;
    if (nativeIterator->advance(entry, data)) {
        nextInfo = QFileInfo(new QFileInfoPrivate(entry, data));
    } else {
        done = true;
        delete nativeIterator;
        nativeIterator = Q_NULLPTR;
    }
}

QString QFSFileEngineIterator::currentFileName() const
{
    return currentInfo.fileName();
}

QFileInfo QFSFileEngineIterator::currentFileInfo() const
{
    return currentInfo;
}

QT_END_NAMESPACE

#endif // QT_NO_FILESYSTEMITERATOR

#endif // QT_NO_FSFILEENGINE
