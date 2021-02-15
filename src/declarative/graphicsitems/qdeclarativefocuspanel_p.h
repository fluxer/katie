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

#ifndef QDECLARATIVEFOCUSPANEL_H
#define QDECLARATIVEFOCUSPANEL_H

#include "qdeclarativeitem.h"


QT_BEGIN_NAMESPACE


class Q_AUTOTEST_EXPORT QDeclarativeFocusPanel : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)
public:
    QDeclarativeFocusPanel(QDeclarativeItem *parent=0);
    virtual ~QDeclarativeFocusPanel();

Q_SIGNALS:
    void activeChanged();

protected:
    bool sceneEvent(QEvent *event);

private:
    Q_DISABLE_COPY(QDeclarativeFocusPanel)
    Q_DECLARE_PRIVATE_D(QGraphicsItem::d_ptr.data(), QDeclarativeItem)
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeFocusPanel)


#endif // QDECLARATIVEFOCUSPANEL_H
