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
        LocaleChange = 54,                      // the system locale changed
        LanguageChange = 55,                    // the application language changed
        LayoutDirectionChange = 56,             // the layout direction changed
        Style = 57,                             // internal style event

        FontChange = 58,                        // font has changed
        EnabledChange = 59,                     // enabled state has changed
        ActivationChange = 60,                  // window activation has changed
        StyleChange = 61,                       // style has changed
        IconTextChange = 62,                    // icon text has changed
        ModifiedChange = 63,                    // modified state has changed
        MouseTrackingChange = 64,               // mouse tracking state has changed

        WindowBlocked = 65,                     // window is about to be blocked modally
        WindowUnblocked = 66,                   // windows modal blocking has ended
        WindowStateChange = 67,

        ToolTip = 68,
        WhatsThis = 69,
        StatusTip = 70,

        ActionChanged = 71,
        ActionAdded = 72,
        ActionRemoved = 73,

        Shortcut = 74,                          // shortcut triggered
        ShortcutOverride = 75,                  // shortcut override request

        WhatsThisClicked = 76,

        ApplicationActivate = 77,               // application has been changed to active
        ApplicationDeactivate = 78,             // application has been changed to inactive

        QueryWhatsThis = 79,                    // query what's this widget help
        EnterWhatsThisMode = 80,
        LeaveWhatsThisMode = 81,

        ZOrderChange = 82,                      // child widget has had its z-order changed

        HoverEnter = 83,                        // mouse cursor enters a hover widget
        HoverLeave = 84,                        // mouse cursor leaves a hover widget
        HoverMove = 85,                         // mouse cursor move inside a hover widget

        AcceptDropsChange = 86,

        GraphicsSceneMouseMove = 87,            // GraphicsView
        GraphicsSceneMousePress = 88,
        GraphicsSceneMouseRelease = 89,
        GraphicsSceneMouseDoubleClick = 90,
        GraphicsSceneContextMenu = 91,
        GraphicsSceneHoverEnter = 92,
        GraphicsSceneHoverMove = 93,
        GraphicsSceneHoverLeave = 94,
        GraphicsSceneLeave = 95,                // internal
        GraphicsSceneHelp = 96,
        GraphicsSceneDragEnter = 97,
        GraphicsSceneDragMove = 98,
        GraphicsSceneDragLeave = 99,
        GraphicsSceneDrop = 100,
        GraphicsSceneWheel = 101,

        KeyboardLayoutChange = 102,             // keyboard layout changed

        DynamicPropertyChange = 103,            // A dynamic property was changed through setProperty/property

        NonClientAreaMouseMove = 104,
        NonClientAreaMouseButtonPress = 105,
        NonClientAreaMouseButtonRelease = 106,
        NonClientAreaMouseButtonDblClick = 107,

        ContentsRectChange = 108,               // sent by QWidget::setContentsMargins (internal)

        FutureCallOut = 109,

        GraphicsSceneResize  = 110,
        GraphicsSceneMove  = 111,

        CursorChange = 112,
        ToolTipChange = 113,

        NetworkReplyUpdated = 114,              // Internal for QNetworkReply

        GrabMouse = 115,
        UngrabMouse = 116,
        GrabKeyboard = 117,
        UngrabKeyboard = 118,

        RequestSoftwareInputPanel = 119,
        CloseSoftwareInputPanel = 120,

        WinIdChange = 121,

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
