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

#ifndef QFILE_P_H
#define QFILE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qiodevice_p.h"
#include "qfilesystementry_p.h"
#include "qfilesystemmetadata_p.h"

QT_BEGIN_NAMESPACE

class QFilePrivate : public QIODevicePrivate
{
    Q_DECLARE_PUBLIC(QFile)

protected:
    QFilePrivate();
    ~QFilePrivate();

    bool openExternalFile(QIODevice::OpenMode mode, int fd, QFile::FileHandleFlags handleFlags);
    bool doStat(QFileSystemMetaData::MetaDataFlags flags) const;
    bool unmap(uchar *ptr);

    QFile::FileError error;
    QString errorString;
    QFileSystemEntry fileEntry;
    mutable QFileSystemMetaData metaData;
    QHash<uchar *, QPair<int /*offset % PageSize*/, size_t /*length + offset % PageSize*/> > maps;
    int fd;
    bool closeFileHandle;

    void setError(QFile::FileError err, const QString &errorString);
};

QT_END_NAMESPACE

#endif // QFILE_P_H
