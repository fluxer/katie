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

#include "qdebug.h"
#include "qwidget.h"
#include "qtextcodec.h"
#include "qkeymapper_p.h"
#include "qapplication_p.h"
#include "qcorecommon_p.h"
#include "qt_x11_p.h"

QT_BEGIN_NAMESPACE

// from qapplication_x11.cpp
extern bool qt_sendSpontaneousEvent(QObject*, QEvent*);

QKeyMapper::QKeyMapper()
    : keyboardInputDirection(Qt::LeftToRight),
    keyMapperCodec(QTextCodec::codecForCStrings())
{
    clearMappings();
}

QKeyMapper::~QKeyMapper()
{
}

void QKeyMapper::clearMappings()
{
    XIM im = XOpenIM(qt_x11Data->display, NULL, NULL, NULL);
    if (!im) {
        return;
    }
    const QString imlocale = QString::fromLatin1(XLocaleOfIM(im));
    keyboardInputLocale = QLocale(imlocale);
    XCloseIM(im);

    keyboardInputDirection = keyboardInputLocale.textDirection();

    // X11 is known to use whatever is set by setlocale() as input method
    // locale, setlocale() and XLocaleOfIM() return string in the form:
    // language[_territory][.codeset][@modifier]
    const int dotindex = imlocale.indexOf(QLatin1Char('.'));
    if (dotindex >= 0) {
        QByteArray codeset = imlocale.mid(dotindex + 1).toLatin1();
        const int atindex = codeset.indexOf('@');
        if (atindex >= 0) {
            codeset = codeset.left(atindex);
        }
        keyMapperCodec = QTextCodec::codecForName(codeset.constData());
    }
}

extern bool qt_sm_blockUserInput;

// keyboard mapping table
static const struct KeyTblData {
    const uint x11key;
    const Qt::Key qtkey;
} KeyTbl[] = {
    // remap all keypad keys for compatibility
    { XK_KP_Space,                Qt::Key_Space },
    { XK_KP_Tab,                  Qt::Key_Tab },
    // { XK_KP_Enter,                Qt::Key_Enter },
    { XK_KP_F1,                   Qt::Key_F1 },
    { XK_KP_F2,                   Qt::Key_F2 },
    { XK_KP_F3,                   Qt::Key_F3 },
    { XK_KP_F4,                   Qt::Key_F4 },
    { XK_KP_Home,                 Qt::Key_Home },
    { XK_KP_Left,                 Qt::Key_Left },
    { XK_KP_Up,                   Qt::Key_Up },
    { XK_KP_Right,                Qt::Key_Right },
    { XK_KP_Down,                 Qt::Key_Down },
    { XK_KP_Prior,                Qt::Key_PageUp },
    { XK_KP_Page_Up,              Qt::Key_Page_Up },
    { XK_KP_Next,                 Qt::Key_PageDown },
    { XK_KP_Page_Down,            Qt::Key_Page_Down },
    { XK_KP_End,                  Qt::Key_End },
    { XK_KP_Begin,                Qt::Key_Clear },
    { XK_KP_Insert,               Qt::Key_Insert },
    { XK_KP_Delete,               Qt::Key_Delete },
    { XK_KP_Equal,                Qt::Key_Equal },
    { XK_KP_Multiply,             Qt::Key_Asterisk },
    { XK_KP_Add,                  Qt::Key_Plus },
    { XK_KP_Separator,            Qt::Key_Comma },
    { XK_KP_Subtract,             Qt::Key_Minus },
    { XK_KP_Decimal,              Qt::Key_Period },
    { XK_KP_Divide,               Qt::Key_Slash },
    { XK_KP_0,                    Qt::Key_0 },
    { XK_KP_1,                    Qt::Key_1 },
    { XK_KP_2,                    Qt::Key_2 },
    { XK_KP_3,                    Qt::Key_3 },
    { XK_KP_4,                    Qt::Key_4 },
    { XK_KP_5,                    Qt::Key_5 },
    { XK_KP_6,                    Qt::Key_6 },
    { XK_KP_7,                    Qt::Key_7 },
    { XK_KP_8,                    Qt::Key_8 },
    { XK_KP_9,                    Qt::Key_9 },

    // remap lower-case to upper for compatibility
    { XK_a,                       Qt::Key_A },
    { XK_b,                       Qt::Key_B },
    { XK_c,                       Qt::Key_C },
    { XK_d,                       Qt::Key_D },
    { XK_e,                       Qt::Key_E },
    { XK_f,                       Qt::Key_F },
    { XK_g,                       Qt::Key_G },
    { XK_h,                       Qt::Key_H },
    { XK_i,                       Qt::Key_I },
    { XK_j,                       Qt::Key_J },
    { XK_k,                       Qt::Key_K },
    { XK_l,                       Qt::Key_L },
    { XK_m,                       Qt::Key_M },
    { XK_n,                       Qt::Key_N },
    { XK_o,                       Qt::Key_O },
    { XK_p,                       Qt::Key_P },
    { XK_q,                       Qt::Key_Q },
    { XK_r,                       Qt::Key_R },
    { XK_s,                       Qt::Key_S },
    { XK_t,                       Qt::Key_T },
    { XK_u,                       Qt::Key_U },
    { XK_v,                       Qt::Key_V },
    { XK_w,                       Qt::Key_W },
    { XK_x,                       Qt::Key_X },
    { XK_y,                       Qt::Key_Y },
    { XK_z,                       Qt::Key_Z },

    // pseudo
    { XK_Shift_L,                 Qt::Key_Shift },
    { XK_Shift_R,                 Qt::Key_Shift },
    { XK_Shift_Lock,              Qt::Key_Shift },
    { XK_Control_L,               Qt::Key_Control },
    { XK_Control_R,               Qt::Key_Control },
    { XK_Meta_L,                  Qt::Key_Meta },
    { XK_Meta_R,                  Qt::Key_Meta },
    { XK_Alt_L,                   Qt::Key_Alt },
    { XK_Alt_R,                   Qt::Key_Alt },
    { 0x1000FF74,                 Qt::Key_Backtab },        // hardcoded HP backtab
    { 0xfd05,                     Qt::Key_Backtab },        // hardcoded XK_3270_BackTab

    // misc
    { XK_Clear,                   Qt::Key_Delete },
    { 0x1005FF60,                 Qt::Key_SysReq },         // hardcoded Sun SysReq
    { 0x1007ff00,                 Qt::Key_SysReq },         // hardcoded X386 SysReq
    { XK_Prior,                   Qt::Key_PageUp },
    { XK_Next,                    Qt::Key_PageDown },
    { 0x1005FF10,                 Qt::Key_F11 },            // hardcoded Sun F36 (labeled F11)
    { 0x1005FF11,                 Qt::Key_F12 },            // hardcoded Sun F37 (labeled F12)
    { XK_script_switch,           Qt::Key_Mode_switch },
    { XK_Henkan_Mode,             Qt::Key_Henkan },
    { XK_Kanji_Bangou,            Qt::Key_Codeinput },
    { XK_Zen_Koho,                Qt::Key_MultipleCandidate },
    { XK_Hangul_Codeinput,        Qt::Key_Codeinput },
    { XK_Hangul_SingleCandidate,  Qt::Key_SingleCandidate },
    { XK_Hangul_MultipleCandidate,Qt::Key_MultipleCandidate },
    { XK_Hangul_PreviousCandidate,Qt::Key_PreviousCandidate },
    { XK_Hangul_switch,           Qt::Key_Mode_switch },
};
static const qint16 KeyTblSize = sizeof(KeyTbl) / sizeof(KeyTblData);

static int translateKeySym(const uint key)
{
    for (qint16 i = 0; i < KeyTblSize; i++) {
        if (KeyTbl[i].x11key == key) {
            return KeyTbl[i].qtkey;
        }
    }
    return key;
}

static int getX11AutoRepeat() {
    XKeyboardState state;
    XGetKeyboardControl(qt_x11Data->display, &state);
    if (state.global_auto_repeat == AutoRepeatModeOn) {
        // NOTE: according to `xset -q` the delay is 250,
        // QApplication::keyboardInputInterval() is set to 400 but it does not
        // have the same meaning nor is it clear if the events are delayed by
        // the delay value X11 holds or they appear in the range of the value
        // so the time-window for auto repeat events that are registered is
        // likely incorrect. there are two ways to query the actual X11 value,
        // either via XkbGetControls() or XF86MiscGetKbdSettings() but the
        // second is deprecated
        return QApplication::keyboardInputInterval();
    }
    return -1;
}

struct qt_auto_repeat_data
{
    // simulated auto-repeat X11 events have the same serial
    ulong serial;
    // anything else is guesswork, X11 events queue may be shuffled
    Window window;
    unsigned int keycode;
    Time time;
};

bool QKeyMapper::translateKeyEvent(QWidget *keyWidget, const XEvent *event)
{
    if (qt_sm_blockUserInput) // block user interaction during session management
        return true;

    Q_ASSERT(keyWidget->isEnabled());

    QEvent::Type type = (event->type == XKeyPress) ? QEvent::KeyPress : QEvent::KeyRelease;
    Qt::KeyboardModifiers modifiers = translateModifiers(event->xkey.state);
    XKeyEvent xkeyevent = event->xkey;
    QSTACKARRAY(char, lookupbuff, 10);
    KeySym keysym = 0;
    int count = XLookupString(&xkeyevent, lookupbuff, sizeof(lookupbuff), &keysym, 0);
    int code = translateKeySym(keysym);
    QString text = keyMapperCodec->toUnicode(lookupbuff, count);

    bool autorepeat = false;
    static const int qt_x11_autorepeat = getX11AutoRepeat();
    // modifier keys should not auto-repeat
    if (qt_x11_autorepeat && code != Qt::Key_Shift && code != Qt::Key_Control
        && code != Qt::Key_Meta && code != Qt::Key_Alt) {
        static qt_auto_repeat_data curr_autorep = { 0, 0, 0, 0 };
        if (curr_autorep.serial == event->xkey.serial ||
            (event->xkey.window == curr_autorep.window &&
            event->xkey.keycode == curr_autorep.keycode &&
            event->xkey.time - curr_autorep.time < qulonglong(qt_x11_autorepeat))) {
            autorepeat = true;
        }
        curr_autorep = {
            event->xkey.serial,
            event->xkey.window,
            event->xkey.keycode,
            event->xkey.time
        };
    }

#if 0
    qDebug() << "translateKeyEvent" << modifiers << count << text << autorepeat << qt_x11_autorepeat;
#endif

    // try the menu key first
    if (type == QEvent::KeyPress && code == Qt::Key_Menu) {
        QPoint globalPos = QCursor::pos();
        QPoint pos = keyWidget->mapFromGlobal(globalPos);
        QContextMenuEvent e(QContextMenuEvent::Keyboard, pos, globalPos);
        qt_sendSpontaneousEvent(keyWidget, &e);
        if (e.isAccepted())
            return true;
    }

    QKeyEvent e(type, code, modifiers,
                  event->xkey.keycode, keysym, event->xkey.state,
                  text, autorepeat, qMax(qMax(count, 1), text.length()));
    return qt_sendSpontaneousEvent(keyWidget, &e);
}

Qt::KeyboardModifiers QKeyMapper::translateModifiers(int state)
{
    // xmodmap -pm
    Qt::KeyboardModifiers ret = 0;
    if (state & ShiftMask)
        ret |= Qt::ShiftModifier;
    if (state & ControlMask)
        ret |= Qt::ControlModifier;
    if (state & Mod1Mask)
        ret |= Qt::AltModifier;
    if (state & Mod4Mask)
        ret |= Qt::MetaModifier;
    if (state & Mod2Mask)
        ret |= Qt::KeypadModifier;
    if (state & Mod3Mask)
        ret |= Qt::GroupSwitchModifier;

#if 0
    qDebug() << "translateModifiers" << state << ret;
#endif

    return ret;
}

QT_END_NAMESPACE
