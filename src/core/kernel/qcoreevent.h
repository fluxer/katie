/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#ifndef QCOREEVENT_H
#define QCOREEVENT_H

#include <QtCore/qbytearray.h>
#include <QtCore/qobjectdefs.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class Q_CORE_EXPORT QEvent           // event base class
{
    Q_GADGET
    Q_PROPERTY(bool accepted READ isAccepted WRITE setAccepted)
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
        ChildAdded = 45,                        // new child widget
        ChildPolished = 46,                     // polished child widget
        ChildRemoved = 47,                      // deleted child widget
        ShowWindowRequest = 48,                 // widget's window should be mapped
        PolishRequest = 49,                     // widget should be polished
        Polish = 50,                            // widget is polished
        LayoutRequest = 51,                     // widget should be relayouted
        UpdateRequest = 52,                     // widget should be repainted
        UpdateLater = 53,                       // request update() later

        ContextMenu = 54,                       // context popup menu
        AccessibilityPrepare = 55,              // accessibility information is requested
        LocaleChange = 56,                      // the system locale changed
        LanguageChange = 57,                    // the application language changed
        LayoutDirectionChange = 58,             // the layout direction changed
        Style = 59,                             // internal style event

        FontChange = 60,                        // font has changed
        EnabledChange = 61,                     // enabled state has changed
        ActivationChange = 62,                  // window activation has changed
        StyleChange = 63,                       // style has changed
        IconTextChange = 64,                    // icon text has changed
        ModifiedChange = 65,                    // modified state has changed
        MouseTrackingChange = 66,               // mouse tracking state has changed

        WindowBlocked = 67,                     // window is about to be blocked modally
        WindowUnblocked = 68,                   // windows modal blocking has ended
        WindowStateChange = 69,

        ToolTip = 70,
        WhatsThis = 71,
        StatusTip = 72,

        ActionChanged = 73,
        ActionAdded = 74,
        ActionRemoved = 75,

        Shortcut = 76,                          // shortcut triggered
        ShortcutOverride = 77,                  // shortcut override request

        WhatsThisClicked = 78,

        ApplicationActivate = 79,               // application has been changed to active
        ApplicationDeactivate = 80,             // application has been changed to inactive

        QueryWhatsThis = 81,                    // query what's this widget help
        EnterWhatsThisMode = 82,
        LeaveWhatsThisMode = 83,

        ZOrderChange = 84,                      // child widget has had its z-order changed

        HoverEnter = 85,                        // mouse cursor enters a hover widget
        HoverLeave = 86,                        // mouse cursor leaves a hover widget
        HoverMove = 87,                         // mouse cursor move inside a hover widget

        AccessibilityHelp = 88,                 // accessibility help text request
        AccessibilityDescription = 89,          // accessibility description text request

        AcceptDropsChange = 90,

        GraphicsSceneMouseMove = 91,            // GraphicsView
        GraphicsSceneMousePress = 92,
        GraphicsSceneMouseRelease = 93,
        GraphicsSceneMouseDoubleClick = 94,
        GraphicsSceneContextMenu = 95,
        GraphicsSceneHoverEnter = 96,
        GraphicsSceneHoverMove = 97,
        GraphicsSceneHoverLeave = 98,
        GraphicsSceneLeave = 99,                // internal
        GraphicsSceneHelp = 100,
        GraphicsSceneDragEnter = 101,
        GraphicsSceneDragMove = 102,
        GraphicsSceneDragLeave = 103,
        GraphicsSceneDrop = 104,
        GraphicsSceneWheel = 105,

        KeyboardLayoutChange = 106,             // keyboard layout changed

        DynamicPropertyChange = 107,            // A dynamic property was changed through setProperty/property

        NonClientAreaMouseMove = 108,
        NonClientAreaMouseButtonPress = 109,
        NonClientAreaMouseButtonRelease = 110,
        NonClientAreaMouseButtonDblClick = 111,

        ContentsRectChange = 112,               // sent by QWidget::setContentsMargins (internal)

        FutureCallOut = 113,

        GraphicsSceneResize  = 114,
        GraphicsSceneMove  = 115,

        CursorChange = 116,
        ToolTipChange = 117,

        NetworkReplyUpdated = 118,              // Internal for QNetworkReply

        GrabMouse = 119,
        UngrabMouse = 120,
        GrabKeyboard = 121,
        UngrabKeyboard = 122,

        TouchBegin = 123,
        TouchUpdate = 124,
        TouchEnd = 125,

#ifndef QT_NO_GESTURES
        NativeGesture = 126,                    // Internal for platform gesture support
#endif
        RequestSoftwareInputPanel = 127,
        CloseSoftwareInputPanel = 128,

        WinIdChange = 129,
#ifndef QT_NO_GESTURES
        Gesture = 130,
        GestureOverride = 131,
#endif

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
    Type t;

private:
    bool posted;
    bool spont;
    bool m_accept;
    int looplevel;

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
    friend class QSpontaneKeyEvent;
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
