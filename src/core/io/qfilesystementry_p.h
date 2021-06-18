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

#ifndef QFILESYSTEMENTRY_P_H_INCLUDED
#define QFILESYSTEMENTRY_P_H_INCLUDED

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

#include "qstring.h"

QT_BEGIN_NAMESPACE

class QFileSystemEntry
{
public:
    typedef QByteArray NativePath;
    struct FromNativePath{};

    QFileSystemEntry();
    explicit QFileSystemEntry(const QString &filePath);
    explicit QFileSystemEntry(const NativePath &nativeFilePath, FromNativePath dummy);
    explicit QFileSystemEntry(const QString &filePath, const NativePath &nativeFilePath);

    QString filePath() const;
    QString fileName() const;
    QString path() const;
    NativePath nativeFilePath() const;
    QString baseName() const;
    QString completeBaseName() const;
    QString suffix() const;
    QString completeSuffix() const;
    bool isAbsolute() const;
    bool isRelative() const;
    bool isClean() const;

    bool isRoot() const;

    bool isEmpty() const;
    void clear();

private:
    // creates the QString version out of the bytearray version
    void resolveFilePath();
    // creates the bytearray version out of the QString version
    void resolveNativeFilePath();
    // resolves the separator
    void findLastSeparator();
    // resolves the dots and the separator
    void findFileNameSeparators();

    QString m_filePath; // always has slashes as separator
    NativePath m_nativeFilePath; // native encoding and separators

    qint16 m_lastSeparator; // index in m_filePath of last separator
    qint16 m_firstDotInFileName; // index after m_filePath for first dot (.)
    qint16 m_lastDotInFileName; // index after m_firstDotInFileName for last dot (.)
};

QT_END_NAMESPACE

#endif // include guard
