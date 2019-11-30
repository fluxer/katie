/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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

#include "qkeymapper_p.h"

#include "qdebug.h"
#include "qtextcodec.h"
#include "qwidget.h"

#include "qapplication_p.h"
#include "qevent_p.h"
#include "qt_x11_p.h"

#include <ctype.h>

#ifdef QT_LINUXBASE
// LSB's IsKeypadKey define is wrong - see
// http://bugs.linuxbase.org/show_bug.cgi?id=2521
#undef IsKeypadKey
#define IsKeypadKey(keysym) \
      (((KeySym)(keysym) >= XK_KP_Space) && ((KeySym)(keysym) <= XK_KP_Equal))

#undef IsPrivateKeypadKey
#define IsPrivateKeypadKey(keysym) \
      (((KeySym)(keysym) >= 0x11000000) && ((KeySym)(keysym) <= 0x1100FFFF))
#endif

QT_BEGIN_NAMESPACE

// from qapplication_x11.cpp
extern uchar qt_alt_mask;
extern uchar qt_meta_mask;
extern uchar qt_super_mask;
extern uchar qt_hyper_mask;
extern uchar qt_mode_switch_mask;
uchar qt_num_lock_mask = 0;
extern bool qt_sendSpontaneousEvent(QObject*, QEvent*);

// ### we should really resolve conflicts with other masks by
// ### decomposing the Qt::KeyboardModifers in possibleKeys()
#define SETMASK(sym, mask)                                              \
    do {                                                                \
        if (qt_alt_mask == 0                                            \
            && qt_meta_mask != mask                                     \
            && qt_super_mask != mask                                    \
            && qt_hyper_mask != mask                                    \
            && (sym == XK_Alt_L || sym == XK_Alt_R)) {                  \
            qt_alt_mask = mask;                                         \
        }                                                               \
        if (qt_meta_mask == 0                                           \
            && qt_alt_mask != mask                                      \
            && qt_super_mask != mask                                    \
            && qt_hyper_mask != mask                                    \
            && (sym == XK_Meta_L || sym == XK_Meta_R)) {                \
            qt_meta_mask = mask;                                        \
        }                                                               \
        if (qt_super_mask == 0                                          \
            && qt_alt_mask != mask                                      \
            && qt_meta_mask != mask                                     \
            && qt_hyper_mask != mask                                    \
            && (sym == XK_Super_L || sym == XK_Super_R)) {              \
            qt_super_mask = mask;                                       \
        }                                                               \
        if (qt_hyper_mask == 0                                          \
            && qt_alt_mask != mask                                      \
            && qt_meta_mask != mask                                     \
            && qt_super_mask != mask                                    \
            && (sym == XK_Hyper_L || sym == XK_Hyper_R)) {              \
            qt_hyper_mask = mask;                                       \
        }                                                               \
        if (qt_mode_switch_mask == 0                                    \
            && qt_alt_mask != mask                                      \
            && qt_meta_mask != mask                                     \
            && qt_super_mask != mask                                    \
            && qt_hyper_mask != mask                                    \
            && sym == XK_Mode_switch) {                                 \
            qt_mode_switch_mask = mask;                                 \
        }                                                               \
        if (qt_num_lock_mask == 0                                       \
            && sym == XK_Num_Lock) {                                    \
            qt_num_lock_mask = mask;                                    \
        }                                                               \
    } while(false)

// qt_XTranslateKey() is based on _XTranslateKey() taken from:

/* $Xorg: KeyBind.c,v 1.4 2001/02/09 02:03:34 xorgcvs Exp $ */

/*

Copyright 1985, 1987, 1998  The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from The Open Group.

*/
static int
qt_XTranslateKey(QXCoreDesc *dpy,
                 KeyCode keycode,
                 unsigned int modifiers,
                 unsigned int *modifiers_return,
                 KeySym *keysym_return)
{
    int per;
    KeySym *syms;
    KeySym sym, lsym, usym;

    if (! dpy->keysyms)
	return 0;
    *modifiers_return = ((ShiftMask|LockMask)
			 | dpy->mode_switch | dpy->num_lock);
    if (((int)keycode < dpy->min_keycode) || ((int)keycode > dpy->max_keycode))
    {
	*keysym_return = NoSymbol;
	return 1;
    }
    per = dpy->keysyms_per_keycode;
    syms = &dpy->keysyms[(keycode - dpy->min_keycode) * per];
    while ((per > 2) && (syms[per - 1] == NoSymbol))
	per--;
    if ((per > 2) && (modifiers & dpy->mode_switch)) {
	syms += 2;
	per -= 2;
    }
    if ((modifiers & dpy->num_lock) &&
	(per > 1 && (IsKeypadKey(syms[1]) || IsPrivateKeypadKey(syms[1])))) {
	if ((modifiers & ShiftMask) ||
	    ((modifiers & LockMask) && (dpy->lock_meaning == XK_Shift_Lock)))
	    *keysym_return = syms[0];
	else
	    *keysym_return = syms[1];
    } else if (!(modifiers & ShiftMask) &&
	(!(modifiers & LockMask) || (dpy->lock_meaning == NoSymbol))) {
	if ((per == 1) || (syms[1] == NoSymbol))
	    XConvertCase(syms[0], keysym_return, &usym);
	else
	    *keysym_return = syms[0];
    } else if (!(modifiers & LockMask) ||
	       (dpy->lock_meaning != XK_Caps_Lock)) {
	if ((per == 1) || ((usym = syms[1]) == NoSymbol))
	    XConvertCase(syms[0], &lsym, &usym);
	*keysym_return = usym;
    } else {
	if ((per == 1) || ((sym = syms[1]) == NoSymbol))
	    sym = syms[0];
	XConvertCase(sym, &lsym, &usym);
	if (!(modifiers & ShiftMask) && (sym != syms[0]) &&
	    ((sym != usym) || (lsym == usym)))
	    XConvertCase(syms[0], &lsym, &usym);
	*keysym_return = usym;
    }
    if (*keysym_return == XK_VoidSymbol)
	*keysym_return = NoSymbol;
    return 1;
}




QKeyMapperPrivate::QKeyMapperPrivate()
    : keyboardInputDirection(Qt::LeftToRight)
{
    memset(&coreDesc, 0, sizeof(coreDesc));
}

QKeyMapperPrivate::~QKeyMapperPrivate()
{
    if (coreDesc.keysyms)
        XFree(coreDesc.keysyms);
}

enum { MaxKeyBits = sizeof(uint) * 8 };
static QString translateKeySym(KeySym keysym, uint xmodifiers,
                               int &code, Qt::KeyboardModifiers &modifiers,
                               QByteArray &chars, int &count);

QList<int> QKeyMapperPrivate::possibleKeys(QKeyEvent *event)
{
    const int xkeycode = event->nativeScanCode();
    const uint xmodifiers = event->nativeModifiers();

    // first, translate key only using lock modifiers (there are no Qt equivalents for these, so we must
    // always use them when determining the baseKeySym)
    KeySym baseKeySym;
    uint consumedModifiers;
    if (!qt_XTranslateKey(&coreDesc, xkeycode, (xmodifiers & (LockMask | qt_num_lock_mask)),
                          &consumedModifiers, &baseKeySym))
        return QList<int>();

    QList<int> result;

    // translate sym -> code
    Qt::KeyboardModifiers baseModifiers = 0;
    int baseCode = -1;
    QByteArray chars;
    int count = 0;
    QString text = translateKeySym(baseKeySym, xmodifiers, baseCode, baseModifiers, chars, count);
    if (baseCode == -1) {
        if (text.isEmpty())
            return QList<int>();
        baseCode = text.at(0).unicode();
    }

    if (baseCode && baseCode < 0xfffe)
        baseCode = QChar(baseCode).toUpper().unicode();
    result += (baseCode | baseModifiers);

    int pos1Bits[MaxKeyBits];
    int num1Bits = 0;

    for (int i = 0; i < MaxKeyBits; ++i) {
        if (consumedModifiers & (1 << i))
            pos1Bits[num1Bits++] = i;
    }

    const int numPerms = (1 << num1Bits);

    // translate the key again using each permutation of consumedModifiers
    for (int i = 1; i < numPerms; ++i) {
        uint val = 0;
        for (int j = 0; j < num1Bits; ++j) {
            if (i & (1 << j))
                val |= (1 << pos1Bits[j]);
        }

        if ((xmodifiers & val) != val)
            continue;

        KeySym sym;
        uint mods;
        if (!qt_XTranslateKey(&coreDesc, xkeycode, val, &mods, &sym))
            continue;

        // translate sym -> code
        Qt::KeyboardModifiers modifiers = 0;
        int code = -1;
        chars.clear();
        count = 0;
        // mask out the modifiers needed to translate keycode
        text = translateKeySym(sym, xmodifiers & ~val, code, modifiers, chars, count);
        if (code == -1) {
            if (text.isEmpty())
                continue;
            code = text.at(0).unicode();
        }

        if (code && code < 0xfffe)
            code = QChar(code).toUpper().unicode();

        if (code == Qt::Key_Tab && (baseModifiers & Qt::ShiftModifier)) {
            // map shift+tab to shift+backtab
            code = Qt::Key_Backtab;
            text = QString();
        }

        if (code == baseCode)
            continue;

        result += (code | modifiers);
    }

#if 0
    qDebug() << "possibleKeys()" << hex << result;
#endif
    return result;
}

void QKeyMapperPrivate::clearMappings()
{
    if (coreDesc.keysyms)
        XFree(coreDesc.keysyms);

    coreDesc.min_keycode = 8;
    coreDesc.max_keycode = 255;
    XDisplayKeycodes(qt_x11Data->display, &coreDesc.min_keycode, &coreDesc.max_keycode);

    coreDesc.keysyms_per_keycode = 0;
    coreDesc.keysyms = XGetKeyboardMapping(qt_x11Data->display,
                                            coreDesc.min_keycode,
                                            coreDesc.max_keycode - coreDesc.min_keycode + 1,
                                            &coreDesc.keysyms_per_keycode);

#if 0
    qDebug() << "min_keycode =" << coreDesc.min_keycode;
    qDebug() << "max_keycode =" << coreDesc.max_keycode;
    qDebug() << "keysyms_per_keycode =" << coreDesc.keysyms_per_keycode;
    qDebug() << "keysyms =" << coreDesc.keysyms;
#endif

    // ### cannot get/guess the locale with the core protocol
    keyboardInputLocale = QLocale::c();
    // ### could examine group 0 for RTL keys
    keyboardInputDirection = Qt::LeftToRight;

    qt_alt_mask = 0;
    qt_meta_mask = 0;
    qt_super_mask = 0;
    qt_hyper_mask = 0;
    qt_mode_switch_mask = 0;

    coreDesc.lock_meaning = NoSymbol;

    XModifierKeymap *map = XGetModifierMapping(qt_x11Data->display);

    if (map) {
        int i, maskIndex = 0, mapIndex = 0;
        for (maskIndex = 0; maskIndex < 8; maskIndex++) {
            for (i = 0; i < map->max_keypermod; i++) {
                if (map->modifiermap[mapIndex]) {
                    KeySym sym;
                    int x = 0;
                    do {
                        sym = XKeycodeToKeysym(qt_x11Data->display, map->modifiermap[mapIndex], x++);
                    } while (sym == NoSymbol && x < coreDesc.keysyms_per_keycode);
                    const uchar mask = 1 << maskIndex;
                    SETMASK(sym, mask);
                }
                mapIndex++;
            }
        }

        // determine the meaning of the Lock modifier
        for (i = 0; i < map->max_keypermod; ++i) {
            for (int x = 0; x < coreDesc.keysyms_per_keycode; ++x) {
                KeySym sym = XKeycodeToKeysym(qt_x11Data->display, map->modifiermap[LockMapIndex], x);
                if (sym == XK_Caps_Lock || sym == XK_ISO_Lock) {
                    coreDesc.lock_meaning = XK_Caps_Lock;
                    break;
                } else if (sym == XK_Shift_Lock) {
                    coreDesc.lock_meaning = XK_Shift_Lock;
                }
            }
        }

        XFreeModifiermap(map);
    }

    // for qt_XTranslateKey()
    coreDesc.num_lock = qt_num_lock_mask;
    coreDesc.mode_switch = qt_mode_switch_mask;

#if 0
    qDebug() << "lock_meaning =" << coreDesc.lock_meaning;
    qDebug() << "num_lock =" << coreDesc.num_lock;
    qDebug() << "mode_switch =" << coreDesc.mode_switch;
#endif

    // set default modifier masks if needed
    if( qt_alt_mask == 0 )
        qt_alt_mask = Mod1Mask;
    if( qt_meta_mask == 0 )
        qt_meta_mask = Mod4Mask;

    // if we don't have a meta key (or it's hidden behind alt), use super or hyper to generate
    // Qt::Key_Meta and Qt::MetaModifier, since most newer XFree86/Xorg installations map the Windows
    // key to Super
    if (qt_meta_mask == 0 || qt_meta_mask == qt_alt_mask) {
        // no meta keys... s,meta,super,
        qt_meta_mask = qt_super_mask;
        if (qt_meta_mask == 0 || qt_meta_mask == qt_alt_mask) {
            // no super keys either? guess we'll use hyper then
            qt_meta_mask = qt_hyper_mask;
        }
    }

#if 0
    qDebug() << "qt_alt_mask =" << hex << qt_alt_mask;
    qDebug() << "qt_meta_mask =" << hex << qt_meta_mask;
    qDebug() << "qt_super_mask =" << hex << qt_super_mask;
    qDebug() << "qt_hyper_mask =" << hex << qt_hyper_mask;
    qDebug() << "qt_mode_switch_mask =" << hex << qt_mode_switch_mask;
    qDebug() << "qt_num_lock_mask =" << hex << qt_num_lock_mask;
#endif
}

extern bool qt_sm_blockUserInput;

//
// Keyboard event translation
//

// keyboard mapping table
static const struct KeyTblData {
    const uint x11key;
    const Qt::Key qtkey;
} KeyTbl[] = {
    // misc keys
    { XK_Escape,                  Qt::Key_Escape } ,
    { XK_Tab,                     Qt::Key_Tab },
    { XK_ISO_Left_Tab,            Qt::Key_Backtab },
    { XK_BackSpace,               Qt::Key_Backspace },
    { XK_Return,                  Qt::Key_Return },
    { XK_Insert,                  Qt::Key_Insert },
    { XK_Delete,                  Qt::Key_Delete },
    { XK_Clear,                   Qt::Key_Delete },
    { XK_Pause,                   Qt::Key_Pause },
    { XK_Print,                   Qt::Key_Print },
    { 0x1005FF60,                 Qt::Key_SysReq },         // hardcoded Sun SysReq
    { 0x1007ff00,                 Qt::Key_SysReq },         // hardcoded X386 SysReq

    // cursor movement
    { XK_Home,                    Qt::Key_Home },
    { XK_End,                     Qt::Key_End },
    { XK_Left,                    Qt::Key_Left },
    { XK_Up,                      Qt::Key_Up },
    { XK_Right,                   Qt::Key_Right },
    { XK_Down,                    Qt::Key_Down },
    { XK_Prior,                   Qt::Key_PageUp },
    { XK_Next,                    Qt::Key_PageDown },

    // modifiers
    { XK_Shift_L,                 Qt::Key_Shift },
    { XK_Shift_R,                 Qt::Key_Shift },
    { XK_Shift_Lock,              Qt::Key_Shift },
    { XK_Control_L,               Qt::Key_Control },
    { XK_Control_R,               Qt::Key_Control },
    { XK_Meta_L,                  Qt::Key_Meta },
    { XK_Meta_R,                  Qt::Key_Meta },
    { XK_Alt_L,                   Qt::Key_Alt },
    { XK_Alt_R,                   Qt::Key_Alt },
    { XK_Caps_Lock,               Qt::Key_CapsLock },
    { XK_Num_Lock,                Qt::Key_NumLock },
    { XK_Scroll_Lock,             Qt::Key_ScrollLock },
    { XK_Super_L,                 Qt::Key_Super_L },
    { XK_Super_R,                 Qt::Key_Super_R },
    { XK_Menu,                    Qt::Key_Menu },
    { XK_Hyper_L,                 Qt::Key_Hyper_L },
    { XK_Hyper_R,                 Qt::Key_Hyper_R },
    { XK_Help,                    Qt::Key_Help },
    { 0x1000FF74,                 Qt::Key_Backtab },        // hardcoded HP backtab
    { 0x1005FF10,                 Qt::Key_F11 },            // hardcoded Sun F36 (labeled F11)
    { 0x1005FF11,                 Qt::Key_F12 },            // hardcoded Sun F37 (labeled F12)

    // numeric and function keypad keys
    { XK_KP_Space,                Qt::Key_Space },
    { XK_KP_Tab,                  Qt::Key_Tab },
    { XK_KP_Enter,                Qt::Key_Enter },
    // { XK_KP_F1,                 Qt::Key_F1 },
    // { XK_KP_F2,                 Qt::Key_F2 },
    // { XK_KP_F3,                 Qt::Key_F3 },
    // { XK_KP_F4,                 Qt::Key_F4 },
    { XK_KP_Home,                 Qt::Key_Home },
    { XK_KP_Left,                 Qt::Key_Left },
    { XK_KP_Up,                   Qt::Key_Up },
    { XK_KP_Right,                Qt::Key_Right },
    { XK_KP_Down,                 Qt::Key_Down },
    { XK_KP_Prior,                Qt::Key_PageUp },
    { XK_KP_Next,                 Qt::Key_PageDown },
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

    // International input method support keys

    // International & multi-key character composition
    { XK_ISO_Level3_Shift,        Qt::Key_AltGr },
    { XK_Multi_key,               Qt::Key_Multi_key },
    { XK_Codeinput,               Qt::Key_Codeinput },
    { XK_SingleCandidate,         Qt::Key_SingleCandidate },
    { XK_MultipleCandidate,       Qt::Key_MultipleCandidate },
    { XK_PreviousCandidate,       Qt::Key_PreviousCandidate },

    // Misc Functions
    { XK_Mode_switch,             Qt::Key_Mode_switch },
    { XK_script_switch,           Qt::Key_Mode_switch },

    // Japanese keyboard support
    { XK_Kanji,                   Qt::Key_Kanji },
    { XK_Muhenkan,                Qt::Key_Muhenkan },
    // { XK_Henkan_Mode,           Qt::Key_Henkan_Mode },
    { XK_Henkan_Mode,             Qt::Key_Henkan },
    { XK_Henkan,                  Qt::Key_Henkan },
    { XK_Romaji,                  Qt::Key_Romaji },
    { XK_Hiragana,                Qt::Key_Hiragana },
    { XK_Katakana,                Qt::Key_Katakana },
    { XK_Hiragana_Katakana,       Qt::Key_Hiragana_Katakana },
    { XK_Zenkaku,                 Qt::Key_Zenkaku },
    { XK_Hankaku,                 Qt::Key_Hankaku },
    { XK_Zenkaku_Hankaku,         Qt::Key_Zenkaku_Hankaku },
    { XK_Touroku,                 Qt::Key_Touroku },
    { XK_Massyo,                  Qt::Key_Massyo },
    { XK_Kana_Lock,               Qt::Key_Kana_Lock },
    { XK_Kana_Shift,              Qt::Key_Kana_Shift },
    { XK_Eisu_Shift,              Qt::Key_Eisu_Shift },
    { XK_Eisu_toggle,             Qt::Key_Eisu_toggle },
    // { XK_Kanji_Bangou,          Qt::Key_Kanji_Bangou },
    // { XK_Zen_Koho,              Qt::Key_Zen_Koho },
    // { XK_Mae_Koho,              Qt::Key_Mae_Koho },
    { XK_Kanji_Bangou,            Qt::Key_Codeinput },
    { XK_Zen_Koho,                Qt::Key_MultipleCandidate },
    { XK_Mae_Koho,                Qt::Key_PreviousCandidate },

#ifdef XK_KOREAN
    // Korean keyboard support
    { XK_Hangul,                  Qt::Key_Hangul },
    { XK_Hangul_Start,            Qt::Key_Hangul_Start },
    { XK_Hangul_End,              Qt::Key_Hangul_End },
    { XK_Hangul_Hanja,            Qt::Key_Hangul_Hanja },
    { XK_Hangul_Jamo,             Qt::Key_Hangul_Jamo },
    { XK_Hangul_Romaja,           Qt::Key_Hangul_Romaja },
    // { XK_Hangul_Codeinput,      Qt::Key_Hangul_Codeinput },
    { XK_Hangul_Codeinput,        Qt::Key_Codeinput },
    { XK_Hangul_Jeonja,           Qt::Key_Hangul_Jeonja },
    { XK_Hangul_Banja,            Qt::Key_Hangul_Banja },
    { XK_Hangul_PreHanja,         Qt::Key_Hangul_PreHanja },
    { XK_Hangul_PostHanja,        Qt::Key_Hangul_PostHanja },
    // { XK_Hangul_SingleCandidate, Qt::Key_Hangul_SingleCandidate },
    // { XK_Hangul_MultipleCandidate, Qt::Key_Hangul_MultipleCandidate },
    // { XK_Hangul_PreviousCandidate, Qt::Key_Hangul_PreviousCandidate },
    { XK_Hangul_SingleCandidate,  Qt::Key_SingleCandidate },
    { XK_Hangul_MultipleCandidate,Qt::Key_MultipleCandidate },
    { XK_Hangul_PreviousCandidate,Qt::Key_PreviousCandidate },
    { XK_Hangul_Special,          Qt::Key_Hangul_Special },
    // { XK_Hangul_switch,         Qt::Key_Hangul_switch },
    { XK_Hangul_switch,           Qt::Key_Mode_switch },
#endif  // XK_KOREAN

    // dead keys
    { XK_dead_grave,              Qt::Key_Dead_Grave },
    { XK_dead_acute,              Qt::Key_Dead_Acute },
    { XK_dead_circumflex,         Qt::Key_Dead_Circumflex },
    { XK_dead_tilde,              Qt::Key_Dead_Tilde },
    { XK_dead_macron,             Qt::Key_Dead_Macron },
    { XK_dead_breve,              Qt::Key_Dead_Breve },
    { XK_dead_abovedot,           Qt::Key_Dead_Abovedot },
    { XK_dead_diaeresis,          Qt::Key_Dead_Diaeresis },
    { XK_dead_abovering,          Qt::Key_Dead_Abovering },
    { XK_dead_doubleacute,        Qt::Key_Dead_Doubleacute },
    { XK_dead_caron,              Qt::Key_Dead_Caron },
    { XK_dead_cedilla,            Qt::Key_Dead_Cedilla },
    { XK_dead_ogonek,             Qt::Key_Dead_Ogonek },
    { XK_dead_iota,               Qt::Key_Dead_Iota },
    { XK_dead_voiced_sound,       Qt::Key_Dead_Voiced_Sound },
    { XK_dead_semivoiced_sound,   Qt::Key_Dead_Semivoiced_Sound },
    { XK_dead_belowdot,           Qt::Key_Dead_Belowdot },
    { XK_dead_hook,               Qt::Key_Dead_Hook },
    { XK_dead_horn,               Qt::Key_Dead_Horn },

    // Special keys from X.org - This include multimedia keys,
    // wireless/bluetooth/uwb keys, special launcher keys, etc.
    { XF86XK_Back,                Qt::Key_Back },
    { XF86XK_Forward,             Qt::Key_Forward },
    { XF86XK_Stop,                Qt::Key_Stop },
    { XF86XK_Refresh,             Qt::Key_Refresh },
    { XF86XK_Favorites,           Qt::Key_Favorites },
    { XF86XK_AudioMedia,          Qt::Key_LaunchMedia },
    { XF86XK_OpenURL,             Qt::Key_OpenUrl },
    { XF86XK_HomePage,            Qt::Key_HomePage },
    { XF86XK_Search,              Qt::Key_Search },
    { XF86XK_AudioLowerVolume,    Qt::Key_VolumeDown },
    { XF86XK_AudioMute,           Qt::Key_VolumeMute },
    { XF86XK_AudioRaiseVolume,    Qt::Key_VolumeUp },
    { XF86XK_AudioPlay,           Qt::Key_MediaPlay },
    { XF86XK_AudioStop,           Qt::Key_MediaStop },
    { XF86XK_AudioPrev,           Qt::Key_MediaPrevious },
    { XF86XK_AudioNext,           Qt::Key_MediaNext },
    { XF86XK_AudioRecord,         Qt::Key_MediaRecord },
    { XF86XK_Mail,                Qt::Key_LaunchMail },
    { XF86XK_MyComputer,          Qt::Key_Launch0 },  // ### Qt 5: remap properly
    { XF86XK_Calculator,          Qt::Key_Calculator },
    { XF86XK_Memo,                Qt::Key_Memo },
    { XF86XK_ToDoList,            Qt::Key_ToDoList },
    { XF86XK_Calendar,            Qt::Key_Calendar },
    { XF86XK_PowerDown,           Qt::Key_PowerDown },
    { XF86XK_ContrastAdjust,      Qt::Key_ContrastAdjust },
    { XF86XK_Standby,             Qt::Key_Standby },
    { XF86XK_MonBrightnessUp,     Qt::Key_MonBrightnessUp },
    { XF86XK_MonBrightnessDown,   Qt::Key_MonBrightnessDown },
    { XF86XK_KbdLightOnOff,       Qt::Key_KeyboardLightOnOff },
    { XF86XK_KbdBrightnessUp,     Qt::Key_KeyboardBrightnessUp },
    { XF86XK_KbdBrightnessDown,   Qt::Key_KeyboardBrightnessDown },
    { XF86XK_PowerOff,            Qt::Key_PowerOff },
    { XF86XK_WakeUp,              Qt::Key_WakeUp },
    { XF86XK_Eject,               Qt::Key_Eject },
    { XF86XK_ScreenSaver,         Qt::Key_ScreenSaver },
    { XF86XK_WWW,                 Qt::Key_WWW },
    { XF86XK_Sleep,               Qt::Key_Sleep },
    { XF86XK_LightBulb,           Qt::Key_LightBulb },
    { XF86XK_Shop,                Qt::Key_Shop },
    { XF86XK_History,             Qt::Key_History },
    { XF86XK_AddFavorite,         Qt::Key_AddFavorite },
    { XF86XK_HotLinks,            Qt::Key_HotLinks },
    { XF86XK_BrightnessAdjust,    Qt::Key_BrightnessAdjust },
    { XF86XK_Finance,             Qt::Key_Finance },
    { XF86XK_Community,           Qt::Key_Community },
    { XF86XK_AudioRewind,         Qt::Key_AudioRewind },
    { XF86XK_BackForward,         Qt::Key_BackForward },
    { XF86XK_ApplicationLeft,     Qt::Key_ApplicationLeft },
    { XF86XK_ApplicationRight,    Qt::Key_ApplicationRight },
    { XF86XK_Book,                Qt::Key_Book },
    { XF86XK_CD,                  Qt::Key_CD },
    { XF86XK_Calculater,          Qt::Key_Calculator },
    { XF86XK_Clear,               Qt::Key_Clear },
    { XF86XK_ClearGrab,           Qt::Key_ClearGrab },
    { XF86XK_Close,               Qt::Key_Close },
    { XF86XK_Copy,                Qt::Key_Copy },
    { XF86XK_Cut,                 Qt::Key_Cut },
    { XF86XK_Display,             Qt::Key_Display },
    { XF86XK_DOS,                 Qt::Key_DOS },
    { XF86XK_Documents,           Qt::Key_Documents },
    { XF86XK_Excel,               Qt::Key_Excel },
    { XF86XK_Explorer,            Qt::Key_Explorer },
    { XF86XK_Game,                Qt::Key_Game },
    { XF86XK_Go,                  Qt::Key_Go },
    { XF86XK_iTouch,              Qt::Key_iTouch },
    { XF86XK_LogOff,              Qt::Key_LogOff },
    { XF86XK_Market,              Qt::Key_Market },
    { XF86XK_Meeting,             Qt::Key_Meeting },
    { XF86XK_MenuKB,              Qt::Key_MenuKB },
    { XF86XK_MenuPB,              Qt::Key_MenuPB },
    { XF86XK_MySites,             Qt::Key_MySites },
    { XF86XK_News,                Qt::Key_News },
    { XF86XK_OfficeHome,          Qt::Key_OfficeHome },
    { XF86XK_Option,              Qt::Key_Option },
    { XF86XK_Paste,               Qt::Key_Paste },
    { XF86XK_Phone,               Qt::Key_Phone },
    { XF86XK_Reply,               Qt::Key_Reply },
    { XF86XK_Reload,              Qt::Key_Reload },
    { XF86XK_RotateWindows,       Qt::Key_RotateWindows },
    { XF86XK_RotationPB,          Qt::Key_RotationPB },
    { XF86XK_RotationKB,          Qt::Key_RotationKB },
    { XF86XK_Save,                Qt::Key_Save },
    { XF86XK_Send,                Qt::Key_Send },
    { XF86XK_Spell,               Qt::Key_Spell },
    { XF86XK_SplitScreen,         Qt::Key_SplitScreen },
    { XF86XK_Support,             Qt::Key_Support },
    { XF86XK_TaskPane,            Qt::Key_TaskPane },
    { XF86XK_Terminal,            Qt::Key_Terminal },
    { XF86XK_Tools,               Qt::Key_Tools },
    { XF86XK_Travel,              Qt::Key_Travel },
    { XF86XK_Video,               Qt::Key_Video },
    { XF86XK_Word,                Qt::Key_Word },
    { XF86XK_Xfer,                Qt::Key_Xfer },
    { XF86XK_ZoomIn,              Qt::Key_ZoomIn },
    { XF86XK_ZoomOut,             Qt::Key_ZoomOut },
    { XF86XK_Away,                Qt::Key_Away },
    { XF86XK_Messenger,           Qt::Key_Messenger },
    { XF86XK_WebCam,              Qt::Key_WebCam },
    { XF86XK_MailForward,         Qt::Key_MailForward },
    { XF86XK_Pictures,            Qt::Key_Pictures },
    { XF86XK_Music,               Qt::Key_Music },
    { XF86XK_Battery,             Qt::Key_Battery },
    { XF86XK_Bluetooth,           Qt::Key_Bluetooth },
    { XF86XK_WLAN,                Qt::Key_WLAN },
    { XF86XK_UWB,                 Qt::Key_UWB },
    { XF86XK_AudioForward,        Qt::Key_AudioForward },
    { XF86XK_AudioRepeat,         Qt::Key_AudioRepeat },
    { XF86XK_AudioRandomPlay,     Qt::Key_AudioRandomPlay },
    { XF86XK_Subtitle,            Qt::Key_Subtitle },
    { XF86XK_AudioCycleTrack,     Qt::Key_AudioCycleTrack },
    { XF86XK_Time,                Qt::Key_Time },
    { XF86XK_Select,              Qt::Key_Select },
    { XF86XK_View,                Qt::Key_View },
    { XF86XK_TopMenu,             Qt::Key_TopMenu },
    { XF86XK_Bluetooth,           Qt::Key_Bluetooth },
    { XF86XK_Suspend,             Qt::Key_Suspend },
    { XF86XK_Hibernate,           Qt::Key_Hibernate },
    { XF86XK_Launch0,             Qt::Key_Launch0 },
    { XF86XK_Launch1,             Qt::Key_Launch1 },
    { XF86XK_Launch2,             Qt::Key_Launch2 },
    { XF86XK_Launch3,             Qt::Key_Launch3 },
    { XF86XK_Launch4,             Qt::Key_Launch4 },
    { XF86XK_Launch5,             Qt::Key_Launch5 },
    { XF86XK_Launch6,             Qt::Key_Launch6 },
    { XF86XK_Launch7,             Qt::Key_Launch7 },
    { XF86XK_Launch8,             Qt::Key_Launch8 },
    { XF86XK_Launch9,             Qt::Key_Launch9 },
    { XF86XK_LaunchA,             Qt::Key_LaunchA },
    { XF86XK_LaunchB,             Qt::Key_LaunchB },
    { XF86XK_LaunchC,             Qt::Key_LaunchC },
    { XF86XK_LaunchD,             Qt::Key_LaunchD },
    { XF86XK_LaunchE,             Qt::Key_LaunchE },
    { XF86XK_LaunchF,             Qt::Key_LaunchF },
    // { XF86XK_LaunchH,             Qt::Key_LaunchH },
};
static const qint16 KeyTblSize = sizeof(KeyTbl) / sizeof(KeyTblData);

static int translateKeySym(const uint key)
{
    int code = -1;
    for (qint16 i = 0; i < KeyTblSize; i++) {
        if (KeyTbl[i].x11key == key) {
            code = KeyTbl[i].qtkey;
            break;
        }
    }

    if (qt_meta_mask) {
        // translate Super/Hyper keys to Meta if we're using them as the MetaModifier
        if (qt_meta_mask == qt_super_mask && (code == Qt::Key_Super_L || code == Qt::Key_Super_R)) {
            code = Qt::Key_Meta;
        } else if (qt_meta_mask == qt_hyper_mask && (code == Qt::Key_Hyper_L || code == Qt::Key_Hyper_R)) {
            code = Qt::Key_Meta;
        }
    }
    return code;
}

static QString translateKeySym(KeySym keysym, uint xmodifiers,
                               int &code, Qt::KeyboardModifiers &modifiers,
                               QByteArray &chars, int &count)
{
    // all keysyms smaller than 0xff00 are actally keys that can be mapped to unicode chars

    extern QTextCodec *qt_input_mapper; // from qapplication_x11.cpp
    QTextCodec *mapper = qt_input_mapper;
    QChar converted;

    if (count == 0 && keysym < 0xff00) {
        unsigned char byte3 = (unsigned char)(keysym >> 8);
        int mib = -1;
        switch(byte3) {
        case 0: // Latin 1
        case 1: // Latin 2
        case 2: //latin 3
        case 3: // latin4
            mib = byte3 + 4; break;
        case 5: // arabic
            mib = 82; break;
        case 12: // Hebrew
            mib = 85; break;
        case 13: // Thai
            mib = 2259; break;
        case 4: // kana
        case 6: // cyrillic
        case 7: // greek
        case 8: // technical, no mapping here at the moment
        case 9: // Special
        case 10: // Publishing
        case 11: // APL
        case 14: // Korean, no mapping
            mib = -1; // manual conversion
            mapper = 0;
            converted = (uint)keysym;
        case 0x20:
            // currency symbols
            if (keysym >= 0x20a0 && keysym <= 0x20ac) {
                mib = -1; // manual conversion
                mapper = 0;
                converted = (uint)keysym;
            }
            break;
        default:
            break;
        }
        if (mib != -1) {
            mapper = QTextCodec::codecForMib(mib);
            if (chars.isEmpty())
                chars.resize(1);
            chars[0] = (unsigned char) (keysym & 0xff); // get only the fourth bit for conversion later
            count++;
        }
    } else if (keysym >= 0x1000000 && keysym <= 0x100ffff) {
        converted = (ushort) (keysym - 0x1000000);
        mapper = 0;
    }
    if (count < (int)chars.size()-1)
        chars[count] = '\0';

    QString text;
    if (!mapper && converted.unicode() != 0x0) {
        text = converted;
    } else if (!chars.isEmpty()) {
        // convert chars (8bit) to text (unicode).
        if (mapper)
            text = mapper->toUnicode(chars.data(), count, 0);
        if (text.isEmpty()) {
            // no mapper, or codec couldn't convert to unicode (this
            // can happen when running in the C locale or with no LANG
            // set). try converting from latin-1
            text = QString::fromLatin1(chars);
        }
    }

    modifiers = qt_x11Data->translateModifiers(xmodifiers);

    // Commentary in X11/keysymdef says that X codes match ASCII, so it
    // is safe to use the locale functions to process X codes in ISO8859-1.
    //
    // This is mainly for compatibility - applications should not use the
    // Qt keycodes between 128 and 255, but should rather use the
    // QKeyEvent::text().
    //
    if (keysym < 128 || (keysym < 256 && (!qt_input_mapper || qt_input_mapper->mibEnum()==4))) {
        // upper-case key, if known
        code = isprint((int)keysym) ? toupper((int)keysym) : 0;
    } else if (keysym >= XK_F1 && keysym <= XK_F35) {
        // function keys
        code = Qt::Key_F1 + ((int)keysym - XK_F1);
    } else if (keysym >= XK_KP_Space && keysym <= XK_KP_9) {
        if (keysym >= XK_KP_0) {
            // numeric keypad keys
            code = Qt::Key_0 + ((int)keysym - XK_KP_0);
        } else {
            code = translateKeySym(keysym);
        }
        modifiers |= Qt::KeypadModifier;
    } else if (text.length() == 1 && text.unicode()->unicode() > 0x1f && text.unicode()->unicode() != 0x7f && !(keysym >= XK_dead_grave && keysym <= XK_dead_horn)) {
        code = text.unicode()->toUpper().unicode();
    } else {
        // any other keys
        code = translateKeySym(keysym);

        if (code == Qt::Key_Tab && (modifiers & Qt::ShiftModifier)) {
            // map shift+tab to shift+backtab, QShortcutMap knows about it
            // and will handle it.
            code = Qt::Key_Backtab;
            text = QString();
        }
    }

    return text;
}

extern bool qt_use_rtl_extensions; // from qapplication_x11.cpp

bool QKeyMapperPrivate::translateKeyEventInternal(QWidget *keyWidget,
                                                  const XEvent *event,
                                                  KeySym &keysym,
                                                  int& count,
                                                  QString& text,
                                                  Qt::KeyboardModifiers &modifiers,
                                                  int& code,
                                                  QEvent::Type &type,
                                                  bool statefulTranslation)
{
    XKeyEvent xkeyevent = event->xkey;
    // save the modifier state, we will use the keystate uint later by passing
    // it to translateButtonState
    uint keystate = event->xkey.state;

    type = (event->type == XKeyPress) ? QEvent::KeyPress : QEvent::KeyRelease;

    static int directionKeyEvent = 0;
    static unsigned int lastWinId = 0;

    // translate pending direction change
    if (statefulTranslation && qt_use_rtl_extensions && type == QEvent::KeyRelease) {
        if (directionKeyEvent == Qt::Key_Direction_R || directionKeyEvent == Qt::Key_Direction_L) {
            type = QEvent::KeyPress;
            code = directionKeyEvent;
            text = QString();
            directionKeyEvent = 0;
            lastWinId = 0;
            return true;
        } else {
            directionKeyEvent = 0;
            lastWinId = 0;
        }
    }

    // some XmbLookupString implementations don't return buffer overflow correctly,
    // so we use large input buffer to allow for long strings...
    // 256 chars * 2 bytes + 1 null-term == 513 bytes
    QByteArray chars(513, Qt::Uninitialized);

    count = XLookupString(&xkeyevent, chars.data(), chars.size(), &keysym, 0);

    // translate the keysym + xmodifiers to Qt::Key_* + Qt::KeyboardModifiers
    text = translateKeySym(keysym, keystate, code, modifiers, chars, count);

    // Watch for keypresses and if its a key belonging to the Ctrl-Shift
    // direction-changing accel, remember it.
    // We keep track of those keys instead of using the event's state
    // (to figure out whether the Ctrl modifier is held while Shift is pressed,
    // or Shift is held while Ctrl is pressed) since the 'state' doesn't tell
    // us whether the modifier held is Left or Right.
    if (statefulTranslation && qt_use_rtl_extensions && type == QEvent::KeyPress) {
        if (keysym == XK_Control_L || keysym == XK_Control_R
            || keysym == XK_Shift_L || keysym == XK_Shift_R) {
            if (!directionKeyEvent) {
                directionKeyEvent = keysym;
                // This code exists in order to check that
                // the event is occurred in the same widget.
                lastWinId = keyWidget->internalWinId();
            }
        } else {
            // this can no longer be a direction-changing accel.
            // if any other key was pressed.
            directionKeyEvent = Qt::Key_Space;
        }

        if (directionKeyEvent && lastWinId == keyWidget->internalWinId()) {
            if ((keysym == XK_Shift_L && directionKeyEvent == XK_Control_L)
                || (keysym == XK_Control_L && directionKeyEvent == XK_Shift_L)) {
                directionKeyEvent = Qt::Key_Direction_L;
            } else if ((keysym == XK_Shift_R && directionKeyEvent == XK_Control_R)
                       || (keysym == XK_Control_R && directionKeyEvent == XK_Shift_R)) {
                directionKeyEvent = Qt::Key_Direction_R;
            }
        } else if (directionKeyEvent == Qt::Key_Direction_L
                   || directionKeyEvent == Qt::Key_Direction_R) {
            directionKeyEvent = Qt::Key_Space; // invalid
        }
    }

    return true;
}


struct qt_auto_repeat_data
{
    // match the window and keycode with timestamp delta of 10 ms
    Window window;
    KeyCode keycode;
    Time timestamp;

    // queue scanner state
    bool release;
    bool error;
};

#if defined(Q_C_CALLBACKS)
extern "C" {
#endif

static Bool qt_keypress_scanner(Display *, XEvent *event, XPointer arg)
{
    if (event->type != XKeyPress && event->type != XKeyRelease)
        return false;

    qt_auto_repeat_data *data = (qt_auto_repeat_data *) arg;
    if (data->error)
        return false;

    if (event->xkey.window  != data->window ||
        event->xkey.keycode != data->keycode) {
        // deal breakers: key events in a different window or an event
        // with a different key code
        data->error = true;
        return false;
    }

    if (event->type == XKeyPress) {
        data->error = (! data->release || event->xkey.time - data->timestamp > 10);
        return (! data->error);
    }

    // must be XKeyRelease event
    if (data->release) {
        // found a second release
        data->error = true;
        return false;
    }

    // found a single release
    data->release = true;
    data->timestamp = event->xkey.time;

    return false;
}

static Bool qt_keyrelease_scanner(Display *, XEvent *event, XPointer arg)
{
    const qt_auto_repeat_data *data = (const qt_auto_repeat_data *) arg;
    return (event->type == XKeyRelease &&
            event->xkey.window  == data->window &&
            event->xkey.keycode == data->keycode);
}

#if defined(Q_C_CALLBACKS)
}
#endif

bool QKeyMapperPrivate::translateKeyEvent(QWidget *keyWidget, const XEvent *event)
{
    int           code = -1;
    int           count = 0;
    Qt::KeyboardModifiers modifiers;

    if (qt_sm_blockUserInput) // block user interaction during session management
        return true;

    Display *dpy = qt_x11Data->display;

    if (!keyWidget->isEnabled())
        return true;

    QEvent::Type type;
    bool    autor = false;
    QString text;

    KeySym keysym = 0;
    translateKeyEventInternal(keyWidget, event, keysym, count, text, modifiers, code, type);

    // was this the last auto-repeater?
    qt_auto_repeat_data auto_repeat_data;
    auto_repeat_data.window = event->xkey.window;
    auto_repeat_data.keycode = event->xkey.keycode;
    auto_repeat_data.timestamp = event->xkey.time;

    static uint curr_autorep = 0;
    if (event->type == XKeyPress) {
        if (curr_autorep == event->xkey.keycode) {
            autor = true;
            curr_autorep = 0;
        }
    } else {
        // look ahead for auto-repeat
        XEvent nextpress;

        auto_repeat_data.release = true;
        auto_repeat_data.error = false;
        if (XCheckIfEvent(dpy, &nextpress, &qt_keypress_scanner,
                          (XPointer) &auto_repeat_data)) {
            autor = true;

            // Put it back... we COULD send the event now and not need
            // the static curr_autorep variable.
            XPutBackEvent(dpy,&nextpress);
        }
        curr_autorep = autor ? event->xkey.keycode : 0;
    }

    // autorepeat compression makes sense for all widgets
    if (event->type == XKeyPress && text.length() <= 1) {
        XEvent dummy;

        for (;;) {
            auto_repeat_data.release = false;
            auto_repeat_data.error = false;
            if (! XCheckIfEvent(dpy, &dummy, &qt_keypress_scanner,
                                (XPointer) &auto_repeat_data))
                break;
            if (! XCheckIfEvent(dpy, &dummy, &qt_keyrelease_scanner,
                                (XPointer) &auto_repeat_data))
                break;

            count++;
            if (!text.isEmpty())
                text += text[0];
        }
    }

    return QKeyMapper::sendKeyEvent(keyWidget, type, code, modifiers, text, autor,
                                    qMax(qMax(count,1), int(text.length())),
                                    event->xkey.keycode, keysym, event->xkey.state);
}

bool QKeyMapper::sendKeyEvent(QWidget *keyWidget,
                              QEvent::Type type, int code, Qt::KeyboardModifiers modifiers,
                              const QString &text, bool autorepeat, int count,
                              quint32 nativeScanCode, quint32 nativeVirtualKey, quint32 nativeModifiers)
{
    // try the menukey first
    if (type == QEvent::KeyPress && code == Qt::Key_Menu) {
        QPoint globalPos=  QCursor::pos();
        QPoint pos = keyWidget->mapFromGlobal(globalPos);
        QContextMenuEvent e(QContextMenuEvent::Keyboard, pos, globalPos);
        qt_sendSpontaneousEvent(keyWidget, &e);
        if(e.isAccepted())
            return true;
    }

    QKeyEvent e(type, code, modifiers,
                  nativeScanCode, nativeVirtualKey, nativeModifiers,
                  text, autorepeat, qMax(qMax(count,1), int(text.length())));
    return qt_sendSpontaneousEvent(keyWidget, &e);
}

QT_END_NAMESPACE


