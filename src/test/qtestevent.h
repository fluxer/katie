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

#ifndef QTESTEVENT_H
#define QTESTEVENT_H

#include <QtTest/qtest_global.h>
#include <QtTest/qtestsystem.h>
#include <QtCore/qlist.h>
#include <QtTest/qtestkeyboard.h>
#include <QtTest/qtestmouse.h>

#include <stdlib.h>


QT_BEGIN_NAMESPACE


class Q_TEST_EXPORT QTestEvent
{
public:
    virtual void simulate(QWidget *w) = 0;
    virtual QTestEvent *clone() const = 0;

    virtual ~QTestEvent() {}
};

#ifdef QT_GUI_LIB
class Q_TEST_EXPORT QTestKeyEvent : public QTestEvent
{
public:
    inline QTestKeyEvent(QTest::KeyAction action, Qt::Key key, Qt::KeyboardModifiers modifiers, int delay)
        : _action(action), _delay(delay), _modifiers(modifiers), _ascii(0), _key(key) {}
    inline QTestKeyEvent(QTest::KeyAction action, char ascii, Qt::KeyboardModifiers modifiers, int delay)
        : _action(action), _delay(delay), _modifiers(modifiers),
          _ascii(ascii), _key(Qt::Key_unknown) {}
    inline QTestEvent *clone() const { return new QTestKeyEvent(*this); }

    inline void simulate(QWidget *w)
    {
        if (_ascii == 0)
            QTest::keyEvent(_action, w, _key, _modifiers, _delay);
        else
            QTest::keyEvent(_action, w, _ascii, _modifiers, _delay);
    }

protected:
    QTest::KeyAction _action;
    int _delay;
    Qt::KeyboardModifiers _modifiers;
    char _ascii;
    Qt::Key _key;
};

class Q_TEST_EXPORT QTestKeyClicksEvent : public QTestEvent
{
public:
    inline QTestKeyClicksEvent(const QString &keys, Qt::KeyboardModifiers modifiers, int delay)
        : _keys(keys), _modifiers(modifiers), _delay(delay) {}
    inline QTestEvent *clone() const { return new QTestKeyClicksEvent(*this); }

    inline void simulate(QWidget *w)
    {
        QTest::keyClicks(w, _keys, _modifiers, _delay);
    }

private:
    QString _keys;
    Qt::KeyboardModifiers _modifiers;
    int _delay;
};

class Q_TEST_EXPORT QTestMouseEvent : public QTestEvent
{
public:
    inline QTestMouseEvent(QTest::MouseAction action, Qt::MouseButton button,
            Qt::KeyboardModifiers modifiers, QPoint position, int delay)
        : _action(action), _button(button), _modifiers(modifiers), _pos(position), _delay(delay) {}
    inline QTestEvent *clone() const { return new QTestMouseEvent(*this); }

    inline void simulate(QWidget *w)
    {
        QTest::mouseEvent(_action, w, _button, _modifiers, _pos, _delay);
    }

private:
    QTest::MouseAction _action;
    Qt::MouseButton _button;
    Qt::KeyboardModifiers _modifiers;
    QPoint _pos;
    int _delay;
};
#endif //QT_GUI_LIB


class Q_TEST_EXPORT QTestDelayEvent : public QTestEvent
{
public:
    inline QTestDelayEvent(int msecs): _delay(msecs) {}
    inline QTestEvent *clone() const { return new QTestDelayEvent(*this); }

    inline void simulate(QWidget * /*w*/) { QTest::qWait(_delay); }

private:
    int _delay;
};

class Q_TEST_EXPORT QTestEventList : public QList<QTestEvent *>
{
public:
    inline QTestEventList() : QList<QTestEvent *>() {}
    inline QTestEventList(const QTestEventList &other): QList<QTestEvent *>()
    { for (int i = 0; i < other.count(); ++i) append(other.at(i)->clone()); }
    inline ~QTestEventList()
    { clear(); }
    inline void clear()
    { qDeleteAll(*this); QList<QTestEvent *>::clear(); }

#ifdef QT_GUI_LIB
    inline void addKeyClick(Qt::Key qtKey, Qt::KeyboardModifiers modifiers = Qt::NoModifier, int msecs = -1)
    { addKeyEvent(QTest::Click, qtKey, modifiers, msecs); }
    inline void addKeyPress(Qt::Key qtKey, Qt::KeyboardModifiers modifiers = Qt::NoModifier, int msecs = -1)
    { addKeyEvent(QTest::Press, qtKey, modifiers, msecs); }
    inline void addKeyRelease(Qt::Key qtKey, Qt::KeyboardModifiers modifiers = Qt::NoModifier, int msecs = -1)
    { addKeyEvent(QTest::Release, qtKey, modifiers, msecs); }
    inline void addKeyEvent(QTest::KeyAction action, Qt::Key qtKey,
                            Qt::KeyboardModifiers modifiers = Qt::NoModifier, int msecs = -1)
    { append(new QTestKeyEvent(action, qtKey, modifiers, msecs)); }

    inline void addKeyClick(char ascii, Qt::KeyboardModifiers modifiers = Qt::NoModifier, int msecs = -1)
    { addKeyEvent(QTest::Click, ascii, modifiers, msecs); }
    inline void addKeyPress(char ascii, Qt::KeyboardModifiers modifiers = Qt::NoModifier, int msecs = -1)
    { addKeyEvent(QTest::Press, ascii, modifiers, msecs); }
    inline void addKeyRelease(char ascii, Qt::KeyboardModifiers modifiers = Qt::NoModifier, int msecs = -1)
    { addKeyEvent(QTest::Release, ascii, modifiers, msecs); }
    inline void addKeyClicks(const QString &keys, Qt::KeyboardModifiers modifiers = Qt::NoModifier, int msecs = -1)
    { append(new QTestKeyClicksEvent(keys, modifiers, msecs)); }
    inline void addKeyEvent(QTest::KeyAction action, char ascii, Qt::KeyboardModifiers modifiers = Qt::NoModifier, int msecs = -1)
    { append(new QTestKeyEvent(action, ascii, modifiers, msecs)); }

    inline void addMousePress(Qt::MouseButton button, Qt::KeyboardModifiers stateKey = 0,
                              QPoint pos = QPoint(), int delay=-1)
    { append(new QTestMouseEvent(QTest::MousePress, button, stateKey, pos, delay)); }
    inline void addMouseRelease(Qt::MouseButton button, Qt::KeyboardModifiers stateKey = 0,
                                QPoint pos = QPoint(), int delay=-1)
    { append(new QTestMouseEvent(QTest::MouseRelease, button, stateKey, pos, delay)); }
    inline void addMouseClick(Qt::MouseButton button, Qt::KeyboardModifiers stateKey = 0,
                              QPoint pos = QPoint(), int delay=-1)
    { append(new QTestMouseEvent(QTest::MouseClick, button, stateKey, pos, delay)); }
    inline void addMouseDClick(Qt::MouseButton button, Qt::KeyboardModifiers stateKey = 0,
                            QPoint pos = QPoint(), int delay=-1)
    { append(new QTestMouseEvent(QTest::MouseDClick, button, stateKey, pos, delay)); }
    inline void addMouseMove(QPoint pos = QPoint(), int delay=-1)
    { append(new QTestMouseEvent(QTest::MouseMove, Qt::NoButton, 0, pos, delay)); }
#endif //QT_GUI_LIB

    inline void addDelay(int msecs)
    { append(new QTestDelayEvent(msecs)); }

    inline void simulate(QWidget *w)
    {
        for (int i = 0; i < count(); ++i)
            at(i)->simulate(w);
    }
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QTestEventList)


#endif
