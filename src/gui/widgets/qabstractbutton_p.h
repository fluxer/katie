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

#ifndef QABSTRACTBUTTON_P_H
#define QABSTRACTBUTTON_P_H

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

#include "qbasictimer.h"
#include "qwidget_p.h"

QT_BEGIN_NAMESPACE

class QAbstractButtonPrivate : public QWidgetPrivate
{
    Q_DECLARE_PUBLIC(QAbstractButton)
public:
    QAbstractButtonPrivate(QSizePolicy::ControlType type = QSizePolicy::DefaultType);

    QString text;
    QIcon icon;
    QSize iconSize;
#ifndef QT_NO_SHORTCUT
    QKeySequence shortcut;
    int shortcutId;
#endif
    bool checkable;
    bool checked;
    bool autoRepeat;
    bool autoExclusive;
    bool down;
    bool blockRefresh;
    bool pressed;

#ifndef QT_NO_BUTTONGROUP
    QButtonGroup* group;
#endif
    QBasicTimer repeatTimer;
    QBasicTimer animateTimer;

    int autoRepeatDelay, autoRepeatInterval;

    QSizePolicy::ControlType controlType;
    mutable QSize sizeHint;

    void init();
    void click();
    void refresh();

    QList<QAbstractButton *>queryButtonList() const;
    QAbstractButton *queryCheckedButton() const;
    void notifyChecked();
    void moveFocus(int key);
    void fixFocusPolicy();

    void emitPressed();
    void emitReleased();
    void emitClicked();
};

QT_END_NAMESPACE

#endif // QABSTRACTBUTTON_P_H
