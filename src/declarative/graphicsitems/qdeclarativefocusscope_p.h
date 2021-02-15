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

#ifndef QDECLARATIVEFOCUSSCOPE_H
#define QDECLARATIVEFOCUSSCOPE_H

#include "qdeclarativeitem.h"


QT_BEGIN_NAMESPACE


//### set component root as focusscope
class Q_AUTOTEST_EXPORT QDeclarativeFocusScope : public QDeclarativeItem
{
    Q_OBJECT
    Q_DECLARE_PRIVATE_D(QGraphicsItem::d_ptr.data(), QDeclarativeItem)
public:
    QDeclarativeFocusScope(QDeclarativeItem *parent=0);
    virtual ~QDeclarativeFocusScope();
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeFocusScope)


#endif // QDECLARATIVEFOCUSSCOPE_H
