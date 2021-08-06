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

#ifndef QLIBRARY_H
#define QLIBRARY_H

#include <QtCore/qobject.h>


QT_BEGIN_NAMESPACE

#ifndef QT_NO_LIBRARY

class QLibraryPrivate;

class Q_CORE_EXPORT QLibrary : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName)
    Q_PROPERTY(LoadHints loadHints READ loadHints WRITE setLoadHints)
    Q_FLAGS(LoadHint LoadHints)
public:
    enum LoadHint {
        ResolveAllSymbolsHint = 0x01,
        ExportExternalSymbolsHint = 0x02
    };
    Q_DECLARE_FLAGS(LoadHints, LoadHint)

    explicit QLibrary(QObject *parent = nullptr);
    explicit QLibrary(const QString& fileName, QObject *parent = nullptr);
    explicit QLibrary(const QString& fileName, int verNum, QObject *parent = nullptr);
    explicit QLibrary(const QString& fileName, const QString &version, QObject *parent = nullptr);
    ~QLibrary();

    void *resolve(const char *symbol);
    static void *resolve(const QString &fileName, const char *symbol);
    static void *resolve(const QString &fileName, int verNum, const char *symbol);
    static void *resolve(const QString &fileName, const QString &version, const char *symbol);

    bool load();
    bool unload();
    bool isLoaded() const;

    static bool isLibrary(const QString &fileName);

    void setFileName(const QString &fileName);
    QString fileName() const;

    void setFileNameAndVersion(const QString &fileName, int verNum);
    void setFileNameAndVersion(const QString &fileName, const QString &version);
    QString errorString() const;

    void setLoadHints(LoadHints hints);
    LoadHints loadHints() const;
private:
    QLibraryPrivate *d;
    Q_DISABLE_COPY(QLibrary)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QLibrary::LoadHints)

#endif //QT_NO_LIBRARY

QT_END_NAMESPACE


#endif //QLIBRARY_H
