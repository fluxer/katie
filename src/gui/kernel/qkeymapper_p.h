/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtGui module of the Qt Toolkit.
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
#ifndef QKEYMAPPER_P_H
#define QKEYMAPPER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <qobject.h>
#include <qobject_p.h>
#include <qkeysequence.h>
#include <qlist.h>
#include <qlocale.h>
#include <qevent.h>

QT_BEGIN_NAMESPACE

class QKeyMapperPrivate;
class QKeyMapper : public QObject
{
    Q_OBJECT
public:
    explicit QKeyMapper();
    ~QKeyMapper();

    static QKeyMapper *instance();
    static void changeKeyboard();
    static bool sendKeyEvent(QWidget *widget,
                             QEvent::Type type, int code, Qt::KeyboardModifiers modifiers,
                             const QString &text, bool autorepeat, int count,
                             quint32 nativeScanCode, quint32 nativeVirtualKey, quint32 nativeModifiers);
    static QList<int> possibleKeys(QKeyEvent *e);

private:
    friend QKeyMapperPrivate *qt_keymapper_private();
    Q_DECLARE_PRIVATE(QKeyMapper)
    Q_DISABLE_COPY(QKeyMapper)
};



#if defined(Q_WS_X11)

QT_BEGIN_INCLUDE_NAMESPACE
typedef ulong XID;
typedef XID KeySym;
QT_END_INCLUDE_NAMESPACE

struct QXCoreDesc {
    int min_keycode;
    int max_keycode;
    int keysyms_per_keycode;
    KeySym *keysyms;
    uchar mode_switch;
    uchar num_lock;
    KeySym lock_meaning;
};

#endif

class QKeyEvent;
class QKeyMapperPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QKeyMapper)
public:
    QKeyMapperPrivate();
    ~QKeyMapperPrivate();

    void clearMappings();
    QList<int> possibleKeys(QKeyEvent *e);

    QLocale keyboardInputLocale;
    Qt::LayoutDirection keyboardInputDirection;

#if defined(Q_WS_X11)
    bool translateKeyEventInternal(QWidget *keywidget,
                                   const XEvent *,
                                   KeySym &keysym,
                                   int& count,
                                   QString& text,
                                   Qt::KeyboardModifiers& modifiers,
                                   int &code,
                                   QEvent::Type &type,
                                   bool statefulTranslation = true);
    bool translateKeyEvent(QWidget *keywidget,
                           const XEvent *,
                           bool grab);

    QXCoreDesc coreDesc;
#endif
};

QKeyMapperPrivate *qt_keymapper_private(); // from qkeymapper.cpp

QT_END_NAMESPACE

#endif // QKEYMAPPER_P_H
