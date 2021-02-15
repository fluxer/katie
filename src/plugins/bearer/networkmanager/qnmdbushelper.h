/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the plugins of the Katie Toolkit.
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

#ifndef QNMDBUSHELPERPRIVATE_H
#define QNMDBUSHELPERPRIVATE_H

#include <QtDBus/qdbusextratypes.h>
#include <QDBusContext>
#include <QMap>

#ifndef QT_NO_DBUS

QT_BEGIN_NAMESPACE

class QNmDBusHelper: public QObject, protected QDBusContext
 {
     Q_OBJECT
 public:
    QNmDBusHelper(QObject *parent = Q_NULLPTR);
    ~QNmDBusHelper();

 public slots:
    void deviceStateChanged(quint32);
    void slotAccessPointAdded( QDBusObjectPath );
    void slotAccessPointRemoved( QDBusObjectPath );
    void slotPropertiesChanged( QMap<QString,QVariant>);
    void slotSettingsRemoved();

Q_SIGNALS:
    void pathForStateChanged(const QString &, quint32);
    void pathForAccessPointAdded(const QString &,  QDBusObjectPath );
    void pathForAccessPointRemoved(const QString &,  QDBusObjectPath );
    void pathForPropertiesChanged(const QString &, QMap<QString,QVariant>);
    void pathForSettingsRemoved(const QString &);
};

QT_END_NAMESPACE

#endif // QT_NO_DBUS

#endif// QNMDBUSHELPERPRIVATE_H
