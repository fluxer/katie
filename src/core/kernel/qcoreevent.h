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

        EmbeddingControl = 55,                  // ActiveX embedding
        ActivateControl = 56,                   // ActiveX activation
        DeactivateControl = 57,                 // ActiveX deactivation
        ContextMenu = 58,                       // context popup menu
        InputMethod = 59,                       // input method
        AccessibilityPrepare = 60,              // accessibility information is requested
        TabletMove = 61,                        // Wacom tablet event
        LocaleChange = 62,                      // the system locale changed
        LanguageChange = 63,                    // the application language changed
        LayoutDirectionChange = 64,             // the layout direction changed
        Style = 65,                             // internal style event
        TabletPress = 66,                       // tablet press
        TabletRelease = 67,                     // tablet release
        OkRequest = 68,                         // CE (Ok) button pressed
        HelpRequest = 69,                       // CE (?)  button pressed

        IconDrag = 70,                          // proxy icon dragged

        FontChange = 71,                        // font has changed
        EnabledChange = 72,                     // enabled state has changed
        ActivationChange = 73,                  // window activation has changed
        StyleChange = 74,                       // style has changed
        IconTextChange = 75,                    // icon text has changed
        ModifiedChange = 76,                    // modified state has changed
        MouseTrackingChange = 77,               // mouse tracking state has changed

        WindowBlocked = 78,                     // window is about to be blocked modally
        WindowUnblocked = 79,                   // windows modal blocking has ended
        WindowStateChange = 80,

        ToolTip = 81,
        WhatsThis = 82,
        StatusTip = 83,

        ActionChanged = 84,
        ActionAdded = 85,
        ActionRemoved = 86,

        FileOpen = 87,                          // file open request

        Shortcut = 88,                          // shortcut triggered
        ShortcutOverride = 89,                  // shortcut override request


        WhatsThisClicked = 90,

        ToolBarChange = 91,                     // toolbar visibility toggled

        ApplicationActivate = 92,               // application has been changed to active
        ApplicationDeactivate = 93,             // application has been changed to inactive

        QueryWhatsThis = 94,                    // query what's this widget help
        EnterWhatsThisMode = 95,
        LeaveWhatsThisMode = 96,

        ZOrderChange = 97,                      // child widget has had its z-order changed

        HoverEnter = 98,                        // mouse cursor enters a hover widget
        HoverLeave = 99,                       // mouse cursor leaves a hover widget
        HoverMove = 100,                        // mouse cursor move inside a hover widget

        AccessibilityHelp = 101,                // accessibility help text request
        AccessibilityDescription = 102,         // accessibility description text request

#ifdef QT_KEYPAD_NAVIGATION
        EnterEditFocus = 103,                   // enter edit mode in keypad navigation (Defined only with QT_KEYPAD_NAVIGATION)
        LeaveEditFocus = 104,                   // leave edit mode in keypad navigation (Defined only with QT_KEYPAD_NAVIGATION)
#endif
        AcceptDropsChange = 105,

        GraphicsSceneMouseMove = 106,           // GraphicsView
        GraphicsSceneMousePress = 107,
        GraphicsSceneMouseRelease = 108,
        GraphicsSceneMouseDoubleClick = 109,
        GraphicsSceneContextMenu = 110,
        GraphicsSceneHoverEnter = 111,
        GraphicsSceneHoverMove = 112,
        GraphicsSceneHoverLeave = 113,
        GraphicsSceneHelp = 114,
        GraphicsSceneDragEnter = 115,
        GraphicsSceneDragMove = 116,
        GraphicsSceneDragLeave = 117,
        GraphicsSceneDrop = 118,
        GraphicsSceneWheel = 119,

        KeyboardLayoutChange = 120,             // keyboard layout changed

        DynamicPropertyChange = 121,            // A dynamic property was changed through setProperty/property

        TabletEnterProximity = 122,
        TabletLeaveProximity = 123,

        NonClientAreaMouseMove = 124,
        NonClientAreaMouseButtonPress = 125,
        NonClientAreaMouseButtonRelease = 126,
        NonClientAreaMouseButtonDblClick = 127,

        ContentsRectChange = 128,               // sent by QWidget::setContentsMargins (internal)

        FutureCallOut = 129,

        GraphicsSceneResize  = 130,
        GraphicsSceneMove  = 131,

        CursorChange = 132,
        ToolTipChange = 133,

        NetworkReplyUpdated = 134,              // Internal for QNetworkReply

        GrabMouse = 135,
        UngrabMouse = 136,
        GrabKeyboard = 137,
        UngrabKeyboard = 138,

        StateMachineSignal = 139,
        StateMachineWrapped = 140,

        TouchBegin = 141,
        TouchUpdate = 142,
        TouchEnd = 143,

#ifndef QT_NO_GESTURES
        NativeGesture = 144,                    // Internal for platform gesture support
#endif
        RequestSoftwareInputPanel = 145,
        CloseSoftwareInputPanel = 146,

        WinIdChange = 147,
#ifndef QT_NO_GESTURES
        Gesture = 148,
        GestureOverride = 149,
#endif

        PlatformPanel = 150,

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
