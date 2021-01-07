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

#ifndef QDECLARATIVEGESTUREAREA_H
#define QDECLARATIVEGESTUREAREA_H

#include "qdeclarativeitem.h"
#include "qdeclarativescriptstring.h"
#include "qdeclarativecustomparser_p.h"
#include "qobject.h"
#include "qstring.h"
#include "qgesture.h"

#ifndef QT_NO_GESTURES


QT_BEGIN_NAMESPACE


class QDeclarativeBoundSignal;
class QDeclarativeContext;
class QDeclarativeGestureAreaPrivate;
class QDeclarativeGestureArea : public QDeclarativeItem
{
    Q_OBJECT

    Q_PROPERTY(QGesture *gesture READ gesture)

public:
    QDeclarativeGestureArea(QDeclarativeItem *parent=0);
    ~QDeclarativeGestureArea();

    QGesture *gesture() const;

protected:
    bool sceneEvent(QEvent *event);

private:
    void connectSignals();
    void componentComplete();
    friend class QDeclarativeGestureAreaParser;

    Q_DISABLE_COPY(QDeclarativeGestureArea)
    Q_DECLARE_PRIVATE_D(QGraphicsItem::d_ptr.data(), QDeclarativeGestureArea)
};

class QDeclarativeGestureAreaParser : public QDeclarativeCustomParser
{
public:
    virtual QByteArray compile(const QList<QDeclarativeCustomParserProperty> &);
    virtual void setCustomData(QObject *, const QByteArray &);
};


QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeGestureArea)


#endif // QT_NO_GESTURES

#endif
