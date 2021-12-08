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

#include "qkeysequence.h"
#include "qkeysequence_p.h"
#include "qapplication_p.h"

#ifndef QT_NO_SHORTCUT

#include "qshortcut.h"
#include "qdebug.h"
#include "qvariant.h"

#ifndef QT_NO_DATASTREAM
# include "qdatastream.h"
#endif

QT_BEGIN_NAMESPACE

/*!
    \class QKeySequence
    \brief The QKeySequence class encapsulates a key sequence as used
    by shortcuts.

    \ingroup shared


    In its most common form, a key sequence describes a combination of
    keys that must be used together to perform some action. Key sequences
    are used with QAction objects to specify which keyboard shortcuts can
    be used to trigger actions.

    Key sequences can be constructed for use as keyboard shortcuts in
    three different ways:

    \list
    \o For standard shortcuts, a \l{QKeySequence::StandardKey}{standard key}
       can be used to request the platform-specific key sequence associated
       with each shortcut.
    \o For custom shortcuts, human-readable strings such as "Ctrl+X" can
       be used, and these can be translated into the appropriate shortcuts
       for users of different languages. Translations are made in the
       "QShortcut" context.
    \o For hard-coded shortcuts, integer key codes can be specified with
       a combination of values defined by the Qt::Key and Qt::Modifier enum
       values. Each key code consists of a single Qt::Key value and zero or
       more modifiers, such as Qt::SHIFT, Qt::CTRL, Qt::ALT and Qt::META.
    \endlist

    For example, \gui{Ctrl P} might be a sequence used as a shortcut for
    printing a document, and can be specified in any of the following
    ways:

    \snippet doc/src/snippets/code/src_gui_kernel_qkeysequence.cpp 0

    Note that, for letters, the case used in the specification string
    does not matter. In the above examples, the user does not need to
    hold down the \key{Shift} key to activate a shortcut specified
    with "Ctrl+P". However, for other keys, the use of \key{Shift} as
    an unspecified extra modifier key can lead to confusion for users
    of an application whose keyboards have different layouts to those
    used by the developers. See the \l{Keyboard Layout Issues} section
    below for more details.

    It is preferable to use standard shortcuts where possible.
    When creating key sequences for non-standard shortcuts, you should use
    human-readable strings in preference to hard-coded integer values.

    QKeySequence objects can be cast to a QString to obtain a human-readable
    translated version of the sequence. Similarly, the toString() function
    produces human-readable strings for use in menus.

    An alternative way to specify hard-coded key codes is to use the Unicode
    code point of the character; for example, 'A' gives the same key sequence
    as Qt::Key_A.

    \section1 Standard Shortcuts

    QKeySequence defines many \l{QKeySequence::StandardKey} {standard
    keyboard shortcuts} to reduce the amount of effort required when
    setting up actions in a typical application.

    \section1 Keyboard Layout Issues

    Many key sequence specifications are chosen by developers based on the
    layout of certain types of keyboard, rather than choosing keys that
    represent the first letter of an action's name, such as \key{Ctrl S}
    ("Ctrl+S") or \key{Ctrl C} ("Ctrl+C").
    Additionally, because certain symbols can only be entered with the
    help of modifier keys on certain keyboard layouts, key sequences intended
    for use with one keyboard layout may map to a different key, map to no
    keys at all, or require an additional modifier key to be used on
    different keyboard layouts.

    For example, the shortcuts, \key{Ctrl plus} and \key{Ctrl minus}, are often
    used as shortcuts for zoom operations in graphics applications, and these
    may be specified as "Ctrl++" and "Ctrl+-" respectively. However, the way
    these shortcuts are specified and interpreted depends on the keyboard layout.
    Users of Norwegian keyboards will note that the \key{+} and \key{-} keys
    are not adjacent on the keyboard, but will still be able to activate both
    shortcuts without needing to press the \key{Shift} key. However, users
    with British keyboards will need to hold down the \key{Shift} key
    to enter the \key{+} symbol, making the shortcut effectively the same as
    "Ctrl+Shift+=".

    Although some developers might resort to fully specifying all the modifiers
    they use on their keyboards to activate a shortcut, this will also result
    in unexpected behavior for users of different keyboard layouts.

    For example, a developer using a British keyboard may decide to specify
    "Ctrl+Shift+=" as the key sequence in order to create a shortcut that
    coincidentally behaves in the same way as \key{Ctrl plus}. However, the
    \key{=} key needs to be accessed using the \key{Shift} key on Norwegian
    keyboard, making the required shortcut effectively \key{Ctrl Shift Shift =}
    (an impossible key combination).

    As a result, both human-readable strings and hard-coded key codes
    can both be problematic to use when specifying a key sequence that
    can be used on a variety of different keyboard layouts. Only the
    use of \l{QKeySequence::StandardKey} {standard shortcuts}
    guarantees that the user will be able to use the shortcuts that
    the developer intended.

    Despite this, we can address this issue by ensuring that human-readable
    strings are used, making it possible for translations of key sequences to
    be made for users of different languages. This approach will be successful
    for users whose keyboards have the most typical layout for the language
    they are using.

    \section1 GNU Emacs Style Key Sequences

    Key sequences similar to those used in \l{GNU Emacs}, allowing up to two
    key codes, can be created by using the multiple argument constructor,
    or by passing a human-readable string of comma-separated key sequences.

    For example, the key sequence, \key{Ctrl X} followed by \key{Ctrl C}, can
    be specified using either of the following ways:

    \snippet doc/src/snippets/code/src_gui_kernel_qkeysequence.cpp 1

    \warning A QApplication instance must have been constructed before a
             QKeySequence is created; otherwise, your application may crash.

    \sa QShortcut
*/

/*!
    \enum QKeySequence::SequenceMatch

    \value NoMatch The key sequences are different; not even partially
    matching.
    \value PartialMatch The key sequences match partially, but are not
    the same.
    \value ExactMatch The key sequences are the same.
    \omitvalue Identical
*/

/*!
    \enum QKeySequence::SequenceFormat

    \value NativeText The key sequence as a platform specific string.
    This means that it will be shown translated. This enum is best
    used when you want to display the string to the user.

    \value PortableText The key sequence is given in a "portable" format,
    suitable for reading and writing to a file. In many cases, it will look
    similar to the native text on X11.
*/

// https://theasciicode.com.ar/ascii-control-characters/null-character-ascii-code-0.html
// pseudo (modifier) keys are excluded on purpose
// TODO: the idea to translate the keys is just bogus, instead native format should be UTF-8 string
// that represents the mapped key. this will require X11 keyboard map lookup to figure out what the
// keys actually print as string so it may have to be wired to QKeyMapper somehow
static const struct KeyNameTblData {
    const Qt::Key key;
    const char* name;
} KeyNameTbl[] = {
    { Qt::Key_Backspace,                QT_TRANSLATE_NOOP("QShortcut", "Backspace") },
    { Qt::Key_Tab,                      QT_TRANSLATE_NOOP("QShortcut", "Tab") },
    { Qt::Key_Linefeed,                 QT_TRANSLATE_NOOP("QShortcut", "Linefeed") },
    { Qt::Key_Clear,                    QT_TRANSLATE_NOOP("QShortcut", "Clear") },
    { Qt::Key_Return,                   QT_TRANSLATE_NOOP("QShortcut", "Return") },
    { Qt::Key_Pause,                    QT_TRANSLATE_NOOP("QShortcut", "Pause") },
    { Qt::Key_Scroll_Lock,              QT_TRANSLATE_NOOP("QShortcut", "Scroll Lock") },
    { Qt::Key_Sys_Req,                  QT_TRANSLATE_NOOP("QShortcut", "System Request") },
    { Qt::Key_Escape,                   QT_TRANSLATE_NOOP("QShortcut", "Escape") },
    { Qt::Key_Delete,                   QT_TRANSLATE_NOOP("QShortcut", "Delete") },
    { Qt::Key_Multi_key,                QT_TRANSLATE_NOOP("QShortcut", "Multi Key") },
    { Qt::Key_Codeinput,                QT_TRANSLATE_NOOP("QShortcut", "Code input") },
    { Qt::Key_SingleCandidate,          QT_TRANSLATE_NOOP("QShortcut", "Single Candidate") },
    { Qt::Key_MultipleCandidate,        QT_TRANSLATE_NOOP("QShortcut", "Multiple Candidate") },
    { Qt::Key_PreviousCandidate,        QT_TRANSLATE_NOOP("QShortcut", "Previous Candidate") },
    { Qt::Key_Kanji,                    QT_TRANSLATE_NOOP("QShortcut", "Kanji") },
    { Qt::Key_Muhenkan,                 QT_TRANSLATE_NOOP("QShortcut", "Muhenkan") },
    { Qt::Key_Henkan,                   QT_TRANSLATE_NOOP("QShortcut", "Henkan") },
    { Qt::Key_Romaji,                   QT_TRANSLATE_NOOP("QShortcut", "Romaji") },
    { Qt::Key_Hiragana,                 QT_TRANSLATE_NOOP("QShortcut", "Hiragana") },
    { Qt::Key_Katakana,                 QT_TRANSLATE_NOOP("QShortcut", "Katakana") },
    { Qt::Key_Hiragana_Katakana,        QT_TRANSLATE_NOOP("QShortcut", "Hiragana Katakana") },
    { Qt::Key_Zenkaku,                  QT_TRANSLATE_NOOP("QShortcut", "Zenkaku") },
    { Qt::Key_Hankaku,                  QT_TRANSLATE_NOOP("QShortcut", "Hankaku") },
    { Qt::Key_Zenkaku_Hankaku,          QT_TRANSLATE_NOOP("QShortcut", "Zenkaku Hankaku") },
    { Qt::Key_Touroku,                  QT_TRANSLATE_NOOP("QShortcut", "Touroku") },
    { Qt::Key_Massyo,                   QT_TRANSLATE_NOOP("QShortcut", "Massyo") },
    { Qt::Key_Kana_Lock,                QT_TRANSLATE_NOOP("QShortcut", "Kana Lock") },
    { Qt::Key_Kana_Shift,               QT_TRANSLATE_NOOP("QShortcut", "Kana Shift") },
    { Qt::Key_Eisu_Shift,               QT_TRANSLATE_NOOP("QShortcut", "Eisu Shift") },
    { Qt::Key_Eisu_toggle,              QT_TRANSLATE_NOOP("QShortcut", "Eisu toggle") },
    { Qt::Key_Home,                     QT_TRANSLATE_NOOP("QShortcut", "Home") },
    { Qt::Key_Left,                     QT_TRANSLATE_NOOP("QShortcut", "Left") },
    { Qt::Key_Up,                       QT_TRANSLATE_NOOP("QShortcut", "Up") },
    { Qt::Key_Right,                    QT_TRANSLATE_NOOP("QShortcut", "Right") },
    { Qt::Key_Down,                     QT_TRANSLATE_NOOP("QShortcut", "Down") },
    { Qt::Key_Page_Up,                  QT_TRANSLATE_NOOP("QShortcut", "Page Up") },
    { Qt::Key_Page_Down,                QT_TRANSLATE_NOOP("QShortcut", "Page Down") },
    { Qt::Key_End,                      QT_TRANSLATE_NOOP("QShortcut", "End") },
    { Qt::Key_Begin,                    QT_TRANSLATE_NOOP("QShortcut", "Begin") },
    { Qt::Key_Select,                   QT_TRANSLATE_NOOP("QShortcut", "Select") },
    { Qt::Key_Print,                    QT_TRANSLATE_NOOP("QShortcut", "Print Screen") },
    { Qt::Key_Execute,                  QT_TRANSLATE_NOOP("QShortcut", "Execute") },
    { Qt::Key_Insert,                   QT_TRANSLATE_NOOP("QShortcut", "Insert") },
    { Qt::Key_Undo,                     QT_TRANSLATE_NOOP("QShortcut", "Undo") },
    { Qt::Key_Redo,                     QT_TRANSLATE_NOOP("QShortcut", "Redo") },
    { Qt::Key_Menu,                     QT_TRANSLATE_NOOP("QShortcut", "Menu") },
    { Qt::Key_Find,                     QT_TRANSLATE_NOOP("QShortcut", "Find") },
    { Qt::Key_Cancel,                   QT_TRANSLATE_NOOP("QShortcut", "Cancel") },
    { Qt::Key_Help,                     QT_TRANSLATE_NOOP("QShortcut", "Help") },
    { Qt::Key_Break,                    QT_TRANSLATE_NOOP("QShortcut", "Break") },
    { Qt::Key_Mode_switch,              QT_TRANSLATE_NOOP("QShortcut", "Mode switch") },
    { Qt::Key_Num_Lock,                 QT_TRANSLATE_NOOP("QShortcut", "Num Lock") },
    { Qt::Key_F1,                       QT_TRANSLATE_NOOP("QShortcut", "F1") },
    { Qt::Key_F2,                       QT_TRANSLATE_NOOP("QShortcut", "F2") },
    { Qt::Key_F3,                       QT_TRANSLATE_NOOP("QShortcut", "F3") },
    { Qt::Key_F4,                       QT_TRANSLATE_NOOP("QShortcut", "F4") },
    { Qt::Key_F5,                       QT_TRANSLATE_NOOP("QShortcut", "F5") },
    { Qt::Key_F6,                       QT_TRANSLATE_NOOP("QShortcut", "F6") },
    { Qt::Key_F7,                       QT_TRANSLATE_NOOP("QShortcut", "F7") },
    { Qt::Key_F8,                       QT_TRANSLATE_NOOP("QShortcut", "F8") },
    { Qt::Key_F9,                       QT_TRANSLATE_NOOP("QShortcut", "F9") },
    { Qt::Key_F10,                      QT_TRANSLATE_NOOP("QShortcut", "F10") },
    { Qt::Key_F11,                      QT_TRANSLATE_NOOP("QShortcut", "F11") },
    { Qt::Key_F12,                      QT_TRANSLATE_NOOP("QShortcut", "F12") },
    { Qt::Key_F13,                      QT_TRANSLATE_NOOP("QShortcut", "F13") },
    { Qt::Key_F14,                      QT_TRANSLATE_NOOP("QShortcut", "F14") },
    { Qt::Key_F15,                      QT_TRANSLATE_NOOP("QShortcut", "F15") },
    { Qt::Key_F16,                      QT_TRANSLATE_NOOP("QShortcut", "F16") },
    { Qt::Key_F17,                      QT_TRANSLATE_NOOP("QShortcut", "F17") },
    { Qt::Key_F18,                      QT_TRANSLATE_NOOP("QShortcut", "F18") },
    { Qt::Key_F19,                      QT_TRANSLATE_NOOP("QShortcut", "F19") },
    { Qt::Key_F20,                      QT_TRANSLATE_NOOP("QShortcut", "F20") },
    { Qt::Key_F21,                      QT_TRANSLATE_NOOP("QShortcut", "F21") },
    { Qt::Key_F22,                      QT_TRANSLATE_NOOP("QShortcut", "F22") },
    { Qt::Key_F23,                      QT_TRANSLATE_NOOP("QShortcut", "F23") },
    { Qt::Key_F24,                      QT_TRANSLATE_NOOP("QShortcut", "F24") },
    { Qt::Key_F25,                      QT_TRANSLATE_NOOP("QShortcut", "F25") },
    { Qt::Key_F26,                      QT_TRANSLATE_NOOP("QShortcut", "F26") },
    { Qt::Key_F27,                      QT_TRANSLATE_NOOP("QShortcut", "F27") },
    { Qt::Key_F28,                      QT_TRANSLATE_NOOP("QShortcut", "F28") },
    { Qt::Key_F29,                      QT_TRANSLATE_NOOP("QShortcut", "F29") },
    { Qt::Key_F30,                      QT_TRANSLATE_NOOP("QShortcut", "F30") },
    { Qt::Key_F31,                      QT_TRANSLATE_NOOP("QShortcut", "F31") },
    { Qt::Key_F32,                      QT_TRANSLATE_NOOP("QShortcut", "F32") },
    { Qt::Key_F33,                      QT_TRANSLATE_NOOP("QShortcut", "F33") },
    { Qt::Key_F34,                      QT_TRANSLATE_NOOP("QShortcut", "F34") },
    { Qt::Key_F35,                      QT_TRANSLATE_NOOP("QShortcut", "F35") },
/*
    { Qt::Key_Shift_L,                  QT_TRANSLATE_NOOP("QShortcut", "Shift_L") },
    { Qt::Key_Shift_R,                  QT_TRANSLATE_NOOP("QShortcut", "Shift_R") },
    { Qt::Key_Control_L,                QT_TRANSLATE_NOOP("QShortcut", "Control_L") },
    { Qt::Key_Control_R,                QT_TRANSLATE_NOOP("QShortcut", "Control_R") },
*/
    { Qt::Key_Caps_Lock,                QT_TRANSLATE_NOOP("QShortcut", "Caps Lock") },
    { Qt::Key_Shift_Lock,               QT_TRANSLATE_NOOP("QShortcut", "Shift Lock") },
/*
    { Qt::Key_Meta_L,                   QT_TRANSLATE_NOOP("QShortcut", "Meta_L") },
    { Qt::Key_Meta_R,                   QT_TRANSLATE_NOOP("QShortcut", "Meta_R") },
    { Qt::Key_Alt_L,                    QT_TRANSLATE_NOOP("QShortcut", "Alt_L") },
    { Qt::Key_Alt_R,                    QT_TRANSLATE_NOOP("QShortcut", "Alt_R") },
*/
    { Qt::Key_Super_L,                  QT_TRANSLATE_NOOP("QShortcut", "Super Left") },
    { Qt::Key_Super_R,                  QT_TRANSLATE_NOOP("QShortcut", "Super Right") },
    { Qt::Key_Hyper_L,                  QT_TRANSLATE_NOOP("QShortcut", "Hyper Left") },
    { Qt::Key_Hyper_R,                  QT_TRANSLATE_NOOP("QShortcut", "Hyper Right") },
    { Qt::Key_Space,                    QT_TRANSLATE_NOOP("QShortcut", "Space") },
    { Qt::Key_Exclam,                   QT_TRANSLATE_NOOP("QShortcut", "!") },
    { Qt::Key_QuoteDbl,                 QT_TRANSLATE_NOOP("QShortcut", "\"") },
    { Qt::Key_NumberSign,               QT_TRANSLATE_NOOP("QShortcut", "#") },
    { Qt::Key_Dollar,                   QT_TRANSLATE_NOOP("QShortcut", "$") },
    { Qt::Key_Percent,                  QT_TRANSLATE_NOOP("QShortcut", "%") },
    { Qt::Key_Ampersand,                QT_TRANSLATE_NOOP("QShortcut", "&") },
    { Qt::Key_Apostrophe,               QT_TRANSLATE_NOOP("QShortcut", "'") },
    { Qt::Key_ParenLeft,                QT_TRANSLATE_NOOP("QShortcut", "(") },
    { Qt::Key_ParenRight,               QT_TRANSLATE_NOOP("QShortcut", ")") },
    { Qt::Key_Asterisk,                 QT_TRANSLATE_NOOP("QShortcut", "*") },
    { Qt::Key_Plus,                     QT_TRANSLATE_NOOP("QShortcut", "+") },
    { Qt::Key_Comma,                    QT_TRANSLATE_NOOP("QShortcut", ",") },
    { Qt::Key_Minus,                    QT_TRANSLATE_NOOP("QShortcut", "-") },
    { Qt::Key_Period,                   QT_TRANSLATE_NOOP("QShortcut", ".") },
    { Qt::Key_Slash,                    QT_TRANSLATE_NOOP("QShortcut", "/") },
    { Qt::Key_0,                        QT_TRANSLATE_NOOP("QShortcut", "0") },
    { Qt::Key_1,                        QT_TRANSLATE_NOOP("QShortcut", "1") },
    { Qt::Key_2,                        QT_TRANSLATE_NOOP("QShortcut", "2") },
    { Qt::Key_3,                        QT_TRANSLATE_NOOP("QShortcut", "3") },
    { Qt::Key_4,                        QT_TRANSLATE_NOOP("QShortcut", "4") },
    { Qt::Key_5,                        QT_TRANSLATE_NOOP("QShortcut", "5") },
    { Qt::Key_6,                        QT_TRANSLATE_NOOP("QShortcut", "6") },
    { Qt::Key_7,                        QT_TRANSLATE_NOOP("QShortcut", "7") },
    { Qt::Key_8,                        QT_TRANSLATE_NOOP("QShortcut", "8") },
    { Qt::Key_9,                        QT_TRANSLATE_NOOP("QShortcut", "9") },
    { Qt::Key_Colon,                    QT_TRANSLATE_NOOP("QShortcut", ":") },
    { Qt::Key_Semicolon,                QT_TRANSLATE_NOOP("QShortcut", ";") },
    { Qt::Key_Less,                     QT_TRANSLATE_NOOP("QShortcut", "<") },
    { Qt::Key_Equal,                    QT_TRANSLATE_NOOP("QShortcut", "=") },
    { Qt::Key_Greater,                  QT_TRANSLATE_NOOP("QShortcut", ">") },
    { Qt::Key_Question,                 QT_TRANSLATE_NOOP("QShortcut", "?") },
    { Qt::Key_At,                       QT_TRANSLATE_NOOP("QShortcut", "@") },
    { Qt::Key_A,                        QT_TRANSLATE_NOOP("QShortcut", "A") },
    { Qt::Key_B,                        QT_TRANSLATE_NOOP("QShortcut", "B") },
    { Qt::Key_C,                        QT_TRANSLATE_NOOP("QShortcut", "C") },
    { Qt::Key_D,                        QT_TRANSLATE_NOOP("QShortcut", "D") },
    { Qt::Key_E,                        QT_TRANSLATE_NOOP("QShortcut", "E") },
    { Qt::Key_F,                        QT_TRANSLATE_NOOP("QShortcut", "F") },
    { Qt::Key_G,                        QT_TRANSLATE_NOOP("QShortcut", "G") },
    { Qt::Key_H,                        QT_TRANSLATE_NOOP("QShortcut", "H") },
    { Qt::Key_I,                        QT_TRANSLATE_NOOP("QShortcut", "I") },
    { Qt::Key_J,                        QT_TRANSLATE_NOOP("QShortcut", "J") },
    { Qt::Key_K,                        QT_TRANSLATE_NOOP("QShortcut", "K") },
    { Qt::Key_L,                        QT_TRANSLATE_NOOP("QShortcut", "L") },
    { Qt::Key_M,                        QT_TRANSLATE_NOOP("QShortcut", "M") },
    { Qt::Key_N,                        QT_TRANSLATE_NOOP("QShortcut", "N") },
    { Qt::Key_O,                        QT_TRANSLATE_NOOP("QShortcut", "O") },
    { Qt::Key_P,                        QT_TRANSLATE_NOOP("QShortcut", "P") },
    { Qt::Key_Q,                        QT_TRANSLATE_NOOP("QShortcut", "Q") },
    { Qt::Key_R,                        QT_TRANSLATE_NOOP("QShortcut", "R") },
    { Qt::Key_S,                        QT_TRANSLATE_NOOP("QShortcut", "S") },
    { Qt::Key_T,                        QT_TRANSLATE_NOOP("QShortcut", "T") },
    { Qt::Key_U,                        QT_TRANSLATE_NOOP("QShortcut", "U") },
    { Qt::Key_V,                        QT_TRANSLATE_NOOP("QShortcut", "V") },
    { Qt::Key_W,                        QT_TRANSLATE_NOOP("QShortcut", "W") },
    { Qt::Key_X,                        QT_TRANSLATE_NOOP("QShortcut", "X") },
    { Qt::Key_Y,                        QT_TRANSLATE_NOOP("QShortcut", "Y") },
    { Qt::Key_Z,                        QT_TRANSLATE_NOOP("QShortcut", "Z") },
    { Qt::Key_BracketLeft,              QT_TRANSLATE_NOOP("QShortcut", "[") },
    { Qt::Key_Backslash,                QT_TRANSLATE_NOOP("QShortcut", "\\") },
    { Qt::Key_BracketRight,             QT_TRANSLATE_NOOP("QShortcut", "]") },
    { Qt::Key_AsciiCircum,              QT_TRANSLATE_NOOP("QShortcut", "^") },
    { Qt::Key_Underscore,               QT_TRANSLATE_NOOP("QShortcut", "_") },
    { Qt::Key_Grave,                    QT_TRANSLATE_NOOP("QShortcut", "`") },
    { Qt::Key_BraceLeft,                QT_TRANSLATE_NOOP("QShortcut", "{") },
    { Qt::Key_Bar,                      QT_TRANSLATE_NOOP("QShortcut", "|") },
    { Qt::Key_BraceRight,               QT_TRANSLATE_NOOP("QShortcut", "}") },
    { Qt::Key_AsciiTilde,               QT_TRANSLATE_NOOP("QShortcut", "~") },
    { Qt::Key_nobreakspace,             QT_TRANSLATE_NOOP("QShortcut", "nobreakspace") },
    { Qt::Key_exclamdown,               QT_TRANSLATE_NOOP("QShortcut", "exclamdown") },
    { Qt::Key_cent,                     QT_TRANSLATE_NOOP("QShortcut", "cent") },
    { Qt::Key_sterling,                 QT_TRANSLATE_NOOP("QShortcut", "sterling") },
    { Qt::Key_currency,                 QT_TRANSLATE_NOOP("QShortcut", "currency") },
    { Qt::Key_yen,                      QT_TRANSLATE_NOOP("QShortcut", "yen") },
    { Qt::Key_brokenbar,                QT_TRANSLATE_NOOP("QShortcut", "brokenbar") },
    { Qt::Key_section,                  QT_TRANSLATE_NOOP("QShortcut", "section") },
    { Qt::Key_diaeresis,                QT_TRANSLATE_NOOP("QShortcut", "diaeresis") },
    { Qt::Key_copyright,                QT_TRANSLATE_NOOP("QShortcut", "copyright") },
    { Qt::Key_ordfeminine,              QT_TRANSLATE_NOOP("QShortcut", "ordfeminine") },
    { Qt::Key_guillemotleft,            QT_TRANSLATE_NOOP("QShortcut", "guillemotleft") },
    { Qt::Key_notsign,                  QT_TRANSLATE_NOOP("QShortcut", "notsign") },
    { Qt::Key_hyphen,                   QT_TRANSLATE_NOOP("QShortcut", "hyphen") },
    { Qt::Key_registered,               QT_TRANSLATE_NOOP("QShortcut", "registered") },
    { Qt::Key_macron,                   QT_TRANSLATE_NOOP("QShortcut", "macron") },
    { Qt::Key_degree,                   QT_TRANSLATE_NOOP("QShortcut", "degree") },
    { Qt::Key_plusminus,                QT_TRANSLATE_NOOP("QShortcut", "plusminus") },
    { Qt::Key_twosuperior,              QT_TRANSLATE_NOOP("QShortcut", "twosuperior") },
    { Qt::Key_threesuperior,            QT_TRANSLATE_NOOP("QShortcut", "threesuperior") },
    { Qt::Key_acute,                    QT_TRANSLATE_NOOP("QShortcut", "acute") },
    { Qt::Key_mu,                       QT_TRANSLATE_NOOP("QShortcut", "mu") },
    { Qt::Key_paragraph,                QT_TRANSLATE_NOOP("QShortcut", "paragraph") },
    { Qt::Key_periodcentered,           QT_TRANSLATE_NOOP("QShortcut", "periodcentered") },
    { Qt::Key_cedilla,                  QT_TRANSLATE_NOOP("QShortcut", "cedilla") },
    { Qt::Key_onesuperior,              QT_TRANSLATE_NOOP("QShortcut", "onesuperior") },
    { Qt::Key_masculine,                QT_TRANSLATE_NOOP("QShortcut", "masculine") },
    { Qt::Key_guillemotright,           QT_TRANSLATE_NOOP("QShortcut", "guillemotright") },
    { Qt::Key_onequarter,               QT_TRANSLATE_NOOP("QShortcut", "onequarter") },
    { Qt::Key_onehalf,                  QT_TRANSLATE_NOOP("QShortcut", "onehalf") },
    { Qt::Key_threequarters,            QT_TRANSLATE_NOOP("QShortcut", "threequarters") },
    { Qt::Key_questiondown,             QT_TRANSLATE_NOOP("QShortcut", "questiondown") },
    { Qt::Key_Agrave,                   QT_TRANSLATE_NOOP("QShortcut", "Agrave") },
    { Qt::Key_Aacute,                   QT_TRANSLATE_NOOP("QShortcut", "Aacute") },
    { Qt::Key_Acircumflex,              QT_TRANSLATE_NOOP("QShortcut", "Acircumflex") },
    { Qt::Key_Atilde,                   QT_TRANSLATE_NOOP("QShortcut", "Atilde") },
    { Qt::Key_Adiaeresis,               QT_TRANSLATE_NOOP("QShortcut", "Adiaeresis") },
    { Qt::Key_Aring,                    QT_TRANSLATE_NOOP("QShortcut", "Aring") },
    { Qt::Key_AE,                       QT_TRANSLATE_NOOP("QShortcut", "AE") },
    { Qt::Key_Ccedilla,                 QT_TRANSLATE_NOOP("QShortcut", "Ccedilla") },
    { Qt::Key_Egrave,                   QT_TRANSLATE_NOOP("QShortcut", "Egrave") },
    { Qt::Key_Eacute,                   QT_TRANSLATE_NOOP("QShortcut", "Eacute") },
    { Qt::Key_Ecircumflex,              QT_TRANSLATE_NOOP("QShortcut", "Ecircumflex") },
    { Qt::Key_Ediaeresis,               QT_TRANSLATE_NOOP("QShortcut", "Ediaeresis") },
    { Qt::Key_Igrave,                   QT_TRANSLATE_NOOP("QShortcut", "Igrave") },
    { Qt::Key_Iacute,                   QT_TRANSLATE_NOOP("QShortcut", "Iacute") },
    { Qt::Key_Icircumflex,              QT_TRANSLATE_NOOP("QShortcut", "Icircumflex") },
    { Qt::Key_Idiaeresis,               QT_TRANSLATE_NOOP("QShortcut", "Idiaeresis") },
    { Qt::Key_ETH,                      QT_TRANSLATE_NOOP("QShortcut", "ETH") },
    { Qt::Key_Ntilde,                   QT_TRANSLATE_NOOP("QShortcut", "Ntilde") },
    { Qt::Key_Ograve,                   QT_TRANSLATE_NOOP("QShortcut", "Ograve") },
    { Qt::Key_Oacute,                   QT_TRANSLATE_NOOP("QShortcut", "Oacute") },
    { Qt::Key_Ocircumflex,              QT_TRANSLATE_NOOP("QShortcut", "Ocircumflex") },
    { Qt::Key_Otilde,                   QT_TRANSLATE_NOOP("QShortcut", "Otilde") },
    { Qt::Key_Odiaeresis,               QT_TRANSLATE_NOOP("QShortcut", "Odiaeresis") },
    { Qt::Key_multiply,                 QT_TRANSLATE_NOOP("QShortcut", "multiply") },
    { Qt::Key_Oslash,                   QT_TRANSLATE_NOOP("QShortcut", "Oslash") },
    { Qt::Key_Ugrave,                   QT_TRANSLATE_NOOP("QShortcut", "Ugrave") },
    { Qt::Key_Uacute,                   QT_TRANSLATE_NOOP("QShortcut", "Uacute") },
    { Qt::Key_Ucircumflex,              QT_TRANSLATE_NOOP("QShortcut", "Ucircumflex") },
    { Qt::Key_Udiaeresis,               QT_TRANSLATE_NOOP("QShortcut", "Udiaeresis") },
    { Qt::Key_Yacute,                   QT_TRANSLATE_NOOP("QShortcut", "Yacute") },
    { Qt::Key_THORN,                    QT_TRANSLATE_NOOP("QShortcut", "THORN") },
    { Qt::Key_ssharp,                   QT_TRANSLATE_NOOP("QShortcut", "ssharp") },
    { Qt::Key_agrave,                   QT_TRANSLATE_NOOP("QShortcut", "agrave") },
    { Qt::Key_aacute,                   QT_TRANSLATE_NOOP("QShortcut", "aacute") },
    { Qt::Key_acircumflex,              QT_TRANSLATE_NOOP("QShortcut", "acircumflex") },
    { Qt::Key_atilde,                   QT_TRANSLATE_NOOP("QShortcut", "atilde") },
    { Qt::Key_adiaeresis,               QT_TRANSLATE_NOOP("QShortcut", "adiaeresis") },
    { Qt::Key_aring,                    QT_TRANSLATE_NOOP("QShortcut", "aring") },
    { Qt::Key_ae,                       QT_TRANSLATE_NOOP("QShortcut", "ae") },
    { Qt::Key_ccedilla,                 QT_TRANSLATE_NOOP("QShortcut", "ccedilla") },
    { Qt::Key_egrave,                   QT_TRANSLATE_NOOP("QShortcut", "egrave") },
    { Qt::Key_eacute,                   QT_TRANSLATE_NOOP("QShortcut", "eacute") },
    { Qt::Key_ecircumflex,              QT_TRANSLATE_NOOP("QShortcut", "ecircumflex") },
    { Qt::Key_ediaeresis,               QT_TRANSLATE_NOOP("QShortcut", "ediaeresis") },
    { Qt::Key_igrave,                   QT_TRANSLATE_NOOP("QShortcut", "igrave") },
    { Qt::Key_iacute,                   QT_TRANSLATE_NOOP("QShortcut", "iacute") },
    { Qt::Key_icircumflex,              QT_TRANSLATE_NOOP("QShortcut", "icircumflex") },
    { Qt::Key_idiaeresis,               QT_TRANSLATE_NOOP("QShortcut", "idiaeresis") },
    { Qt::Key_eth,                      QT_TRANSLATE_NOOP("QShortcut", "eth") },
    { Qt::Key_ntilde,                   QT_TRANSLATE_NOOP("QShortcut", "ntilde") },
    { Qt::Key_ograve,                   QT_TRANSLATE_NOOP("QShortcut", "ograve") },
    { Qt::Key_oacute,                   QT_TRANSLATE_NOOP("QShortcut", "oacute") },
    { Qt::Key_ocircumflex,              QT_TRANSLATE_NOOP("QShortcut", "ocircumflex") },
    { Qt::Key_otilde,                   QT_TRANSLATE_NOOP("QShortcut", "otilde") },
    { Qt::Key_odiaeresis,               QT_TRANSLATE_NOOP("QShortcut", "odiaeresis") },
    { Qt::Key_division,                 QT_TRANSLATE_NOOP("QShortcut", "division") },
    { Qt::Key_oslash,                   QT_TRANSLATE_NOOP("QShortcut", "oslash") },
    { Qt::Key_ugrave,                   QT_TRANSLATE_NOOP("QShortcut", "ugrave") },
    { Qt::Key_uacute,                   QT_TRANSLATE_NOOP("QShortcut", "uacute") },
    { Qt::Key_ucircumflex,              QT_TRANSLATE_NOOP("QShortcut", "ucircumflex") },
    { Qt::Key_udiaeresis,               QT_TRANSLATE_NOOP("QShortcut", "udiaeresis") },
    { Qt::Key_yacute,                   QT_TRANSLATE_NOOP("QShortcut", "yacute") },
    { Qt::Key_thorn,                    QT_TRANSLATE_NOOP("QShortcut", "thorn") },
    { Qt::Key_ydiaeresis,               QT_TRANSLATE_NOOP("QShortcut", "ydiaeresis") },
    { Qt::Key_ModeLock,                 QT_TRANSLATE_NOOP("QShortcut", "Mode Lock") },
    { Qt::Key_MonBrightnessUp,          QT_TRANSLATE_NOOP("QShortcut", "Monitor Brightness Up") },
    { Qt::Key_MonBrightnessDown,        QT_TRANSLATE_NOOP("QShortcut", "Monitor Brightness Down") },
    { Qt::Key_KbdLightOnOff,            QT_TRANSLATE_NOOP("QShortcut", "Keyboard Light On/Off") },
    { Qt::Key_KbdBrightnessUp,          QT_TRANSLATE_NOOP("QShortcut", "Keyboard Brightness Up") },
    { Qt::Key_KbdBrightnessDown,        QT_TRANSLATE_NOOP("QShortcut", "Keyboard Brightness Down") },
    { Qt::Key_MonBrightnessCycle,       QT_TRANSLATE_NOOP("QShortcut", "Monitor Brightness Cycle") },
    { Qt::Key_Standby,                  QT_TRANSLATE_NOOP("QShortcut", "Standby") },
    { Qt::Key_AudioLowerVolume,         QT_TRANSLATE_NOOP("QShortcut", "Volume Down") },
    { Qt::Key_AudioMute,                QT_TRANSLATE_NOOP("QShortcut", "Volume Mute") },
    { Qt::Key_AudioRaiseVolume,         QT_TRANSLATE_NOOP("QShortcut", "Volume Up") },
    { Qt::Key_AudioPlay,                QT_TRANSLATE_NOOP("QShortcut", "Media Play") },
    { Qt::Key_AudioStop,                QT_TRANSLATE_NOOP("QShortcut", "Media Stop") },
    { Qt::Key_AudioPrev,                QT_TRANSLATE_NOOP("QShortcut", "Media Previous") },
    { Qt::Key_AudioNext,                QT_TRANSLATE_NOOP("QShortcut", "Media Next") },
    { Qt::Key_HomePage,                 QT_TRANSLATE_NOOP("QShortcut", "Home Page") },
    { Qt::Key_Mail,                     QT_TRANSLATE_NOOP("QShortcut", "Launch Mail") },
    { Qt::Key_Start,                    QT_TRANSLATE_NOOP("QShortcut", "Start") },
    { Qt::Key_Search,                   QT_TRANSLATE_NOOP("QShortcut", "Search") },
    { Qt::Key_AudioRecord,              QT_TRANSLATE_NOOP("QShortcut", "Media Record") },
    { Qt::Key_Calculator,               QT_TRANSLATE_NOOP("QShortcut", "Calculator") },
    { Qt::Key_Memo,                     QT_TRANSLATE_NOOP("QShortcut", "Memo") },
    { Qt::Key_ToDoList,                 QT_TRANSLATE_NOOP("QShortcut", "ToDo List") },
    { Qt::Key_Calendar,                 QT_TRANSLATE_NOOP("QShortcut", "Calendar") },
    { Qt::Key_PowerDown,                QT_TRANSLATE_NOOP("QShortcut", "Power Down") },
    { Qt::Key_ContrastAdjust,           QT_TRANSLATE_NOOP("QShortcut", "Contrast Adjust") },
    { Qt::Key_RockerUp,                 QT_TRANSLATE_NOOP("QShortcut", "Rocker Up") },
    { Qt::Key_RockerDown,               QT_TRANSLATE_NOOP("QShortcut", "Rocker Down") },
    { Qt::Key_RockerEnter,              QT_TRANSLATE_NOOP("QShortcut", "Rocker Enter") },
    { Qt::Key_Back,                     QT_TRANSLATE_NOOP("QShortcut", "Back") },
    { Qt::Key_Forward,                  QT_TRANSLATE_NOOP("QShortcut", "Forward") },
    { Qt::Key_Stop,                     QT_TRANSLATE_NOOP("QShortcut", "Stop") },
    { Qt::Key_Refresh,                  QT_TRANSLATE_NOOP("QShortcut", "Refresh") },
    { Qt::Key_PowerOff,                 QT_TRANSLATE_NOOP("QShortcut", "Power Off") },
    { Qt::Key_WakeUp,                   QT_TRANSLATE_NOOP("QShortcut", "Wake Up") },
    { Qt::Key_Eject,                    QT_TRANSLATE_NOOP("QShortcut", "Eject") },
    { Qt::Key_ScreenSaver,              QT_TRANSLATE_NOOP("QShortcut", "Screensaver") },
    { Qt::Key_WWW,                      QT_TRANSLATE_NOOP("QShortcut", "WWW") },
    { Qt::Key_Sleep,                    QT_TRANSLATE_NOOP("QShortcut", "Sleep") },
    { Qt::Key_Favorites,                QT_TRANSLATE_NOOP("QShortcut", "Favorites") },
    { Qt::Key_AudioPause,               QT_TRANSLATE_NOOP("QShortcut", "Media Pause") },
    { Qt::Key_AudioMedia,               QT_TRANSLATE_NOOP("QShortcut", "Launch Media") },
    { Qt::Key_MyComputer,               QT_TRANSLATE_NOOP("QShortcut", "My Computer") },
    { Qt::Key_VendorHome,               QT_TRANSLATE_NOOP("QShortcut", "Vendor Home") },
    { Qt::Key_LightBulb,                QT_TRANSLATE_NOOP("QShortcut", "LightBulb") },
    { Qt::Key_Shop,                     QT_TRANSLATE_NOOP("QShortcut", "Shop") },
    { Qt::Key_History,                  QT_TRANSLATE_NOOP("QShortcut", "History") },
    { Qt::Key_OpenURL,                  QT_TRANSLATE_NOOP("QShortcut", "Open URL") },
    { Qt::Key_AddFavorite,              QT_TRANSLATE_NOOP("QShortcut", "Add Favorite") },
    { Qt::Key_HotLinks,                 QT_TRANSLATE_NOOP("QShortcut", "Hot Links") },
    { Qt::Key_BrightnessAdjust,         QT_TRANSLATE_NOOP("QShortcut", "Adjust Brightness") },
    { Qt::Key_Finance,                  QT_TRANSLATE_NOOP("QShortcut", "Finance") },
    { Qt::Key_Community,                QT_TRANSLATE_NOOP("QShortcut", "Community") },
    { Qt::Key_AudioRewind,              QT_TRANSLATE_NOOP("QShortcut", "Audio Rewind") },
    { Qt::Key_BackForward,              QT_TRANSLATE_NOOP("QShortcut", "Back Forward") },
    { Qt::Key_Launch0,                  QT_TRANSLATE_NOOP("QShortcut", "Launch (0)") },
    { Qt::Key_Launch1,                  QT_TRANSLATE_NOOP("QShortcut", "Launch (1)") },
    { Qt::Key_Launch2,                  QT_TRANSLATE_NOOP("QShortcut", "Launch (2)") },
    { Qt::Key_Launch3,                  QT_TRANSLATE_NOOP("QShortcut", "Launch (3)") },
    { Qt::Key_Launch4,                  QT_TRANSLATE_NOOP("QShortcut", "Launch (4)") },
    { Qt::Key_Launch5,                  QT_TRANSLATE_NOOP("QShortcut", "Launch (5)") },
    { Qt::Key_Launch6,                  QT_TRANSLATE_NOOP("QShortcut", "Launch (6)") },
    { Qt::Key_Launch7,                  QT_TRANSLATE_NOOP("QShortcut", "Launch (7)") },
    { Qt::Key_Launch8,                  QT_TRANSLATE_NOOP("QShortcut", "Launch (8)") },
    { Qt::Key_Launch9,                  QT_TRANSLATE_NOOP("QShortcut", "Launch (9)") },
    { Qt::Key_LaunchA,                  QT_TRANSLATE_NOOP("QShortcut", "Launch (A)") },
    { Qt::Key_LaunchB,                  QT_TRANSLATE_NOOP("QShortcut", "Launch (B)") },
    { Qt::Key_LaunchC,                  QT_TRANSLATE_NOOP("QShortcut", "Launch (C)") },
    { Qt::Key_LaunchD,                  QT_TRANSLATE_NOOP("QShortcut", "Launch (D)") },
    { Qt::Key_LaunchE,                  QT_TRANSLATE_NOOP("QShortcut", "Launch (E)") },
    { Qt::Key_LaunchF,                  QT_TRANSLATE_NOOP("QShortcut", "Launch (F)") },
    { Qt::Key_ApplicationLeft,          QT_TRANSLATE_NOOP("QShortcut", "Application Left") },
    { Qt::Key_ApplicationRight,         QT_TRANSLATE_NOOP("QShortcut", "Application Right") },
    { Qt::Key_Book,                     QT_TRANSLATE_NOOP("QShortcut", "Book") },
    { Qt::Key_CD,                       QT_TRANSLATE_NOOP("QShortcut", "CD") },
    { Qt::Key_Calculater,               QT_TRANSLATE_NOOP("QShortcut", "Calculater") },
    { Qt::Key_Close,                    QT_TRANSLATE_NOOP("QShortcut", "Close") },
    { Qt::Key_Copy,                     QT_TRANSLATE_NOOP("QShortcut", "Copy") },
    { Qt::Key_Cut,                      QT_TRANSLATE_NOOP("QShortcut", "Cut") },
    { Qt::Key_Display,                  QT_TRANSLATE_NOOP("QShortcut", "Display") },
    { Qt::Key_DOS,                      QT_TRANSLATE_NOOP("QShortcut", "DOS") },
    { Qt::Key_Documents,                QT_TRANSLATE_NOOP("QShortcut", "Documents") },
    { Qt::Key_Excel,                    QT_TRANSLATE_NOOP("QShortcut", "Spreadsheet") },
    { Qt::Key_Explorer,                 QT_TRANSLATE_NOOP("QShortcut", "Browser") },
    { Qt::Key_Game,                     QT_TRANSLATE_NOOP("QShortcut", "Game") },
    { Qt::Key_Go,                       QT_TRANSLATE_NOOP("QShortcut", "Go") },
    { Qt::Key_iTouch,                   QT_TRANSLATE_NOOP("QShortcut", "iTouch") },
    { Qt::Key_LogOff,                   QT_TRANSLATE_NOOP("QShortcut", "Logoff") },
    { Qt::Key_Market,                   QT_TRANSLATE_NOOP("QShortcut", "Market") },
    { Qt::Key_Meeting,                  QT_TRANSLATE_NOOP("QShortcut", "Meeting") },
    { Qt::Key_MenuKB,                   QT_TRANSLATE_NOOP("QShortcut", "Keyboard Menu") },
    { Qt::Key_MenuPB,                   QT_TRANSLATE_NOOP("QShortcut", "Menu PB") },
    { Qt::Key_MySites,                  QT_TRANSLATE_NOOP("QShortcut", "My Sites") },
    { Qt::Key_New,                      QT_TRANSLATE_NOOP("QShortcut", "New") },
    { Qt::Key_News,                     QT_TRANSLATE_NOOP("QShortcut", "News") },
    { Qt::Key_OfficeHome,               QT_TRANSLATE_NOOP("QShortcut", "Home Office") },
    { Qt::Key_Open,                     QT_TRANSLATE_NOOP("QShortcut", "Open") },
    { Qt::Key_Option,                   QT_TRANSLATE_NOOP("QShortcut", "Option") },
    { Qt::Key_Paste,                    QT_TRANSLATE_NOOP("QShortcut", "Paste") },
    { Qt::Key_Phone,                    QT_TRANSLATE_NOOP("QShortcut", "Phone") },
    { Qt::Key_Reply,                    QT_TRANSLATE_NOOP("QShortcut", "Reply") },
    { Qt::Key_Reload,                   QT_TRANSLATE_NOOP("QShortcut", "Reload") },
    { Qt::Key_RotateWindows,            QT_TRANSLATE_NOOP("QShortcut", "Rotate Windows") },
    { Qt::Key_RotationPB,               QT_TRANSLATE_NOOP("QShortcut", "Rotation PB") },
    { Qt::Key_RotationKB,               QT_TRANSLATE_NOOP("QShortcut", "Rotation KB") },
    { Qt::Key_Save,                     QT_TRANSLATE_NOOP("QShortcut", "Save") },
    { Qt::Key_ScrollUp,                 QT_TRANSLATE_NOOP("QShortcut", "Scroll Up") },
    { Qt::Key_ScrollDown,               QT_TRANSLATE_NOOP("QShortcut", "Scroll Down") },
    { Qt::Key_ScrollClick,              QT_TRANSLATE_NOOP("QShortcut", "Scroll Click") },
    { Qt::Key_Send,                     QT_TRANSLATE_NOOP("QShortcut", "Send") },
    { Qt::Key_Spell,                    QT_TRANSLATE_NOOP("QShortcut", "Spellchecker") },
    { Qt::Key_SplitScreen,              QT_TRANSLATE_NOOP("QShortcut", "Split Screen") },
    { Qt::Key_Support,                  QT_TRANSLATE_NOOP("QShortcut", "Support") },
    { Qt::Key_TaskPane,                 QT_TRANSLATE_NOOP("QShortcut", "Task Panel") },
    { Qt::Key_Terminal,                 QT_TRANSLATE_NOOP("QShortcut", "Terminal") },
    { Qt::Key_Tools,                    QT_TRANSLATE_NOOP("QShortcut", "Tools") },
    { Qt::Key_Travel,                   QT_TRANSLATE_NOOP("QShortcut", "Travel") },
    { Qt::Key_UserPB,                   QT_TRANSLATE_NOOP("QShortcut", "User PB") },
    { Qt::Key_User1KB,                  QT_TRANSLATE_NOOP("QShortcut", "User1 KB") },
    { Qt::Key_User2KB,                  QT_TRANSLATE_NOOP("QShortcut", "User2 KB") },
    { Qt::Key_Video,                    QT_TRANSLATE_NOOP("QShortcut", "Video") },
    { Qt::Key_WheelButton,              QT_TRANSLATE_NOOP("QShortcut", "Wheel Button") },
    { Qt::Key_Word,                     QT_TRANSLATE_NOOP("QShortcut", "Word Processor") },
    { Qt::Key_Xfer,                     QT_TRANSLATE_NOOP("QShortcut", "Xfer") },
    { Qt::Key_ZoomIn,                   QT_TRANSLATE_NOOP("QShortcut", "Zoom In") },
    { Qt::Key_ZoomOut,                  QT_TRANSLATE_NOOP("QShortcut", "Zoom Out") },
    { Qt::Key_Away,                     QT_TRANSLATE_NOOP("QShortcut", "Away") },
    { Qt::Key_Messenger,                QT_TRANSLATE_NOOP("QShortcut", "Messenger") },
    { Qt::Key_WebCam,                   QT_TRANSLATE_NOOP("QShortcut", "WebCam") },
    { Qt::Key_MailForward,              QT_TRANSLATE_NOOP("QShortcut", "Mail Forward") },
    { Qt::Key_Pictures,                 QT_TRANSLATE_NOOP("QShortcut", "Pictures") },
    { Qt::Key_Music,                    QT_TRANSLATE_NOOP("QShortcut", "Music") },
    { Qt::Key_Battery,                  QT_TRANSLATE_NOOP("QShortcut", "Battery") },
    { Qt::Key_Bluetooth,                QT_TRANSLATE_NOOP("QShortcut", "Bluetooth") },
    { Qt::Key_WLAN,                     QT_TRANSLATE_NOOP("QShortcut", "WLAN") },
    { Qt::Key_UWB,                      QT_TRANSLATE_NOOP("QShortcut", "Ultra Wide Band") },
    { Qt::Key_AudioForward,             QT_TRANSLATE_NOOP("QShortcut", "Media Forward") },
    { Qt::Key_AudioRepeat,              QT_TRANSLATE_NOOP("QShortcut", "Media Repeat") },
    { Qt::Key_AudioRandomPlay,          QT_TRANSLATE_NOOP("QShortcut", "Media Random Play") },
    { Qt::Key_Subtitle,                 QT_TRANSLATE_NOOP("QShortcut", "Subtitle") },
    { Qt::Key_AudioCycleTrack,          QT_TRANSLATE_NOOP("QShortcut", "Media Cycle Track") },
    { Qt::Key_CycleAngle,               QT_TRANSLATE_NOOP("QShortcut", "Cycle Angle") },
    { Qt::Key_FrameBack,                QT_TRANSLATE_NOOP("QShortcut", "Frame Back") },
    { Qt::Key_FrameForward,             QT_TRANSLATE_NOOP("QShortcut", "Frame Forward") },
    { Qt::Key_Time,                     QT_TRANSLATE_NOOP("QShortcut", "Time") },
    { Qt::Key_View,                     QT_TRANSLATE_NOOP("QShortcut", "View") },
    { Qt::Key_TopMenu,                  QT_TRANSLATE_NOOP("QShortcut", "Top Menu") },
    { Qt::Key_Red,                      QT_TRANSLATE_NOOP("QShortcut", "Red") },
    { Qt::Key_Green,                    QT_TRANSLATE_NOOP("QShortcut", "Green") },
    { Qt::Key_Yellow,                   QT_TRANSLATE_NOOP("QShortcut", "Yellow") },
    { Qt::Key_Blue,                     QT_TRANSLATE_NOOP("QShortcut", "Blue") },
    { Qt::Key_Suspend,                  QT_TRANSLATE_NOOP("QShortcut", "Suspend") },
    { Qt::Key_Hibernate,                QT_TRANSLATE_NOOP("QShortcut", "Hibernate") },
    { Qt::Key_TouchpadToggle,           QT_TRANSLATE_NOOP("QShortcut", "Touchpad Toggle") },
    { Qt::Key_TouchpadOn,               QT_TRANSLATE_NOOP("QShortcut", "Touchpad On") },
    { Qt::Key_TouchpadOff,              QT_TRANSLATE_NOOP("QShortcut", "Touchpad Off") },
    { Qt::Key_AudioMicMute,             QT_TRANSLATE_NOOP("QShortcut", "Media Microphone Mute") },
    { Qt::Key_Keyboard,                 QT_TRANSLATE_NOOP("QShortcut", "Keyboard") },
    { Qt::Key_WWAN,                     QT_TRANSLATE_NOOP("QShortcut", "WWAN") },
    { Qt::Key_RFKill,                   QT_TRANSLATE_NOOP("QShortcut", "RFKill") },
    { Qt::Key_AudioPreset,              QT_TRANSLATE_NOOP("QShortcut", "Media Preset") },
    { Qt::Key_RotationLockToggle,       QT_TRANSLATE_NOOP("QShortcut", "Rotation Lock Toggle") },
    { Qt::Key_FullScreen,               QT_TRANSLATE_NOOP("QShortcut", "FullScreen") },
    { Qt::Key_Switch_VT_1,              QT_TRANSLATE_NOOP("QShortcut", "Switch VT (1)") },
    { Qt::Key_Switch_VT_2,              QT_TRANSLATE_NOOP("QShortcut", "Switch VT (2)") },
    { Qt::Key_Switch_VT_3,              QT_TRANSLATE_NOOP("QShortcut", "Switch VT (3)") },
    { Qt::Key_Switch_VT_4,              QT_TRANSLATE_NOOP("QShortcut", "Switch VT (4)") },
    { Qt::Key_Switch_VT_5,              QT_TRANSLATE_NOOP("QShortcut", "Switch VT (5)") },
    { Qt::Key_Switch_VT_6,              QT_TRANSLATE_NOOP("QShortcut", "Switch VT (6)") },
    { Qt::Key_Switch_VT_7,              QT_TRANSLATE_NOOP("QShortcut", "Switch VT (7)") },
    { Qt::Key_Switch_VT_8,              QT_TRANSLATE_NOOP("QShortcut", "Switch VT (8)") },
    { Qt::Key_Switch_VT_9,              QT_TRANSLATE_NOOP("QShortcut", "Switch VT (9)") },
    { Qt::Key_Switch_VT_10,             QT_TRANSLATE_NOOP("QShortcut", "Switch VT (10)") },
    { Qt::Key_Switch_VT_11,             QT_TRANSLATE_NOOP("QShortcut", "Switch VT (11)") },
    { Qt::Key_Switch_VT_12,             QT_TRANSLATE_NOOP("QShortcut", "Switch VT (12)") },
    { Qt::Key_Ungrab,                   QT_TRANSLATE_NOOP("QShortcut", "Ungrab") },
    { Qt::Key_ClearGrab,                QT_TRANSLATE_NOOP("QShortcut", "Clear Grab") },
    { Qt::Key_Next_VMode,               QT_TRANSLATE_NOOP("QShortcut", "Next VMode") },
    { Qt::Key_Prev_VMode,               QT_TRANSLATE_NOOP("QShortcut", "Prev VMode") },
    { Qt::Key_LogWindowTree,            QT_TRANSLATE_NOOP("QShortcut", "Log Window Tree") },
    { Qt::Key_LogGrabInfo,              QT_TRANSLATE_NOOP("QShortcut", "Log Grab Info") },
/*
    { Qt::Key_Shift,                    QT_TRANSLATE_NOOP("QShortcut", "Shift") },
    { Qt::Key_Control,                  QT_TRANSLATE_NOOP("QShortcut", "Control") },
    { Qt::Key_Meta,                     QT_TRANSLATE_NOOP("QShortcut", "Meta") },
    { Qt::Key_Alt,                      QT_TRANSLATE_NOOP("QShortcut", "Alt") },
*/
    { Qt::Key_Backtab,                  QT_TRANSLATE_NOOP("QShortcut", "Backtab") },
/*
    { Qt::Key_Direction_L,              QT_TRANSLATE_NOOP("QShortcut", "Direction_L") },
    { Qt::Key_Direction_R,              QT_TRANSLATE_NOOP("QShortcut", "Direction_R") },
*/
    { Qt::Key_Enter,                    QT_TRANSLATE_NOOP("QShortcut", "Enter") },
/*
    { Qt::Key_AltGr,                    QT_TRANSLATE_NOOP("QShortcut", "AltGr") }
*/

    // decoding compatibility
    { Qt::Key_Print,                    QT_TRANSLATE_NOOP("QShortcut", "Print") },
    { Qt::Key_Insert,                   QT_TRANSLATE_NOOP("QShortcut", "Ins") },
    { Qt::Key_Escape,                   QT_TRANSLATE_NOOP("QShortcut", "Esc") },
    { Qt::Key_Delete,                   QT_TRANSLATE_NOOP("QShortcut", "Del") },
    { Qt::Key_Page_Up,                  QT_TRANSLATE_NOOP("QShortcut", "PgUp") },
    { Qt::Key_Page_Down,                QT_TRANSLATE_NOOP("QShortcut", "PgDown") },
    { Qt::Key_Sys_Req,                  QT_TRANSLATE_NOOP("QShortcut", "SysReq") },
    { Qt::Key_Caps_Lock,                QT_TRANSLATE_NOOP("QShortcut", "CapsLock") },
    { Qt::Key_Num_Lock,                 QT_TRANSLATE_NOOP("QShortcut", "NumLock") },
    { Qt::Key_Scroll_Lock,              QT_TRANSLATE_NOOP("QShortcut", "ScrollLock") },
    { Qt::Key_Shift_Lock,               QT_TRANSLATE_NOOP("QShortcut", "ShiftLock") },
};

static const qint16 KeyNameTblSize = sizeof(KeyNameTbl) / sizeof(KeyNameTblData);

// Table of key bindings, must be sorted by standard key priority
const QKeyBinding QKeySequencePrivate::keyBindings[] = {
    // StandardKey                             Key Sequence
    { QKeySequence::Delete,                    Qt::Key_Delete },
    { QKeySequence::Copy,                      Qt::CTRL | Qt::Key_C },
    { QKeySequence::Copy,                      Qt::Key_F16 }, // Copy on sun keyboards
    { QKeySequence::Paste,                     Qt::CTRL | Qt::Key_V },
    { QKeySequence::Paste,                     Qt::Key_F18 }, //Paste on sun keyboards
    { QKeySequence::Paste,                     Qt::SHIFT | Qt::Key_Insert },
    { QKeySequence::Cut,                       Qt::CTRL | Qt::Key_X },
    { QKeySequence::Cut,                       Qt::Key_F20 }, // Cut on sun keyboards
    { QKeySequence::Undo,                      Qt::CTRL | Qt::Key_Z },
    { QKeySequence::NextChild,                 Qt::CTRL | Qt::Key_Tab },
    { QKeySequence::PreviousChild,             Qt::CTRL | Qt::SHIFT | Qt::Key_Backtab },
    { QKeySequence::InsertParagraphSeparator,  Qt::Key_Return },
    { QKeySequence::InsertLineSeparator,       Qt::SHIFT | Qt::Key_Return },
    // Non-priority
    { QKeySequence::Back,                      Qt::ALT  | Qt::Key_Left },
    { QKeySequence::Forward,                   Qt::ALT  | Qt::Key_Right },
    { QKeySequence::MoveToPreviousPage,        Qt::Key_PageUp },
    { QKeySequence::MoveToNextPage,            Qt::Key_PageDown },
    { QKeySequence::WhatsThis,                 Qt::SHIFT | Qt::Key_F1 },
    { QKeySequence::ZoomIn,                    Qt::CTRL | Qt::Key_Plus },
    { QKeySequence::ZoomOut,                   Qt::CTRL | Qt::Key_Minus },
    { QKeySequence::SelectAll,                 Qt::CTRL | Qt::Key_A },
    { QKeySequence::Bold,                      Qt::CTRL | Qt::Key_B },
    { QKeySequence::New,                       Qt::CTRL | Qt::Key_N },
    { QKeySequence::Open,                      Qt::CTRL | Qt::Key_O },
    { QKeySequence::Print,                     Qt::CTRL | Qt::Key_P },
    { QKeySequence::Save,                      Qt::CTRL | Qt::Key_S },
    { QKeySequence::Underline,                 Qt::CTRL | Qt::Key_U },
    { QKeySequence::InsertParagraphSeparator,  Qt::Key_Enter },
    { QKeySequence::MoveToStartOfLine,         Qt::Key_Home },
    { QKeySequence::MoveToEndOfLine,           Qt::Key_End },
    { QKeySequence::MoveToPreviousChar,        Qt::Key_Left },
    { QKeySequence::MoveToPreviousLine,        Qt::Key_Up },
    { QKeySequence::MoveToNextChar,            Qt::Key_Right },
    { QKeySequence::MoveToNextLine,            Qt::Key_Down },
    { QKeySequence::HelpContents,              Qt::Key_F1 },
    { QKeySequence::FindNext,                  Qt::Key_F3 },
    { QKeySequence::Refresh,                   Qt::Key_F5 },
    { QKeySequence::Undo,                      Qt::Key_F14 }, // Undo on sun keyboards
    { QKeySequence::PreviousChild,             Qt::Key_Back },
    { QKeySequence::NextChild,                 Qt::Key_Forward },
    { QKeySequence::InsertLineSeparator,       Qt::SHIFT | Qt::Key_Enter },
    { QKeySequence::Cut,                       Qt::SHIFT | Qt::Key_Delete },
    { QKeySequence::SelectStartOfLine,         Qt::SHIFT | Qt::Key_Home },
    { QKeySequence::SelectEndOfLine,           Qt::SHIFT | Qt::Key_End },
    { QKeySequence::SelectPreviousChar,        Qt::SHIFT | Qt::Key_Left },
    { QKeySequence::SelectPreviousLine,        Qt::SHIFT | Qt::Key_Up },
    { QKeySequence::SelectNextChar,            Qt::SHIFT | Qt::Key_Right },
    { QKeySequence::SelectNextLine,            Qt::SHIFT | Qt::Key_Down },
    { QKeySequence::SelectPreviousPage,        Qt::SHIFT | Qt::Key_PageUp },
    { QKeySequence::SelectNextPage,            Qt::SHIFT | Qt::Key_PageDown },
    { QKeySequence::FindPrevious,              Qt::SHIFT | Qt::Key_F3 },
    { QKeySequence::Delete,                    Qt::CTRL | Qt::Key_D }, // Emacs (line edit only)
    { QKeySequence::Find,                      Qt::CTRL | Qt::Key_F },
    { QKeySequence::Italic,                    Qt::CTRL | Qt::Key_I },
    { QKeySequence::DeleteEndOfLine,           Qt::CTRL | Qt::Key_K }, // Emacs (line edit only)
    { QKeySequence::AddTab,                    Qt::CTRL | Qt::Key_T },
    { QKeySequence::Close,                     Qt::CTRL | Qt::Key_W },
    { QKeySequence::DeleteStartOfWord,         Qt::CTRL | Qt::Key_Backspace },
    { QKeySequence::Copy,                      Qt::CTRL | Qt::Key_Insert },
    { QKeySequence::DeleteEndOfWord,           Qt::CTRL | Qt::Key_Delete },
    { QKeySequence::MoveToStartOfDocument,     Qt::CTRL | Qt::Key_Home },
    { QKeySequence::MoveToEndOfDocument,       Qt::CTRL | Qt::Key_End },
    { QKeySequence::MoveToPreviousWord,        Qt::CTRL | Qt::Key_Left },
    { QKeySequence::MoveToNextWord,            Qt::CTRL | Qt::Key_Right },
    { QKeySequence::Redo,                      Qt::CTRL | Qt::SHIFT | Qt::Key_Z },
    { QKeySequence::Paste,                     Qt::CTRL | Qt::SHIFT | Qt::Key_Insert },
    { QKeySequence::SelectStartOfDocument,     Qt::CTRL | Qt::SHIFT | Qt::Key_Home },
    { QKeySequence::SelectEndOfDocument,       Qt::CTRL | Qt::SHIFT | Qt::Key_End },
    { QKeySequence::SelectPreviousWord,        Qt::CTRL | Qt::SHIFT | Qt::Key_Left },
    { QKeySequence::SelectNextWord,            Qt::CTRL | Qt::SHIFT | Qt::Key_Right },
};

const short QKeySequencePrivate::numberOfKeyBindings = sizeof(QKeySequencePrivate::keyBindings) / sizeof(QKeyBinding);

/*!
    \enum QKeySequence::StandardKey
    \since 4.2

    This enum represent standard key bindings. They can be used to
    assign platform dependent keyboard shortcuts to a QAction.

    Note that the key bindings are platform dependent. The currently
    bound shortcuts can be queried using keyBindings().

    \value AddTab           Add new tab.
    \value Back             Navigate back.
    \value Bold             Bold text.
    \value Close            Close document/tab.
    \value Copy             Copy.
    \value Cut              Cut.
    \value Delete           Delete.
    \value DeleteEndOfLine          Delete end of line.
    \value DeleteEndOfWord          Delete word from the end of the cursor.
    \value DeleteStartOfWord        Delete the beginning of a word up to the cursor.
    \value Find             Find in document.
    \value FindNext         Find next result.
    \value FindPrevious     Find previous result.
    \value Forward          Navigate forward.
    \value HelpContents     Open help contents.
    \value InsertLineSeparator      Insert a new line.
    \value InsertParagraphSeparator Insert a new paragraph.
    \value Italic           Italic text.
    \value MoveToEndOfBlock         Move cursor to end of block. This shortcut is only used on the OS X.
    \value MoveToEndOfDocument      Move cursor to end of document.
    \value MoveToEndOfLine          Move cursor to end of line.
    \value MoveToNextChar           Move cursor to next character.
    \value MoveToNextLine           Move cursor to next line.
    \value MoveToNextPage           Move cursor to next page.
    \value MoveToNextWord           Move cursor to next word.
    \value MoveToPreviousChar       Move cursor to previous character.
    \value MoveToPreviousLine       Move cursor to previous line.
    \value MoveToPreviousPage       Move cursor to previous page.
    \value MoveToPreviousWord       Move cursor to previous word.
    \value MoveToStartOfBlock       Move cursor to start of a block. This shortcut is only used on OS X.
    \value MoveToStartOfDocument    Move cursor to start of document.
    \value MoveToStartOfLine        Move cursor to start of line.
    \value New              Create new document.
    \value NextChild        Navigate to next tab or child window.
    \value Open             Open document.
    \value Paste            Paste.
    \value Preferences      Open the preferences dialog.
    \value PreviousChild    Navigate to previous tab or child window.
    \value Print            Print document.
    \value Quit             Quit the application.
    \value Redo             Redo.
    \value Refresh          Refresh or reload current document.
    \value Replace          Find and replace.
    \value SaveAs           Save document after prompting the user for a file name.
    \value Save             Save document.
    \value SelectAll        Select all text.
    \value SelectEndOfBlock         Extend selection to the end of a text block. This shortcut is only used on OS X.
    \value SelectEndOfDocument      Extend selection to end of document.
    \value SelectEndOfLine          Extend selection to end of line.
    \value SelectNextChar           Extend selection to next character.
    \value SelectNextLine           Extend selection to next line.
    \value SelectNextPage           Extend selection to next page.
    \value SelectNextWord           Extend selection to next word.
    \value SelectPreviousChar       Extend selection to previous character.
    \value SelectPreviousLine       Extend selection to previous line.
    \value SelectPreviousPage       Extend selection to previous page.
    \value SelectPreviousWord       Extend selection to previous word.
    \value SelectStartOfBlock       Extend selection to the start of a text block. This shortcut is only used on OS X.
    \value SelectStartOfDocument    Extend selection to start of document. 
    \value SelectStartOfLine        Extend selection to start of line.
    \value Underline        Underline text.
    \value Undo             Undo.
    \value UnknownKey       Unbound key.
    \value WhatsThis        Activate whats this.
    \value ZoomIn           Zoom in.
    \value ZoomOut          Zoom out.
*/

/*!
    \since 4.2

    Constructs a QKeySequence object for the given \a key. 
    The result will depend on the currently running platform. 

    The resulting object will be based on the first element in the 
    list of key bindings for the \a key.
*/
QKeySequence::QKeySequence(StandardKey key)
{
    const QList <QKeySequence> bindings = keyBindings(key);
    // pick only the first/primary shortcut from current bindings
    if (bindings.size() > 0) {
        const QKeySequence &first = bindings.first();
        key1 = first.key1;
        key2 = first.key2;
    } else {
        key1 = 0;
        key2 = 0;
    }
}


/*!
    Constructs an empty key sequence.
*/
QKeySequence::QKeySequence()
    : key1(0), key2(0)
{
}

/*!
    Creates a key sequence from the \a key string based on
    \a format.. For example "Ctrl+O" gives CTRL+'O'. The strings
    "Ctrl", "Shift", "Alt" and "Meta" are recognized, as well as
    their translated equivalents in the "QShortcut" context (using
    QObject::tr()).

    Up to two key codes may be entered by separating them with
    commas, e.g. "Alt+X,Ctrl+S".

    \a key should be in NativeText format.

    This constructor is typically used with \link QObject::tr() tr
    \endlink(), so that shortcut keys can be replaced in
    translations:

    \snippet doc/src/snippets/code/src_gui_kernel_qkeysequence.cpp 2

    Note the "File|Open" translator comment. It is by no means
    necessary, but it provides some context for the human translator.
*/
QKeySequence::QKeySequence(const QString &key, QKeySequence::SequenceFormat format)
    : key1(0), key2(0)
{
    QString keyseq = key;
    QString part;
    int n = 0;
    int p = 0, diff = 0;

    // Run through the whole string, but stop
    // if we have 2 keys before the end.
    while (keyseq.length() && n < 2) {
        // We MUST use something to separate each sequence, and space
        // does not cut it, since some of the key names have space
        // in them.. (Let's hope no one translate with a comma in it)
        p = keyseq.indexOf(QLatin1Char(','));
        if (-1 != p) {
            if (p == keyseq.count() - 1) { // Last comma 'Ctrl+,'
                p = -1;
            } else {
                if (QLatin1Char(',') == keyseq.at(p+1)) // e.g. 'Ctrl+,, Shift+,,'
                    p++;
                if (QLatin1Char(' ') == keyseq.at(p+1)) { // Space after comma
                    diff = 1;
                    p++;
                } else {
                    diff = 0;
                }
            }
        }
        part = keyseq.left(-1 == p ? keyseq.length() : p - diff);
        keyseq = keyseq.right(-1 == p ? 0 : keyseq.length() - (p + 1));
        if (n == 0) {
            key1 = QKeySequencePrivate::decodeString(part, format);
        } else {
            key2 = QKeySequencePrivate::decodeString(part, format);
        }
        ++n;
    }
}

/*!
    Constructs a key sequence with up to 2 keys \a k1 and \a k2.

    The key codes are listed in Qt::Key and can be combined with
    modifiers (see Qt::Modifier) such as Qt::SHIFT, Qt::CTRL,
    Qt::ALT, or Qt::META.
*/
QKeySequence::QKeySequence(int k1, int k2)
    : key1(k1), key2(k2)
{
}

/*!
    Copy constructor. Makes a copy of \a keysequence.
 */
QKeySequence::QKeySequence(const QKeySequence& keysequence)
    : key1(keysequence.key1), key2(keysequence.key2)
{
}

/*!
    \since 4.2

    Returns a list of key bindings for the given \a key.
    The result of calling this function will vary based on the target platform. 
    The first element of the list indicates the primary shortcut for the given platform. 
    If the result contains more than one result, these can
    be considered alternative shortcuts on the same platform for the given \a key.
*/
QList<QKeySequence> QKeySequence::keyBindings(StandardKey key)
{
    QList <QKeySequence> list;
    for (short i = 0; i < QKeySequencePrivate::numberOfKeyBindings; i++) {
        if (QKeySequencePrivate::keyBindings[i].standardKey == key) {
            list.append(QKeySequence(QKeySequencePrivate::keyBindings[i].shortcut));
        }
    }
    return list;
}

/*!
    Destroys the key sequence.
 */
QKeySequence::~QKeySequence()
{
}

/*!
    Returns the number of keys in the key sequence.
    The maximum is 2.
 */
int QKeySequence::count() const
{
    if (!key1)
        return 0;
    if (!key2)
        return 1;
    return 2;
}


/*!
    Returns true if the key sequence is empty; otherwise returns
    false.
*/
bool QKeySequence::isEmpty() const
{
    return (key1 <= 0);
}


/*!
    Returns the shortcut key sequence for the mnemonic in \a text,
    or an empty key sequence if no mnemonics are found.

    For example, mnemonic("E&xit") returns \c{Qt::ALT+Qt::Key_X},
    mnemonic("&Quit") returns \c{ALT+Key_Q}, and mnemonic("Quit")
    returns an empty QKeySequence.

    We provide a \l{accelerators.html}{list of common mnemonics}
    in English. At the time of writing, Microsoft and Open Group do
    not appear to have issued equivalent recommendations for other
    languages.
*/
QKeySequence QKeySequence::mnemonic(const QString &text)
{
    QKeySequence ret;
    bool found = false;
    int p = 0;
    while (p >= 0) {
        p = text.indexOf(QLatin1Char('&'), p) + 1;
        if (p <= 0 || p >= text.length())
            break;
        QChar c = text.at(p);
        if (c != QLatin1Char('&')) {
            if (c.isPrint()) {
                if (!found) {
                    c = c.toUpper();
                    ret = QKeySequence(c.unicode() + Qt::ALT);
#ifdef QT_NO_DEBUG
                    return ret;
#else
                    found = true;
                } else {
                    qWarning("QKeySequence::mnemonic: \"%s\" contains multiple occurrences of '&'", qPrintable(text));
#endif
                }
            }
        }
        p++;
    }
    return ret;
}

struct QModifKeyName {
    QModifKeyName() : qt_key(Qt::CTRL) { }
    QModifKeyName(Qt::Modifier q, QChar n) : qt_key(q), name(n) { }
    QModifKeyName(Qt::Modifier q, const QString &n) : qt_key(q), name(n) { }
    const Qt::Modifier qt_key;
    const QString name;
};

int QKeySequencePrivate::decodeString(const QString &str, QKeySequence::SequenceFormat format)
{
    int ret = 0;
    QString accel = str.toLower();
    const bool nativeText = (format == QKeySequence::NativeText);

    QList<QModifKeyName> modifs;
    if (nativeText) {
        modifs << QModifKeyName(Qt::CTRL, QShortcut::tr("Ctrl").toLower().append(QLatin1Char('+')))
               << QModifKeyName(Qt::SHIFT, QShortcut::tr("Shift").toLower().append(QLatin1Char('+')))
               << QModifKeyName(Qt::ALT, QShortcut::tr("Alt").toLower().append(QLatin1Char('+')))
               << QModifKeyName(Qt::META, QShortcut::tr("Meta").toLower().append(QLatin1Char('+')));
    }
    // Test non-translated ones last
    modifs << QModifKeyName(Qt::CTRL, QLatin1String("ctrl+"));
    modifs << QModifKeyName(Qt::SHIFT, QLatin1String("shift+"));
    modifs << QModifKeyName(Qt::ALT, QLatin1String("alt+"));
    modifs << QModifKeyName(Qt::META, QLatin1String("meta+"));

    int i = 0;
    int lastI = 0;
    while ((i = accel.indexOf(QLatin1Char('+'), i + 1)) != -1) {
        const QString sub = accel.mid(lastI, i - lastI + 1);
        // Just shortcut the check here if we only have one character.
        // Rational: A modifier will contain the name AND +, so longer than 1, a length of 1 is just
        // the remaining part of the shortcut (ei. The 'C' in "Ctrl+C"), so no need to check that.
        if (sub.length() > 1) {
            bool foundmatch = false;
            foreach (const QModifKeyName &mkf, modifs) {
                if (sub == mkf.name) {
                    foundmatch = true;
                    ret |= mkf.qt_key;
                    break; // Shortcut, since if we find an other it would/should just be a dup
                }
            }

            if (!foundmatch) {
                // invalid meta key, that's invalid shortcut
                return 0;
            }
        }
        lastI = i + 1;
    }

    const int p = accel.lastIndexOf(QLatin1Char('+'), str.length() - 2); // -2 so that Ctrl++ works
    if(p > 0)
        accel = accel.mid(p + 1);

    bool foundmatch = false;
    // For NativeText, check the traslation table first,
    // if we don't find anything then try it out with just the untranlated stuff.
    // PortableText will only try the untranlated table.
    for (qint16 i = 0; i < KeyNameTblSize; ++i) {
        QString keyName(nativeText
                        ? QShortcut::tr(KeyNameTbl[i].name)
                        : QString::fromLatin1(KeyNameTbl[i].name));
        if (accel == keyName.toLower()) {
            ret |= KeyNameTbl[i].key;
            foundmatch = true;
            break;
        }
    }

    if (!foundmatch) {
        // that's invalid shortcut
        return 0;
    }

    return ret;
}

static inline void addKey(QString &str, const QString &theKey, QKeySequence::SequenceFormat format)
{
    if (!str.isEmpty())
        str += (format == QKeySequence::NativeText) ? QShortcut::tr("+")
                                                    : QString::fromLatin1("+");
    str += theKey;
}

QString QKeySequencePrivate::encodeString(int key, QKeySequence::SequenceFormat format)
{
    bool nativeText = (format == QKeySequence::NativeText);
    QString s;
    // On other systems the order is Meta, Control, Alt, Shift
    if ((key & Qt::META) == Qt::META)
        s = nativeText ? QShortcut::tr("Meta") : QString::fromLatin1("Meta");
    if ((key & Qt::CTRL) == Qt::CTRL)
        addKey(s, nativeText ? QShortcut::tr("Ctrl") : QString::fromLatin1("Ctrl"), format);
    if ((key & Qt::ALT) == Qt::ALT)
        addKey(s, nativeText ? QShortcut::tr("Alt") : QString::fromLatin1("Alt"), format);
    if ((key & Qt::SHIFT) == Qt::SHIFT)
        addKey(s, nativeText ? QShortcut::tr("Shift") : QString::fromLatin1("Shift"), format);


    key &= ~(Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier);
    QString p;

    bool foundmatch = false;
    for (qint16 i = 0; i < KeyNameTblSize; i++) {
        if (KeyNameTbl[i].key == key) {
            p = nativeText ? QShortcut::tr(KeyNameTbl[i].name)
                            : QString::fromLatin1(KeyNameTbl[i].name);
            foundmatch = true;
            break;
        }
    }

    if (!foundmatch) {
        // that's invalid shortcut
        return QString();
    }

    addKey(s, p, format);
    return s;
}
/*!
    Matches the sequence with \a seq. Returns ExactMatch if
    successful, PartialMatch if \a seq matches incompletely,
    and NoMatch if the sequences have nothing in common.
    Returns NoMatch if \a seq is shorter.
*/
QKeySequence::SequenceMatch QKeySequence::matches(const QKeySequence &seq) const
{
    const int userN = count();
    const int seqN = seq.count();

    if (userN > seqN)
        return SequenceMatch::NoMatch;

    if (userN == 1) {
        if (key1 != seq.key1)
            return SequenceMatch::NoMatch;
    } else if (userN == 2) {
        if (key2 != seq.key2)
            return SequenceMatch::NoMatch;
    }

    // If equal in length, we have a potential ExactMatch sequence,
    // else we already know it can only be partial.
    if (userN == seqN)
        return SequenceMatch::ExactMatch;
    return SequenceMatch::PartialMatch;
}

/*!
   Returns the key sequence as a QVariant
*/
QKeySequence::operator QVariant() const
{
    return QVariant(QVariant::KeySequence, this);
}

/*!
    \obsolete
    For backward compatibility: returns the first keycode
    as integer. If the key sequence is empty, 0 is returned.
 */
QKeySequence::operator int () const
{
    if (1 <= count())
        return key1;
    return 0;
}


/*!
    Returns a reference to the element at position \a index in the key
    sequence. This can only be used to read an element.
 */
int QKeySequence::operator[](uint index) const
{
    Q_ASSERT_X(index < 2, "QKeySequence::operator[]", "index out of range");
    switch (index) {
        case 0: {
            return key1;
        }
        case 1: {
            return key2;
        }
    }
    Q_UNREACHABLE();
}


/*!
    Assignment operator. Assigns the \a other key sequence to this
    object.
 */
QKeySequence &QKeySequence::operator=(const QKeySequence &other)
{
    key1 = other.key1;
    key2 = other.key2;
    return *this;
}

/*!
    \fn void QKeySequence::swap(QKeySequence &other)
    \since 4.8

    Swaps key sequence \a other with this key sequence. This operation is very
    fast and never fails.
*/

/*!
    \fn bool QKeySequence::operator!=(const QKeySequence &other) const

    Returns true if this key sequence is not equal to the \a other
    key sequence; otherwise returns false.
*/


/*!
    Returns true if this key sequence is equal to the \a other
    key sequence; otherwise returns false.
 */
bool QKeySequence::operator==(const QKeySequence &other) const
{
    return (key1 == other.key1 && key2 == other.key2);
}


/*!
    Provides an arbitrary comparison of this key sequence and
    \a other key sequence. All that is guaranteed is that the
    operator returns false if both key sequences are equal and
    that (ks1 \< ks2) == !( ks2 \< ks1) if the key sequences
    are not equal.

    This function is useful in some circumstances, for example
    if you want to use QKeySequence objects as keys in a QMap.

    \sa operator==() operator!=() operator>() operator<=() operator>=()
*/
bool QKeySequence::operator< (const QKeySequence &other) const
{
    if (key1 != other.key1)
        return key1 < other.key1;
    if (key2 != other.key2)
        return key2 < other.key2;
    return false;
}

/*!
    \fn bool QKeySequence::operator> (const QKeySequence &other) const

    Returns true if this key sequence is larger than the \a other key
    sequence; otherwise returns false.

    \sa operator==() operator!=() operator<() operator<=() operator>=()
*/

/*!
    \fn bool QKeySequence::operator<= (const QKeySequence &other) const

    Returns true if this key sequence is smaller or equal to the
    \a other key sequence; otherwise returns false.

    \sa operator==() operator!=() operator<() operator>() operator>=()
*/

/*!
    \fn bool QKeySequence::operator>= (const QKeySequence &other) const

    Returns true if this key sequence is larger or equal to the
    \a other key sequence; otherwise returns false.

    \sa operator==() operator!=() operator<() operator>() operator<=()
*/

/*!
    \since 4.1

    Return a string representation of the key sequence,
    based on \a format.

    For example, the value Qt::CTRL+Qt::Key_O results in "Ctrl+O".
    If the key sequence has multiple key codes, each is separated
    by commas in the string returned, such as "Alt+X, Ctrl+Y, Z".
    The strings, "Ctrl", "Shift", etc. are translated using
    QObject::tr() in the "QShortcut" context.

    If the key sequence has no keys, an empty string is returned.

    \sa fromString()
*/
QString QKeySequence::toString(SequenceFormat format) const
{
    // A standard string, with no translation or anything like that. In some ways it will
    // look like our latin case on X11
    switch (count()) {
        case 0: {
            return QString();
        }
        case 1: {
            return QKeySequencePrivate::encodeString(key1, format);
        }
        case 2: {
            const QString firststring(QKeySequencePrivate::encodeString(key1, format));
            const QString secondstring(QKeySequencePrivate::encodeString(key2, format));
            return QString::fromLatin1("%1, %2").arg(firststring, secondstring);
        }
    }
    Q_UNREACHABLE();
}

/*!
    \since 4.1

    Return a QKeySequence from the string \a str based on \a format.

    \sa toString()
*/
QKeySequence QKeySequence::fromString(const QString &str, SequenceFormat format)
{
    return QKeySequence(str, format);
}

/*****************************************************************************
  QKeySequence stream functions
 *****************************************************************************/
#if !defined(QT_NO_DATASTREAM)
/*!
    \fn QDataStream &operator<<(QDataStream &stream, const QKeySequence &sequence)
    \relates QKeySequence

    Writes the key \a sequence to the \a stream.

    \sa \link datastreamformat.html Format of the QDataStream operators \endlink
*/
QDataStream &operator<<(QDataStream &s, const QKeySequence &keysequence)
{
    s << keysequence.key1;
    s << keysequence.key2;
    return s;
}


/*!
    \fn QDataStream &operator>>(QDataStream &stream, QKeySequence &sequence)
    \relates QKeySequence

    Reads a key sequence from the \a stream into the key \a sequence.

    \sa \link datastreamformat.html Format of the QDataStream operators \endlink
*/
QDataStream &operator>>(QDataStream &s, QKeySequence &keysequence)
{
    int keys[2] = { 0, 0 };
    for (int i = 0; i < 2; ++i) {
        if (Q_UNLIKELY(s.atEnd())) {
            keysequence = QKeySequence();
            s.setStatus(QDataStream::ReadCorruptData);
            return s;
        }
        s >> keys[i];
    }
    keysequence = QKeySequence(keys[0], keys[1]);
    return s;
}

#endif //QT_NO_DATASTREAM

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QKeySequence &p)
{
#ifndef Q_BROKEN_DEBUG_STREAM
    dbg.nospace() << "QKeySequence(" << p.toString() << ')';
    return dbg.space();
#else
    qWarning("This compiler doesn't support streaming QKeySequence to QDebug");
    return dbg;
    Q_UNUSED(p);
#endif
}
#endif

#endif // QT_NO_SHORTCUT

QT_END_NAMESPACE




