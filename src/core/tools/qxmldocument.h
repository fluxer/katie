/****************************************************************************
**
** Copyright (C) 2022 Ivailo Monev
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

#ifndef QXMLDOCUMENT_H
#define QXMLDOCUMENT_H

#include <QtCore/qdebug.h>

QT_BEGIN_NAMESPACE

class QXmlDocumentPrivate;

class Q_CORE_EXPORT QXmlAttribute
{
public:
    QByteArray name;
    QByteArray value;
};

class Q_CORE_EXPORT QXmlDocument
{
    Q_DECLARE_PRIVATE(QXmlDocument)
public:
    QXmlDocument();
    virtual ~QXmlDocument();

    QXmlDocument(const QXmlDocument &other);
    QXmlDocument &operator=(const QXmlDocument &other);

    virtual bool beginElement(const QByteArray &element, const QList<QXmlAttribute> &attributes);
    virtual bool endElement(const QByteArray &element);

    void parse(const QByteArray &xml);

    bool isNull() const;
    QString errorString() const;

    bool operator==(const QXmlDocument &other) const;
    inline bool operator!=(const QXmlDocument &other) const {
        return !operator==(other);
    }

private:
    QXmlDocumentPrivate *d_ptr;
};

QT_END_NAMESPACE

#endif // QXMLDOCUMENT_H
