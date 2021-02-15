/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QTESTACCESSIBLE_H
#define QTESTACCESSIBLE_H

#ifndef QT_NO_ACCESSIBILITY

#define QTEST_ACCESSIBILITY

#define QVERIFY_EVENT(object, child, event) \
    QVERIFY(QTestAccessibility::verifyEvent(object, child, (int)event))

#include <QtCore/qlist.h>
#include <QtGui/qaccessible.h>
#include <QtGui/qapplication.h>


QT_BEGIN_NAMESPACE

class QObject;

struct QTestAccessibilityEvent
{
    QTestAccessibilityEvent(QObject* o = 0, int c = 0, int e = 0)
        : object(o), child(c), event(e) {}

    bool operator==(const QTestAccessibilityEvent &o) const
    {
        return o.object == object && o.child == child && o.event == event;
    }

    QObject* object;
    int child;
    int event;
};

typedef QList<QTestAccessibilityEvent> EventList;

class QTestAccessibility
{
public:
    static void initialize()
    {
        if (!instance()) {
            instance() = new QTestAccessibility;
            qAddPostRoutine(cleanup);
        }
    }
    static void cleanup()
    {
        delete instance();
        instance() = 0;
    }
    static void clearEvents() { eventList().clear(); }
    static EventList events() { return eventList(); }
    static bool verifyEvent(const QTestAccessibilityEvent& ev)
    {
        if (eventList().isEmpty())
            return false;
        return eventList().takeFirst() == ev;
    }

    static bool verifyEvent(QObject *o, int c, int e)
    {
        return verifyEvent(QTestAccessibilityEvent(o, c, e));
    }

private:
    QTestAccessibility()
    {
        QAccessible::installUpdateHandler(updateHandler);
        QAccessible::installRootObjectHandler(rootObjectHandler);
    }

    ~QTestAccessibility()
    {
        QAccessible::installUpdateHandler(0);
        QAccessible::installRootObjectHandler(0);
    }

    static void rootObjectHandler(QObject *object)
    {
        //    qDebug("rootObjectHandler called %p", object);
        if (object) {
            QApplication* app = qobject_cast<QApplication*>(object);
            if ( !app )
                qWarning("QTEST_ACCESSIBILITY: root Object is not a QApplication!");
        } else {
            qWarning("QTEST_ACCESSIBILITY: root Object called with 0 pointer");
        }
    }

    static void updateHandler(QObject *o, int c, QAccessible::Event e)
    {
        //    qDebug("updateHandler called: %p %d %d", o, c, (int)e);
        eventList().append(QTestAccessibilityEvent(o, c, (int)e));
    }

    static EventList &eventList()
    {
        static EventList list;
        return list;
    }

    static QTestAccessibility *&instance()
    {
        static QTestAccessibility *ta = 0;
        return ta;
    }
};

#endif

QT_END_NAMESPACE


#endif
