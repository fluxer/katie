/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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

#ifndef QAPPLICATION_P_H
#define QAPPLICATION_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of qapplication_*.cpp, qwidget*.cpp, qcolor_x11.cpp, qfiledialog.cpp
// and many other.  This header file may change from version to version
// without notice, or even be removed.
//
// We mean it.
//

#include "qplatformdefs.h"
#include "QtCore/qmutex.h"
#include "QtCore/qtranslator.h"
#include "QtCore/qbasictimer.h"
#include "QtCore/qhash.h"
#include "QtCore/qpointer.h"
#include "QtCore/qpoint.h"
#include "QtCore/qdatetime.h"
#include "QtGui/qapplication.h"
#include "QtGui/qevent.h"
#include "QtGui/qfont.h"
#include "QtGui/qcursor.h"
#include "QtGui/qregion.h"
#include "qcoreapplication_p.h"
#include "qshortcutmap_p.h"

QT_BEGIN_NAMESPACE

class QTimer;
class QClipboard;
class QGraphicsScene;
class QObject;
class QWidget;

#ifndef QT_NO_CLIPBOARD
extern QClipboard *qt_clipboard;
#endif

typedef QHash<QByteArray, QFont> FontHash;
FontHash *qt_app_fonts_hash();

typedef QHash<QByteArray, QPalette> PaletteHash;
PaletteHash *qt_app_palettes_hash();

class Q_GUI_EXPORT QApplicationPrivate : public QCoreApplicationPrivate
{
    Q_DECLARE_PUBLIC(QApplication)
public:
    QApplicationPrivate(int &argc, char **argv);
    ~QApplicationPrivate();

#if defined(Q_WS_X11)
    static bool x11_apply_settings();
#endif
    static bool quitOnLastWindowClosed;
    static void emitLastWindowClosed();
    static bool autoSipEnabled;
    static QString desktopStyleKey();

    QAbstractEventDispatcher* createEventDispatcher();
    static void dispatchEnterLeave(QWidget *enter, QWidget *leave);

    //modality
    static void enterModal(QWidget*);
    static void leaveModal(QWidget*);
    static void enterModal_sys(QWidget*);
    static void leaveModal_sys(QWidget*);
    static bool isBlockedByModal(QWidget *widget);
    static bool modalState();
    static bool tryModalHelper(QWidget *widget);

    bool notify_helper(QObject *receiver, QEvent * e);

    void construct(Display *dpy = 0, Qt::HANDLE visual = 0, Qt::HANDLE cmap = 0);
    void process_cmdline();

    bool inPopupMode() const;
    void closePopup(QWidget *popup);
    void openPopup(QWidget *popup);
    static void setFocusWidget(QWidget *focus, Qt::FocusReason reason);
    static QWidget *focusNextPrevChild_helper(QWidget *toplevel, bool next);

#ifndef QT_NO_SESSIONMANAGER
    QSessionManager *session_manager;
    QString session_id;
    QString session_key;
    bool is_session_restored;
#endif

#ifndef QT_NO_CURSOR
    QList<QCursor> cursor_list;
#endif
#ifndef QT_NO_GRAPHICSVIEW
    // Maintain a list of all scenes to ensure font and palette propagation to
    // all scenes.
    QList<QGraphicsScene *> scene_list;
#endif

    QBasicTimer toolTipWakeUp, toolTipFallAsleep;
    QPoint toolTipPos, toolTipGlobalPos, hoverGlobalPos;
    QPointer<QWidget> toolTipWidget;
#ifndef QT_NO_SHORTCUT
    QShortcutMap shortcutMap;
#endif

    static Qt::MouseButtons mouse_buttons;
    static Qt::KeyboardModifiers modifier_buttons;

    static QSize app_strut;
    static QWidgetList *popupWidgets;
    static QStyle *app_style;
    static QPalette *app_pal;
    static QPalette *sys_pal;
    static QPalette *set_pal;

private:
    static QFont *app_font; // private for a reason! Always use QApplication::font() instead!
public:
    static QFont *sys_font;
    static QFont *set_font;
    static QWidget *focus_widget;
    static QWidget *hidden_focus_widget;
    static QWidget *active_window;
    static QIcon *app_icon;
    static bool obey_desktop_settings;
    static int  cursor_flash_time;
    static int  mouse_double_click_time;
    static int  keyboard_input_time;
#ifndef QT_NO_WHEELEVENT
    static int  wheel_scroll_lines;
#endif

    static bool animate_ui;
    static bool animate_menu;
    static bool animate_tooltip;
    static bool animate_combo;
    static bool fade_menu;
    static bool fade_tooltip;
    static bool animate_toolbox;

    static void setSystemPalette(const QPalette &pal);
    static void setPalette_helper(const QPalette &palette, const char* className, bool clearWidgetPaletteHash);
    static void setSystemFont(const QFont &font);

#if defined(Q_WS_X11)
    static void applyX11SpecificCommandLineArguments(QWidget *main_widget);
#endif

    static QApplicationPrivate *instance() { return self; }

    static QString styleOverride;


#if defined(Q_WS_X11)
    void _q_alertTimeOut();
    QHash<QWidget *, QTimer *> alertTimerHash;
#endif
#ifndef QT_NO_STYLE_STYLESHEET
    static QString styleSheet;
#endif
    static QPointer<QWidget> leaveAfterRelease;
    static QWidget *pickMouseReceiver(QWidget *candidate, const QPoint &globalPos, QPoint &pos,
                                      QEvent::Type type, Qt::MouseButtons buttons,
                                      QWidget *buttonDown, QWidget *alienWidget);
    static bool sendMouseEvent(QWidget *receiver, QMouseEvent *event, QWidget *alienWidget,
                               QWidget *native, QWidget **buttonDown, QPointer<QWidget> &lastMouseReceiver,
                               bool spontaneous = true);
#if defined(Q_WS_X11)
    void sendSyntheticEnterLeave(QWidget *widget);
#endif

private:
    static QApplicationPrivate *self;

    static void giveFocusAccordingToFocusPolicy(QWidget *w,
                                                Qt::FocusPolicy focusPolicy,
                                                Qt::FocusReason focusReason);
    static bool shouldSetFocus(QWidget *w, Qt::FocusPolicy policy);
};

#if defined(Q_WS_X11)
  extern void qt_x11_enforce_cursor(QWidget *, bool);
#endif

QT_END_NAMESPACE

#endif // QAPPLICATION_P_H
