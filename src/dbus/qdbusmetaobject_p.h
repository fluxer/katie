/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtDBus module of the Katie Toolkit.
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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the public API.  This header file may
// change from version to version without notice, or even be
// removed.
//
// We mean it.
//
//

#ifndef QDBUSMETAOBJECTPRIVATE_H
#define QDBUSMETAOBJECTPRIVATE_H

#include <QtCore/qmetaobject.h>

QT_BEGIN_NAMESPACE

class QDBusError;

struct QDBusMetaObjectPrivate;
struct Q_DBUS_EXPORT QDBusMetaObject: public QMetaObject
{
    bool cached;

    static QDBusMetaObject *createMetaObject(const QString &interface, const QString &xml,
                                             QHash<QString, QDBusMetaObject *> &map,
                                             QDBusError &error);
    ~QDBusMetaObject()
    {
        delete [] d.stringdata;
        delete [] d.data;
    }

    // methods (slots & signals):
    const char *dbusNameForMethod(int id) const;
    const char *inputSignatureForMethod(int id) const;
    const char *outputSignatureForMethod(int id) const;
    const int *inputTypesForMethod(int id) const;
    const int *outputTypesForMethod(int id) const;

    // properties:
    int propertyMetaType(int id) const;

private:
    QDBusMetaObject();
};

QT_END_NAMESPACE

#endif
