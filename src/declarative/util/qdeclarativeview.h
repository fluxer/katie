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

#ifndef QDECLARATIVEVIEW_H
#define QDECLARATIVEVIEW_H

#include <QtCore/qdatetime.h>
#include <QtCore/qurl.h>
#include <QtGui/qgraphicssceneevent.h>
#include <QtGui/qgraphicsview.h>
#include <QtGui/qwidget.h>


QT_BEGIN_NAMESPACE


class QGraphicsObject;
class QDeclarativeEngine;
class QDeclarativeContext;
class QDeclarativeError;

class QDeclarativeViewPrivate;
class Q_DECLARATIVE_EXPORT QDeclarativeView : public QGraphicsView
{
    Q_OBJECT
    Q_PROPERTY(ResizeMode resizeMode READ resizeMode WRITE setResizeMode)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QUrl source READ source WRITE setSource DESIGNABLE true)
    Q_ENUMS(ResizeMode Status)
public:
    explicit QDeclarativeView(QWidget *parent = nullptr);
    QDeclarativeView(const QUrl &source, QWidget *parent = nullptr);
    virtual ~QDeclarativeView();

    QUrl source() const;
    void setSource(const QUrl&);

    QDeclarativeEngine* engine() const;
    QDeclarativeContext* rootContext() const;

    QGraphicsObject *rootObject() const;

    enum ResizeMode { SizeViewToRootObject, SizeRootObjectToView };
    ResizeMode resizeMode() const;
    void setResizeMode(ResizeMode);

    enum Status { Null, Ready, Loading, Error };
    Status status() const;

    QList<QDeclarativeError> errors() const;

    QSize sizeHint() const;
    QSize initialSize() const;

Q_SIGNALS:
    void sceneResized(QSize size); // ???
    void statusChanged(QDeclarativeView::Status);

private Q_SLOTS:
    void continueExecute();

protected:
    virtual void resizeEvent(QResizeEvent *);
    virtual void paintEvent(QPaintEvent *event);
    virtual void timerEvent(QTimerEvent*);
    virtual void setRootObject(QObject *obj);
    virtual bool eventFilter(QObject *watched, QEvent *e);

private:
    Q_DISABLE_COPY(QDeclarativeView)
    Q_DECLARE_PRIVATE(QDeclarativeView)
};

QT_END_NAMESPACE


#endif // QDECLARATIVEVIEW_H
