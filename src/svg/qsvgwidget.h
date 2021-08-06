/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtSvg module of the Katie Toolkit.
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

#ifndef QSVGWIDGET_H
#define QSVGWIDGET_H

#include <QtGui/qwidget.h>

#ifndef QT_NO_SVGWIDGET


QT_BEGIN_NAMESPACE


class QSvgWidgetPrivate;
class QPaintEvent;
class QSvgRenderer;

class Q_SVG_EXPORT QSvgWidget : public QWidget
{
    Q_OBJECT
public:
    QSvgWidget(QWidget *parent = nullptr);
    QSvgWidget(const QString &file, QWidget *parent = nullptr);
    ~QSvgWidget();

    QSvgRenderer *renderer() const;

    QSize sizeHint() const;
public Q_SLOTS:
    void load(const QString &file);
    void load(const QByteArray &contents);
protected:
    void paintEvent(QPaintEvent *event);
private:
    Q_DISABLE_COPY(QSvgWidget)
    Q_DECLARE_PRIVATE(QSvgWidget)
};

QT_END_NAMESPACE


#endif // QT_NO_SVGWIDGET
#endif // QSVGWIDGET_H
