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
        WinEventAct = 40,                       // win event activation
        DeferredDelete = 41,                    // deferred delete event
        DragEnter = 42,                         // drag moves into widget
        DragMove = 43,                          // drag moves in widget
        DragLeave = 44,                         // drag leaves or is cancelled
        Drop = 45,                              // actual drop
        DragResponse = 46,                      // drag accepted/rejected
        ChildAdded = 47,                        // new child widget
        ChildPolished = 48,                     // polished child widget
        ChildRemoved = 49,                      // deleted child widget
        ShowWindowRequest = 50,                 // widget's window should be mapped
        PolishRequest = 51,                     // widget should be polished
        Polish = 52,                            // widget is polished
        LayoutRequest = 53,                     // widget should be relayouted
        UpdateRequest = 54,                     // widget should be repainted
        UpdateLater = 55,                       // request update() later

        EmbeddingControl = 56,                  // ActiveX embedding
        ActivateControl = 57,                   // ActiveX activation
        DeactivateControl = 58,                 // ActiveX deactivation
        ContextMenu = 59,                       // context popup menu
        InputMethod = 60,                       // input method
        AccessibilityPrepare = 61,              // accessibility information is requested
        TabletMove = 62,                        // Wacom tablet event
        LocaleChange = 63,                      // the system locale changed
        LanguageChange = 64,                    // the application language changed
        LayoutDirectionChange = 65,             // the layout direction changed
        Style = 66,                             // internal style event
        TabletPress = 67,                       // tablet press
        TabletRelease = 68,                     // tablet release
        OkRequest = 69,                         // CE (Ok) button pressed
        HelpRequest = 70,                       // CE (?)  button pressed

        IconDrag = 71,                          // proxy icon dragged

        FontChange = 72,                        // font has changed
        EnabledChange = 73,                     // enabled state has changed
        ActivationChange = 74,                  // window activation has changed
        StyleChange = 75,                       // style has changed
        IconTextChange = 76,                    // icon text has changed
        ModifiedChange = 77,                    // modified state has changed
        MouseTrackingChange = 78,               // mouse tracking state has changed

        WindowBlocked = 79,                     // window is about to be blocked modally
        WindowUnblocked = 80,                   // windows modal blocking has ended
        WindowStateChange = 81,

        ToolTip = 82,
        WhatsThis = 83,
        StatusTip = 84,

        ActionChanged = 85,
        ActionAdded = 86,
        ActionRemoved = 87,

        FileOpen = 88,                          // file open request

        Shortcut = 89,                          // shortcut triggered
        ShortcutOverride = 90,                  // shortcut override request


        WhatsThisClicked = 91,

        ToolBarChange = 92,                     // toolbar visibility toggled

        ApplicationActivate = 93,               // application has been changed to active
        ApplicationDeactivate = 94,             // application has been changed to inactive

        QueryWhatsThis = 95,                    // query what's this widget help
        EnterWhatsThisMode = 96,
        LeaveWhatsThisMode = 97,

        ZOrderChange = 98,                      // child widget has had its z-order changed

        HoverEnter = 99,                        // mouse cursor enters a hover widget
        HoverLeave = 100,                       // mouse cursor leaves a hover widget
        HoverMove = 101,                        // mouse cursor move inside a hover widget

        AccessibilityHelp = 102,                // accessibility help text request
        AccessibilityDescription = 103,         // accessibility description text request

#ifdef QT_KEYPAD_NAVIGATION
        EnterEditFocus = 104,                   // enter edit mode in keypad navigation (Defined only with QT_KEYPAD_NAVIGATION)
        LeaveEditFocus = 105,                   // leave edit mode in keypad navigation (Defined only with QT_KEYPAD_NAVIGATION)
#endif
        AcceptDropsChange = 106,

        ZeroTimerEvent = 107,                   // Used for Windows Zero timer events

        GraphicsSceneMouseMove = 108,           // GraphicsView
        GraphicsSceneMousePress = 109,
        GraphicsSceneMouseRelease = 110,
        GraphicsSceneMouseDoubleClick = 111,
        GraphicsSceneContextMenu = 112,
        GraphicsSceneHoverEnter = 113,
        GraphicsSceneHoverMove = 114,
        GraphicsSceneHoverLeave = 115,
        GraphicsSceneHelp = 116,
        GraphicsSceneDragEnter = 117,
        GraphicsSceneDragMove = 118,
        GraphicsSceneDragLeave = 119,
        GraphicsSceneDrop = 120,
        GraphicsSceneWheel = 121,

        KeyboardLayoutChange = 122,             // keyboard layout changed

        DynamicPropertyChange = 123,            // A dynamic property was changed through setProperty/property

        TabletEnterProximity = 124,
        TabletLeaveProximity = 125,

        NonClientAreaMouseMove = 126,
        NonClientAreaMouseButtonPress = 127,
        NonClientAreaMouseButtonRelease = 128,
        NonClientAreaMouseButtonDblClick = 129,

        MacSizeChange = 130,                    // when the Qt::WA_Mac{Normal,Small,Mini}Size changes

        ContentsRectChange = 131,               // sent by QWidget::setContentsMargins (internal)

        MacGLWindowChange = 132,                // Internal! the window of the GLWidget has changed

        FutureCallOut = 133,

        GraphicsSceneResize  = 134,
        GraphicsSceneMove  = 135,

        CursorChange = 136,
        ToolTipChange = 137,

        NetworkReplyUpdated = 138,              // Internal for QNetworkReply

        GrabMouse = 139,
        UngrabMouse = 140,
        GrabKeyboard = 141,
        UngrabKeyboard = 142,
        MacGLClearDrawable = 143,               // Internal Cocoa, the window has changed, so we must clear

        StateMachineSignal = 144,
        StateMachineWrapped = 145,

        TouchBegin = 146,
        TouchUpdate = 147,
        TouchEnd = 148,

#ifndef QT_NO_GESTURES
        NativeGesture = 149,                    // Internal for platform gesture support
#endif
        RequestSoftwareInputPanel = 150,
        CloseSoftwareInputPanel = 151,

        WinIdChange = 152,
#ifndef QT_NO_GESTURES
        Gesture = 153,
        GestureOverride = 154,
#endif

        PlatformPanel = 155,

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
