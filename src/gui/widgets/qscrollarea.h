/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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

#ifndef QSCROLLAREA_H
#define QSCROLLAREA_H

#include <QtGui/qabstractscrollarea.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_SCROLLAREA

class QScrollAreaPrivate;

class Q_GUI_EXPORT QScrollArea : public QAbstractScrollArea
{
    Q_OBJECT
    Q_PROPERTY(bool widgetResizable READ widgetResizable WRITE setWidgetResizable)
    Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment)

public:
    explicit QScrollArea(QWidget* parent = nullptr);
    ~QScrollArea();

    QWidget *widget() const;
    void setWidget(QWidget *widget);
    QWidget *takeWidget();

    bool widgetResizable() const;
    void setWidgetResizable(bool resizable);

    QSize sizeHint() const;
    bool focusNextPrevChild(bool next);

    Qt::Alignment alignment() const;
    void setAlignment(Qt::Alignment);

    void ensureVisible(int x, int y, int xmargin = 50, int ymargin = 50);
    void ensureWidgetVisible(QWidget *childWidget, int xmargin = 50, int ymargin = 50);

protected:
    QScrollArea(QScrollAreaPrivate &dd, QWidget *parent = nullptr);
    bool event(QEvent *);
    bool eventFilter(QObject *, QEvent *);
    void resizeEvent(QResizeEvent *);
    void scrollContentsBy(int dx, int dy);

private:
    Q_DECLARE_PRIVATE(QScrollArea)
    Q_DISABLE_COPY(QScrollArea)
};

#endif // QT_NO_SCROLLAREA

QT_END_NAMESPACE


#endif // QSCROLLAREA_H
