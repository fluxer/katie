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

#ifndef QT_X11_P_H
#define QT_X11_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "QtGui/qwindowdefs.h"
#include "QtCore/qlist.h"
#include "QtCore/qvariant.h"

// the following is necessary to work around breakage in many versions
// of XFree86's Xlib.h still in use
// ### which versions?
#if defined(_XLIB_H_) || defined(_X11_XLIB_H_) // crude hack, but...
#error "cannot include <X11/Xlib.h> before this file"
#endif

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/Xproto.h>

#define XK_MISCELLANY
#define XK_LATIN1
#include <X11/keysymdef.h>
#include <X11/XF86keysym.h>

#ifdef index
#  undef index
#endif
#ifdef rindex
#  undef rindex
#endif

#ifndef QT_NO_XSHAPE
#  include <X11/extensions/shape.h>
#endif // QT_NO_XSHAPE

#ifndef QT_NO_XINERAMA
#  include <X11/extensions/Xinerama.h>
#endif // QT_NO_XINERAMA

#ifndef QT_NO_XRANDR
#  include <X11/extensions/Xrandr.h>
#endif // QT_NO_XRANDR

#ifndef QT_NO_XRENDER
#  include <X11/extensions/Xrender.h>
#endif // QT_NO_XRENDER

#ifndef QT_NO_XSYNC
# include "X11/extensions/sync.h"
#endif

#ifndef QT_NO_XCURSOR
#include <X11/Xcursor/Xcursor.h>
#include <X11/cursorfont.h>
#endif // QT_NO_XCURSOR

#ifndef QT_NO_XFIXES
#  include <X11/extensions/Xfixes.h>
#endif // QT_NO_XFIXES

#ifndef QT_NO_SESSIONMANAGER
#include <X11/SM/SMlib.h>
#endif

QT_BEGIN_NAMESPACE

class QWidget;

struct QX11InfoData {
    uint ref;
    int screen;
    int dpiX;
    int dpiY;
    int depth;
    int cells;
    Colormap colormap;
    Visual *visual;
    bool defaultColormap;
    bool defaultVisual;
    int subpixel;
};

class QDrag;
struct QXdndDropTransaction
{
    Time timestamp;
    Window target;
    Window proxy_target;
    QWidget *targetWidget;
    QWidget *embedding_widget;
    QDrag *object;
};

class QMimeData;
struct QX11Data
{
    Window findClientWindow(Window, Atom);

    // from qclipboard_x11.cpp
    bool clipboardWaitForEvent(Window win, int type, XEvent *event, int timeout, bool checkManager = false);
    bool clipboardReadProperty(Window win, Atom property, bool deleteProperty,
                            QByteArray *buffer, int *size, Atom *type, int *format);
    QByteArray clipboardReadIncrementalProperty(Window win, Atom property, int nbytes);

    // from qdnd_x11.cpp
    bool dndEnable(QWidget* w, bool on);
    static void xdndSetup();
    void xdndHandleEnter(const XEvent *);
    void xdndHandlePosition(QWidget *, const XEvent *, bool);
    void xdndHandleStatus(const XEvent *);
    void xdndHandleLeave(QWidget *, const XEvent *);
    void xdndHandleDrop(const XEvent *, bool);
    void xdndHandleFinished(const XEvent *, bool);
    void xdndHandleSelectionRequest(const XSelectionRequestEvent *);
    static bool xdndHandleBadwindow();
    QByteArray xdndAtomToString(Atom a);

    QString xdndMimeAtomToString(Atom a);
    Atom xdndMimeStringToAtom(const QString &mimeType);
    QStringList xdndMimeFormatsForAtom(Atom a);
    bool xdndMimeDataForAtom(Atom a, QMimeData *mimeData, QByteArray *data, Atom *atomFormat, int *dataFormat);
    QList<Atom> xdndMimeAtomsForFormat(const QString &format);
    QVariant xdndMimeConvertToFormat(Atom a, const QByteArray &data, const QString &format, QVariant::Type requestedType, const QByteArray &encoding);
    Atom xdndMimeAtomForFormat(const QString &format, QVariant::Type requestedType, const QList<Atom> &atoms, QByteArray *requestedEncoding);

    QList<QXdndDropTransaction> dndDropTransactions;

    Display *display;
    char *displayName;
    bool foreignDisplay;
    // current focus model
    enum FocusModel {
        FM_Unknown = -1,
        FM_Other = 0,
        FM_PointerRoot = 1
    };
    FocusModel focus_model;

    // true if compiled w/ RANDR support and RANDR is supported on the connected Display
    bool use_xrandr;
    int xrandr_major;
    int xrandr_eventbase;

    // true if compiled w/ RENDER support and RENDER is supported on the connected Display
    bool use_xrender;
    int xrender_major;
    int xrender_minor;

    // true if compiled w/ XFIXES support and XFIXES is supported on the connected Display
    bool use_xfixes;
    int xfixes_eventbase;

    // true if compiled w/ XINERAMA support and XINERAMA is supported on the connected Display
    bool use_xinerama;

    QList<QWidget *> deferred_map;
    struct ScrollInProgress {
        long id;
        QWidget* scrolled_widget;
        int dx, dy;
    };
    long sip_serial;
    QList<ScrollInProgress> sip_list;

    // window managers list of supported "stuff"
    Atom *net_supported_list;
    // list of virtual root windows
    Window *net_virtual_root_list;
    // client leader window
    Window wm_client_leader;

    QX11InfoData *screens;
    Visual **argbVisuals;
    Colormap *argbColormaps;
    int screenCount;
    int defaultScreen;

    Time time;
    Time userTime;

    // starts to ignore bad window errors from X
    void ignoreBadwindow() {
        ignore_badwindow = true;
        seen_badwindow = false;
    }

    // ends ignoring bad window errors and returns whether an error had happened.
    bool badwindow() {
        ignore_badwindow = false;
        return seen_badwindow;
    }

    bool ignore_badwindow;
    bool seen_badwindow;

    // options
    int visual_class;
    int visual_id;
    int color_count;
    bool custom_cmap;

    // outside visual/colormap
    Visual *visual;
    Colormap colormap;

#ifndef QT_NO_XRENDER
    enum { solid_fill_count = 20 };
    struct SolidFills {
        XRenderColor color;
        int screen;
        Picture picture;
    } solid_fills[solid_fill_count];
    enum { pattern_fill_count = 20 };
    struct PatternFills {
        XRenderColor color;
        XRenderColor bg_color;
        int screen;
        int style;
        Picture picture;
    } pattern_fills[pattern_fill_count];
    Picture getSolidFill(int screen, const QColor &c);
    XRenderColor preMultiply(const QColor &c);
#endif

    bool has_fontconfig;
    bool fc_antialias;
    int fc_hint_style;

    char *startupId;

    /* Warning: if you modify this list, modify the names of atoms in qapplication_x11.cpp as well! */
    enum X11Atom {
        // window-manager <-> client protocols
        WM_PROTOCOLS,
        WM_DELETE_WINDOW,
        WM_TAKE_FOCUS,
        _NET_WM_PING,
        _NET_WM_CONTEXT_HELP,
        _NET_WM_SYNC_REQUEST,
        _NET_WM_SYNC_REQUEST_COUNTER,

        // ICCCM window state
        WM_STATE,
        WM_CHANGE_STATE,

        // Session management
        WM_CLIENT_LEADER,
        WM_WINDOW_ROLE,
        SM_CLIENT_ID,

        // Clipboard
        CLIPBOARD,
        INCR,
        TARGETS,
        MULTIPLE,
        TIMESTAMP,
        SAVE_TARGETS,
        CLIP_TEMPORARY,
        _QT_SELECTION,
        _QT_CLIPBOARD_SENTINEL,
        _QT_SELECTION_SENTINEL,
        CLIPBOARD_MANAGER,

        _QT_SCROLL_DONE,

        _MOTIF_WM_HINTS,

        // EWMH (aka NETWM)
        _NET_SUPPORTED,
        _NET_VIRTUAL_ROOTS,
        _NET_WORKAREA,

        _NET_MOVERESIZE_WINDOW,
        _NET_WM_MOVERESIZE,

        _NET_WM_NAME,
        _NET_WM_ICON_NAME,
        _NET_WM_ICON,

        _NET_WM_PID,

        _NET_WM_WINDOW_OPACITY,

        _NET_WM_STATE,
        _NET_WM_STATE_ABOVE,
        _NET_WM_STATE_BELOW,
        _NET_WM_STATE_FULLSCREEN,
        _NET_WM_STATE_MAXIMIZED_HORZ,
        _NET_WM_STATE_MAXIMIZED_VERT,
        _NET_WM_STATE_MODAL,
        _NET_WM_STATE_STAYS_ON_TOP,
        _NET_WM_STATE_DEMANDS_ATTENTION,

        _NET_WM_USER_TIME,
        _NET_WM_USER_TIME_WINDOW,
        _NET_WM_FULL_PLACEMENT,

        _NET_WM_WINDOW_TYPE,
        _NET_WM_WINDOW_TYPE_DESKTOP,
        _NET_WM_WINDOW_TYPE_DOCK,
        _NET_WM_WINDOW_TYPE_TOOLBAR,
        _NET_WM_WINDOW_TYPE_MENU,
        _NET_WM_WINDOW_TYPE_UTILITY,
        _NET_WM_WINDOW_TYPE_SPLASH,
        _NET_WM_WINDOW_TYPE_DIALOG,
        _NET_WM_WINDOW_TYPE_DROPDOWN_MENU,
        _NET_WM_WINDOW_TYPE_POPUP_MENU,
        _NET_WM_WINDOW_TYPE_TOOLTIP,
        _NET_WM_WINDOW_TYPE_NOTIFICATION,
        _NET_WM_WINDOW_TYPE_COMBO,
        _NET_WM_WINDOW_TYPE_DND,
        _NET_WM_WINDOW_TYPE_NORMAL,

        _NET_STARTUP_INFO,
        _NET_STARTUP_INFO_BEGIN,

        _NET_SUPPORTING_WM_CHECK,

        _NET_WM_CM_S0,

        _NET_SYSTEM_TRAY_VISUAL,
        _NET_SYSTEM_TRAY_OPCODE,
        MANAGER,

        _NET_ACTIVE_WINDOW,

        // Property formats
        COMPOUND_TEXT,
        TEXT,
        UTF8_STRING,

        // Xdnd
        XdndEnter,
        XdndPosition,
        XdndStatus,
        XdndLeave,
        XdndDrop,
        XdndFinished,
        XdndTypelist,

        XdndSelection,

        XdndAware,
        XdndProxy,

        XdndActionCopy,
        XdndActionLink,
        XdndActionMove,

        // XEMBED
        _XEMBED,
        _XEMBED_INFO,

        _QT_SETTINGS_TIMESTAMP,
        NPredefinedAtoms
    };
    Atom atoms[NPredefinedAtoms];

    bool isSupportedByWM(Atom atom);

    bool compositingManagerRunning;
};

extern Q_GUI_EXPORT QX11Data *qt_x11Data;

#define ATOM(x) qt_x11Data->atoms[QX11Data::x]

// rename a couple of X defines to get rid of name clashes
// resolve the conflict between X11's FocusIn and QEvent::FocusIn
enum {
    XFocusOut = FocusOut,
    XFocusIn = FocusIn,
    XKeyPress = KeyPress,
    XKeyRelease = KeyRelease,
    XNone = None,
    XGrayScale = GrayScale,
    XCursorShape = CursorShape,
    XUnsorted = Unsorted,
    XFontChange = FontChange,
    XButtonPress = ButtonPress,
    XButtonRelease = ButtonRelease
};
#undef FocusOut
#undef FocusIn
#undef KeyPress
#undef KeyRelease
#undef None
#undef GrayScale
#undef CursorShape
#undef Unsorted
#undef FontChange
#undef ButtonPress
#undef ButtonRelease

Q_DECLARE_TYPEINFO(XPoint, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(XRectangle, Q_PRIMITIVE_TYPE);
#ifndef QT_NO_XRENDER
Q_DECLARE_TYPEINFO(XGlyphElt32, Q_PRIMITIVE_TYPE);
#endif


QT_END_NAMESPACE

#endif // QT_X11_P_H
