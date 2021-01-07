/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QDECLARATIVEGLOBAL_H
#define QDECLARATIVEGLOBAL_H

#include <QtCore/qglobal.h>
#include <QtCore/QObject>


QT_BEGIN_NAMESPACE


#define DEFINE_BOOL_CONFIG_OPTION(name, var) \
    static bool name() \
    { \
        static bool var ## __variable__ = qgetenv(#var).toInt(); \
        return var ## __variable__; \
    }

#define Q_DECLARATIVE_PRIVATE_EXPORT Q_DECLARATIVE_EXPORT

struct QDeclarativeGraphics_DerivedObject : public QObject
{
    void setParent_noEvent(QObject *parent) {
        bool sce = d_ptr->sendChildEvents;
        d_ptr->sendChildEvents = false;
        setParent(parent);
        d_ptr->sendChildEvents = sce;
    }
};

/*!
    Makes the \a object a child of \a parent.  Note that when using this method,
    neither \a parent nor the object's previous parent (if it had one) will
    receive ChildRemoved or ChildAdded events.
*/
inline void QDeclarative_setParent_noEvent(QObject *object, QObject *parent)
{
    static_cast<QDeclarativeGraphics_DerivedObject *>(object)->setParent_noEvent(parent);
}

#define QML_PARSER_EXPORT

QT_END_NAMESPACE


#endif // QDECLARATIVEGLOBAL_H
