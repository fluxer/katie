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

#include "qfilesystementry_p.h"
#include "qdir.h"
#include "qfile.h"
#include "qfsfileengine_p.h"

QT_BEGIN_NAMESPACE

QFileSystemEntry::QFileSystemEntry()
    : m_lastSeparator(0),
    m_firstDotInFileName(0),
    m_lastDotInFileName(0)
{
}

/*!
   \internal
   Use this constructor when the path is supplied by user code, as it may contain a mix
   of '/' and the native separator.
 */
QFileSystemEntry::QFileSystemEntry(const QString &filePath)
    : m_filePath(filePath),
    m_lastSeparator(-2),
    m_firstDotInFileName(-2),
    m_lastDotInFileName(0)
{
    resolveFilePath();
    resolveNativeFilePath();
    findLastSeparator();
    findFileNameSeparators();
}

/*!
   \internal
   Use this constructor when the path comes from a native API
 */
QFileSystemEntry::QFileSystemEntry(const NativePath &nativeFilePath, FromNativePath /* dummy */)
    : m_nativeFilePath(nativeFilePath),
    m_lastSeparator(-2),
    m_firstDotInFileName(-2),
    m_lastDotInFileName(0)
{
    resolveFilePath();
    resolveNativeFilePath();
    findLastSeparator();
    findFileNameSeparators();
}

QFileSystemEntry::QFileSystemEntry(const QString &filePath, const NativePath &nativeFilePath)
    : m_filePath(filePath),
    m_nativeFilePath(nativeFilePath),
    m_lastSeparator(-2),
    m_firstDotInFileName(-2),
    m_lastDotInFileName(0)
{
    resolveFilePath();
    resolveNativeFilePath();
    findLastSeparator();
    findFileNameSeparators();
}

QString QFileSystemEntry::filePath() const
{
    return m_filePath;
}

QFileSystemEntry::NativePath QFileSystemEntry::nativeFilePath() const
{
    return m_nativeFilePath;
}

void QFileSystemEntry::resolveFilePath()
{
    if (m_filePath.isEmpty() && !m_nativeFilePath.isEmpty()) {
        m_filePath = QFile::decodeName(m_nativeFilePath);
    }
}

void QFileSystemEntry::resolveNativeFilePath()
{
    if (!m_filePath.isEmpty() && m_nativeFilePath.isEmpty()) {
        m_nativeFilePath = QFile::encodeName(m_filePath);
    }
}

QString QFileSystemEntry::fileName() const
{
    return m_filePath.mid(m_lastSeparator + 1);
}

QString QFileSystemEntry::path() const
{
    if (m_lastSeparator == -1) {
        return QString(QLatin1Char('.'));
    }
    if (m_lastSeparator == 0)
        return QString(QLatin1Char('/'));
    return m_filePath.left(m_lastSeparator);
}

QString QFileSystemEntry::baseName() const
{
    int length = -1;
    if (m_firstDotInFileName >= 0) {
        length = m_firstDotInFileName;
        if (m_lastSeparator != -1) // avoid off by one
            length--;
    }
    return m_filePath.mid(m_lastSeparator + 1, length);
}

QString QFileSystemEntry::completeBaseName() const
{
    int length = -1;
    if (m_firstDotInFileName >= 0) {
        length = m_firstDotInFileName + m_lastDotInFileName;
        if (m_lastSeparator != -1) // avoid off by one
            length--;
    }
    return m_filePath.mid(m_lastSeparator + 1, length);
}

QString QFileSystemEntry::suffix() const
{
    if (m_lastDotInFileName == -1)
        return QString();

    return m_filePath.mid(qMax((qint16)0, m_lastSeparator) + m_firstDotInFileName + m_lastDotInFileName + 1);
}

QString QFileSystemEntry::completeSuffix() const
{
    if (m_firstDotInFileName == -1)
        return QString();

    return m_filePath.mid(qMax((qint16)0, m_lastSeparator) + m_firstDotInFileName + 1);
}

bool QFileSystemEntry::isRelative() const
{
    return !isAbsolute();
}

bool QFileSystemEntry::isAbsolute() const
{
    return (!m_filePath.isEmpty() && (m_filePath.at(0).unicode() == '/'));
}

bool QFileSystemEntry::isRoot() const
{
    return m_filePath == QLatin1String("/");
}

bool QFileSystemEntry::isEmpty() const
{
    return m_nativeFilePath.isEmpty();
}

void QFileSystemEntry::clear()
{
    m_filePath.clear();
    m_nativeFilePath.clear();
    m_lastSeparator = 0;
    m_firstDotInFileName = 0;
    m_lastDotInFileName = 0;
}

// private methods

void QFileSystemEntry::findLastSeparator()
{
    if (m_lastSeparator == -2) {
        resolveFilePath();
        m_lastSeparator = -1;
        for (int i = m_filePath.size() - 1; i >= 0; --i) {
            if (m_filePath.at(i).unicode() == '/') {
                m_lastSeparator = i;
                break;
            }
        }
    }
}

void QFileSystemEntry::findFileNameSeparators()
{
    if (m_firstDotInFileName == -2) {
        resolveFilePath();
        int firstDotInFileName = -1;
        int lastDotInFileName = -1;
        int lastSeparator = m_lastSeparator;

        int stop;
        if (lastSeparator < 0) {
            lastSeparator = -1;
            stop = 0;
        } else {
            stop = lastSeparator;
        }

        int i = m_filePath.size() - 1;
        for (; i >= stop; --i) {
            if (m_filePath.at(i).unicode() == '.') {
                firstDotInFileName = lastDotInFileName = i;
                break;
            } else if (m_filePath.at(i).unicode() == '/') {
                lastSeparator = i;
                break;
            }
        }

        if (lastSeparator != i) {
            for (--i; i >= stop; --i) {
                if (m_filePath.at(i).unicode() == '.')
                    firstDotInFileName = i;
                else if (m_filePath.at(i).unicode() == '/') {
                    lastSeparator = i;
                    break;
                }
            }
        }
        m_lastSeparator = lastSeparator;
        m_firstDotInFileName = firstDotInFileName == -1 ? -1 : firstDotInFileName - qMax(0, lastSeparator);
        if (lastDotInFileName == -1)
            m_lastDotInFileName = -1;
        else if (firstDotInFileName == lastDotInFileName)
            m_lastDotInFileName = 0;
        else
            m_lastDotInFileName = lastDotInFileName - firstDotInFileName;
    }
}

bool QFileSystemEntry::isClean() const
{
    int dots = 0;
    bool dotok = true; // checking for ".." or "." starts to relative paths
    bool slashok = true;
    for (QString::const_iterator iter = m_filePath.constBegin(); iter != m_filePath.constEnd(); iter++) {
        if (*iter == QLatin1Char('/')) {
            if (dots == 1 || dots == 2)
                return false; // path contains "./" or "../"
            if (!slashok)
                return false; // path contains "//"
            dots = 0;
            dotok = true;
            slashok = false;
        } else if (dotok) {
            slashok = true;
            if (*iter == QLatin1Char('.')) {
                dots++;
                if (dots > 2)
                    dotok = false;
            } else {
                //path element contains a character other than '.', it's clean
                dots = 0;
                dotok = false;
            }
        }
    }
    return (dots != 1 && dots != 2); // clean if path doesn't end in . or ..
}

QT_END_NAMESPACE
