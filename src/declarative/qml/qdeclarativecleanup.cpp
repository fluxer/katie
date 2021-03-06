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

#include "qdeclarativecleanup_p.h"

#include "qdeclarativeengine_p.h"

QT_BEGIN_NAMESPACE

/*!
\internal
\class QDeclarativeCleanup
\brief The QDeclarativeCleanup provides a callback when a QDeclarativeEngine is deleted. 

Any object that needs cleanup to occur before the QDeclarativeEngine's QScriptEngine is
destroyed should inherit from QDeclarativeCleanup.  The clear() virtual method will be
called by QDeclarativeEngine just before it deletes the QScriptEngine.
*/

/*!
\internal

Create a QDeclarativeCleanup for \a engine
*/
QDeclarativeCleanup::QDeclarativeCleanup(QDeclarativeEngine *engine)
: prev(0), next(0)
{
    if (!engine)
        return;

    QDeclarativeEnginePrivate *p = QDeclarativeEnginePrivate::get(engine);

    if (p->cleanup) next = p->cleanup;
    p->cleanup = this;
    prev = &p->cleanup;
    if (next) next->prev = &next;
}

/*!
\internal
*/
QDeclarativeCleanup::~QDeclarativeCleanup()
{
    if (prev) *prev = next;
    if (next) next->prev = prev;
    prev = 0; 
    next = 0;
}
QT_END_NAMESPACE
