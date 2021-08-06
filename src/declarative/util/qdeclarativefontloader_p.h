/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtDeclarative module of the Katie Toolkit.
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

#ifndef QDECLARATIVEFONTLOADER_H
#define QDECLARATIVEFONTLOADER_H

#include "qdeclarative.h"

#include <QtCore/qobject.h>
#include <QtCore/qurl.h>


QT_BEGIN_NAMESPACE


class QDeclarativeFontLoaderPrivate;
class Q_AUTOTEST_EXPORT QDeclarativeFontLoader : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativeFontLoader)
    Q_ENUMS(Status)

    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)

public:
    enum Status { Null = 0, Ready, Loading, Error };

    QDeclarativeFontLoader(QObject *parent = nullptr);
    ~QDeclarativeFontLoader();

    QUrl source() const;
    void setSource(const QUrl &url);

    QString name() const;
    void setName(const QString &name);

    Status status() const;

private Q_SLOTS:
    void updateFontInfo(const QString&, QDeclarativeFontLoader::Status);

Q_SIGNALS:
    void sourceChanged();
    void nameChanged();
    void statusChanged();
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeFontLoader)


#endif // QDECLARATIVEFONTLOADER_H

