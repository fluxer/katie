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


#ifndef SERVEROBJECT_H
#define SERVEROBJECT_H

#include <QObject>
#include <QtDBus/QtDBus>

class ServerObject: public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.trolltech.autotests.Performance")
public:
    ServerObject(const QString &objectPath, QDBusConnection conn, QObject *parent = 0)
        : QObject(parent)
    {
        conn.registerObject(objectPath, this, QDBusConnection::ExportAllSlots);
    }

public slots:
    void noReply(const QByteArray &)
    {
        // black hole
    }
    void noReply(const QString &)
    {
        // black hole
    }
    void noReply(const QDBusVariant &)
    {
        // black hole
    }

    int size(const QByteArray &data)
    {
        return data.size();
    }
    int size(const QString &data)
    {
        return data.size();
    }
    int size(const QDBusVariant &data)
    {
        QVariant v = data.variant();
        switch (v.type())
        {
        case QVariant::ByteArray:
            return v.toByteArray().size();
        case QVariant::StringList:
            return v.toStringList().size();
        case QVariant::String:
        default:
            return v.toString().size();
        }
    }

    QByteArray echo(const QByteArray &data)
    {
        return data;
    }
    QString echo(const QString &data)
    {
        return data;
    }
    QDBusVariant echo(const QDBusVariant &data)
    {
        return data;
    }

    void nothing()
    {
    }
};

#endif
