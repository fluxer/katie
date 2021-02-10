/****************************************************************************
**
** Copyright (C) 2020 Ivailo Monev
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

#ifndef QJSONDOCUMENT_H
#define QJSONDOCUMENT_H

#include <QtCore/qdebug.h>

QT_BEGIN_NAMESPACE

class QJsonDocumentPrivate;

class Q_CORE_EXPORT QJsonDocument
{
    Q_DECLARE_PRIVATE(QJsonDocument)
public:
    QJsonDocument();
    ~QJsonDocument();

    QJsonDocument(const QJsonDocument &other);
    QJsonDocument &operator =(const QJsonDocument &other);

    static QJsonDocument fromVariant(const QVariant &variant);
    QVariant toVariant() const;

    static QJsonDocument fromJson(const QByteArray &json);
    QByteArray toJson() const;

    bool isNull() const;
    QString errorString() const;

    bool operator==(const QJsonDocument &other) const;
    bool operator!=(const QJsonDocument &other) const {
        return !(*this == other);
    }

private:
    friend Q_CORE_EXPORT QDebug operator<<(QDebug, const QJsonDocument &);

    QJsonDocumentPrivate *d_ptr;
};

Q_CORE_EXPORT QDebug operator<<(QDebug, const QJsonDocument &);

QT_END_NAMESPACE

#endif // QJSONDOCUMENT_H
