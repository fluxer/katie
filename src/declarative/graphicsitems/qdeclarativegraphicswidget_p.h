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

#ifndef QDECLARATIVEGRAPHICSWIDGET_P_H
#define QDECLARATIVEGRAPHICSWIDGET_P_H

#include <QObject>
#include <QtDeclarative/qdeclarativecomponent.h>


QT_BEGIN_NAMESPACE


class QDeclarativeAnchorLine;
class QDeclarativeAnchors;
class QGraphicsObject;
class QDeclarativeGraphicsWidgetPrivate;

// ### TODO can the extension object be the anchor directly? We save one allocation -> awesome.
class QDeclarativeGraphicsWidget : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDeclarativeAnchors * anchors READ anchors DESIGNABLE false CONSTANT FINAL)
    Q_PROPERTY(QDeclarativeAnchorLine left READ left CONSTANT FINAL)
    Q_PROPERTY(QDeclarativeAnchorLine right READ right CONSTANT FINAL)
    Q_PROPERTY(QDeclarativeAnchorLine horizontalCenter READ horizontalCenter CONSTANT FINAL)
    Q_PROPERTY(QDeclarativeAnchorLine top READ top CONSTANT FINAL)
    Q_PROPERTY(QDeclarativeAnchorLine bottom READ bottom CONSTANT FINAL)
    Q_PROPERTY(QDeclarativeAnchorLine verticalCenter READ verticalCenter CONSTANT FINAL)
    // ### TODO : QGraphicsWidget don't have a baseline concept yet.
    //Q_PROPERTY(QDeclarativeAnchorLine baseline READ baseline CONSTANT FINAL)
public:
    QDeclarativeGraphicsWidget(QObject *parent = nullptr);
    ~QDeclarativeGraphicsWidget();
    QDeclarativeAnchors *anchors();
    QDeclarativeAnchorLine left() const;
    QDeclarativeAnchorLine right() const;
    QDeclarativeAnchorLine horizontalCenter() const;
    QDeclarativeAnchorLine top() const;
    QDeclarativeAnchorLine bottom() const;
    QDeclarativeAnchorLine verticalCenter() const;
    Q_DISABLE_COPY(QDeclarativeGraphicsWidget)
    Q_DECLARE_PRIVATE(QDeclarativeGraphicsWidget)
};

QT_END_NAMESPACE


#endif // QDECLARATIVEGRAPHICSWIDGET_P_H
