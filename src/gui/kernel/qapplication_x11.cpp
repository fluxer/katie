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

#include "qplatformdefs.h"
#include "qcolormap.h"
#include "qdesktopwidget.h"
#include "qapplication.h"
#include "qapplication_p.h"
#include "qcursor.h"
#include "qwidget.h"
#include "qbitarray.h"
#include "qpainter.h"
#include "qfile.h"
#include "qpixmapcache.h"
#include "qdatetime.h"
#include "qtextcodec.h"
#include "qdatastream.h"
#include "qsocketnotifier.h"
#include "qsessionmanager.h"
#include "qclipboard.h"
#include "qwhatsthis.h"
#include "qsettings.h"
#include "qstylefactory.h"
#include "qfileinfo.h"
#include "qdir.h"
#include "qhash.h"
#include "qevent.h"
#include "qevent_p.h"
#include "qdebug.h"
#include "qcursor_p.h"
#include "qiconloader_p.h"
#include "qstyle.h"
#include "qmetaobject.h"
#include "qtimer.h"
#include "qlibrary.h"
#include "qguiplatformplugin.h"
#include "qthread_p.h"
#include "qeventdispatcher_x11_p.h"
#include "qpaintengine_x11_p.h"
#include "qkeymapper_p.h"
#include "qwidget_p.h"
#include "qbackingstore_p.h"
#include "qcorecommon_p.h"

#define QT_GUI_DOUBLE_CLICK_RADIUS 5

//#define ALIEN_DEBUG

#include "qt_x11_p.h"
#include "qx11info_x11.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pwd.h>

#ifndef QT_NO_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

QT_BEGIN_NAMESPACE

/* Warning: if you modify this string, modify the list of atoms in qt_x11_p.h as well! */
static const char* X11AtomsTbl[QX11Data::NPredefinedAtoms] = {
    // window-manager <-> client protocols
    "WM_PROTOCOLS\0",
    "WM_DELETE_WINDOW\0",
    "WM_TAKE_FOCUS\0",
    "_NET_WM_PING\0",
    "_NET_WM_CONTEXT_HELP\0",
    "_NET_WM_SYNC_REQUEST\0",
    "_NET_WM_SYNC_REQUEST_COUNTER\0",

    // ICCCM window state
    "WM_STATE\0",
    "WM_CHANGE_STATE\0",

    // Session management
    "WM_CLIENT_LEADER\0",
    "WM_WINDOW_ROLE\0",
    "SM_CLIENT_ID\0",

    // Clipboard
    "CLIPBOARD\0",
    "INCR\0",
    "TARGETS\0",
    "MULTIPLE\0",
    "TIMESTAMP\0",
    "SAVE_TARGETS\0",
    "CLIP_TEMPORARY\0",
    "_QT_SELECTION\0",
    "_QT_CLIPBOARD_SENTINEL\0",
    "_QT_SELECTION_SENTINEL\0",
    "CLIPBOARD_MANAGER\0",

    "_QT_SCROLL_DONE\0",

    "_MOTIF_WM_HINTS\0",

    // EWMH (aka NETWM)
    "_NET_SUPPORTED\0",
    "_NET_VIRTUAL_ROOTS\0",
    "_NET_WORKAREA\0",

    "_NET_MOVERESIZE_WINDOW\0",
    "_NET_WM_MOVERESIZE\0",

    "_NET_WM_NAME\0",
    "_NET_WM_ICON_NAME\0",
    "_NET_WM_ICON\0",

    "_NET_WM_PID\0",

    "_NET_WM_WINDOW_OPACITY\0",

    "_NET_WM_STATE\0",
    "_NET_WM_STATE_ABOVE\0",
    "_NET_WM_STATE_BELOW\0",
    "_NET_WM_STATE_FULLSCREEN\0",
    "_NET_WM_STATE_MAXIMIZED_HORZ\0",
    "_NET_WM_STATE_MAXIMIZED_VERT\0",
    "_NET_WM_STATE_MODAL\0",
    "_NET_WM_STATE_STAYS_ON_TOP\0",
    "_NET_WM_STATE_DEMANDS_ATTENTION\0",

    "_NET_WM_USER_TIME\0",
    "_NET_WM_USER_TIME_WINDOW\0",
    "_NET_WM_FULL_PLACEMENT\0",

    "_NET_WM_WINDOW_TYPE\0",
    "_NET_WM_WINDOW_TYPE_DESKTOP\0",
    "_NET_WM_WINDOW_TYPE_DOCK\0",
    "_NET_WM_WINDOW_TYPE_TOOLBAR\0",
    "_NET_WM_WINDOW_TYPE_MENU\0",
    "_NET_WM_WINDOW_TYPE_UTILITY\0",
    "_NET_WM_WINDOW_TYPE_SPLASH\0",
    "_NET_WM_WINDOW_TYPE_DIALOG\0",
    "_NET_WM_WINDOW_TYPE_DROPDOWN_MENU\0",
    "_NET_WM_WINDOW_TYPE_POPUP_MENU\0",
    "_NET_WM_WINDOW_TYPE_TOOLTIP\0",
    "_NET_WM_WINDOW_TYPE_NOTIFICATION\0",
    "_NET_WM_WINDOW_TYPE_COMBO\0",
    "_NET_WM_WINDOW_TYPE_DND\0",
    "_NET_WM_WINDOW_TYPE_NORMAL\0",

    "_NET_STARTUP_INFO\0",
    "_NET_STARTUP_INFO_BEGIN\0",

    "_NET_SUPPORTING_WM_CHECK\0",

    "_NET_WM_CM_S0\0",

    "_NET_SYSTEM_TRAY_VISUAL\0",
    "_NET_SYSTEM_TRAY_OPCODE\0",
    "MANAGER\0",

    "_NET_ACTIVE_WINDOW\0",

    // Property formats
    "COMPOUND_TEXT\0",
    "TEXT\0",
    "UTF8_STRING\0",

    // xdnd
    "XdndEnter\0",
    "XdndPosition\0",
    "XdndStatus\0",
    "XdndLeave\0",
    "XdndDrop\0",
    "XdndFinished\0",
    "XdndTypeList\0",

    "XdndSelection\0",

    "XdndAware\0",
    "XdndProxy\0",

    "XdndActionCopy\0",
    "XdndActionLink\0",
    "XdndActionMove\0",

    // XEMBED
    "_XEMBED\0",
    "_XEMBED_INFO\0",

    "_QT_SETTINGS_TIMESTAMP\0"
};

Q_GUI_EXPORT QX11Data *qt_x11Data = 0;

/*****************************************************************************
  Internal variables and functions
 *****************************************************************************/
static const char *mwGeometry = 0;                      // main widget geometry
static const char *mwTitle = 0;                         // main widget title
static bool        appSync = false;                     // X11 synchronization
#ifndef QT_NO_DEBUG
static bool        appNoGrab = false;                   // X11 grabbing enabled
static bool        appDoGrab = false;                   // X11 grabbing override (gdb)
#endif
static bool        app_do_modal = false;                // modal mode
static Window      curWin = 0;                          // current window


// function to update the workarea of the screen - in qdesktopwidget_x11.cpp
extern void qt_desktopwidget_update_workarea();

// Function to change the window manager state (from qwidget_x11.cpp)
extern void qt_change_net_wm_state(const QWidget *w, bool set, Atom one, Atom two);

// flags for extensions for special Languages, currently only for RTL languages
bool qt_use_rtl_extensions = false;

static Window           mouseActWindow       = 0;            // window where mouse is
static Qt::MouseButton  mouseButtonPressed   = Qt::NoButton; // last mouse button pressed
static Time             mouseButtonPressTime = 0;            // when was a button pressed
static short            mouseXPos, mouseYPos = 0;            // mouse pres position in act window
static short            mouseGlobalXPos, mouseGlobalYPos;    // global mouse press position

extern QWidgetList *qt_modal_stack;                // stack of modal widgets

// window where mouse buttons have been pressed
static Window pressed_window = XNone;

// popup control
static bool replayPopupMouseEvent = false;
static bool popupGrabOk = false;

bool qt_sm_blockUserInput = false;                // session management

static bool qt_x11EventFilter(XEvent* ev)
{
    long unused;
    if (qApp->filterEvent(ev, &unused))
        return true;
    return qApp->x11EventFilter(ev);
}

extern bool qt_check_clipboard_sentinel(); //def in qclipboard_x11.cpp
extern bool qt_check_selection_sentinel(); //def in qclipboard_x11.cpp
extern bool qt_xfixes_clipboard_changed(Window clipboardOwner, Time timestamp); //def in qclipboard_x11.cpp
extern bool qt_xfixes_selection_changed(Window selectionOwner, Time timestamp); //def in qclipboard_x11.cpp

QWidget *qt_button_down = 0; // last widget to be pressed with the mouse
QPointer<QWidget> qt_last_mouse_receiver = 0;
static QWidget *qt_popup_down = 0;  // popup that contains the pressed widget

extern bool qt_xdnd_dragging;

#ifndef QT_NO_XFIXES

struct qt_xfixes_selection_event_data
{
    // which selection to filter out.
    Atom selection;
};

#if defined(Q_C_CALLBACKS)
extern "C" {
#endif

static Bool qt_xfixes_scanner(Display*, XEvent *event, XPointer arg)
{
    qt_xfixes_selection_event_data *data = reinterpret_cast<qt_xfixes_selection_event_data*>(arg);
    if (event->type == qt_x11Data->xfixes_eventbase + XFixesSelectionNotify) {
        XFixesSelectionNotifyEvent *xfixes_event = reinterpret_cast<XFixesSelectionNotifyEvent*>(event);
        if (xfixes_event->selection == data->selection)
            return true;
    }
    return false;
}

#if defined(Q_C_CALLBACKS)
}
#endif

#endif // QT_NO_XFIXES

class QETWidget : public QWidget                // event translator widget
{
public:
    inline QWidgetPrivate* d_func() { return QWidget::d_func(); }
    bool translateMouseEvent(const XEvent *);
    void translatePaintEvent(const XEvent *);
    bool translateConfigEvent(const XEvent *);
    bool translateCloseEvent(const XEvent *);
    bool translateScrollDoneEvent(const XEvent *);
    bool translateWheelEvent(int global_x, int global_y, int delta, Qt::MouseButtons buttons,
                             Qt::KeyboardModifiers modifiers, Qt::Orientation orient);
    bool translatePropertyEvent(const XEvent *);

    void doDeferredMap()
    {
        Q_ASSERT(testAttribute(Qt::WA_WState_Created));
        if (!testAttribute(Qt::WA_Resized)) {
            adjustSize();
            setAttribute(Qt::WA_Resized, false);
        }

        /*
          workaround for WM's that throw away ConfigureRequests from the following:

          window->hide();
          window->move(x, y); // could also be resize(), move()+resize(), or setGeometry()
          window->show();
        */
        QRect r = geometry();

        XMoveResizeWindow(qt_x11Data->display,
                          internalWinId(),
                          r.x(),
                          r.y(),
                          r.width(),
                          r.height());

        // static gravity!
        XSizeHints sh;
        memset(&sh, 0, sizeof(sh));
        long unused;
        XGetWMNormalHints(qt_x11Data->display, internalWinId(), &sh, &unused);
        sh.flags |= USPosition | PPosition | USSize | PSize | PWinGravity;
        sh.x = r.x();
        sh.y = r.y();
        sh.width = r.width();
        sh.height = r.height();
        sh.win_gravity = StaticGravity;
        XSetWMNormalHints(qt_x11Data->display, internalWinId(), &sh);

        setAttribute(Qt::WA_Mapped);
        if (testAttribute(Qt::WA_DontShowOnScreen))
            return;
        d_func()->topData()->waitingForMapNotify = 1;
        XMapWindow(qt_x11Data->display, internalWinId());
    }
};


QAbstractEventDispatcher* QApplicationPrivate::createEventDispatcher()
{
    Q_Q(QApplication);
    return (q->type() != QApplication::Tty
                ? new QEventDispatcherX11(q)
                : new QEventDispatcherUNIX(q));
}

/*****************************************************************************
  Default X error handlers
 *****************************************************************************/

#if defined(Q_C_CALLBACKS)
extern "C" {
#endif

static int (*original_x_errhandler)(Display *dpy, XErrorEvent *);
static int (*original_xio_errhandler)(Display *dpy);

static int qt_x_errhandler(Display *dpy, XErrorEvent *err)
{
    if (qt_x11Data->display != dpy) {
        // only handle X errors for our display
        return 0;
    }

    switch (err->error_code) {
    case BadAtom:
        if (err->request_code == X_GetProperty
            && (err->resourceid == XA_RESOURCE_MANAGER
                || err->resourceid == XA_RGB_DEFAULT_MAP
                || err->resourceid == ATOM(_NET_SUPPORTED)
                || err->resourceid == ATOM(_NET_SUPPORTING_WM_CHECK)
                || err->resourceid == ATOM(XdndProxy)
                || err->resourceid == ATOM(XdndAware))) {
            // Perhaps we're running under SECURITY reduction? :/
            return 0;
        }
        break;

    case BadWindow:
        if (err->request_code == X_ChangeWindowAttributes
            || err->request_code == X_QueryPointer) {
            for (int i = 0; i < ScreenCount(dpy); ++i) {
                if (err->resourceid == RootWindow(dpy, i)) {
                    // Perhaps we're running under SECURITY reduction? :/
                    return 0;
                }
            }
        }
        qt_x11Data->seen_badwindow = true;
        if (err->request_code == X_SendEvent) {
            for (int i = 0; i < ScreenCount(dpy); ++i) {
                if (err->resourceid == RootWindow(dpy, i)) {
                    // Perhaps we're running under SECURITY reduction? :/
                    return 0;
                }
            }
            if (qt_x11Data->xdndHandleBadwindow()) {
                qDebug("xdndHandleBadwindow returned true");
                return 0;
            }
        }
        if (qt_x11Data->ignore_badwindow)
            return 0;
        break;

    default:
        break;
    }

    static const int maxerrlen = 256;
    QSTACKARRAY(char, errstr, maxerrlen);
    XGetErrorText(dpy, err->error_code, errstr, maxerrlen);
    QSTACKARRAY(char, buffer, maxerrlen);
    QSTACKARRAY(char, request_str, maxerrlen);
    ::snprintf(buffer, maxerrlen, "%d", err->request_code);
    XGetErrorDatabaseText(dpy, "XRequest", buffer, "", request_str, maxerrlen);
    if (err->request_code < 128) {
        // X error for a normal protocol request
        qWarning( "X Error: %s %d\n"
                  "  Major opcode: %d (%s)\n"
                  "  Resource id:  0x%lx",
                  errstr, err->error_code,
                  err->request_code,
                  request_str,
                  err->resourceid );
    } else {
        // X error for an extension request
        const char *extensionName = 0;
        if (err->request_code == qt_x11Data->xrender_major)
            extensionName = "RENDER";
        else if (err->request_code == qt_x11Data->xrandr_major)
            extensionName = "RANDR";

        QSTACKARRAY(char, minor_str, maxerrlen);
        if (extensionName) {
            ::snprintf(buffer, maxerrlen, "%s.%d", extensionName, err->minor_code);
            XGetErrorDatabaseText(dpy, "XRequest", buffer, "", minor_str, maxerrlen);
        } else {
            extensionName = "Unknown extension";
            ::snprintf(minor_str, maxerrlen, "Unknown request");
        }
        qWarning("X Error: %s %d\n"
                 "  Extension:    %d (%s)\n"
                 "  Minor opcode: %d (%s)\n"
                 "  Resource id:  0x%lx",
                 errstr, err->error_code,
                 err->request_code, extensionName,
                 err->minor_code, minor_str,
                 err->resourceid);
    }

    // ### we really should distinguish between severe, non-severe and
    // ### application specific errors

    return 0;
}


static int qt_xio_errhandler(Display *)
{
    QApplication::exit(1);
    QByteArray appName = QApplication::applicationName().toLocal8Bit();
    qFatal("%s: Fatal IO error: client killed", appName.constData());
    return 0;
}

#if defined(Q_C_CALLBACKS)
}
#endif


#ifndef QT_NO_XSYNC
struct qt_sync_request_event_data
{
    WId window;
};

#if defined(Q_C_CALLBACKS)
extern "C" {
#endif

static Bool qt_sync_request_scanner(Display*, XEvent *event, XPointer arg)
{
    qt_sync_request_event_data *data = reinterpret_cast<qt_sync_request_event_data*>(arg);
    if (event->type == ClientMessage &&
        event->xany.window == data->window &&
        event->xclient.message_type == ATOM(WM_PROTOCOLS) &&
        (Atom)event->xclient.data.l[0] == ATOM(_NET_WM_SYNC_REQUEST)) {
        QWidget *w = QWidget::find(event->xany.window);
        if (QTLWExtra *tlw = ((QETWidget*)w)->d_func()->maybeTopData()) {
            const Time timestamp = event->xclient.data.l[1];
            if (timestamp > qt_x11Data->time)
                qt_x11Data->time = timestamp;
            if (timestamp == CurrentTime || timestamp > tlw->syncRequestTimestamp) {
                tlw->syncRequestTimestamp = timestamp;
                tlw->newCounterValueLo = event->xclient.data.l[2];
                tlw->newCounterValueHi = event->xclient.data.l[3];
            }
        }
        return true;
    }
    return false;
}

#if defined(Q_C_CALLBACKS)
}
#endif

#endif // QT_NO_XSYNC

static void qt_x11_create_intern_atoms()
{
#if defined(XlibSpecificationRelease) && (XlibSpecificationRelease >= 6)
    XInternAtoms(qt_x11Data->display, const_cast<char**>(X11AtomsTbl),
        QX11Data::NPredefinedAtoms, False, qt_x11Data->atoms);
#else
    for (qint16 i = 0; i < QX11Data::NPredefinedAtoms; i++) {
        qt_x11Data->atoms[i] = XInternAtom(qt_x11Data->display, const_cast<char*>(X11AtomsTbl[i]), False);
    }
#endif
}

Q_GUI_EXPORT void qt_x11_apply_settings_in_all_apps()
{
    QByteArray stamp;
    QDataStream s(&stamp, QIODevice::WriteOnly);
    s << QDateTime::currentDateTime();

    XChangeProperty(QX11Info::display(), QX11Info::appRootWindow(0),
                    ATOM(_QT_SETTINGS_TIMESTAMP), ATOM(_QT_SETTINGS_TIMESTAMP), 8,
                    PropModeReplace, (unsigned char *)stamp.data(), stamp.size());
}

/*! \internal
    apply the settings to the application
*/
bool QApplicationPrivate::x11_apply_settings()
{
    if (!QApplication::desktopSettingsAware()) {
        return true;
    }

    QSettings *settings = QCoreApplicationPrivate::staticConf();

    /*
      Qt settings. This is now they are written into the datastream.

      Palette / *                - QPalette
      font                       - QFont
      libraryPath                - QStringList
      style                      - QString
      doubleClickInterval        - int
      keyboardInputInterval      - int
      cursorFlashTime            - int
      wheelScrollLines           - int
      defaultCodec               - QString
      globalStrut/width          - int
      globalStrut/height         - int
      GUIEffects                 - QStringList
      Font Substitutions/ *      - QStringList
      Font Substitutions/...     - QStringList
    */

    QPalette pal(Qt::black);
    int groupCount = 0;
    QStringList strlist = settings->value(QLatin1String("Qt/Palette/active")).toStringList();
    if (!strlist.isEmpty()) {
        ++groupCount;
        for (int i = 0; i < qMin(strlist.count(), int(QPalette::NColorRoles)); i++)
            pal.setColor(QPalette::Active, (QPalette::ColorRole) i,
                         QColor(strlist[i]));
    }
    strlist = settings->value(QLatin1String("Qt/Palette/inactive")).toStringList();
    if (!strlist.isEmpty()) {
        ++groupCount;
        for (int i = 0; i < qMin(strlist.count(), int(QPalette::NColorRoles)); i++)
            pal.setColor(QPalette::Inactive, (QPalette::ColorRole) i,
                         QColor(strlist[i]));
    }
    strlist = settings->value(QLatin1String("Qt/Palette/disabled")).toStringList();
    if (!strlist.isEmpty()) {
        ++groupCount;
        for (int i = 0; i < qMin(strlist.count(), int(QPalette::NColorRoles)); i++)
            pal.setColor(QPalette::Disabled, (QPalette::ColorRole) i,
                         QColor(strlist[i]));
    }

    // ### Fix properly for 4.6
    if (groupCount == QPalette::NColorGroups)
        QApplicationPrivate::setSystemPalette(pal);

    QString fontDescription = settings->value(QLatin1String("Qt/font")).toString();
    if (!fontDescription.isEmpty()) {
        QFont font(QApplication::font());
        font.fromString(fontDescription );
        QApplicationPrivate::setSystemFont(font);
    }

    // read library (ie. plugin) path list
    QStringList pathlist = settings->value(QLatin1String("Qt/libraryPath")).toString().split(QLatin1Char(':'));
    if (! pathlist.isEmpty()) {
        QStringList::ConstIterator it = pathlist.constBegin();
        while (it != pathlist.constEnd())
            QApplication::addLibraryPath(*it++);
    }

    // read new QStyle
    QString stylename = settings->value(QLatin1String("Qt/style")).toString();

    if (stylename.isEmpty() && QApplicationPrivate::styleOverride.isNull() && qt_x11Data->use_xrender) {
        stylename = qt_guiPlatformPlugin()->styleName();
    }

    if (QCoreApplication::startingUp()) {
        if (!stylename.isEmpty() && QApplicationPrivate::styleOverride.isNull())
            QApplicationPrivate::styleOverride = stylename;
    } else {
        if (stylename.compare(QApplication::style()->objectName(), Qt::CaseInsensitive) != 0) {
            QApplication::setStyle(stylename);
        }
    }

    int num = settings->value(QLatin1String("Qt/doubleClickInterval"),
                             QApplication::doubleClickInterval()).toInt();
    QApplication::setDoubleClickInterval(num);

    num = settings->value(QLatin1String("Qt/cursorFlashTime"),
                         QApplication::cursorFlashTime()).toInt();
    QApplication::setCursorFlashTime(num);

#ifndef QT_NO_WHEELEVENT
    num = settings->value(QLatin1String("Qt/wheelScrollLines"),
                         QApplication::wheelScrollLines()).toInt();
    QApplication::setWheelScrollLines(num);
#endif

    QString defaultcodec = settings->value(QLatin1String("Qt/defaultCodec"),
                                          QVariant(QLatin1String("none"))).toString();
    if (defaultcodec != QLatin1String("none")) {
        QTextCodec *codec = QTextCodec::codecForName(defaultcodec.toLatin1());
        if (codec)
            QTextCodec::setCodecForTr(codec);
    }

    int w = settings->value(QLatin1String("Qt/globalStrut/width")).toInt();
    int h = settings->value(QLatin1String("Qt/globalStrut/height")).toInt();
    QSize strut(w, h);
    if (strut.isValid())
        QApplication::setGlobalStrut(strut);

    QStringList effects = settings->value(QLatin1String("Qt/GUIEffects")).toStringList();
    QApplication::setEffectEnabled(Qt::UI_General,
                                   effects.contains(QLatin1String("general")));
    QApplication::setEffectEnabled(Qt::UI_AnimateMenu,
                                   effects.contains(QLatin1String("animatemenu")));
    QApplication::setEffectEnabled(Qt::UI_FadeMenu,
                                   effects.contains(QLatin1String("fademenu")));
    QApplication::setEffectEnabled(Qt::UI_AnimateCombo,
                                   effects.contains(QLatin1String("animatecombo")));
    QApplication::setEffectEnabled(Qt::UI_AnimateTooltip,
                                   effects.contains(QLatin1String("animatetooltip")));
    QApplication::setEffectEnabled(Qt::UI_FadeTooltip,
                                   effects.contains(QLatin1String("fadetooltip")));
    QApplication::setEffectEnabled(Qt::UI_AnimateToolBox,
                                   effects.contains(QLatin1String("animatetoolbox")));

    if (!qt_x11Data->has_fontconfig) {
        foreach (const QString &fam, settings->keys()) {
            if (!fam.startsWith(QLatin1String("Qt/Font Substitutions/"))) {
                continue;
            }
            QStringList subs = settings->value(fam).toStringList();
            QFont::insertSubstitutions(fam, subs);
        }
    }

    qt_use_rtl_extensions = settings->value(QLatin1String("Qt/useRtlExtensions"), false).toBool();

    QIconLoader::instance()->updateSystemTheme();

    return true;
}

// update the supported array
static void qt_get_net_supported()
{
    Atom type;
    int format;
    long offset = 0;
    unsigned long nitems, after;
    unsigned char *data = 0;

    int e = XGetWindowProperty(qt_x11Data->display, QX11Info::appRootWindow(),
                               ATOM(_NET_SUPPORTED), 0, 0,
                               False, XA_ATOM, &type, &format, &nitems, &after, &data);
    if (data)
        XFree(data);

    if (qt_x11Data->net_supported_list)
        delete [] qt_x11Data->net_supported_list;
    qt_x11Data->net_supported_list = 0;

    if (e == Success && type == XA_ATOM && format == 32) {
        QByteArray buffer;

        while (after > 0) {
            XGetWindowProperty(qt_x11Data->display, QX11Info::appRootWindow(),
                               ATOM(_NET_SUPPORTED), offset, 1024,
                               False, XA_ATOM, &type, &format, &nitems, &after, &data);

            if (type == XA_ATOM && format == 32) {
                buffer = QByteArray(reinterpret_cast<char *>(data), nitems * sizeof(long));
                offset += nitems;
            } else
                after = 0;
            if (data)
                XFree(data);
        }

        // compute nitems
        nitems = buffer.size() / sizeof(Atom);
        qt_x11Data->net_supported_list = new Atom[nitems + 1];
        Atom *a = (Atom *) buffer.data();
        for (uint i = 0; i < nitems; i++)
            qt_x11Data->net_supported_list[i] = a[i];
        qt_x11Data->net_supported_list[nitems] = 0;
    }
}


bool QX11Data::isSupportedByWM(Atom atom)
{
    if (!qt_x11Data->net_supported_list)
        return false;

    int i = 0;
    while (qt_x11Data->net_supported_list[i] != 0) {
        if (qt_x11Data->net_supported_list[i++] == atom) {
            return true;
        }
    }

    return false;
}


// update the virtual roots array
static void qt_get_net_virtual_roots()
{
    if (qt_x11Data->net_virtual_root_list)
        delete [] qt_x11Data->net_virtual_root_list;
    qt_x11Data->net_virtual_root_list = 0;

    if (!qt_x11Data->isSupportedByWM(ATOM(_NET_VIRTUAL_ROOTS)))
        return;

    Atom type;
    int format;
    long offset = 0;
    unsigned long nitems, after;
    unsigned char *data;

    int e = XGetWindowProperty(qt_x11Data->display, QX11Info::appRootWindow(),
                               ATOM(_NET_VIRTUAL_ROOTS), 0, 0,
                               False, XA_ATOM, &type, &format, &nitems, &after, &data);
    if (data)
        XFree(data);

    if (e == Success && type == XA_ATOM && format == 32) {
        QByteArray buffer;

        while (after > 0) {
            XGetWindowProperty(qt_x11Data->display, QX11Info::appRootWindow(),
                               ATOM(_NET_VIRTUAL_ROOTS), offset, 1024,
                               False, XA_ATOM, &type, &format, &nitems, &after, &data);

            if (type == XA_ATOM && format == 32) {
                buffer = QByteArray(reinterpret_cast<char *>(data), nitems * 4);
                offset += nitems;
            } else
                after = 0;
            if (data)
                XFree(data);
        }

        // compute nitems
        nitems = buffer.size() / sizeof(Window);
        qt_x11Data->net_virtual_root_list = new Window[nitems + 1];
        Window *a = (Window *) buffer.data();
        for (uint i = 0; i < nitems; i++)
            qt_x11Data->net_virtual_root_list[i] = a[i];
        qt_x11Data->net_virtual_root_list[nitems] = 0;
    }
}

void qt_net_remove_user_time(QWidget *tlw)
{
    Q_ASSERT(tlw);
    QTLWExtra *extra = tlw->d_func()->maybeTopData();
    if (extra && extra->userTimeWindow) {
        Q_ASSERT(tlw->internalWinId());
        XDeleteProperty(qt_x11Data->display, tlw->internalWinId(), ATOM(_NET_WM_USER_TIME_WINDOW));
        XDestroyWindow(qt_x11Data->display, extra->userTimeWindow);
        extra->userTimeWindow = 0;
    }
}

void qt_net_update_user_time(QWidget *tlw, unsigned long timestamp)
{
    Q_ASSERT(tlw);
    Q_ASSERT(tlw->isWindow());
    Q_ASSERT(tlw->testAttribute(Qt::WA_WState_Created));
    QTLWExtra *extra = tlw->d_func()->topData();
    WId wid = tlw->internalWinId();
    const bool isSupportedByWM = qt_x11Data->isSupportedByWM(ATOM(_NET_WM_USER_TIME_WINDOW));
    if (extra->userTimeWindow || isSupportedByWM) {
        if (!extra->userTimeWindow) {
            extra->userTimeWindow = XCreateSimpleWindow(qt_x11Data->display,
                                                        tlw->internalWinId(),
                                                        -1, -1, 1, 1, 0, 0, 0);
            wid = extra->userTimeWindow;
            XChangeProperty(qt_x11Data->display, tlw->internalWinId(), ATOM(_NET_WM_USER_TIME_WINDOW),
                            XA_WINDOW, 32, PropModeReplace,
                            (unsigned char *)&wid, 1);
            XDeleteProperty(qt_x11Data->display, tlw->internalWinId(), ATOM(_NET_WM_USER_TIME));
        } else if (!isSupportedByWM) {
            // WM no longer supports it, then we should remove the
            // _NET_WM_USER_TIME_WINDOW atom.
            qt_net_remove_user_time(tlw);
        } else {
            wid = extra->userTimeWindow;
        }
    }
    XChangeProperty(qt_x11Data->display, wid, ATOM(_NET_WM_USER_TIME),
                    XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &timestamp, 1);
}

static void qt_check_focus_model()
{
    Window fw = XNone;
    int unused;
    XGetInputFocus(qt_x11Data->display, &fw, &unused);
    if (fw == PointerRoot)
        qt_x11Data->focus_model = QX11Data::FM_PointerRoot;
    else
        qt_x11Data->focus_model = QX11Data::FM_Other;
}

/*****************************************************************************
  qt_init() - initializes Qt for X11
 *****************************************************************************/

#if !defined(QT_NO_FONTCONFIG)
static void getXDefault(const char *group, const char *key, int *val)
{
    char *str = XGetDefault(qt_x11Data->display, group, key);
    if (str) {
        char *end = 0;
        int v = strtol(str, &end, 0);
        if (str != end)
            *val = v;
        // otherwise use fontconfig to convert the string to integer
        else
            FcNameConstant((FcChar8 *) str, val);
    }
}

static void getXDefault(const char *group, const char *key, bool *val)
{
    char *str = XGetDefault(qt_x11Data->display, group, key);
    if (str) {
        char c = str[0];
        if (isupper((int)c))
            c = tolower(c);
        if (c == 't' || c == 'y' || c == '1')
            *val = true;
        else if (c == 'f' || c == 'n' || c == '0')
            *val = false;
        if (c == 'o') {
            c = str[1];
            if (isupper((int)c))
                c = tolower(c);
            if (c == 'n')
                *val = true;
            if (c == 'f')
                *val = false;
        }
    }
}
#endif // QT_NO_FONTCONFIG

#if !defined(QT_NO_DEBUG) && defined(QT_HAVE_PROC_CMDLINE) && defined(QT_HAVE_PROC_EXE)
// Find out if our parent process is gdb by looking at the 'exe' symlink under /proc,.
// or, for older Linuxes, read out 'cmdline'.
bool runningUnderDebugger()
{
    const QString parentProc = QString::fromLatin1("/proc/%1").arg(::getppid());
    const QFileInfo parentProcExe(parentProc + QLatin1String("/exe"));
    if (parentProcExe.isSymLink())
        return parentProcExe.readLink().endsWith(QLatin1String("/gdb"));
    QFile f(parentProc + QLatin1String("/cmdline"));
    if (!f.open(QIODevice::ReadOnly))
        return false;
    QByteArray s;
    char c;
    while (f.getChar(&c) && c) {
        if (c == '/')
            s.clear();
        else
            s += c;
    }
    return s == "gdb";
}
#endif

// ### This should be static but it isn't because of the friend declaration
// ### in qpaintdevice.h which then should have a static too but can't have
// ### it because "storage class specifiers invalid in friend function
// ### declarations" :-) Ideas anyone?
void qt_init(QApplicationPrivate *priv, Display *display,
             Qt::HANDLE visual, Qt::HANDLE colormap)
{
    qt_x11Data = new QX11Data;
    qt_x11Data->display = display;
    qt_x11Data->displayName = 0;
    qt_x11Data->foreignDisplay = (display != 0);
    qt_x11Data->focus_model = QX11Data::FM_Unknown;

    // RANDR
    qt_x11Data->use_xrandr = false;
    qt_x11Data->xrandr_major = 0;
    qt_x11Data->xrandr_eventbase = 0;

    // RENDER
    qt_x11Data->use_xrender = false;
    qt_x11Data->xrender_major = 0;
    qt_x11Data->xrender_minor = 0;

    // XFIXES
    qt_x11Data->use_xfixes = false;
    qt_x11Data->xfixes_eventbase = 0;

    // XINERAMA
    qt_x11Data->use_xinerama = false;

    qt_x11Data->sip_serial = 0;
    qt_x11Data->net_supported_list = 0;
    qt_x11Data->net_virtual_root_list = 0;
    qt_x11Data->wm_client_leader = 0;
    qt_x11Data->screens = 0;
    qt_x11Data->argbVisuals = 0;
    qt_x11Data->argbColormaps = 0;
    qt_x11Data->screenCount = 0;
    qt_x11Data->time = CurrentTime;
    qt_x11Data->userTime = CurrentTime;
    qt_x11Data->ignore_badwindow = false;
    qt_x11Data->seen_badwindow = false;

    // colormap control
    qt_x11Data->visual_class = -1;
    qt_x11Data->visual_id = -1;
    qt_x11Data->color_count = 0;
    qt_x11Data->custom_cmap = false;

    // outside visual/colormap
    qt_x11Data->visual = reinterpret_cast<Visual *>(visual);
    qt_x11Data->colormap = colormap;

    // Fontconfig
    qt_x11Data->has_fontconfig = false;
#if !defined(QT_NO_FONTCONFIG)
    if (qgetenv("QT_X11_NO_FONTCONFIG").isNull())
        qt_x11Data->has_fontconfig = FcInit();
    qt_x11Data->fc_antialias = true;
#endif

#ifndef QT_NO_XRENDER
    memset(qt_x11Data->solid_fills, 0, sizeof(qt_x11Data->solid_fills));
    for (int i = 0; i < qt_x11Data->solid_fill_count; ++i)
        qt_x11Data->solid_fills[i].screen = -1;
    memset(qt_x11Data->pattern_fills, 0, sizeof(qt_x11Data->pattern_fills));
    for (int i = 0; i < qt_x11Data->pattern_fill_count; ++i)
        qt_x11Data->pattern_fills[i].screen = -1;
#endif

    qt_x11Data->startupId = 0;

    int argc = priv->argc;
    char **argv = priv->argv;

    if (!qt_x11Data->display && QApplication::testAttribute(Qt::AA_X11InitThreads)) {
        XInitThreads();
    }

    // Install default error handlers
    original_x_errhandler = XSetErrorHandler(qt_x_errhandler);
    original_xio_errhandler = XSetIOErrorHandler(qt_xio_errhandler);

    // Get command line params
    for (int i=1; i<argc; i++) {
        if (argv[i] && *argv[i] != '-') {
            continue;
        }
        QByteArray arg(argv[i]);
        if (arg == "-display") {
            if (++i < argc && !qt_x11Data->display)
                qt_x11Data->displayName = argv[i];
        } else if (arg == "-title") {
            if (++i < argc)
                mwTitle = argv[i];
        } else if (arg == "-geometry") {
            if (++i < argc)
                mwGeometry = argv[i];
        } else if (arg == "-ncols") {   // xv and netscape use this name
            if (++i < argc)
                qt_x11Data->color_count = qMax(0,atoi(argv[i]));
        } else if (arg == "-visual") {  // xv and netscape use this name
            if (++i < argc && !qt_x11Data->visual) {
                QString s = QString::fromLocal8Bit(argv[i]).toLower();
                if (s == QLatin1String("staticgray"))
                    qt_x11Data->visual_class = StaticGray;
                else if (s == QLatin1String("grayscale"))
                    qt_x11Data->visual_class = XGrayScale;
                else if (s == QLatin1String("staticcolor"))
                    qt_x11Data->visual_class = StaticColor;
                else if (s == QLatin1String("pseudocolor"))
                    qt_x11Data->visual_class = PseudoColor;
                else if (s == QLatin1String("truecolor"))
                    qt_x11Data->visual_class = TrueColor;
                else if (s == QLatin1String("directcolor"))
                    qt_x11Data->visual_class = DirectColor;
                else
                    qt_x11Data->visual_id = static_cast<int>(strtol(argv[i], 0, 0));
            }
        } else if (arg == "-cmap") {    // xv uses this name
            if (!qt_x11Data->colormap)
                qt_x11Data->custom_cmap = true;
        }
        else if (arg == "-sync")
            appSync = !appSync;
#ifndef QT_NO_DEBUG
        else if (arg == "-nograb")
            appNoGrab = !appNoGrab;
        else if (arg == "-dograb")
            appDoGrab = !appDoGrab;
#endif
    }

#if !defined(QT_NO_DEBUG) && defined(QT_HAVE_PROC_CMDLINE) && defined(QT_HAVE_PROC_EXE)
    if (!appNoGrab && !appDoGrab && runningUnderDebugger()) {
        appNoGrab = true;
        qDebug("Qt: gdb: -nograb added to command-line options.\n"
               "\t Use the -dograb option to enforce grabbing.");
    }
#endif

    // Connect to X server
    if (!qt_x11Data->display) {
        if ((qt_x11Data->display = XOpenDisplay(qt_x11Data->displayName)) == 0) {
            QApplication::exit(1);
            QByteArray appName = QApplication::applicationName().toLocal8Bit();
            qFatal("%s: cannot connect to X server %s", appName.constData(),
                     XDisplayName(qt_x11Data->displayName));
        }

        if (appSync)                                // if "-sync" argument
            XSynchronize(qt_x11Data->display, true);
    }

    // Common code, regardless of whether display is foreign.

    // Get X parameters
    qt_x11Data->defaultScreen = DefaultScreen(qt_x11Data->display);
    qt_x11Data->screenCount = ScreenCount(qt_x11Data->display);

    qt_x11Data->screens = new QX11InfoData[qt_x11Data->screenCount];
    qt_x11Data->argbVisuals = new Visual *[qt_x11Data->screenCount];
    qt_x11Data->argbColormaps = new Colormap[qt_x11Data->screenCount];

    for (int s = 0; s < qt_x11Data->screenCount; s++) {
        QX11InfoData *screen = qt_x11Data->screens + s;
        screen->ref = 1; // ensures it doesn't get deleted
        screen->screen = s;

        int widthMM = DisplayWidthMM(qt_x11Data->display, s);
        if (widthMM != 0) {
            screen->dpiX = (DisplayWidth(qt_x11Data->display, s) * 254 + widthMM * 5) / (widthMM * 10);
        } else {
            screen->dpiX = 72;
        }

        int heightMM = DisplayHeightMM(qt_x11Data->display, s);
        if (heightMM != 0) {
            screen->dpiY = (DisplayHeight(qt_x11Data->display, s) * 254 + heightMM * 5) / (heightMM * 10);
        } else {
            screen->dpiY = 72;
        }

        qt_x11Data->argbVisuals[s] = 0;
        qt_x11Data->argbColormaps[s] = 0;
    }


#ifndef QT_NO_XRENDER
    int xrender_eventbase,  xrender_errorbase;
    // See if XRender is supported on the connected display
    if (qgetenv("QT_X11_NO_XRENDER").isNull()
        && XQueryExtension(qt_x11Data->display, "RENDER", &qt_x11Data->xrender_major,
                        &xrender_eventbase, &xrender_errorbase)
        && XRenderQueryExtension(qt_x11Data->display, &xrender_eventbase,
                                    &xrender_errorbase)) {
        // Check the version as well - we need v0.4 or higher
        int xrender_major = 0;
        int xrender_minor = 0;
        XRenderQueryVersion(qt_x11Data->display, &xrender_major, &xrender_minor);
        qt_x11Data->use_xrender = (xrender_major >= 0 && xrender_minor >= 5);
        qt_x11Data->xrender_minor = xrender_minor;
    }
#endif // QT_NO_XRENDER

#ifndef QT_NO_XINERAMA
    int xinerama_eventbase;
    int xinerama_errorbase;
    if (qgetenv("QT_X11_NO_XINERAMA").isNull()) {
        qt_x11Data->use_xinerama = (XineramaQueryExtension(qt_x11Data->display,
                                                            &xinerama_eventbase, &xinerama_errorbase)
                                    && XineramaIsActive(qt_x11Data->display));
    }
#endif

    QColormap::initialize();

    // Support protocols
    qt_x11Data->xdndSetup();

    // Finally create all atoms
    qt_x11_create_intern_atoms();

    // initialize NET lists
    qt_get_net_supported();
    qt_get_net_virtual_roots();

#ifndef QT_NO_XRANDR
    // See if XRandR is supported on the connected display
    int xrandr_errorbase;
    if (qgetenv("QT_X11_NO_XRANDR").isNull()
        && XQueryExtension(qt_x11Data->display, "RANDR", &qt_x11Data->xrandr_major,
                           &qt_x11Data->xrandr_eventbase, &xrandr_errorbase)) {
        if (XRRQueryExtension(qt_x11Data->display, &qt_x11Data->xrandr_eventbase, &xrandr_errorbase)) {
            // XRandR is supported
            qt_x11Data->use_xrandr = true;
        }
    }
#endif // QT_NO_XRANDR

#ifndef QT_NO_XRENDER
    if (qt_x11Data->use_xrender) {
        // XRender is supported, let's see if we have a PictFormat for the
        // default visual
        XRenderPictFormat *format =
            XRenderFindVisualFormat(qt_x11Data->display,
                                    (Visual *) QX11Info::appVisual(qt_x11Data->defaultScreen));

        if (!format) {
            qt_x11Data->use_xrender = false;
        }
    }
#endif // QT_NO_XRENDER

#ifndef QT_NO_XFIXES
    // See if Xfixes is supported on the connected display
    int xfixes_major;
    int xfixes_errorbase;
    if (qgetenv("QT_X11_NO_XFIXES").isNull()
        && XQueryExtension(qt_x11Data->display, "XFIXES", &xfixes_major,
                           &qt_x11Data->xfixes_eventbase, &xfixes_errorbase)) {
        if(XFixesQueryExtension(qt_x11Data->display, &qt_x11Data->xfixes_eventbase,
                                &xfixes_errorbase)) {
            // Xfixes is supported.
            // Note: the XFixes protocol version is negotiated using QueryVersion.
            // We supply the highest version we support, the X server replies with
            // the highest version it supports, but no higher than the version we
            // asked for. The version sent back is the protocol version the X server
            // will use to talk us. If this call is removed, the behavior of the
            // X server when it receives an XFixes request is undefined.
            int xfixes_major = 3;
            int xfixes_minor = 0;
            XFixesQueryVersion(qt_x11Data->display, &xfixes_major, &xfixes_minor);
            qt_x11Data->use_xfixes = (xfixes_major >= 1);
        }
    }
#endif // QT_NO_XFIXES

#ifndef QT_NO_XSYNC
    int xsync_evbase;
    int xsync_errbase;
    int xsync_major;
    int xsync_minor;
    if (XSyncQueryExtension(qt_x11Data->display, &xsync_evbase, &xsync_errbase))
        XSyncInitialize(qt_x11Data->display, &xsync_major, &xsync_minor);
#endif // QT_NO_XSYNC

#if !defined(QT_NO_FONTCONFIG)
    int dpi = 0;
    getXDefault("Xft", FC_DPI, &dpi);
    if (dpi) {
        for (int s = 0; s < ScreenCount(qt_x11Data->display); ++s) {
            QX11Info::setAppDpiX(s, dpi);
            QX11Info::setAppDpiY(s, dpi);
        }
    }
    for (int s = 0; s < ScreenCount(qt_x11Data->display); ++s) {
        int subpixel = FC_RGBA_UNKNOWN;
#if !defined(QT_NO_XRENDER) && (RENDER_MAJOR > 0 || RENDER_MINOR >= 6)
        if (qt_x11Data->use_xrender) {
            int rsp = XRenderQuerySubpixelOrder(qt_x11Data->display, s);
            switch (rsp) {
            case SubPixelHorizontalRGB:
                subpixel = FC_RGBA_RGB;
                break;
            case SubPixelHorizontalBGR:
                subpixel = FC_RGBA_BGR;
                break;
            case SubPixelVerticalRGB:
                subpixel = FC_RGBA_VRGB;
                break;
            case SubPixelVerticalBGR:
                subpixel = FC_RGBA_VBGR;
                break;
            case SubPixelNone:
                subpixel = FC_RGBA_NONE;
                break;
            case SubPixelUnknown:
            default:
                subpixel = FC_RGBA_UNKNOWN;
                break;
            }
        }
#endif // QT_NO_XRENDER

        char *rgba = XGetDefault(qt_x11Data->display, "Xft", FC_RGBA);
        if (rgba) {
            char *end = 0;
            int v = strtol(rgba, &end, 0);
            if (rgba != end) {
                subpixel = v;
            } else if (qstrncmp(rgba, "unknown", 7) == 0) {
                subpixel = FC_RGBA_UNKNOWN;
            } else if (qstrncmp(rgba, "rgb", 3) == 0) {
                subpixel = FC_RGBA_RGB;
            } else if (qstrncmp(rgba, "bgr", 3) == 0) {
                subpixel = FC_RGBA_BGR;
            } else if (qstrncmp(rgba, "vrgb", 4) == 0) {
                subpixel = FC_RGBA_VRGB;
            } else if (qstrncmp(rgba, "vbgr", 4) == 0) {
                subpixel = FC_RGBA_VBGR;
            } else if (qstrncmp(rgba, "none", 4) == 0) {
                subpixel = FC_RGBA_NONE;
            }
        }
        qt_x11Data->screens[s].subpixel = subpixel;
    }
    getXDefault("Xft", FC_ANTIALIAS, &qt_x11Data->fc_antialias);
#ifdef FC_HINT_STYLE
    qt_x11Data->fc_hint_style = -1;
    getXDefault("Xft", FC_HINT_STYLE, &qt_x11Data->fc_hint_style);
#endif
#if 0
    // ###### these are implemented by Xft, not sure we need them
    getXDefault("Xft", FC_AUTOHINT, &qt_x11Data->fc_autohint);
    getXDefault("Xft", FC_HINTING, &qt_x11Data->fc_autohint);
    getXDefault("Xft", FC_MINSPACE, &qt_x11Data->fc_autohint);
#endif
#endif // QT_NO_FONTCONFIG

    // initialize key mapper
    QKeyMapper::changeKeyboard();

    // Misc. initialization
    QCursorData::initialize();
    QFont::initialize();

    qApp->setObjectName(qApp->applicationName());

    for (int screen = 0; screen < qt_x11Data->screenCount; ++screen) {
        XSelectInput(qt_x11Data->display, QX11Info::appRootWindow(screen),
                     KeymapStateMask | EnterWindowMask | LeaveWindowMask | PropertyChangeMask);

#ifndef QT_NO_XRANDR
        if (qt_x11Data->use_xrandr)
            XRRSelectInput(qt_x11Data->display, QX11Info::appRootWindow(screen), True);
#endif // QT_NO_XRANDR
    }

        // Attempt to determine if compositor is active
#ifndef QT_NO_XFIXES
    XFixesSelectSelectionInput(qt_x11Data->display, QX11Info::appRootWindow(), ATOM(_NET_WM_CM_S0),
                               XFixesSetSelectionOwnerNotifyMask
                               | XFixesSelectionWindowDestroyNotifyMask
                               | XFixesSelectionClientCloseNotifyMask);
#endif // QT_NO_XFIXES
    qt_x11Data->compositingManagerRunning = XGetSelectionOwner(qt_x11Data->display,
                                                               ATOM(_NET_WM_CM_S0));

    QApplicationPrivate::x11_apply_settings();

    // be smart about the size of the default font. most X servers have helvetica
    // 12 point available at 2 resolutions:
    //     75dpi (12 pixels) and 100dpi (17 pixels).
    // At 95 DPI, a 12 point font should be 16 pixels tall - in which case a 17
    // pixel font is a closer match than a 12 pixel font
    int ptsz = (qt_x11Data->use_xrender
                ? 9
                : (int) (((QX11Info::appDpiY() >= 95 ? 17. : 12.) *
                            72. / (float) QX11Info::appDpiY()) + 0.5));

    if (!QApplicationPrivate::sys_font) {
        // no font from settings, provide a fallback
        QFont f(qt_x11Data->has_fontconfig ? QLatin1String("Sans Serif") : QLatin1String("Helvetica"),
                ptsz);
        QApplicationPrivate::setSystemFont(f);
    }

    qt_x11Data->startupId = getenv("DESKTOP_STARTUP_ID");
    if (qt_x11Data->startupId) {
        ::unsetenv("DESKTOP_STARTUP_ID");
    }
}

/*****************************************************************************
  qt_cleanup() - cleans up when the application is finished
 *****************************************************************************/

void qt_cleanup()
{
    QPixmapCache::clear();
    QCursorData::cleanup();
    QFont::cleanup();
    QColormap::cleanup();

#ifndef QT_NO_XRENDER
    for (int i = 0; i < qt_x11Data->solid_fill_count; ++i) {
        if (qt_x11Data->solid_fills[i].picture)
            XRenderFreePicture(qt_x11Data->display, qt_x11Data->solid_fills[i].picture);
    }
    for (int i = 0; i < qt_x11Data->pattern_fill_count; ++i) {
        if (qt_x11Data->pattern_fills[i].picture)
            XRenderFreePicture(qt_x11Data->display, qt_x11Data->pattern_fills[i].picture);
    }
#endif

    // Reset the error handlers
    XSync(qt_x11Data->display, False); // sync first to process all possible errors
    XSetErrorHandler(original_x_errhandler);
    XSetIOErrorHandler(original_xio_errhandler);

    if (qt_x11Data->argbColormaps) {
        for (int s = 0; s < qt_x11Data->screenCount; s++) {
            if (qt_x11Data->argbColormaps[s])
                XFreeColormap(qt_x11Data->display, qt_x11Data->argbColormaps[s]);
        }
    }

    if (!qt_x11Data->foreignDisplay)
        XCloseDisplay(qt_x11Data->display); // close X display
    qt_x11Data->display = 0;

    delete [] qt_x11Data->screens;
    delete [] qt_x11Data->argbVisuals;
    delete [] qt_x11Data->argbColormaps;

    if (qt_x11Data->net_supported_list)
        delete [] qt_x11Data->net_supported_list;
    qt_x11Data->net_supported_list = 0;

    if (qt_x11Data->net_virtual_root_list)
        delete [] qt_x11Data->net_virtual_root_list;
    qt_x11Data->net_virtual_root_list = 0;

    delete qt_x11Data;
    qt_x11Data = 0;
}


/*****************************************************************************
  Global and internal functions
 *****************************************************************************/

bool qt_nograb()                                // application no-grab option
{
#ifndef QT_NO_DEBUG
    return appNoGrab;
#else
    return false;
#endif
}


/*****************************************************************************
  Platform specific QApplication members
 *****************************************************************************/


void QApplicationPrivate::applyX11SpecificCommandLineArguments(QWidget *main_widget)
{
    static bool beenHereDoneThat = false;
    if (beenHereDoneThat)
        return;
    beenHereDoneThat = true;
    Q_ASSERT(main_widget->testAttribute(Qt::WA_WState_Created));
    if (mwTitle) {
        XStoreName(qt_x11Data->display, main_widget->effectiveWinId(), (char*)mwTitle);
        QByteArray net_wm_name = QString::fromLocal8Bit(mwTitle).toUtf8();
        XChangeProperty(qt_x11Data->display, main_widget->effectiveWinId(), ATOM(_NET_WM_NAME), ATOM(UTF8_STRING), 8,
                        PropModeReplace, (unsigned char *)net_wm_name.data(), net_wm_name.size());
    }
    if (mwGeometry) { // parse geometry
        int x, y;
        int w, h;
        int m = XParseGeometry((char*)mwGeometry, &x, &y, (uint*)&w, (uint*)&h);
        QSize minSize = main_widget->minimumSize();
        QSize maxSize = main_widget->maximumSize();
        if ((m & XValue) == 0)
            x = main_widget->geometry().x();
        if ((m & YValue) == 0)
            y = main_widget->geometry().y();
        if ((m & WidthValue) == 0)
            w = main_widget->width();
        if ((m & HeightValue) == 0)
            h = main_widget->height();
        w = qMin(w,maxSize.width());
        h = qMin(h,maxSize.height());
        w = qMax(w,minSize.width());
        h = qMax(h,minSize.height());
        if ((m & XNegative)) {
            x = QApplication::desktop()->width()  + x - w;
        }
        if ((m & YNegative)) {
            y = QApplication::desktop()->height() + y - h;
        }
        main_widget->setGeometry(x, y, w, h);
    }
}

#ifndef QT_NO_CURSOR

/*****************************************************************************
  QApplication cursor stack
 *****************************************************************************/

void QApplication::setOverrideCursor(const QCursor &cursor)
{
    qApp->d_func()->cursor_list.prepend(cursor);

    foreach (QWidget *it, allWidgets()) {
        if ((it->testAttribute(Qt::WA_SetCursor) || it->isWindow()) && (it->windowType() != Qt::Desktop))
            qt_x11_enforce_cursor(it, false);
    }
    XFlush(qt_x11Data->display);                                // make X execute it NOW
}

void QApplication::restoreOverrideCursor()
{
    if (qApp->d_func()->cursor_list.isEmpty())
        return;
    qApp->d_func()->cursor_list.removeFirst();

    if (QWidgetPrivate::mapper != 0 && !closingDown()) {
        foreach (QWidget *it, allWidgets()) {
            if ((it->testAttribute(Qt::WA_SetCursor) || it->isWindow()) && (it->windowType() != Qt::Desktop))
                qt_x11_enforce_cursor(it, false);
        }
        XFlush(qt_x11Data->display);
    }
}

#endif


/*****************************************************************************
  Routines to find a Qt widget from a screen position
 *****************************************************************************/

Window QX11Data::findClientWindow(Window win, Atom property)
{
    Atom   type = XNone;
    int    format;
    ulong  nitems, after;
    uchar *data = 0;
    Window root, parent, target=0, *children=0;
    uint   nchildren;
    if (XGetWindowProperty(qt_x11Data->display, win, property, 0, 0, false, AnyPropertyType,
                             &type, &format, &nitems, &after, &data) == Success) {
        if (data)
            XFree((char *)data);
        if (type)
            return win;
    }
    if (!XQueryTree(qt_x11Data->display,win,&root,&parent,&children,&nchildren)) {
        if (children)
            XFree((char *)children);
        return 0;
    }
    for (int i=nchildren-1; !target && i >= 0; i--)
        target = qt_x11Data->findClientWindow(children[i], property);
    if (children)
        XFree((char *)children);
    return target;
}

QWidget *QApplication::topLevelAt(const QPoint &p)
{
#ifdef QT_NO_CURSOR
    Q_UNUSED(p);
    return 0;
#else
    int screen = QCursor::x11Screen();
    int unused;

    int x = p.x();
    int y = p.y();
    Window target;
    if (!XTranslateCoordinates(qt_x11Data->display,
                               QX11Info::appRootWindow(screen),
                               QX11Info::appRootWindow(screen),
                               x, y, &unused, &unused, &target)) {
        return 0;
    }
    if (!target || target == QX11Info::appRootWindow(screen))
        return 0;
    QWidget *w = QWidget::find((WId)target);

    if (!w) {
        qt_x11Data->ignoreBadwindow();
        target = qt_x11Data->findClientWindow(target, ATOM(WM_STATE));
        if (qt_x11Data->badwindow())
            return 0;
        w = QWidget::find((WId)target);
        if (!w) {
            // Perhaps the widget at (x,y) is inside a foreign application?
            // Search all toplevel widgets to see if one is within target
            foreach (QWidget *widget, QApplication::topLevelWidgets()) {
                Window ctarget = target;
                if (widget->isVisible() && !(widget->windowType() == Qt::Desktop)) {
                    Q_ASSERT(widget->testAttribute(Qt::WA_WState_Created));
                    Window wid = widget->internalWinId();
                    while (ctarget && !w) {
                        qt_x11Data->ignoreBadwindow();
                        if (!XTranslateCoordinates(qt_x11Data->display,
                                                   QX11Info::appRootWindow(screen),
                                                   ctarget, x, y, &unused, &unused, &ctarget)
                                || qt_x11Data->badwindow())
                            break;
                        if (ctarget == wid) {
                            // Found!
                            w = widget;
                            break;
                        }
                    }
                }
                if (w)
                    break;
            }
        }
    }
    return w ? w->window() : 0;
#endif
}

void QApplication::syncX()
{
    if (qt_x11Data->display)
        XSync(qt_x11Data->display, False);  // don't discard events
}


void QApplication::beep()
{
    if (qt_x11Data->display)
        XBell(qt_x11Data->display, 0);
    else
        printf("\7");
}

void QApplication::alert(QWidget *widget, int msec)
{
    if (!QApplicationPrivate::checkInstance("alert"))
        return;

    QWidgetList windowsToMark;
    if (!widget) {
        windowsToMark += topLevelWidgets();
    } else {
        windowsToMark.append(widget->window());
    }

    foreach (QWidget *window, windowsToMark) {
        if (!window->isActiveWindow()) {
            qt_change_net_wm_state(window, true, ATOM(_NET_WM_STATE_DEMANDS_ATTENTION), 0);
            if (msec != 0) {
                QTimer *timer = new QTimer(qApp);
                timer->setSingleShot(true);
                connect(timer, SIGNAL(timeout()), qApp, SLOT(_q_alertTimeOut()));
                if (QTimer *oldTimer = qApp->d_func()->alertTimerHash.value(window)) {
                    qApp->d_func()->alertTimerHash.remove(window);
                    delete oldTimer;
                }
                qApp->d_func()->alertTimerHash.insert(window, timer);
                timer->start(msec);
            }
        }
    }
}

void QApplicationPrivate::_q_alertTimeOut()
{
    if (QTimer *timer = qobject_cast<QTimer *>(q_func()->sender())) {
        QHash<QWidget *, QTimer *>::iterator it = alertTimerHash.begin();
        while (it != alertTimerHash.end()) {
            if (it.value() == timer) {
                QWidget *window = it.key();
                qt_change_net_wm_state(window, false, ATOM(_NET_WM_STATE_DEMANDS_ATTENTION), 0);
                alertTimerHash.erase(it);
                timer->deleteLater();
                break;
            }
            ++it;
        }
    }
}

Qt::KeyboardModifiers QApplication::queryKeyboardModifiers()
{
    Window root;
    Window child;
    int root_x, root_y, win_x, win_y;
    uint keybstate;
    for (int i = 0; i < ScreenCount(qt_x11Data->display); ++i) {
        if (XQueryPointer(qt_x11Data->display, QX11Info::appRootWindow(i), &root, &child,
                          &root_x, &root_y, &win_x, &win_y, &keybstate))
            return qt_keymapper()->translateModifiers(keybstate & 0x00ff);
    }
    return 0;

}

/*****************************************************************************
  Special lookup functions for windows that have been reparented recently
 *****************************************************************************/

static QWidgetMapper *wPRmapper = 0;                // alternative widget mapper

void qPRCreate(QWidget *widget, Window oldwin)
{                                                // QWidget::reparent mechanism
    if (!wPRmapper)
        wPRmapper = new QWidgetMapper;

    QETWidget *w = static_cast<QETWidget *>(widget);
    wPRmapper->insert((int)oldwin, w);        // add old window to mapper
    w->setAttribute(Qt::WA_WState_Reparented);        // set reparented flag
}

void qPRCleanup(QWidget *widget)
{
    QETWidget *etw = static_cast<QETWidget *>(widget);
    if (!(wPRmapper && widget->testAttribute(Qt::WA_WState_Reparented)))
        return;                                        // not a reparented widget
    QWidgetMapper::iterator it = wPRmapper->begin();
    while (it != wPRmapper->end()) {
        QWidget *w = *it;
        if (w == etw) {                       // found widget
            etw->setAttribute(Qt::WA_WState_Reparented, false); // clear flag
            it = wPRmapper->erase(it);// old window no longer needed
        } else {
            ++it;
        }
    }
    if (wPRmapper->size() == 0) {        // became empty
        delete wPRmapper;                // then reset alt mapper
        wPRmapper = 0;
    }
}

static QETWidget *qPRFindWidget(Window oldwin)
{
    return wPRmapper ? (QETWidget*)wPRmapper->value((int)oldwin, 0) : 0;
}

int QApplication::x11ClientMessage(QWidget* w, XEvent* event, bool passive_only)
{
    if (w && !w->internalWinId())
        return 0;
    QETWidget *widget = (QETWidget*)w;
    if (event->xclient.format == 32 && event->xclient.message_type) {
        if (event->xclient.message_type == ATOM(WM_PROTOCOLS)) {
            Atom a = event->xclient.data.l[0];
            if (a == ATOM(WM_DELETE_WINDOW)) {
                if (passive_only) return 0;
                widget->translateCloseEvent(event);
            }
            else if (a == ATOM(WM_TAKE_FOCUS)) {
                if ((ulong) event->xclient.data.l[1] > qt_x11Data->time)
                    qt_x11Data->time = event->xclient.data.l[1];
                QWidget *amw = activeModalWidget();
                if (amw && amw->testAttribute(Qt::WA_X11DoNotAcceptFocus))
                    amw = 0;
                if (amw && !QApplicationPrivate::tryModalHelper(widget)) {
                    QWidget *p = amw->parentWidget();
                    while (p && p != widget)
                        p = p->parentWidget();
                    if (!p || !qt_x11Data->net_supported_list)
                        amw->raise(); // help broken window managers
                    amw->activateWindow();
                }
#ifndef QT_NO_WHATSTHIS
            } else if (a == ATOM(_NET_WM_CONTEXT_HELP)) {
                QWhatsThis::enterWhatsThisMode();
#endif // QT_NO_WHATSTHIS
            } else if (a == ATOM(_NET_WM_PING)) {
                // avoid send/reply loops
                Window root = RootWindow(qt_x11Data->display, w->x11Info().screen());
                if (event->xclient.window != root) {
                    event->xclient.window = root;
                    XSendEvent(event->xclient.display, event->xclient.window,
                                False, SubstructureNotifyMask|SubstructureRedirectMask, event);
                }
#ifndef QT_NO_XSYNC
            } else if (a == ATOM(_NET_WM_SYNC_REQUEST)) {
                const Time timestamp = event->xclient.data.l[1];
                if (timestamp > qt_x11Data->time)
                    qt_x11Data->time = timestamp;
                if (QTLWExtra *tlw = w->d_func()->maybeTopData()) {
                    if (timestamp == CurrentTime || timestamp > tlw->syncRequestTimestamp) {
                        tlw->syncRequestTimestamp = timestamp;
                        tlw->newCounterValueLo = event->xclient.data.l[2];
                        tlw->newCounterValueHi = event->xclient.data.l[3];
                    }
                }
#endif
            }
        } else if (event->xclient.message_type == ATOM(_QT_SCROLL_DONE)) {
            widget->translateScrollDoneEvent(event);
        } else if (event->xclient.message_type == ATOM(XdndPosition)) {
            qt_x11Data->xdndHandlePosition(widget, event, passive_only);
        } else if (event->xclient.message_type == ATOM(XdndEnter)) {
            qt_x11Data->xdndHandleEnter(event);
        } else if (event->xclient.message_type == ATOM(XdndStatus)) {
            qt_x11Data->xdndHandleStatus(event);
        } else if (event->xclient.message_type == ATOM(XdndLeave)) {
            qt_x11Data->xdndHandleLeave(widget, event);
        } else if (event->xclient.message_type == ATOM(XdndDrop)) {
            qt_x11Data->xdndHandleDrop(event, passive_only);
        } else if (event->xclient.message_type == ATOM(XdndFinished)) {
            qt_x11Data->xdndHandleFinished(event, passive_only);
        }
        // All other are interactions
    }

    return 0;
}

static bool qt_try_modal(QWidget *widget, XEvent *event)
{
    if (qt_xdnd_dragging) {
        // allow mouse events while DnD is active
        switch (event->type) {
        case XButtonPress:
        case XButtonRelease:
        case MotionNotify:
            return true;
        default:
            break;
        }
    }

    // allow mouse release events to be sent to widgets that have been pressed
    if (event->type == XButtonRelease) {
        QWidget *alienWidget = widget->childAt(widget->mapFromGlobal(QPoint(event->xbutton.x_root,
                                                                            event->xbutton.y_root)));
        if (widget == qt_button_down || (alienWidget && alienWidget == qt_button_down))
            return true;
    }

    if (QApplicationPrivate::tryModalHelper(widget))
        return true;

    // disallow mouse/key events
    switch (event->type) {
    case XButtonPress:
    case XButtonRelease:
    case MotionNotify:
    case XKeyPress:
    case XKeyRelease:
    case EnterNotify:
    case LeaveNotify:
    case ClientMessage:
        return false;
    default:
        break;
    }

    return true;
}

int QApplication::x11ProcessEvent(XEvent* event)
{
    Q_D(QApplication);
    QScopedLoopLevelCounter loopLevelCounter(d->threadData);

#ifdef ALIEN_DEBUG
    //qDebug() << "QApplication::x11ProcessEvent:" << event->type;
#endif
    switch (event->type) {
    case XButtonPress:
        pressed_window = event->xbutton.window;
        qt_x11Data->userTime = event->xbutton.time;
        // fallthrough intended
    case XButtonRelease:
        qt_x11Data->time = event->xbutton.time;
        break;
    case MotionNotify:
        qt_x11Data->time = event->xmotion.time;
        break;
    case XKeyPress:
        qt_x11Data->userTime = event->xkey.time;
        // fallthrough intended
    case XKeyRelease:
        qt_x11Data->time = event->xkey.time;
        break;
    case PropertyNotify:
        qt_x11Data->time = event->xproperty.time;
        break;
    case EnterNotify:
    case LeaveNotify:
        qt_x11Data->time = event->xcrossing.time;
        break;
    case SelectionClear:
        qt_x11Data->time = event->xselectionclear.time;
        break;
    default:
        break;
    }
#ifndef QT_NO_XFIXES
    if (qt_x11Data->use_xfixes && event->type == (qt_x11Data->xfixes_eventbase + XFixesSelectionNotify)) {
        XFixesSelectionNotifyEvent *req =
            reinterpret_cast<XFixesSelectionNotifyEvent *>(event);
        qt_x11Data->time = req->selection_timestamp;
        if (req->selection == ATOM(_NET_WM_CM_S0))
            qt_x11Data->compositingManagerRunning = req->owner;
    }
#endif

    QETWidget *widget = (QETWidget*)QWidget::find((WId)event->xany.window);

    if (wPRmapper) {                                // just did a widget reparent?
        if (widget == 0) {                        // not in std widget mapper
            switch (event->type) {                // only for mouse/key events
            case XButtonPress:
            case XButtonRelease:
            case MotionNotify:
            case XKeyPress:
            case XKeyRelease:
                widget = qPRFindWidget(event->xany.window);
                break;
            }
        }
        else if (widget->testAttribute(Qt::WA_WState_Reparented))
            qPRCleanup(widget);                // remove from alt mapper
    }

    QETWidget *keywidget=0;
    if (event->type==XKeyPress || event->type==XKeyRelease) {
        keywidget = (QETWidget*)QWidget::keyboardGrabber();
        if (!keywidget) {
            if (d->inPopupMode()) // no focus widget, see if we have a popup
                keywidget = (QETWidget*) (activePopupWidget()->focusWidget() ? activePopupWidget()->focusWidget() : activePopupWidget());
            else if (QApplicationPrivate::focus_widget)
                keywidget = (QETWidget*)QApplicationPrivate::focus_widget;
            else if (widget)
                keywidget = (QETWidget*)widget->window();
        }
    }

    if (XFilterEvent(event, XNone) || qt_x11EventFilter(event)) // send through event filters
        return 1;

    if (event->type == MappingNotify) {
        // keyboard mapping changed
        XRefreshKeyboardMapping(&event->xmapping);

        QKeyMapper::changeKeyboard();
        return 0;
    }

    if (!widget) {                                // don't know this windows
        QWidget* popup = QApplication::activePopupWidget();
        if (popup) {

            /*
              That is more than suboptimal. The real solution should
              do some keyevent and buttonevent translation, so that
              the popup still continues to work as the user expects.
              Unfortunately this translation is currently only
              possible with a known widget. I'll change that soon
              (Matthias).
            */

            // Danger - make sure we don't lock the server
            switch (event->type) {
            case XButtonPress:
            case XButtonRelease:
            case XKeyPress:
            case XKeyRelease:
                do {
                    popup->close();
                } while ((popup = qApp->activePopupWidget()));
                return 1;
            }
        }
        return -1;
    }

    if (event->type == XKeyPress || event->type == XKeyRelease)
        widget = keywidget; // send XKeyEvents through keywidget->x11Event()

    if (app_do_modal)                                // modal event handling
        if (!qt_try_modal(widget, event)) {
            if (event->type == ClientMessage && !widget->x11Event(event))
                x11ClientMessage(widget, event, true);
            return 1;
        }


    if (widget->x11Event(event))                // send through widget filter
        return 1;

#ifndef QT_NO_XRANDR
    if (qt_x11Data->use_xrandr && event->type == (qt_x11Data->xrandr_eventbase + RRScreenChangeNotify)) {
        // update Xlib internals with the latest screen configuration
        XRRUpdateConfiguration(event);

        // update the size for desktop widget
        int scr = XRRRootToScreen(qt_x11Data->display, event->xany.window);
        QDesktopWidget *desktop = QApplication::desktop();
        QWidget *w = desktop->screen(scr);
        QSize oldSize(w->size());
        w->data->crect.setWidth(DisplayWidth(qt_x11Data->display, scr));
        w->data->crect.setHeight(DisplayHeight(qt_x11Data->display, scr));
        QResizeEvent e(w->size(), oldSize);
        QApplication::sendEvent(w, &e);
        if (w != desktop)
            QApplication::sendEvent(desktop, &e);
    }
#endif // QT_NO_XRANDR

#ifndef QT_NO_XFIXES
    if (qt_x11Data->use_xfixes && event->type == (qt_x11Data->xfixes_eventbase + XFixesSelectionNotify)) {
        XFixesSelectionNotifyEvent *req = reinterpret_cast<XFixesSelectionNotifyEvent *>(event);

        // compress all XFixes events related to this selection
        // we don't want to handle old SelectionNotify events.
        qt_xfixes_selection_event_data xfixes_event;
        xfixes_event.selection = req->selection;
        for (XEvent ev;;) {
            if (!XCheckIfEvent(qt_x11Data->display, &ev, &qt_xfixes_scanner, (XPointer)&xfixes_event))
                break;
        }

        if (req->selection == ATOM(CLIPBOARD)) {
            if (qt_xfixes_clipboard_changed(req->owner, req->selection_timestamp)) {
                emit clipboard()->changed(QClipboard::Clipboard);
                emit clipboard()->dataChanged();
            }
        } else if (req->selection == XA_PRIMARY) {
            if (qt_xfixes_selection_changed(req->owner, req->selection_timestamp)) {
                emit clipboard()->changed(QClipboard::Selection);
                emit clipboard()->selectionChanged();
            }
        }
    }
#endif // QT_NO_XFIXES

    switch (event->type) {

    case XButtonRelease:                        // mouse event
        if (!d->inPopupMode() && !QWidget::mouseGrabber() && pressed_window != widget->internalWinId()
            && (widget = (QETWidget*) QWidget::find((WId)pressed_window)) == 0)
            break;
        // fall through intended
    case XButtonPress:
        if (event->xbutton.root != RootWindow(qt_x11Data->display, widget->x11Info().screen())
            && ! qt_xdnd_dragging) {
            while (activePopupWidget())
                activePopupWidget()->close();
            return 1;
        }
        if (event->type == XButtonPress)
            qt_net_update_user_time(widget->window(), qt_x11Data->userTime);
        // fall through intended
    case MotionNotify:
        if (widget->testAttribute(Qt::WA_TransparentForMouseEvents)) {
            QPoint pos(event->xbutton.x, event->xbutton.y);
            pos = widget->d_func()->mapFromWS(pos);
            QWidget *window = widget->window();
            pos = widget->mapTo(window, pos);
            if (QWidget *child = window->childAt(pos)) {
                widget = static_cast<QETWidget *>(child);
                pos = child->mapFrom(window, pos);
                event->xbutton.x = pos.x();
                event->xbutton.y = pos.y();
            }
        }
        widget->translateMouseEvent(event);
        break;

    case XKeyPress:                                // keyboard event
        qt_net_update_user_time(widget->window(), qt_x11Data->userTime);
        // fallthrough intended
    case XKeyRelease: {
        if (keywidget && keywidget->isEnabled()) { // should always exist
            // qDebug("sending key event");
            qt_keymapper()->translateKeyEvent(keywidget, event);
        }
        break;
    }

    case GraphicsExpose:
    case Expose:                                // paint event
        widget->translatePaintEvent(event);
        break;

    case ConfigureNotify:                        // window move/resize event
        if (event->xconfigure.event == event->xconfigure.window)
            widget->translateConfigEvent(event);
        break;

    case XFocusIn: {                                // got focus
        if ((widget->windowType() == Qt::Desktop))
            break;
        if (d->inPopupMode()) // some delayed focus event to ignore
            break;
        if (!widget->isWindow())
            break;
        if (event->xfocus.detail != NotifyAncestor &&
            event->xfocus.detail != NotifyInferior &&
            event->xfocus.detail != NotifyNonlinear)
            break;
        setActiveWindow(widget);
        if (qt_x11Data->focus_model == QX11Data::FM_PointerRoot) {
            // We got real input focus from somewhere, but we were in PointerRoot
            // mode, so we don't trust this event.  Check the focus model to make
            // sure we know what focus mode we are using...
            qt_check_focus_model();
        }
        break;
    }

    case XFocusOut:                                // lost focus
        if ((widget->windowType() == Qt::Desktop))
            break;
        if (!widget->isWindow())
            break;
        if (event->xfocus.mode == NotifyGrab) {
            break;
        }
        if (event->xfocus.detail != NotifyAncestor &&
            event->xfocus.detail != NotifyNonlinearVirtual &&
            event->xfocus.detail != NotifyNonlinear)
            break;
        if (!d->inPopupMode() && widget == QApplicationPrivate::active_window) {
            XEvent ev;
            bool focus_will_change = false;
            if (XCheckTypedEvent(qt_x11Data->display, XFocusIn, &ev)) {
                // we're about to get an XFocusIn, if we know we will
                // get a new active window, we don't want to set the
                // active window to 0 now
                QWidget *w2 = QWidget::find(ev.xany.window);
                if (w2
                    && w2->windowType() != Qt::Desktop
                    && !d->inPopupMode() // some delayed focus event to ignore
                    && w2->isWindow()
                    && (ev.xfocus.detail == NotifyAncestor
                        || ev.xfocus.detail == NotifyInferior
                        || ev.xfocus.detail == NotifyNonlinear))
                    focus_will_change = true;

                XPutBackEvent(qt_x11Data->display, &ev);
            }
            if (!focus_will_change)
                setActiveWindow(0);
        }
        break;

    case EnterNotify: {                        // enter window
        if (QWidget::mouseGrabber() && (!d->inPopupMode() || widget->window() != activePopupWidget()))
            break;
        if ((event->xcrossing.mode != NotifyNormal
             && event->xcrossing.mode != NotifyUngrab)
            || event->xcrossing.detail == NotifyVirtual
            || event->xcrossing.detail == NotifyNonlinearVirtual)
            break;
        if (event->xcrossing.focus &&
            !(widget->windowType() == Qt::Desktop) && !widget->isActiveWindow()) {
            if (qt_x11Data->focus_model == QX11Data::FM_Unknown) // check focus model
                qt_check_focus_model();
            if (qt_x11Data->focus_model == QX11Data::FM_PointerRoot) // PointerRoot mode
                setActiveWindow(widget);
        }

        if (qt_button_down && !d->inPopupMode())
            break;

        QWidget *alien = widget->childAt(widget->d_func()->mapFromWS(QPoint(event->xcrossing.x,
                                                                            event->xcrossing.y)));
        QWidget *enter = alien ? alien : widget;
        QWidget *leave = 0;
        if (qt_last_mouse_receiver && !qt_last_mouse_receiver->internalWinId())
            leave = qt_last_mouse_receiver;
        else
            leave = QWidget::find(curWin);

        // ### Alien: enter/leave might be wrong here with overlapping siblings
        // if the enter widget is native and stacked under a non-native widget.
        QApplicationPrivate::dispatchEnterLeave(enter, leave);
        curWin = widget->internalWinId();
        qt_last_mouse_receiver = enter;
        if (!d->inPopupMode() || widget->window() == activePopupWidget())
            widget->translateMouseEvent(event); //we don't get MotionNotify, emulate it
        break;
    }
    case LeaveNotify: {                        // leave window
        QWidget *mouseGrabber = QWidget::mouseGrabber();
        if (mouseGrabber && !d->inPopupMode())
            break;
        if (curWin && widget->internalWinId() != curWin)
            break;
        if ((event->xcrossing.mode != NotifyNormal
            && event->xcrossing.mode != NotifyUngrab)
            || event->xcrossing.detail == NotifyInferior)
            break;
        if (!(widget->windowType() == Qt::Desktop))
            widget->translateMouseEvent(event); //we don't get MotionNotify, emulate it

        QWidget* enter = 0;
        QPoint enterPoint;
        XEvent ev;
        while (XCheckMaskEvent(qt_x11Data->display, EnterWindowMask | LeaveWindowMask , &ev)
               && !qt_x11EventFilter(&ev)) {
            QWidget* event_widget = QWidget::find(ev.xcrossing.window);
            if(event_widget && event_widget->x11Event(&ev))
                break;
            if (ev.type == LeaveNotify
                || (ev.xcrossing.mode != NotifyNormal
                    && ev.xcrossing.mode != NotifyUngrab)
                || ev.xcrossing.detail == NotifyVirtual
                || ev.xcrossing.detail == NotifyNonlinearVirtual)
                continue;
            enter = event_widget;
            if (enter)
                enterPoint = enter->d_func()->mapFromWS(QPoint(ev.xcrossing.x, ev.xcrossing.y));
            if (ev.xcrossing.focus &&
                enter && !(enter->windowType() == Qt::Desktop) && !enter->isActiveWindow()) {
                if (qt_x11Data->focus_model == QX11Data::FM_Unknown) // check focus model
                    qt_check_focus_model();
                if (qt_x11Data->focus_model == QX11Data::FM_PointerRoot) // PointerRoot mode
                    setActiveWindow(enter);
            }
            break;
        }

        if ((! enter || (enter->windowType() == Qt::Desktop)) &&
            event->xcrossing.focus && widget == QApplicationPrivate::active_window &&
            qt_x11Data->focus_model == QX11Data::FM_PointerRoot // PointerRoot mode
            ) {
            setActiveWindow(0);
        }

        if (qt_button_down && !d->inPopupMode())
            break;

        if (!curWin)
            QApplicationPrivate::dispatchEnterLeave(widget, 0);

        if (enter) {
            QWidget *alienEnter = enter->childAt(enterPoint);
            if (alienEnter)
                enter = alienEnter;
        }

        QWidget *leave = qt_last_mouse_receiver ? qt_last_mouse_receiver : widget;
        QWidget *activePopupWidget = qApp->activePopupWidget();

        if (mouseGrabber && activePopupWidget && leave == activePopupWidget)
            enter = mouseGrabber;
        else if (enter != widget && mouseGrabber) {
            if (!widget->rect().contains(widget->d_func()->mapFromWS(QPoint(event->xcrossing.x,
                                                                            event->xcrossing.y))))
                break;
        }

        QApplicationPrivate::dispatchEnterLeave(enter, leave);
        qt_last_mouse_receiver = enter;

        if (enter && QApplicationPrivate::tryModalHelper(enter)) {
            QWidget *nativeEnter = enter->internalWinId() ? enter : enter->nativeParentWidget();
            curWin = nativeEnter->internalWinId();
            static_cast<QETWidget *>(nativeEnter)->translateMouseEvent(&ev); //we don't get MotionNotify, emulate it
        } else {
            curWin = 0;
            qt_last_mouse_receiver = 0;
        }
        break;
    }

    case UnmapNotify:                                // window hidden
        if (widget->isWindow()) {
            Q_ASSERT(widget->testAttribute(Qt::WA_WState_Created));
            widget->d_func()->topData()->waitingForMapNotify = false;

            if (widget->windowType() != Qt::Popup && !widget->testAttribute(Qt::WA_DontShowOnScreen)) {
                widget->setAttribute(Qt::WA_Mapped, false);
                if (widget->isVisible()) {
                    widget->d_func()->topData()->spont_unmapped = true;
                    QHideEvent e;
                    QApplication::sendSpontaneousEvent(widget, &e);
                    widget->d_func()->hideChildren(true);
                }
            }

            if (!widget->d_func()->topData()->validWMState && qt_x11Data->deferred_map.removeAll(widget))
                widget->doDeferredMap();
        }
        break;

    case MapNotify:                                // window shown
        if (widget->isWindow()) {
            // if we got a MapNotify when we were not waiting for it, it most
            // likely means the user has already asked to hide the window before
            // it ever being shown, so we try to withdraw a window after sending
            // the QShowEvent.
            bool pendingHide = widget->testAttribute(Qt::WA_WState_ExplicitShowHide) && widget->testAttribute(Qt::WA_WState_Hidden);
            widget->d_func()->topData()->waitingForMapNotify = false;

            if (widget->windowType() != Qt::Popup) {
                widget->setAttribute(Qt::WA_Mapped);
                if (widget->d_func()->topData()->spont_unmapped) {
                    widget->d_func()->topData()->spont_unmapped = false;
                    widget->d_func()->showChildren(true);
                    QShowEvent e;
                    QApplication::sendSpontaneousEvent(widget, &e);

                    // show() must have been called on this widget in
                    // order to reach this point, but we could have
                    // cleared these 2 attributes in case something
                    // previously forced us into WithdrawnState
                    // (e.g. kdocker)
                    widget->setAttribute(Qt::WA_WState_ExplicitShowHide, true);
                    widget->setAttribute(Qt::WA_WState_Visible, true);
                }
            }
            if (pendingHide) // hide the window
                XWithdrawWindow(qt_x11Data->display, widget->internalWinId(), widget->x11Info().screen());
        }
        break;

    case ClientMessage:                        // client message
        return x11ClientMessage(widget, event, false);

    case ReparentNotify: {                      // window manager reparents
        // compress old reparent events to self
        XEvent ev;
        while (XCheckTypedWindowEvent(qt_x11Data->display,
                                      widget->effectiveWinId(),
                                      ReparentNotify,
                                      &ev)) {
            if (ev.xreparent.window != ev.xreparent.event) {
                XPutBackEvent(qt_x11Data->display, &ev);
                break;
            }
        }
        if (widget->isWindow()) {
            QTLWExtra *topData = widget->d_func()->topData();

            // store the parent. Useful for many things, embedding for instance.
            topData->parentWinId = event->xreparent.parent;

            // the widget frame strut should also be invalidated
            widget->data->fstrut_dirty = true;

            // work around broken window managers... if we get a
            // ReparentNotify before the MapNotify, we assume that
            // we're being managed by a reparenting window
            // manager.
            //
            // however, the WM_STATE property may not have been set
            // yet, but we are going to assume that it will
            // be... otherwise we could try to map again after getting
            // an UnmapNotify... which could then, in turn, trigger a
            // race in the window manager which causes the window to
            // disappear when it really should be hidden.
            if (topData->waitingForMapNotify && !topData->validWMState) {
                topData->waitingForMapNotify = false;
                topData->validWMState = true;
            }

            if (qt_x11Data->focus_model != QX11Data::FM_Unknown) {
                // toplevel reparented...
                QWidget *newparent = QWidget::find(event->xreparent.parent);
                if (! newparent || (newparent->windowType() == Qt::Desktop)) {
                    // we don't know about the new parent (or we've been
                    // reparented to root), perhaps a window manager
                    // has been (re)started?  reset the focus model to unknown
                    qt_x11Data->focus_model = QX11Data::FM_Unknown;
                }
            }
        }
        break;
    }
    case SelectionRequest: {
        XSelectionRequestEvent *req = &event->xselectionrequest;
        if (! req)
            break;

        if (ATOM(XdndSelection) && req->selection == ATOM(XdndSelection)) {
            qt_x11Data->xdndHandleSelectionRequest(req);

        } else if (qt_clipboard) {
            QClipboardEvent e(event);
            QApplication::sendSpontaneousEvent(qt_clipboard, &e);
        }
        break;
    }
    case SelectionClear: {
        XSelectionClearEvent *req = &event->xselectionclear;
        // don't deliver dnd events to the clipboard, it gets confused
        if (! req || (ATOM(XdndSelection) && req->selection == ATOM(XdndSelection)))
            break;

        if (qt_clipboard && !qt_x11Data->use_xfixes) {
            QClipboardEvent e(event);
            QApplication::sendSpontaneousEvent(qt_clipboard, &e);
        }
        break;
    }

    case SelectionNotify: {
        XSelectionEvent *req = &event->xselection;
        // don't deliver dnd events to the clipboard, it gets confused
        if (! req || (ATOM(XdndSelection) && req->selection == ATOM(XdndSelection)))
            break;

        if (qt_clipboard) {
            QClipboardEvent e(event);
            QApplication::sendSpontaneousEvent(qt_clipboard, &e);
        }
        break;
    }
    case PropertyNotify:
        // some properties changed
        if (event->xproperty.window == QX11Info::appRootWindow(0)) {
            // root properties for the first screen
            if (!qt_x11Data->use_xfixes && event->xproperty.atom == ATOM(_QT_CLIPBOARD_SENTINEL)) {
                if (qt_check_clipboard_sentinel()) {
                    emit clipboard()->changed(QClipboard::Clipboard);
                    emit clipboard()->dataChanged();
                }
            } else if (!qt_x11Data->use_xfixes && event->xproperty.atom == ATOM(_QT_SELECTION_SENTINEL)) {
                if (qt_check_selection_sentinel()) {
                    emit clipboard()->changed(QClipboard::Selection);
                    emit clipboard()->selectionChanged();
                }
            } else if (QApplicationPrivate::obey_desktop_settings &&event->xproperty.atom == ATOM(_QT_SETTINGS_TIMESTAMP)) {
                QApplicationPrivate::x11_apply_settings();
            }
        }
        if (event->xproperty.window == QX11Info::appRootWindow()) {
            // root properties for the default screen
            if (event->xproperty.atom == ATOM(_NET_SUPPORTED)) {
                qt_get_net_supported();
            } else if (event->xproperty.atom == ATOM(_NET_VIRTUAL_ROOTS)) {
                qt_get_net_virtual_roots();
            } else if (event->xproperty.atom == ATOM(_NET_WORKAREA)) {
                qt_desktopwidget_update_workarea();

                // emit the workAreaResized() signal
                QDesktopWidget *desktop = QApplication::desktop();
                for (int i = 0; i < desktop->screenCount(); ++i)
                    emit desktop->workAreaResized(i);
            }
        } else if (widget) {
            widget->translatePropertyEvent(event);
        }  else {
            return -1; // don't know this window
        }
        break;

    default:
        break;
    }

    return 0;
}

bool QApplication::x11EventFilter(XEvent *)
{
    return false;
}



/*****************************************************************************
  Modal widgets; Since Xlib has little support for this we roll our own
  modal widget mechanism.
  A modal widget without a parent becomes application-modal.
  A modal widget with a parent becomes modal to its parent and grandparents..

  QApplicationPrivate::enterModal()
        Enters modal state
        Arguments:
            QWidget *widget        A modal widget

  QApplicationPrivate::leaveModal()
        Leaves modal state for a widget
        Arguments:
            QWidget *widget        A modal widget
 *****************************************************************************/

bool QApplicationPrivate::modalState()
{
    return app_do_modal;
}

void QApplicationPrivate::enterModal_sys(QWidget *widget)
{
    if (!qt_modal_stack)
        qt_modal_stack = new QWidgetList;

    QWidget *leave = qt_last_mouse_receiver;
    if (!leave)
        leave = QWidget::find((WId)curWin);
    QApplicationPrivate::dispatchEnterLeave(0, leave);
    qt_modal_stack->insert(0, widget);
    app_do_modal = true;
    curWin = 0;
    qt_last_mouse_receiver = 0;
}

void QApplicationPrivate::leaveModal_sys(QWidget *widget)
{
    if (qt_modal_stack && qt_modal_stack->removeAll(widget)) {
        if (qt_modal_stack->isEmpty()) {
            delete qt_modal_stack;
            qt_modal_stack = 0;
            QPoint p(QCursor::pos());
            QWidget* w = QApplication::widgetAt(p.x(), p.y());
            QWidget *leave = qt_last_mouse_receiver;
            if (!leave)
                leave = QWidget::find((WId)curWin);
            if (QWidget *grabber = QWidget::mouseGrabber()) {
                w = grabber;
                if (leave == w)
                    leave = 0;
            }
            QApplicationPrivate::dispatchEnterLeave(w, leave); // send synthetic enter event
            curWin = w ? w->effectiveWinId() : 0;
            qt_last_mouse_receiver = w;
        }
    }
    app_do_modal = qt_modal_stack != 0;
}

/*****************************************************************************
  Popup widget mechanism

  openPopup()
        Adds a widget to the list of popup widgets
        Arguments:
            QWidget *widget        The popup widget to be added

  closePopup()
        Removes a widget from the list of popup widgets
        Arguments:
            QWidget *widget        The popup widget to be removed
 *****************************************************************************/


static int openPopupCount = 0;
void QApplicationPrivate::openPopup(QWidget *popup)
{
    Q_Q(QApplication);
    openPopupCount++;
    if (!QApplicationPrivate::popupWidgets) {                        // create list
        QApplicationPrivate::popupWidgets = new QWidgetList;
    }
    QApplicationPrivate::popupWidgets->append(popup);                // add to end of list
    Display *dpy = qt_x11Data->display;
    if (QApplicationPrivate::popupWidgets->count() == 1 && !qt_nograb()){ // grab mouse/keyboard
        Q_ASSERT(popup->testAttribute(Qt::WA_WState_Created));
        int r = XGrabKeyboard(dpy, popup->effectiveWinId(), false,
                              GrabModeAsync, GrabModeAsync, qt_x11Data->time);
        if ((popupGrabOk = (r == GrabSuccess))) {
            r = XGrabPointer(dpy, popup->effectiveWinId(), true,
                             (ButtonPressMask | ButtonReleaseMask | ButtonMotionMask
                              | EnterWindowMask | LeaveWindowMask | PointerMotionMask),
                             GrabModeAsync, GrabModeAsync, XNone, XNone, qt_x11Data->time);
            if (!(popupGrabOk = (r == GrabSuccess))) {
                // transfer grab back to the keyboard grabber if any
                if (QWidgetPrivate::keyboardGrabber)
                    QWidgetPrivate::keyboardGrabber->grabKeyboard();
                else
                    XUngrabKeyboard(dpy, qt_x11Data->time);
            }
        }
    }

    // popups are not focus-handled by the window system (the first
    // popup grabbed the keyboard), so we have to do that manually: A
    // new popup gets the focus
    if (popup->focusWidget()) {
        popup->focusWidget()->setFocus(Qt::PopupFocusReason);
    } else if (QApplicationPrivate::popupWidgets->count() == 1) { // this was the first popup
        if (QWidget *fw = QApplication::focusWidget()) {
            QFocusEvent e(QEvent::FocusOut, Qt::PopupFocusReason);
            q->sendEvent(fw, &e);
        }
    }
}

void QApplicationPrivate::closePopup(QWidget *popup)
{
    Q_Q(QApplication);
    if (!QApplicationPrivate::popupWidgets)
        return;
    QApplicationPrivate::popupWidgets->removeAll(popup);
    if (popup == qt_popup_down) {
        qt_button_down = 0;
        qt_popup_down = 0;
    }
    if (QApplicationPrivate::popupWidgets->count() == 0) {                // this was the last popup
        delete QApplicationPrivate::popupWidgets;
        QApplicationPrivate::popupWidgets = nullptr;
        if (!qt_nograb() && popupGrabOk) {        // grabbing not disabled
            Display *dpy = qt_x11Data->display;
            if (popup->geometry().contains(QPoint(mouseGlobalXPos, mouseGlobalYPos))
                || popup->testAttribute(Qt::WA_NoMouseReplay)) {
                // mouse release event or inside
                replayPopupMouseEvent = false;
            } else {                                // mouse press event
                mouseButtonPressTime -= 10000;        // avoid double click
                replayPopupMouseEvent = true;
            }
            // transfer grab back to mouse grabber if any, otherwise release the grab
            if (QWidgetPrivate::mouseGrabber)
                QWidgetPrivate::mouseGrabber->grabMouse();
            else
                XUngrabPointer(dpy, qt_x11Data->time);

            // transfer grab back to keyboard grabber if any, otherwise release the grab
            if (QWidgetPrivate::keyboardGrabber)
                QWidgetPrivate::keyboardGrabber->grabKeyboard();
            else
                XUngrabKeyboard(dpy, qt_x11Data->time);

            XFlush(dpy);
        }
        if (QApplicationPrivate::active_window) {
            if (QWidget *fw = QApplicationPrivate::active_window->focusWidget()) {
                if (fw != QApplication::focusWidget()) {
                    fw->setFocus(Qt::PopupFocusReason);
                } else {
                    QFocusEvent e(QEvent::FocusIn, Qt::PopupFocusReason);
                    q->sendEvent(fw, &e);
                }
            }
        }
    } else {
        // popups are not focus-handled by the window system (the
        // first popup grabbed the keyboard), so we have to do that
        // manually: A popup was closed, so the previous popup gets
        // the focus.
        QWidget* aw = QApplicationPrivate::popupWidgets->last();
        if (QWidget *fw = aw->focusWidget())
            fw->setFocus(Qt::PopupFocusReason);

        // regrab the keyboard and mouse in case 'popup' lost the grab
        if (QApplicationPrivate::popupWidgets->count() == 1 && !qt_nograb()){ // grab mouse/keyboard
            Display *dpy = qt_x11Data->display;
            Q_ASSERT(aw->testAttribute(Qt::WA_WState_Created));
            int r = XGrabKeyboard(dpy, aw->effectiveWinId(), false,
                                  GrabModeAsync, GrabModeAsync, qt_x11Data->time);
            if ((popupGrabOk = (r == GrabSuccess))) {
                r = XGrabPointer(dpy, aw->effectiveWinId(), true,
                                 (ButtonPressMask | ButtonReleaseMask | ButtonMotionMask
                                  | EnterWindowMask | LeaveWindowMask | PointerMotionMask),
                                 GrabModeAsync, GrabModeAsync, XNone, XNone, qt_x11Data->time);
                if (!(popupGrabOk = (r == GrabSuccess))) {
                    // transfer grab back to keyboard grabber
                    if (QWidgetPrivate::keyboardGrabber)
                        QWidgetPrivate::keyboardGrabber->grabKeyboard();
                    else
                        XUngrabKeyboard(dpy, qt_x11Data->time);
                }
            }
        }
    }
}

/*****************************************************************************
  Event translation; translates X11 events to Qt events
 *****************************************************************************/

//
// Mouse event translation
//
// Xlib doesn't give mouse double click events, so we generate them by
// comparing window, time and position between two mouse press events.
//

static Qt::MouseButtons translateMouseButtons(int s)
{
    Qt::MouseButtons ret = 0;
    if (s & Button1Mask)
        ret |= Qt::LeftButton;
    if (s & Button2Mask)
        ret |= Qt::MiddleButton;
    if (s & Button3Mask)
        ret |= Qt::RightButton;
    return ret;
}

bool QETWidget::translateMouseEvent(const XEvent *event)
{
    if (!isWindow() && testAttribute(Qt::WA_NativeWindow))
        Q_ASSERT(internalWinId());

    Q_D(QWidget);
    QEvent::Type type;                                // event parameters
    QPoint pos;
    QPoint globalPos;
    Qt::MouseButton button = Qt::NoButton;
    Qt::MouseButtons buttons;
    Qt::KeyboardModifiers modifiers;
    XEvent nextEvent;

    if (qt_sm_blockUserInput) // block user interaction during session management
        return true;

    if (event->type == MotionNotify) { // mouse move
        if (event->xmotion.root != RootWindow(qt_x11Data->display, x11Info().screen()) &&
            ! qt_xdnd_dragging)
            return false;

        XMotionEvent lastMotion = event->xmotion;
        while(XPending(qt_x11Data->display))  { // compress mouse moves
            XNextEvent(qt_x11Data->display, &nextEvent);
            if (nextEvent.type == ConfigureNotify
                || nextEvent.type == PropertyNotify
                || nextEvent.type == Expose
                || nextEvent.type == GraphicsExpose
                || nextEvent.type == NoExpose
                || nextEvent.type == KeymapNotify
                || ((nextEvent.type == EnterNotify || nextEvent.type == LeaveNotify)
                    && qt_button_down == this)
                || (nextEvent.type == ClientMessage
                    && (nextEvent.xclient.message_type == ATOM(_QT_SCROLL_DONE) ||
                    (nextEvent.xclient.message_type == ATOM(WM_PROTOCOLS) &&
                     (Atom)nextEvent.xclient.data.l[0] == ATOM(_NET_WM_SYNC_REQUEST))))) {
                // Pass the event through the event dispatcher filter so that applications
                // which install an event filter on the dispatcher get to handle it first.
                if (!QAbstractEventDispatcher::instance()->filterEvent(&nextEvent))
                    qApp->x11ProcessEvent(&nextEvent);
                continue;
            } else if (nextEvent.type != MotionNotify ||
                       nextEvent.xmotion.window != event->xmotion.window ||
                       nextEvent.xmotion.state != event->xmotion.state) {
                XPutBackEvent(qt_x11Data->display, &nextEvent);
                break;
            }
            if (!qt_x11EventFilter(&nextEvent)
                && !x11Event(&nextEvent)) // send event through filter
                lastMotion = nextEvent.xmotion;
            else
                break;
        }
        type = QEvent::MouseMove;
        pos.rx() = lastMotion.x;
        pos.ry() = lastMotion.y;
        pos = d->mapFromWS(pos);
        globalPos.rx() = lastMotion.x_root;
        globalPos.ry() = lastMotion.y_root;
        buttons = translateMouseButtons(lastMotion.state);
        modifiers = qt_keymapper()->translateModifiers(lastMotion.state);
        if (qt_button_down && !buttons)
            qt_button_down = 0;
    } else if (event->type == EnterNotify || event->type == LeaveNotify) {
        XEvent *xevent = (XEvent *)event;
        //unsigned int xstate = event->xcrossing.state;
        type = QEvent::MouseMove;
        pos.rx() = xevent->xcrossing.x;
        pos.ry() = xevent->xcrossing.y;
        pos = d->mapFromWS(pos);
        globalPos.rx() = xevent->xcrossing.x_root;
        globalPos.ry() = xevent->xcrossing.y_root;
        buttons = translateMouseButtons(xevent->xcrossing.state);
        modifiers = qt_keymapper()->translateModifiers(xevent->xcrossing.state);
        if (qt_button_down && !buttons)
            qt_button_down = 0;
        if (qt_button_down)
            return true;
    } else {                                        // button press or release
        pos.rx() = event->xbutton.x;
        pos.ry() = event->xbutton.y;
        pos = d->mapFromWS(pos);
        globalPos.rx() = event->xbutton.x_root;
        globalPos.ry() = event->xbutton.y_root;
        buttons = translateMouseButtons(event->xbutton.state);
        modifiers = qt_keymapper()->translateModifiers(event->xbutton.state);
        switch (event->xbutton.button) {
            case Button1: button = Qt::LeftButton; break;
            case Button2: button = Qt::MiddleButton; break;
            case Button3: button = Qt::RightButton; break;
            case Button4:
            case Button5: {
                // the fancy mouse wheel.

                // We are only interested in ButtonPress.
                if (event->type == XButtonPress){
                    // compress wheel events (the X Server will simply
                    // send a button press for each single notch,
                    // regardless whether the application can catch up
                    // or not)
                    int delta = 1;
                    XEvent xevent;
                    while (XCheckTypedWindowEvent(qt_x11Data->display, effectiveWinId(), XButtonPress, &xevent)){
                        if (xevent.xbutton.button != event->xbutton.button){
                            XPutBackEvent(qt_x11Data->display, &xevent);
                            break;
                        }
                        delta++;
                    }

                    // the delta is defined as multiples of
                    // WHEEL_DELTA, which is set to 120. Future wheels
                    // may offer a finer-resolution. A positive delta
                    // indicates forward rotation, a negative one
                    // backward rotation respectively.
                    int btn = event->xbutton.button;
                    delta *= 120 * (btn == Button4 ? 1 : -1);
                    bool hor = ((btn == Button4 || btn == Button5) && (modifiers & Qt::AltModifier));
                    translateWheelEvent(globalPos.x(), globalPos.y(), delta, buttons,
                                        modifiers, (hor) ? Qt::Horizontal: Qt::Vertical);
                }
                return true;
            }
        }
        if (event->type == XButtonPress) {        // mouse button pressed
            buttons |= button;
            if (!qt_button_down) {
                qt_button_down = childAt(pos);        //magic for masked widgets
                if (!qt_button_down)
                    qt_button_down = this;
            }
            if (mouseActWindow == event->xbutton.window &&
                mouseButtonPressed == button &&
                (long)event->xbutton.time -(long)mouseButtonPressTime
                < QApplication::doubleClickInterval() &&
                qAbs(event->xbutton.x - mouseXPos) < QT_GUI_DOUBLE_CLICK_RADIUS &&
                qAbs(event->xbutton.y - mouseYPos) < QT_GUI_DOUBLE_CLICK_RADIUS) {
                type = QEvent::MouseButtonDblClick;
                mouseButtonPressTime -= 2000;        // no double-click next time
            } else {
                type = QEvent::MouseButtonPress;
                mouseButtonPressTime = event->xbutton.time;
            }
            mouseButtonPressed = button;        // save event params for
            mouseXPos = event->xbutton.x;                // future double click tests
            mouseYPos = event->xbutton.y;
            mouseGlobalXPos = globalPos.x();
            mouseGlobalYPos = globalPos.y();
        } else {                                // mouse button released
            buttons &= ~button;
            type = QEvent::MouseButtonRelease;
        }
    }
    mouseActWindow = effectiveWinId();                        // save some event params
    if (type == 0)                                // don't send event
        return false;

    if (QApplicationPrivate::popupWidgets) {                        // in popup mode
        QWidget *popup = QApplicationPrivate::popupWidgets->last();
        QWidget *activePopupWidget = QApplicationPrivate::popupWidgets->last();
        if (popup != this) {
            if (event->type == LeaveNotify)
                return false;
            if ((windowType() == Qt::Popup) && rect().contains(pos) && 0)
                popup = this;
            else                                // send to last popup
                pos = popup->mapFromGlobal(globalPos);
        }
        bool releaseAfter = false;
        QWidget *popupChild  = popup->childAt(pos);

        if (popup != qt_popup_down){
            qt_button_down = 0;
            qt_popup_down = 0;
        }

        switch (type) {
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonDblClick:
            qt_button_down = popupChild;
            qt_popup_down = popup;
            break;
        case QEvent::MouseButtonRelease:
            releaseAfter = true;
            break;
        default:
            break;                                // nothing for mouse move
        }

        int oldOpenPopupCount = openPopupCount;

        if (popup->isEnabled()) {
            // deliver event
            replayPopupMouseEvent = false;
            QWidget *receiver = popup;
            QPoint widgetPos = pos;
            if (qt_button_down)
                receiver = qt_button_down;
            else if (popupChild)
                receiver = popupChild;
            if (receiver != popup)
                widgetPos = receiver->mapFromGlobal(globalPos);
            QWidget *alien = childAt(mapFromGlobal(globalPos));
            QMouseEvent e(type, widgetPos, globalPos, button, buttons, modifiers);
            QApplicationPrivate::sendMouseEvent(receiver, &e, alien, this, &qt_button_down, qt_last_mouse_receiver);
        } else {
            // close disabled popups when a mouse button is pressed or released
            switch (type) {
            case QEvent::MouseButtonPress:
            case QEvent::MouseButtonDblClick:
            case QEvent::MouseButtonRelease:
                popup->close();
                break;
            default:
                break;
            }
        }

        if (qApp->activePopupWidget() != activePopupWidget
            && replayPopupMouseEvent) {
            // the active popup was closed, replay the mouse event
            if (!(windowType() == Qt::Popup)) {
#if 1
                qt_button_down = 0;
#else
                if (buttons == button)
                    qt_button_down = this;
                QMouseEvent e(type, mapFromGlobal(globalPos), globalPos, button,
                              buttons, modifiers);
                QApplication::sendSpontaneousEvent(this, &e);

                if (type == QEvent::MouseButtonPress
                    && button == Qt::RightButton
                    && (openPopupCount == oldOpenPopupCount)) {
                    QContextMenuEvent e(QContextMenuEvent::Mouse, mapFromGlobal(globalPos),
                                        globalPos, modifiers);
                    QApplication::sendSpontaneousEvent(this, &e);
                }
#endif
            }
            replayPopupMouseEvent = false;
        } else if (type == QEvent::MouseButtonPress
                   && button == Qt::RightButton
                   && (openPopupCount == oldOpenPopupCount)) {
            QWidget *popupEvent = popup;
            if (qt_button_down)
                popupEvent = qt_button_down;
            else if(popupChild)
                popupEvent = popupChild;
            QContextMenuEvent e(QContextMenuEvent::Mouse, pos, globalPos, modifiers);
            QApplication::sendSpontaneousEvent(popupEvent, &e);
        }

        if (releaseAfter) {
            qt_button_down = 0;
            qt_popup_down = 0;
        }
    } else {
        QWidget *alienWidget = childAt(pos);
        QWidget *widget = QApplicationPrivate::pickMouseReceiver(this, globalPos, pos, type, buttons,
                                                                 qt_button_down, alienWidget);
        if (!widget) {
            if (type == QEvent::MouseButtonRelease)
                QApplicationPrivate::mouse_buttons &= ~button;
            return false; // don't send event
        }

        int oldOpenPopupCount = openPopupCount;
        QMouseEvent e(type, pos, globalPos, button, buttons, modifiers);
        QApplicationPrivate::sendMouseEvent(widget, &e, alienWidget, this, &qt_button_down,
                                            qt_last_mouse_receiver);
        if (type == QEvent::MouseButtonPress
            && button == Qt::RightButton
            && (openPopupCount == oldOpenPopupCount)) {
            QContextMenuEvent e(QContextMenuEvent::Mouse, pos, globalPos, modifiers);
            QApplication::sendSpontaneousEvent(widget, &e);
        }
    }
    return true;
}


//
// Wheel event translation
//
bool QETWidget::translateWheelEvent(int global_x, int global_y, int delta,
                                    Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers,
                                    Qt::Orientation orient)
{
    const QPoint globalPos = QPoint(global_x, global_y);
    QPoint pos = mapFromGlobal(globalPos);
    QWidget *widget = childAt(pos);
    if (!widget)
        widget = this;
    else if (!widget->internalWinId())
        pos = widget->mapFromGlobal(globalPos);

#ifdef ALIEN_DEBUG
        qDebug() << "QETWidget::translateWheelEvent: receiver:" << widget << "pos:" << pos;
#endif

    // send the event to the widget or its ancestors
    {
        QWidget* popup = qApp->activePopupWidget();
        if (popup && window() != popup)
            popup->close();
#ifndef QT_NO_WHEELEVENT
        QWheelEvent e(pos, globalPos, delta, buttons, modifiers, orient);
        if (QApplication::sendSpontaneousEvent(widget, &e))
#endif
            return true;
    }

    // send the event to the widget that has the focus or its ancestors, if different
    if (widget != qApp->focusWidget() && (widget = qApp->focusWidget())) {
        if (widget && !widget->internalWinId())
            pos = widget->mapFromGlobal(globalPos);
        QWidget* popup = qApp->activePopupWidget();
        if (popup && widget != popup)
            popup->hide();
#ifndef QT_NO_WHEELEVENT
        QWheelEvent e(pos, globalPos, delta, buttons, modifiers, orient);
        if (QApplication::sendSpontaneousEvent(widget, &e))
#endif
            return true;
    }
    return false;
}


//
// XEvent Translation Event
//
bool QETWidget::translatePropertyEvent(const XEvent *event)
{
    Q_D(QWidget);
    if (!isWindow()) return true;

    Atom ret;
    int format, e;
    unsigned char *data = 0;
    unsigned long nitems, after;

    if (event->xproperty.atom == ATOM(_NET_WM_STATE)) {
        bool max = false;
        bool full = false;
        Qt::WindowStates oldState = this->data->window_state;

        if (event->xproperty.state == PropertyNewValue) {
            // using length of 1024 should be safe for all current and
            // possible NET states...
            e = XGetWindowProperty(qt_x11Data->display, event->xproperty.window, ATOM(_NET_WM_STATE), 0, 1024,
                                   False, XA_ATOM, &ret, &format, &nitems, &after, &data);

            if (e == Success && ret == XA_ATOM && format == 32 && nitems > 0) {
                Atom *states = (Atom *) data;

                uint maximized = 0;
                for (unsigned long i = 0; i < nitems; i++) {
                    if (states[i] == ATOM(_NET_WM_STATE_MAXIMIZED_VERT))
                        maximized |= 1;
                    else if (states[i] == ATOM(_NET_WM_STATE_MAXIMIZED_HORZ))
                        maximized |= 2;
                    else if (states[i] == ATOM(_NET_WM_STATE_FULLSCREEN))
                        full = true;
                }
                if (maximized == 3) {
                    // only set maximized if both horizontal and vertical properties are set
                    max = true;
                }
            }
        }

        bool send_event = false;

        if (qt_x11Data->isSupportedByWM(ATOM(_NET_WM_STATE_MAXIMIZED_VERT))
            && qt_x11Data->isSupportedByWM(ATOM(_NET_WM_STATE_MAXIMIZED_HORZ))) {
            if (max && !isMaximized()) {
                this->data->window_state = this->data->window_state | Qt::WindowMaximized;
                send_event = true;
            } else if (!max && isMaximized()) {
                this->data->window_state &= ~Qt::WindowMaximized;
                send_event = true;
            }
        }

        if (qt_x11Data->isSupportedByWM(ATOM(_NET_WM_STATE_FULLSCREEN))) {
            if (full && !isFullScreen()) {
                this->data->window_state = this->data->window_state | Qt::WindowFullScreen;
                send_event = true;
            } else if (!full && isFullScreen()) {
                this->data->window_state &= ~Qt::WindowFullScreen;
                send_event = true;
            }
        }

        if (send_event) {
            QWindowStateChangeEvent e(oldState);
            QApplication::sendSpontaneousEvent(this, &e);
        }
    } else if (event->xproperty.atom == ATOM(WM_STATE)) {
        // the widget frame strut should also be invalidated
        this->data->fstrut_dirty = true;

        if (event->xproperty.state == PropertyDelete) {
            // the window manager has removed the WM State property,
            // so it is now in the withdrawn state (ICCCM 4.1.3.1) and
            // we are free to reuse this window
            d->topData()->parentWinId = 0;
            d->topData()->validWMState = false;
            // map the window if we were waiting for a transition to
            // withdrawn
            if (qt_x11Data->deferred_map.removeAll(this)) {
                doDeferredMap();
            } else if (isVisible()
                       && !testAttribute(Qt::WA_Mapped)
                       && !testAttribute(Qt::WA_OutsideWSRange)) {
                // so that show() will work again. As stated in the
                // ICCCM section 4.1.4: "Only the client can effect a
                // transition into or out of the Withdrawn state.",
                // but apparently this particular window manager
                // doesn't seem to care
                setAttribute(Qt::WA_WState_ExplicitShowHide, false);
                setAttribute(Qt::WA_WState_Visible, false);
            }
        } else {
            // the window manager has changed the WM State property...
            // we are wanting to see if we are withdrawn so that we
            // can reuse this window...
            e = XGetWindowProperty(qt_x11Data->display, internalWinId(), ATOM(WM_STATE), 0, 2, False,
                                   ATOM(WM_STATE), &ret, &format, &nitems, &after, &data);

            if (e == Success && ret == ATOM(WM_STATE) && format == 32 && nitems > 0) {
                long *state = (long *) data;
                switch (state[0]) {
                case WithdrawnState:
                    // if we are in the withdrawn state, we are free
                    // to reuse this window provided we remove the
                    // WM_STATE property (ICCCM 4.1.3.1)
                    XDeleteProperty(qt_x11Data->display, internalWinId(), ATOM(WM_STATE));

                    // set the parent id to zero, so that show() will
                    // work again
                    d->topData()->parentWinId = 0;
                    d->topData()->validWMState = false;
                    // map the window if we were waiting for a
                    // transition to withdrawn
                    if (qt_x11Data->deferred_map.removeAll(this)) {
                        doDeferredMap();
                    } else if (isVisible()
                               && !testAttribute(Qt::WA_Mapped)
                               && !testAttribute(Qt::WA_OutsideWSRange)) {
                        // so that show() will work again. As stated
                        // in the ICCCM section 4.1.4: "Only the
                        // client can effect a transition into or out
                        // of the Withdrawn state.", but apparently
                        // this particular window manager doesn't seem
                        // to care
                        setAttribute(Qt::WA_WState_ExplicitShowHide, false);
                        setAttribute(Qt::WA_WState_Visible, false);
                    }
                    break;

                case IconicState:
                    d->topData()->validWMState = true;
                    if (!isMinimized()) {
                        // window was minimized
                        this->data->window_state = this->data->window_state | Qt::WindowMinimized;
                        QWindowStateChangeEvent e(this->data->window_state & ~Qt::WindowMinimized);
                        QApplication::sendSpontaneousEvent(this, &e);
                    }
                    break;

                default:
                    d->topData()->validWMState = true;
                    if (isMinimized()) {
                        // window was un-minimized
                        this->data->window_state &= ~Qt::WindowMinimized;
                        QWindowStateChangeEvent e(this->data->window_state | Qt::WindowMinimized);
                        QApplication::sendSpontaneousEvent(this, &e);
                    }
                    break;
                }
            }
        }
    } else if (event->xproperty.atom == ATOM(_NET_WM_WINDOW_OPACITY)) {
        // the window opacity was changed
        if (event->xproperty.state == PropertyNewValue) {
            e = XGetWindowProperty(event->xclient.display,
                                   event->xclient.window,
                                   ATOM(_NET_WM_WINDOW_OPACITY),
                                   0, 1, False, XA_CARDINAL,
                                   &ret, &format, &nitems, &after, &data);

            if (e == Success && ret == XA_CARDINAL && format == 32 && nitems == 1
                && after == 0 && data) {
                ulong value = *(ulong*)(data);
                d->topData()->opacity = uint(value >> 24);
            }
        } else
            d->topData()->opacity = 255;
    }

    if (data)
        XFree(data);

    return true;
}


//
// Paint event translation
//
// When receiving many expose events, we compress them (union of all expose
// rectangles) into one event which is sent to the widget.

struct PaintEventInfo {
    Window window;
};

#if defined(Q_C_CALLBACKS)
extern "C" {
#endif

static Bool isPaintOrScrollDoneEvent(Display *, XEvent *ev, XPointer a)
{
    PaintEventInfo *info = (PaintEventInfo *)a;
    if (ev->type == Expose || ev->type == GraphicsExpose
        || (ev->type == ClientMessage && ev->xclient.message_type == ATOM(_QT_SCROLL_DONE)))
    {
        if (ev->xexpose.window == info->window)
            return True;
    }
    return False;
}

#if defined(Q_C_CALLBACKS)
}
#endif



static void translateBySips(QWidget* that, QRect& paintRect)
{
    int dx = 0, dy = 0;
    int sips = 0;
    foreach (const QX11Data::ScrollInProgress &sip, qt_x11Data->sip_list) {
        if (sip.scrolled_widget == that) {
            if (sips) {
                dx += sip.dx;
                dy += sip.dy;
            }
            sips++;
        }
    }
    if (sips > 1) {
        paintRect.translate(dx, dy);
    }
}

void QETWidget::translatePaintEvent(const XEvent *event)
{
    if (!isWindow() && testAttribute(Qt::WA_NativeWindow))
        Q_ASSERT(internalWinId());

    Q_D(QWidget);
    QRect  paintRect(event->xexpose.x, event->xexpose.y,
                     event->xexpose.width, event->xexpose.height);
    XEvent xevent;
    PaintEventInfo info;
    info.window = internalWinId();
    translateBySips(this, paintRect);

    QRegion paintRegion(d->mapFromWS(paintRect));

    // WARNING: this is O(number_of_events * number_of_matching_events)
    while (XCheckIfEvent(qt_x11Data->display,&xevent,isPaintOrScrollDoneEvent,
                         (XPointer)&info) &&
           !qt_x11EventFilter(&xevent)  &&
           !x11Event(&xevent)) // send event through filter
    {
        if (xevent.type == Expose || xevent.type == GraphicsExpose) {
            QRect exposure(xevent.xexpose.x,
                           xevent.xexpose.y,
                           xevent.xexpose.width,
                           xevent.xexpose.height);
            translateBySips(this, exposure);
            exposure = d->mapFromWS(exposure);
            paintRegion |= exposure;
        } else {
            translateScrollDoneEvent(&xevent);
        }
    }

    if (!paintRegion.isEmpty() && !testAttribute(Qt::WA_WState_ConfigPending))
        d->syncBackingStore(paintRegion);
}

//
// Scroll-done event translation.
//

bool QETWidget::translateScrollDoneEvent(const XEvent *event)
{
    long id = event->xclient.data.l[0];

    // Remove any scroll-in-progress record for the given id.
    for (int i = 0; i < qt_x11Data->sip_list.size(); ++i) {
        const QX11Data::ScrollInProgress &sip = qt_x11Data->sip_list.at(i);
        if (sip.id == id) {
            qt_x11Data->sip_list.removeAt(i);
            return true;
        }
    }

    return false;
}

//
// ConfigureNotify (window move and resize) event translation

bool QETWidget::translateConfigEvent(const XEvent *event)
{
    Q_ASSERT((!isWindow() && !testAttribute(Qt::WA_NativeWindow)) ? internalWinId() : true);

    Q_D(QWidget);
    bool wasResize = testAttribute(Qt::WA_WState_ConfigPending); // set in QWidget::setGeometry_sys()
    setAttribute(Qt::WA_WState_ConfigPending, false);

    if (testAttribute(Qt::WA_OutsideWSRange)) {
        // discard events for windows that have a geometry X can't handle
        XEvent xevent;
        while (XCheckTypedWindowEvent(qt_x11Data->display,internalWinId(), ConfigureNotify,&xevent) &&
               !qt_x11EventFilter(&xevent)  &&
               !x11Event(&xevent)) // send event through filter
            ;
        return true;
    }

    const QSize oldSize = size();

    if (isWindow()) {
        QPoint newCPos(geometry().topLeft());
        QSize  newSize(event->xconfigure.width, event->xconfigure.height);

        bool trust = isVisible()
                     && (d->topData()->parentWinId == XNone ||
                         d->topData()->parentWinId == QX11Info::appRootWindow());
        bool isCPos = false;

        if (event->xconfigure.send_event || trust) {
            // if a ConfigureNotify comes from a real sendevent request, we can
            // trust its values.
            newCPos.rx() = event->xconfigure.x + event->xconfigure.border_width;
            newCPos.ry() = event->xconfigure.y + event->xconfigure.border_width;
            isCPos = true;
        }
        if (isVisible())
            QApplication::syncX();

        if (d->extra->compress_events) {
            // ConfigureNotify compression for faster opaque resizing
            XEvent otherEvent;
            while (XCheckTypedWindowEvent(qt_x11Data->display, internalWinId(), ConfigureNotify,
                                          &otherEvent)) {
                if (qt_x11EventFilter(&otherEvent))
                    continue;

                if (x11Event(&otherEvent))
                    continue;

                if (otherEvent.xconfigure.event != otherEvent.xconfigure.window)
                    continue;

                newSize.setWidth(otherEvent.xconfigure.width);
                newSize.setHeight(otherEvent.xconfigure.height);

                if (otherEvent.xconfigure.send_event || trust) {
                    newCPos.rx() = otherEvent.xconfigure.x +
                                   otherEvent.xconfigure.border_width;
                    newCPos.ry() = otherEvent.xconfigure.y +
                                   otherEvent.xconfigure.border_width;
                    isCPos = true;
                }
            }
#ifndef QT_NO_XSYNC
            qt_sync_request_event_data sync_event;
            sync_event.window = internalWinId();
            for (XEvent ev;;) {
                if (!XCheckIfEvent(qt_x11Data->display, &ev, &qt_sync_request_scanner, (XPointer)&sync_event))
                    break;
            }
#endif // QT_NO_XSYNC
        }

        if (!isCPos) {
            // we didn't get an updated position of the toplevel.
            // either we haven't moved or there is a bug in the window manager.
            // anyway, let's query the position to be certain.
            int x, y;
            Window child;
            XTranslateCoordinates(qt_x11Data->display, internalWinId(),
                                  QApplication::desktop()->screen(d->xinfo.screen())->internalWinId(),
                                  0, 0, &x, &y, &child);
            newCPos.rx() = x;
            newCPos.ry() = y;
        }

        QRect cr (geometry());
        if (newCPos != cr.topLeft()) { // compare with cpos (exluding frame)
            QPoint oldPos = geometry().topLeft();
            cr.moveTopLeft(newCPos);
            data->crect = cr;
            if (isVisible()) {
                QMoveEvent e(newCPos, oldPos); // pos (including frame), not cpos
                QApplication::sendSpontaneousEvent(this, &e);
            } else {
                setAttribute(Qt::WA_PendingMoveEvent, true);
            }
        }
        if (newSize != cr.size()) { // size changed
            cr.setSize(newSize);
            data->crect = cr;

            Qt::WindowStates old_state = data->window_state;
            if (!qt_x11Data->isSupportedByWM(ATOM(_NET_WM_STATE_MAXIMIZED_VERT))
                && !qt_x11Data->isSupportedByWM(ATOM(_NET_WM_STATE_MAXIMIZED_HORZ)))
                data->window_state &= ~Qt::WindowMaximized;
            if (!qt_x11Data->isSupportedByWM(ATOM(_NET_WM_STATE_FULLSCREEN)))
                data->window_state &= ~Qt::WindowFullScreen;

            if (old_state != data->window_state) {
                QWindowStateChangeEvent e((Qt::WindowStates) old_state);
                QApplication::sendEvent(this, &e);
            }

            if (!isVisible())
                setAttribute(Qt::WA_PendingResizeEvent, true);
            wasResize = true;
        }

    } else {
        XEvent xevent;
        while (XCheckTypedWindowEvent(qt_x11Data->display,internalWinId(), ConfigureNotify,&xevent) &&
               !qt_x11EventFilter(&xevent)  &&
               !x11Event(&xevent)) // send event through filter
            ;
    }

    if (wasResize) {
        if (isVisible() && data->crect.size() != oldSize) {
            Q_ASSERT(d->extra->topextra);
            QWidgetBackingStore *bs = d->extra->topextra->backingStore.data();
            const bool hasStaticContents = bs && bs->hasStaticContents();
            // If we have a backing store with static contents, we have to disable the top-level
            // resize optimization in order to get invalidated regions for resized widgets.
            // The optimization discards all invalidateBuffer() calls since we're going to
            // repaint everything anyways, but that's not the case with static contents.
            if (!hasStaticContents)
                d->extra->topextra->inTopLevelResize = true;
            QResizeEvent e(data->crect.size(), oldSize);
            QApplication::sendSpontaneousEvent(this, &e);
        }

        const bool waitingForMapNotify = d->extra->topextra && d->extra->topextra->waitingForMapNotify;
        if (!waitingForMapNotify) {
            if (d->paintOnScreen()) {
                QRegion updateRegion(rect());
                if (testAttribute(Qt::WA_StaticContents))
                    updateRegion -= QRect(0, 0, oldSize.width(), oldSize.height());
                d->syncBackingStore(updateRegion);
            } else {
                d->syncBackingStore();
            }
        }

        if (d->extra && d->extra->topextra)
            d->extra->topextra->inTopLevelResize = false;
    }
#ifndef QT_NO_XSYNC
    if (QTLWExtra *tlwExtra = d->maybeTopData()) {
        if (tlwExtra->newCounterValueLo != 0 || tlwExtra->newCounterValueHi != 0) {
            XSyncValue value;
            XSyncIntsToValue(&value,
                             tlwExtra->newCounterValueLo,
                             tlwExtra->newCounterValueHi);

            XSyncSetCounter(qt_x11Data->display, tlwExtra->syncUpdateCounter, value);
            tlwExtra->newCounterValueHi = 0;
            tlwExtra->newCounterValueLo = 0;
        }
    }
#endif
    return true;
}

//
// Close window event translation.
//
bool QETWidget::translateCloseEvent(const XEvent *)
{
    Q_D(QWidget);
    return d->close_helper(QWidgetPrivate::CloseWithSpontaneousEvent);
}


void QApplication::setCursorFlashTime(int msecs)
{
    QApplicationPrivate::cursor_flash_time = msecs;
}

int QApplication::cursorFlashTime()
{
    return QApplicationPrivate::cursor_flash_time;
}

void QApplication::setDoubleClickInterval(int ms)
{
    QApplicationPrivate::mouse_double_click_time = ms;
}

int QApplication::doubleClickInterval()
{
    return QApplicationPrivate::mouse_double_click_time;
}

void QApplication::setKeyboardInputInterval(int ms)
{
    QApplicationPrivate::keyboard_input_time = ms;
}

int QApplication::keyboardInputInterval()
{
    return QApplicationPrivate::keyboard_input_time;
}

#ifndef QT_NO_WHEELEVENT
void QApplication::setWheelScrollLines(int n)
{
    QApplicationPrivate::wheel_scroll_lines = n;
}

int QApplication::wheelScrollLines()
{
    return QApplicationPrivate::wheel_scroll_lines;
}
#endif

void QApplication::setEffectEnabled(Qt::UIEffect effect, bool enable)
{
    switch (effect) {
    case Qt::UI_AnimateMenu:
        if (enable) QApplicationPrivate::fade_menu = false;
        QApplicationPrivate::animate_menu = enable;
        break;
    case Qt::UI_FadeMenu:
        if (enable)
            QApplicationPrivate::animate_menu = true;
        QApplicationPrivate::fade_menu = enable;
        break;
    case Qt::UI_AnimateCombo:
        QApplicationPrivate::animate_combo = enable;
        break;
    case Qt::UI_AnimateTooltip:
        if (enable) QApplicationPrivate::fade_tooltip = false;
        QApplicationPrivate::animate_tooltip = enable;
        break;
    case Qt::UI_FadeTooltip:
        if (enable)
            QApplicationPrivate::animate_tooltip = true;
        QApplicationPrivate::fade_tooltip = enable;
        break;
    case Qt::UI_AnimateToolBox:
        QApplicationPrivate::animate_toolbox = enable;
        break;
    default:
        QApplicationPrivate::animate_ui = enable;
        break;
    }
}

bool QApplication::isEffectEnabled(Qt::UIEffect effect)
{
    if (QColormap::instance().depth() < 16 || !QApplicationPrivate::animate_ui)
        return false;

    switch(effect) {
    case Qt::UI_AnimateMenu:
        return QApplicationPrivate::animate_menu;
    case Qt::UI_FadeMenu:
        return QApplicationPrivate::fade_menu;
    case Qt::UI_AnimateCombo:
        return QApplicationPrivate::animate_combo;
    case Qt::UI_AnimateTooltip:
        return QApplicationPrivate::animate_tooltip;
    case Qt::UI_FadeTooltip:
        return QApplicationPrivate::fade_tooltip;
    case Qt::UI_AnimateToolBox:
        return QApplicationPrivate::animate_toolbox;
    default:
        return QApplicationPrivate::animate_ui;
    }
}

/*****************************************************************************
  Session management support
 *****************************************************************************/

#ifndef QT_NO_SESSIONMANAGER
class QSessionManagerPrivate : public QObjectPrivate
{
public:
    QSessionManagerPrivate(QSessionManager* mgr, QString& id, QString& key)
        : QObjectPrivate(), sm(mgr), sessionId(id), sessionKey(key),
            restartHint(QSessionManager::RestartIfRunning), eventLoop(0) {}
    QSessionManager* sm;
    QStringList restartCommand;
    QStringList discardCommand;
    QString& sessionId;
    QString& sessionKey;
    QSessionManager::RestartHint restartHint;
    QEventLoop *eventLoop;
};

class QSmSocketReceiver : public QObject
{
    Q_OBJECT
public:
    QSmSocketReceiver(int socket)
        {
            QSocketNotifier* sn = new QSocketNotifier(socket, QSocketNotifier::Read, this);
            connect(sn, SIGNAL(activated(int)), this, SLOT(socketActivated(int)));
        }

public slots:
     void socketActivated(int);
};

static SmcConn smcConnection = 0;
static bool sm_interactionActive = false;
static bool sm_smActive = false;
static int sm_interactStyle = SmInteractStyleNone;
static int sm_saveType = SmSaveLocal;
static bool sm_cancel = false;
static bool sm_waitingForInteraction = false;
static bool sm_isshutdown = false;
static bool sm_phase2 = false;
static bool sm_in_phase2 = false;

static QSmSocketReceiver* sm_receiver = 0;

static void resetSmState();
static void sm_setProperty(const char* name, const char* type,
                            int num_vals, SmPropValue* vals);
static void sm_saveYourselfCallback(SmcConn smcConn, SmPointer clientData,
                                  int saveType, Bool shutdown , int interactStyle, Bool fast);
static void sm_saveYourselfPhase2Callback(SmcConn smcConn, SmPointer clientData) ;
static void sm_dieCallback(SmcConn smcConn, SmPointer clientData) ;
static void sm_shutdownCancelledCallback(SmcConn smcConn, SmPointer clientData);
static void sm_saveCompleteCallback(SmcConn smcConn, SmPointer clientData);
static void sm_interactCallback(SmcConn smcConn, SmPointer clientData);
static void sm_performSaveYourself(QSessionManagerPrivate*);

static void resetSmState()
{
    sm_waitingForInteraction = false;
    sm_interactionActive = false;
    sm_interactStyle = SmInteractStyleNone;
    sm_smActive = false;
    qt_sm_blockUserInput = false;
    sm_isshutdown = false;
    sm_phase2 = false;
    sm_in_phase2 = false;
}


// theoretically it's possible to set several properties at once. For
// simplicity, however, we do just one property at a time
static void sm_setProperty(const char* name, const char* type,
                            int num_vals, SmPropValue* vals)
{
    if (num_vals) {
      SmProp prop;
      prop.name = (char*)name;
      prop.type = (char*)type;
      prop.num_vals = num_vals;
      prop.vals = vals;

      SmProp* props[1];
      props[0] = &prop;
      SmcSetProperties(smcConnection, 1, props);
    }
    else {
      char* names[1];
      names[0] = (char*) name;
      SmcDeleteProperties(smcConnection, 1, names);
    }
}

static void sm_setProperty(const QString& name, const QString& value)
{
    QByteArray v = value.toUtf8();
    SmPropValue prop;
    prop.length = v.length();
    prop.value = (SmPointer) v.constData();
    sm_setProperty(name.toLatin1().data(), SmARRAY8, 1, &prop);
}

static void sm_setProperty(const QString& name, const QStringList& value)
{
    SmPropValue *prop = new SmPropValue[value.count()];
    int count = 0;
    QList<QByteArray> vl;
    for (QStringList::ConstIterator it = value.begin(); it != value.end(); ++it) {
      prop[count].length = (*it).length();
      vl.append((*it).toUtf8());
      prop[count].value = (char*)vl.last().data();
      ++count;
    }
    sm_setProperty(name.toLatin1().data(), SmLISTofARRAY8, count, prop);
    delete [] prop;
}

static void sm_saveYourselfCallback(SmcConn smcConn, SmPointer clientData,
                                  int saveType, Bool shutdown , int interactStyle, Bool /*fast*/)
{
    if (smcConn != smcConnection)
        return;
    sm_cancel = false;
    sm_smActive = true;
    sm_isshutdown = shutdown;
    sm_saveType = saveType;
    sm_interactStyle = interactStyle;

    sm_performSaveYourself((QSessionManagerPrivate*) clientData);
    if (!sm_isshutdown) // we cannot expect a confirmation message in that case
        resetSmState();
}

Q_CORE_EXPORT QString qt_resolveUserName(uint userId);

static void sm_performSaveYourself(QSessionManagerPrivate* smd)
{
    if (sm_isshutdown)
        qt_sm_blockUserInput = true;

    QSessionManager* sm = smd->sm;

    // generate a new session key
    struct timeval tv;
    ::gettimeofday(&tv, nullptr);
    smd->sessionKey  = QString::number(qulonglong(tv.tv_sec)) + QLatin1Char('_') + QString::number(qulonglong(tv.tv_usec));

    QStringList arguments = qApp->arguments();
    QString argument0 = arguments.isEmpty() ? qApp->applicationFilePath() : arguments.at(0);

    // tell the session manager about our program in best POSIX style
    sm_setProperty(QString::fromLatin1(SmProgram), argument0);
    // tell the session manager about our user as well.
    QString username = qt_resolveUserName(::geteuid());
    if (!username.isEmpty())
        sm_setProperty(QString::fromLatin1(SmUserID), username);

    // generate a restart and discard command that makes sense
    QStringList restart;
    restart  << argument0 << QLatin1String("-session")
             << smd->sessionId + QLatin1Char('_') + smd->sessionKey;
    sm->setRestartCommand(restart);
    QStringList discard;
    sm->setDiscardCommand(discard);

    switch (sm_saveType) {
    case SmSaveBoth:
        qApp->commitData(*sm);
        if (sm_isshutdown && sm_cancel)
            break; // we cancelled the shutdown, no need to save state
    // fall through
    case SmSaveLocal:
        qApp->saveState(*sm);
        break;
    case SmSaveGlobal:
        qApp->commitData(*sm);
        break;
    default:
        break;
    }

    if (sm_phase2 && !sm_in_phase2) {
        SmcRequestSaveYourselfPhase2(smcConnection, sm_saveYourselfPhase2Callback, (SmPointer*) smd);
        qt_sm_blockUserInput = false;
    }
    else {
        // close eventual interaction monitors and cancel the
        // shutdown, if required. Note that we can only cancel when
        // performing a shutdown, it does not work for checkpoints
        if (sm_interactionActive) {
            SmcInteractDone(smcConnection, sm_isshutdown && sm_cancel);
            sm_interactionActive = false;
        }
        else if (sm_cancel && sm_isshutdown) {
            if (sm->allowsErrorInteraction()) {
                SmcInteractDone(smcConnection, True);
                sm_interactionActive = false;
            }
        }

        // set restart and discard command in session manager
        sm_setProperty(QString::fromLatin1(SmRestartCommand), sm->restartCommand());
        sm_setProperty(QString::fromLatin1(SmDiscardCommand), sm->discardCommand());

        // set the restart hint
        SmPropValue prop;
        prop.length = sizeof(int);
        int value = sm->restartHint();
        prop.value = (SmPointer) &value;
        sm_setProperty(SmRestartStyleHint, SmCARD8, 1, &prop);

        // we are done
        SmcSaveYourselfDone(smcConnection, !sm_cancel);
    }
}

static void sm_dieCallback(SmcConn smcConn, SmPointer /* clientData */)
{
    if (smcConn != smcConnection)
        return;
    resetSmState();
    QEvent quitEvent(QEvent::Quit);
    QApplication::sendEvent(qApp, &quitEvent);
}

static void sm_shutdownCancelledCallback(SmcConn smcConn, SmPointer clientData)
{
    if (smcConn != smcConnection)
        return;
    if (sm_waitingForInteraction)
        ((QSessionManagerPrivate *) clientData)->eventLoop->exit();
    resetSmState();
}

static void sm_saveCompleteCallback(SmcConn smcConn, SmPointer /*clientData */)
{
    if (smcConn != smcConnection)
        return;
    resetSmState();
}

static void sm_interactCallback(SmcConn smcConn, SmPointer clientData)
{
    if (smcConn != smcConnection)
        return;
    if (sm_waitingForInteraction)
        ((QSessionManagerPrivate *) clientData)->eventLoop->exit();
}

static void sm_saveYourselfPhase2Callback(SmcConn smcConn, SmPointer clientData)
{
    if (smcConn != smcConnection)
        return;
    sm_in_phase2 = true;
    sm_performSaveYourself((QSessionManagerPrivate*) clientData);
}


void QSmSocketReceiver::socketActivated(int)
{
    IceProcessMessages(SmcGetIceConnection(smcConnection), 0, 0);
}

QSessionManager::QSessionManager(QApplication * app, QString &id, QString& key)
    : QObject(*new QSessionManagerPrivate(this, id, key), app)
{
    Q_D(QSessionManager);
    d->restartHint = RestartIfRunning;

    resetSmState();
    QSTACKARRAY(char, cerror, 256);
    char* myId = 0;
    QByteArray b_id = id.toLatin1();
    char* prevId = b_id.data();

    SmcCallbacks cb;
    cb.save_yourself.callback = sm_saveYourselfCallback;
    cb.save_yourself.client_data = (SmPointer) d;
    cb.die.callback = sm_dieCallback;
    cb.die.client_data = (SmPointer) d;
    cb.save_complete.callback = sm_saveCompleteCallback;
    cb.save_complete.client_data = (SmPointer) d;
    cb.shutdown_cancelled.callback = sm_shutdownCancelledCallback;
    cb.shutdown_cancelled.client_data = (SmPointer) d;

    // avoid showing a warning message below
    if (qgetenv("SESSION_MANAGER").isEmpty())
        return;

    smcConnection = SmcOpenConnection(0, 0, 1, 0,
                                       SmcSaveYourselfProcMask |
                                       SmcDieProcMask |
                                       SmcSaveCompleteProcMask |
                                       SmcShutdownCancelledProcMask,
                                       &cb,
                                       prevId,
                                       &myId,
                                       256, cerror);

    id = QString::fromLatin1(myId);
    ::free(myId); // it was allocated by C

    QString error = QString::fromLocal8Bit(cerror);
    if (!smcConnection) {
        qWarning("Qt: Session management error: %s", qPrintable(error));
    }
    else {
        sm_receiver = new QSmSocketReceiver(IceConnectionNumber(SmcGetIceConnection(smcConnection)));
    }
}

QSessionManager::~QSessionManager()
{
    if (smcConnection)
        SmcCloseConnection(smcConnection, 0, 0);
    smcConnection = 0;
    delete sm_receiver;
}

QString QSessionManager::sessionId() const
{
    Q_D(const QSessionManager);
    return d->sessionId;
}

QString QSessionManager::sessionKey() const
{
    Q_D(const QSessionManager);
    return d->sessionKey;
}


void* QSessionManager::handle() const
{
    return (void*) smcConnection;
}


bool QSessionManager::allowsInteraction()
{
    Q_D(QSessionManager);
    if (sm_interactionActive)
        return true;

    if (sm_waitingForInteraction)
        return false;

    if (sm_interactStyle == SmInteractStyleAny) {
        sm_waitingForInteraction =  SmcInteractRequest(smcConnection, SmDialogNormal,
                                                        sm_interactCallback, (SmPointer*) d);
    }
    if (sm_waitingForInteraction) {
        QEventLoop eventLoop;
        d->eventLoop = &eventLoop;
        (void) eventLoop.exec();
        d->eventLoop = 0;

        sm_waitingForInteraction = false;
        if (sm_smActive) { // not cancelled
            sm_interactionActive = true;
            qt_sm_blockUserInput = false;
            return true;
        }
    }
    return false;
}

bool QSessionManager::allowsErrorInteraction()
{
    Q_D(QSessionManager);
    if (sm_interactionActive)
        return true;

    if (sm_waitingForInteraction)
        return false;

    if (sm_interactStyle == SmInteractStyleAny || sm_interactStyle == SmInteractStyleErrors) {
        sm_waitingForInteraction =  SmcInteractRequest(smcConnection, SmDialogError,
                                                        sm_interactCallback, (SmPointer*) d);
    }
    if (sm_waitingForInteraction) {
        QEventLoop eventLoop;
        d->eventLoop = &eventLoop;
        (void) eventLoop.exec();
        d->eventLoop = 0;

        sm_waitingForInteraction = false;
        if (sm_smActive) { // not cancelled
            sm_interactionActive = true;
            qt_sm_blockUserInput = false;
            return true;
        }
    }
    return false;
}

void QSessionManager::release()
{
    if (sm_interactionActive) {
        SmcInteractDone(smcConnection, False);
        sm_interactionActive = false;
        if (sm_smActive && sm_isshutdown)
            qt_sm_blockUserInput = true;
    }
}

void QSessionManager::cancel()
{
    sm_cancel = true;
}

void QSessionManager::setRestartHint(QSessionManager::RestartHint hint)
{
    Q_D(QSessionManager);
    d->restartHint = hint;
}

QSessionManager::RestartHint QSessionManager::restartHint() const
{
    Q_D(const QSessionManager);
    return d->restartHint;
}

void QSessionManager::setRestartCommand(const QStringList& command)
{
    Q_D(QSessionManager);
    d->restartCommand = command;
}

QStringList QSessionManager::restartCommand() const
{
    Q_D(const QSessionManager);
    return d->restartCommand;
}

void QSessionManager::setDiscardCommand(const QStringList& command)
{
    Q_D(QSessionManager);
    d->discardCommand = command;
}

QStringList QSessionManager::discardCommand() const
{
    Q_D(const QSessionManager);
    return d->discardCommand;
}

void QSessionManager::setManagerProperty(const QString& name, const QString& value)
{
    sm_setProperty(name, value);
}

void QSessionManager::setManagerProperty(const QString& name, const QStringList& value)
{
    sm_setProperty(name, value);
}

bool QSessionManager::isPhase2() const
{
    return sm_in_phase2;
}

void QSessionManager::requestPhase2()
{
    sm_phase2 = true;
}

#endif // QT_NO_SESSIONMANAGER

QT_END_NAMESPACE

#include "moc_qapplication_x11.cpp"
#include "moc_qsessionmanager.h"
