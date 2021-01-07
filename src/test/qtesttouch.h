/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
**
** This file is part of the QtTest module of the Katie Toolkit.
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

#ifndef QTESTTOUCH_H
#define QTESTTOUCH_H

#include <QtTest/qtest_global.h>

#ifdef QT_GUI_LIB

#include <QtTest/qtestassert.h>
#include <QtTest/qtestsystem.h>
#include <QtTest/qtestspontaneevent.h>
#include <QtCore/qmap.h>
#include <QtGui/qevent.h>
#include <QtGui/qwidget.h>


QT_BEGIN_NAMESPACE

extern Q_GUI_EXPORT void qt_translateRawTouchEvent(QWidget *window,
                                                   QTouchEvent::DeviceType deviceType,
                                                   const QList<QTouchEvent::TouchPoint> &touchPoints);

namespace QTest
{

    class QTouchEventSequence
    {
    public:
        ~QTouchEventSequence()
        {
            commit();
            points.clear();
        }
        QTouchEventSequence& press(int touchId, const QPoint &pt, QWidget *widget = 0)
        {
            QTouchEvent::TouchPoint &p = point(touchId);
            p.setScreenPos(mapToScreen(widget, pt));
            p.setState(Qt::TouchPointPressed);
            return *this;
        }
        QTouchEventSequence& move(int touchId, const QPoint &pt, QWidget *widget = 0)
        {
            QTouchEvent::TouchPoint &p = point(touchId);
            p.setScreenPos(mapToScreen(widget, pt));
            p.setState(Qt::TouchPointMoved);
            return *this;
        }
        QTouchEventSequence& release(int touchId, const QPoint &pt, QWidget *widget = 0)
        {
            QTouchEvent::TouchPoint &p = point(touchId);
            p.setScreenPos(mapToScreen(widget, pt));
            p.setState(Qt::TouchPointReleased);
            return *this;
        }
        QTouchEventSequence& stationary(int touchId)
        {
            QTouchEvent::TouchPoint &p = point(touchId);
            p.setState(Qt::TouchPointStationary);
            return *this;
        }

    private:
        QTouchEventSequence(QWidget *widget, QTouchEvent::DeviceType aDeviceType)
            : targetWidget(widget), deviceType(aDeviceType)
        {
        }
        QTouchEventSequence(const QTouchEventSequence &v);
        void operator=(const QTouchEventSequence&);

        QTouchEvent::TouchPoint &point(int touchId)
        {
            if (!points.contains(touchId))
                points[touchId] = QTouchEvent::TouchPoint(touchId);
            return points[touchId];
        }
        QPoint mapToScreen(QWidget *widget, const QPoint &pt)
        {
            if (widget)
                return widget->mapToGlobal(pt);
            return targetWidget ? targetWidget->mapToGlobal(pt) : pt;
        }
        void commit()
        {
            qt_translateRawTouchEvent(targetWidget, deviceType, points.values());
            targetWidget = 0;
        }

        QMap<int, QTouchEvent::TouchPoint> points;
        QWidget *targetWidget;
        QTouchEvent::DeviceType deviceType;
        friend QTouchEventSequence touchEvent(QWidget *, QTouchEvent::DeviceType);
    };

    inline
    QTouchEventSequence touchEvent(QWidget *widget = 0,
                                   QTouchEvent::DeviceType deviceType = QTouchEvent::TouchScreen)
    {
        return QTouchEventSequence(widget, deviceType);
    }

}

QT_END_NAMESPACE


#endif // QT_GUI_LIB

#endif // QTESTTOUCH_H
