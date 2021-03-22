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
#ifndef QKEYMAPPER_P_H
#define QKEYMAPPER_P_H

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

#include "qkeysequence.h"
#include "qlist.h"
#include "qlocale.h"
#include "qevent.h"

#if defined(Q_WS_X11)
#    include "qt_x11_p.h"
#endif

QT_BEGIN_NAMESPACE

class QKeyMapper
{
public:
    explicit QKeyMapper();
    ~QKeyMapper();


    static void changeKeyboard();
    static QList<int> possibleKeys(QKeyEvent *e);

    void clearMappings();

    QLocale keyboardInputLocale;
    Qt::LayoutDirection keyboardInputDirection;
    QTextCodec *keyMapperCodec;

#if defined(Q_WS_X11)
    bool translateKeyEvent(QWidget *keywidget, const XEvent *event);
    Qt::KeyboardModifiers translateModifiers(int state);
#endif

private:
    Q_DISABLE_COPY(QKeyMapper)
};

QKeyMapper *qt_keymapper(); // from qkeymapper.cpp

QT_END_NAMESPACE

#endif // QKEYMAPPER_P_H
