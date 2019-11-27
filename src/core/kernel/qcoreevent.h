/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
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
        LocaleChange = 57,                      // the system locale changed
        LanguageChange = 58,                    // the application language changed
        LayoutDirectionChange = 59,             // the layout direction changed
        Style = 60,                             // internal style event

        FontChange = 61,                        // font has changed
        EnabledChange = 62,                     // enabled state has changed
        ActivationChange = 63,                  // window activation has changed
        StyleChange = 64,                       // style has changed
        IconTextChange = 65,                    // icon text has changed
        ModifiedChange = 66,                    // modified state has changed
        MouseTrackingChange = 67,               // mouse tracking state has changed

        WindowBlocked = 68,                     // window is about to be blocked modally
        WindowUnblocked = 69,                   // windows modal blocking has ended
        WindowStateChange = 70,

        ToolTip = 71,
        WhatsThis = 72,
        StatusTip = 73,

        ActionChanged = 74,
        ActionAdded = 75,
        ActionRemoved = 76,

        Shortcut = 77,                          // shortcut triggered
        ShortcutOverride = 78,                  // shortcut override request

        WhatsThisClicked = 79,

        ApplicationActivate = 80,               // application has been changed to active
        ApplicationDeactivate = 81,             // application has been changed to inactive

        QueryWhatsThis = 82,                    // query what's this widget help
        EnterWhatsThisMode = 83,
        LeaveWhatsThisMode = 84,

        ZOrderChange = 85,                      // child widget has had its z-order changed

        HoverEnter = 86,                        // mouse cursor enters a hover widget
        HoverLeave = 87,                        // mouse cursor leaves a hover widget
        HoverMove = 88,                         // mouse cursor move inside a hover widget

        AccessibilityHelp = 89,                 // accessibility help text request
        AccessibilityDescription = 90,          // accessibility description text request

        AcceptDropsChange = 91,

        GraphicsSceneMouseMove = 92,            // GraphicsView
        GraphicsSceneMousePress = 93,
        GraphicsSceneMouseRelease = 94,
        GraphicsSceneMouseDoubleClick = 95,
        GraphicsSceneContextMenu = 96,
        GraphicsSceneHoverEnter = 97,
        GraphicsSceneHoverMove = 98,
        GraphicsSceneHoverLeave = 99,
        GraphicsSceneLeave = 100,               // internal
        GraphicsSceneHelp = 101,
        GraphicsSceneDragEnter = 102,
        GraphicsSceneDragMove = 103,
        GraphicsSceneDragLeave = 104,
        GraphicsSceneDrop = 105,
        GraphicsSceneWheel = 106,

        KeyboardLayoutChange = 107,             // keyboard layout changed

        DynamicPropertyChange = 108,            // A dynamic property was changed through setProperty/property

        NonClientAreaMouseMove = 109,
        NonClientAreaMouseButtonPress = 110,
        NonClientAreaMouseButtonRelease = 111,
        NonClientAreaMouseButtonDblClick = 112,

        ContentsRectChange = 113,               // sent by QWidget::setContentsMargins (internal)

        FutureCallOut = 114,

        GraphicsSceneResize  = 115,
        GraphicsSceneMove  = 116,

        CursorChange = 117,
        ToolTipChange = 118,

        NetworkReplyUpdated = 119,              // Internal for QNetworkReply

        GrabMouse = 120,
        UngrabMouse = 121,
        GrabKeyboard = 122,
        UngrabKeyboard = 123,

        TouchBegin = 124,
        TouchUpdate = 125,
        TouchEnd = 126,

#ifndef QT_NO_GESTURES
        NativeGesture = 127,                    // Internal for platform gesture support
#endif
        RequestSoftwareInputPanel = 128,
        CloseSoftwareInputPanel = 129,

        WinIdChange = 130,
#ifndef QT_NO_GESTURES
        Gesture = 131,
        GestureOverride = 132,
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
