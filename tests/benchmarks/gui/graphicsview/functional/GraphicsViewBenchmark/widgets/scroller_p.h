/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the examples of the Katie Toolkit.
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

#ifndef SCROLLER_P_H
#define SCROLLER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QPoint>
#include <QTimer>
#include <QTime>

#include "scroller.h"

class AbstractScrollArea;

class ScrollerPrivate
{
    Q_DECLARE_PUBLIC(Scroller)

public:
    enum State  {
        Stopped,
        Started,
        ManualScrolling,
        AutoScrolling
    };

    ScrollerPrivate(Scroller *scroller);
    ~ScrollerPrivate();
    void stopScrolling();
    bool eventFilter(QObject *obj, QEvent *ev);

    AbstractScrollArea *m_scrollArea;
    qreal m_scrollFactor;
    QPoint m_cursorPos;
    QPointF m_speed;
    State m_state;
    QTime m_lastCursorTime;
    QTime m_lastFrameTime;
    QTimer m_scrollTimer;
    int m_scrollSlowAccum;

private Q_SLOTS:

    void updateScrolling();

private:

    Q_DISABLE_COPY(ScrollerPrivate)
    Scroller * const q_ptr;
    QPointF mapToScrollArea(const QPoint &point);
    QWidget *m_eventViewport;
};

#endif // SCROLLER_P_H
