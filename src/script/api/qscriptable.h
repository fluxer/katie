/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtScript module of the Katie Toolkit.
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

#ifndef QSCRIPTABLE_H
#define QSCRIPTABLE_H

#include <QtCore/qobjectdefs.h>


QT_BEGIN_NAMESPACE



class QScriptEngine;
class QScriptContext;
class QScriptValue;

class QScriptablePrivate;

class Q_SCRIPT_EXPORT QScriptable
{
public:
    QScriptable();
    ~QScriptable();

    QScriptEngine *engine() const;
    QScriptContext *context() const;
    QScriptValue thisObject() const;
    int argumentCount() const;
    QScriptValue argument(int index) const;

private:
    QScriptablePrivate* d_ptr;

    Q_DISABLE_COPY(QScriptable)
    Q_DECLARE_PRIVATE(QScriptable)
};


QT_END_NAMESPACE


#endif // QSCRIPTABLE_H
