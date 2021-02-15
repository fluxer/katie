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

#ifndef QDECLARATIVEEXTENSIONINTERFACE_H
#define QDECLARATIVEEXTENSIONINTERFACE_H

#include <QtCore/qobject.h>


QT_BEGIN_NAMESPACE

class QDeclarativeEngine;

struct Q_DECLARATIVE_EXPORT QDeclarativeExtensionInterface
{
    virtual ~QDeclarativeExtensionInterface() {}
    virtual void registerTypes(const char *uri) = 0;
    virtual void initializeEngine(QDeclarativeEngine *engine, const char *uri) = 0;
};

QT_END_NAMESPACE

Q_DECLARE_INTERFACE(QDeclarativeExtensionInterface, "Katie.QDeclarativeExtensionInterface/1.0")


#endif // QDECLARATIVEEXTENSIONINTERFACE_H
