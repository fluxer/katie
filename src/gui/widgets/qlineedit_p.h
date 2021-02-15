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

#ifndef QLINEEDIT_P_H
#define QLINEEDIT_P_H

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

#include "QtCore/qglobal.h"

#ifndef QT_NO_LINEEDIT
#include "qwidget_p.h"
#include "QtGui/qlineedit.h"
#include "QtGui/qtextlayout.h"
#include "QtGui/qstyleoption.h"
#include "QtCore/qbasictimer.h"
#include "QtGui/qcompleter.h"
#include "QtCore/qpointer.h"

#include "qlinecontrol_p.h"

QT_BEGIN_NAMESPACE

class QLineEditPrivate : public QWidgetPrivate
{
    Q_DECLARE_PUBLIC(QLineEdit)
public:

    QLineEditPrivate()
        : control(0), frame(true), contextMenuEnabled(true), cursorVisible(false),
        dragEnabled(false), clickCausedFocus(false), hscroll(0), vscroll(0),
        alignment(Qt::AlignLeft | Qt::AlignVCenter),
        leftTextMargin(0), topTextMargin(0), rightTextMargin(0), bottomTextMargin(0)
    {
    }

    ~QLineEditPrivate()
    {
    }

    QLineControl *control;

#ifndef QT_NO_CONTEXTMENU
    QPointer<QAction> selectAllAction;
#endif
    void init(const QString&);

    QRect adjustedControlRect(const QRect &) const;

    int xToPos(int x, QTextLine::CursorPosition = QTextLine::CursorBetweenCharacters) const;
    QRect cursorRect() const;
    void setCursorVisible(bool visible);

    void updatePasswordEchoEditing(bool);

    QPoint tripleClick;
    QBasicTimer tripleClickTimer;
    bool frame;
    bool contextMenuEnabled;
    bool cursorVisible;
    bool dragEnabled;
    bool clickCausedFocus;
    int hscroll;
    int vscroll;
    Qt::Alignment alignment;
    static const int verticalMargin;
    static const int horizontalMargin;

    QRect adjustedContentsRect() const;

    void _q_handleWindowActivate();
    void _q_textEdited(const QString &);
    void _q_cursorPositionChanged(int, int);
    void _q_selectionChanged();
    void _q_updateNeeded(const QRect &);
#ifndef QT_NO_COMPLETER
    void _q_completionHighlighted(const QString&);
#endif
#ifndef QT_NO_DRAGANDDROP
    QPoint dndPos;
    QBasicTimer dndTimer;
    void drag();
#endif

    int leftTextMargin;
    int topTextMargin;
    int rightTextMargin;
    int bottomTextMargin;

    QString placeholderText;
};

#endif // QT_NO_LINEEDIT

QT_END_NAMESPACE

#endif // QLINEEDIT_P_H
