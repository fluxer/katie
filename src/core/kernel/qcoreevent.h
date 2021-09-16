/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QCOREEVENT_H
#define QCOREEVENT_H

#include <QtCore/qbytearray.h>
#include <QtCore/qobjectdefs.h>


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
        MetaCall =  37,                         // meta call event
        SockAct = 38,                           // socket activation
        DeferredDelete = 39,                    // deferred delete event
        DragEnter = 40,                         // drag moves into widget
        DragMove = 41,                          // drag moves in widget
        DragLeave = 42,                         // drag leaves or is cancelled
        Drop = 43,                              // actual drop
        ChildAdded = 44,                        // new child widget
        ChildPolished = 45,                     // polished child widget
        ChildRemoved = 46,                      // deleted child widget
        ShowWindowRequest = 47,                 // widget's window should be mapped
        PolishRequest = 48,                     // widget should be polished
        Polish = 49,                            // widget is polished
        LayoutRequest = 50,                     // widget should be relayouted
        UpdateRequest = 51,                     // widget should be repainted
        UpdateLater = 52,                       // request update() later

        ContextMenu = 53,                       // context popup menu
        AccessibilityPrepare = 54,              // accessibility information is requested
        LocaleChange = 55,                      // the system locale changed
        LanguageChange = 56,                    // the application language changed
        LayoutDirectionChange = 57,             // the layout direction changed
        Style = 58,                             // internal style event

        FontChange = 59,                        // font has changed
        EnabledChange = 60,                     // enabled state has changed
        ActivationChange = 61,                  // window activation has changed
        StyleChange = 62,                       // style has changed
        IconTextChange = 63,                    // icon text has changed
        ModifiedChange = 64,                    // modified state has changed
        MouseTrackingChange = 65,               // mouse tracking state has changed

        WindowBlocked = 66,                     // window is about to be blocked modally
        WindowUnblocked = 67,                   // windows modal blocking has ended
        WindowStateChange = 68,

        ToolTip = 69,
        WhatsThis = 70,
        StatusTip = 71,

        ActionChanged = 72,
        ActionAdded = 73,
        ActionRemoved = 74,

        Shortcut = 75,                          // shortcut triggered
        ShortcutOverride = 76,                  // shortcut override request

        WhatsThisClicked = 77,

        ApplicationActivate = 78,               // application has been changed to active
        ApplicationDeactivate = 79,             // application has been changed to inactive

        QueryWhatsThis = 80,                    // query what's this widget help
        EnterWhatsThisMode = 81,
        LeaveWhatsThisMode = 82,

        ZOrderChange = 83,                      // child widget has had its z-order changed

        HoverEnter = 84,                        // mouse cursor enters a hover widget
        HoverLeave = 85,                        // mouse cursor leaves a hover widget
        HoverMove = 86,                         // mouse cursor move inside a hover widget

        AccessibilityHelp = 87,                 // accessibility help text request
        AccessibilityDescription = 88,          // accessibility description text request

        AcceptDropsChange = 89,

        GraphicsSceneMouseMove = 90,            // GraphicsView
        GraphicsSceneMousePress = 91,
        GraphicsSceneMouseRelease = 92,
        GraphicsSceneMouseDoubleClick = 93,
        GraphicsSceneContextMenu = 94,
        GraphicsSceneHoverEnter = 95,
        GraphicsSceneHoverMove = 96,
        GraphicsSceneHoverLeave = 97,
        GraphicsSceneLeave = 98,                // internal
        GraphicsSceneHelp = 99,
        GraphicsSceneDragEnter = 100,
        GraphicsSceneDragMove = 101,
        GraphicsSceneDragLeave = 102,
        GraphicsSceneDrop = 103,
        GraphicsSceneWheel = 104,

        KeyboardLayoutChange = 105,             // keyboard layout changed

        DynamicPropertyChange = 106,            // A dynamic property was changed through setProperty/property

        NonClientAreaMouseMove = 107,
        NonClientAreaMouseButtonPress = 108,
        NonClientAreaMouseButtonRelease = 109,
        NonClientAreaMouseButtonDblClick = 110,

        ContentsRectChange = 111,               // sent by QWidget::setContentsMargins (internal)

        FutureCallOut = 112,

        GraphicsSceneResize  = 113,
        GraphicsSceneMove  = 114,

        CursorChange = 115,
        ToolTipChange = 116,

        NetworkReplyUpdated = 117,              // Internal for QNetworkReply

        GrabMouse = 118,
        UngrabMouse = 119,
        GrabKeyboard = 120,
        UngrabKeyboard = 121,

        RequestSoftwareInputPanel = 122,
        CloseSoftwareInputPanel = 123,

        WinIdChange = 124,

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


#endif // QCOREEVENT_H
