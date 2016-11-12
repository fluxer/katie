/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
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

#ifndef QCOREEVENT_H
#define QCOREEVENT_H

#include <QtCore/qbytearray.h>
#include <QtCore/qobjectdefs.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE


class QEventPrivate;
class Q_CORE_EXPORT QEvent           // event base class
{
    Q_GADGET
    QDOC_PROPERTY(bool accepted READ isAccepted WRITE setAccepted)
    Q_ENUMS(Type)
public:
    enum Type {
        /*
          If you get a strange compiler error on the line with None,
          it's probably because you're also including X11 headers,
          which #define the symbol None. Put the X11 includes after
          the Qt includes to solve this problem.
        */
        None = 0,                               // invalid event
        Timer = 1,                              // timer event
        MouseButtonPress = 2,                   // mouse button pressed
        MouseButtonRelease = 3,                 // mouse button released
        MouseButtonDblClick = 4,                // mouse button double click
        MouseMove = 5,                          // mouse move
        KeyPress = 6,                           // key pressed
        KeyRelease = 7,                         // key released
        FocusIn = 8,                            // keyboard focus received
        FocusOut = 9,                           // keyboard focus lost
        Enter = 10,                             // mouse enters widget
        Leave = 11,                             // mouse leaves widget
        Paint = 12,                             // paint widget
        Move = 13,                              // move widget
        Resize = 14,                            // resize widget
        Create = 15,                            // after widget creation
        Destroy = 16,                           // during widget destruction
        Show = 17,                              // widget is shown
        Hide = 18,                              // widget is hidden
        Close = 19,                             // request to close widget
        Quit = 20,                              // request to quit application
        ParentChange = 21,                      // widget has been reparented
        ParentAboutToChange = 22,               // sent just before the parent change is done
        ThreadChange = 23,                      // object has changed threads
        WindowActivate = 24,                    // window was activated
        WindowDeactivate = 25,                  // window was deactivated
        ShowToParent = 26,                      // widget is shown to parent
        HideToParent = 27,                      // widget is hidden to parent
        Wheel = 28,                             // wheel event
        WindowTitleChange = 29,                 // window title changed
        WindowIconChange = 30,                  // icon changed
        ApplicationWindowIconChange = 31,       // application icon changed
        ApplicationFontChange = 32,             // application font changed
        ApplicationLayoutDirectionChange = 33,  // application layout direction changed
        ApplicationPaletteChange = 34,          // application palette changed
        PaletteChange = 35,                     // widget palette changed
        Clipboard = 36,                         // internal clipboard event
        Speech = 37,                            // reserved for speech input
        MetaCall =  38,                         // meta call event
        SockAct = 39,                           // socket activation
        DeferredDelete = 40,                    // deferred delete event
        DragEnter = 41,                         // drag moves into widget
        DragMove = 42,                          // drag moves in widget
        DragLeave = 43,                         // drag leaves or is cancelled
        Drop = 44,                              // actual drop
        DragResponse = 45,                      // drag accepted/rejected
        ChildAdded = 46,                        // new child widget
        ChildPolished = 47,                     // polished child widget
        ChildRemoved = 48,                      // deleted child widget
        ShowWindowRequest = 49,                 // widget's window should be mapped
        PolishRequest = 50,                     // widget should be polished
        Polish = 51,                            // widget is polished
        LayoutRequest = 52,                     // widget should be relayouted
        UpdateRequest = 53,                     // widget should be repainted
        UpdateLater = 54,                       // request update() later

        ContextMenu = 55,                       // context popup menu
        AccessibilityPrepare = 56,              // accessibility information is requested
        TabletMove = 57,                        // Wacom tablet event
        LocaleChange = 58,                      // the system locale changed
        LanguageChange = 59,                    // the application language changed
        LayoutDirectionChange = 60,             // the layout direction changed
        Style = 61,                             // internal style event
        TabletPress = 62,                       // tablet press
        TabletRelease = 63,                     // tablet release

        IconDrag = 64,                          // proxy icon dragged

        FontChange = 65,                        // font has changed
        EnabledChange = 66,                     // enabled state has changed
        ActivationChange = 67,                  // window activation has changed
        StyleChange = 68,                       // style has changed
        IconTextChange = 69,                    // icon text has changed
        ModifiedChange = 70,                    // modified state has changed
        MouseTrackingChange = 71,               // mouse tracking state has changed

        WindowBlocked = 72,                     // window is about to be blocked modally
        WindowUnblocked = 73,                   // windows modal blocking has ended
        WindowStateChange = 74,

        ToolTip = 75,
        WhatsThis = 76,
        StatusTip = 77,

        ActionChanged = 78,
        ActionAdded = 79,
        ActionRemoved = 80,

        FileOpen = 81,                          // file open request

        Shortcut = 82,                          // shortcut triggered
        ShortcutOverride = 83,                  // shortcut override request


        WhatsThisClicked = 84,

        ToolBarChange = 85,                     // toolbar visibility toggled

        ApplicationActivate = 86,               // application has been changed to active
        ApplicationDeactivate = 87,             // application has been changed to inactive

        QueryWhatsThis = 88,                    // query what's this widget help
        EnterWhatsThisMode = 89,
        LeaveWhatsThisMode = 90,

        ZOrderChange = 91,                      // child widget has had its z-order changed

        HoverEnter = 92,                        // mouse cursor enters a hover widget
        HoverLeave = 93,                        // mouse cursor leaves a hover widget
        HoverMove = 94,                         // mouse cursor move inside a hover widget

        AccessibilityHelp = 95,                 // accessibility help text request
        AccessibilityDescription = 96,          // accessibility description text request

#ifdef QT_KEYPAD_NAVIGATION
        EnterEditFocus = 97,                   // enter edit mode in keypad navigation (Defined only with QT_KEYPAD_NAVIGATION)
        LeaveEditFocus = 98,                   // leave edit mode in keypad navigation (Defined only with QT_KEYPAD_NAVIGATION)
#endif
        AcceptDropsChange = 99,

        GraphicsSceneMouseMove = 100,           // GraphicsView
        GraphicsSceneMousePress = 101,
        GraphicsSceneMouseRelease = 102,
        GraphicsSceneMouseDoubleClick = 103,
        GraphicsSceneContextMenu = 104,
        GraphicsSceneHoverEnter = 105,
        GraphicsSceneHoverMove = 106,
        GraphicsSceneHoverLeave = 107,
        GraphicsSceneHelp = 108,
        GraphicsSceneDragEnter = 109,
        GraphicsSceneDragMove = 110,
        GraphicsSceneDragLeave = 111,
        GraphicsSceneDrop = 112,
        GraphicsSceneWheel = 113,

        KeyboardLayoutChange = 114,             // keyboard layout changed

        DynamicPropertyChange = 115,            // A dynamic property was changed through setProperty/property

        TabletEnterProximity = 116,
        TabletLeaveProximity = 117,

        NonClientAreaMouseMove = 118,
        NonClientAreaMouseButtonPress = 119,
        NonClientAreaMouseButtonRelease = 120,
        NonClientAreaMouseButtonDblClick = 121,

        ContentsRectChange = 122,               // sent by QWidget::setContentsMargins (internal)

        FutureCallOut = 123,

        GraphicsSceneResize  = 124,
        GraphicsSceneMove  = 125,

        CursorChange = 126,
        ToolTipChange = 127,

        NetworkReplyUpdated = 128,              // Internal for QNetworkReply

        GrabMouse = 129,
        UngrabMouse = 130,
        GrabKeyboard = 131,
        UngrabKeyboard = 132,

        StateMachineSignal = 133,
        StateMachineWrapped = 134,

        TouchBegin = 135,
        TouchUpdate = 136,
        TouchEnd = 137,

#ifndef QT_NO_GESTURES
        NativeGesture = 138,                    // Internal for platform gesture support
#endif
        RequestSoftwareInputPanel = 139,
        CloseSoftwareInputPanel = 140,

        WinIdChange = 141,
#ifndef QT_NO_GESTURES
        Gesture = 142,
        GestureOverride = 143,
#endif

        PlatformPanel = 144,

        User = 1000,                            // first user event id
        MaxUser = 65535                         // last user event id
    };

    QEvent(Type type);
    virtual ~QEvent();
    inline Type type() const { return t; }
    inline bool spontaneous() const { return spont; }

    inline void setAccepted(bool accepted) { m_accept = accepted; }
    inline bool isAccepted() const { return m_accept; }

    inline void accept() { m_accept = true; }
    inline void ignore() { m_accept = false; }

    static int registerEventType(int hint = -1);

protected:
    QEventPrivate *d;
    Type t;

private:
    bool posted : 1;
    bool spont : 1;
    bool m_accept : 1;

    friend class QCoreApplication;
    friend class QCoreApplicationPrivate;
    friend class QThreadData;
    friend class QApplication;
    friend class QApplicationPrivate;
    friend class QShortcutMap;
    friend class QETWidget;
    friend class QGraphicsView;
    friend class QGraphicsViewPrivate;
    friend class QGraphicsScene;
    friend class QGraphicsScenePrivate;
#ifndef QT_NO_GESTURES
    friend class QGestureManager;
#endif
};

class Q_CORE_EXPORT QTimerEvent : public QEvent
{
public:
    QTimerEvent( int timerId );
    ~QTimerEvent();
    int timerId() const { return id; }
protected:
    int id;
};

class QObject;

class Q_CORE_EXPORT QChildEvent : public QEvent
{
public:
    QChildEvent( Type type, QObject *child );
    ~QChildEvent();
    QObject *child() const { return c; }
    bool added() const { return type() == ChildAdded; }
    bool polished() const { return type() == ChildPolished; }
    bool removed() const { return type() == ChildRemoved; }
protected:
    QObject *c;
};


class Q_CORE_EXPORT QDynamicPropertyChangeEvent : public QEvent
{
public:
    QDynamicPropertyChangeEvent(const QByteArray &name);
    ~QDynamicPropertyChangeEvent();

    inline QByteArray propertyName() const { return n; }

private:
    QByteArray n;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // QCOREEVENT_H
