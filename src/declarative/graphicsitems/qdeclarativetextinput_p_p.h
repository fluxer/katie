/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtDeclarative module of the Katie Toolkit.
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

#ifndef QDECLARATIVETEXTINPUT_P_H
#define QDECLARATIVETEXTINPUT_P_H

#include "qdeclarativetextinput_p.h"
#include "qdeclarativeimplicitsizeitem_p_p.h"
#include "qdeclarative.h"
#include "qpointer.h"
#include "qlinecontrol_p.h"

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef QT_NO_LINEEDIT

QT_BEGIN_NAMESPACE

class Q_AUTOTEST_EXPORT QDeclarativeTextInputPrivate : public QDeclarativeImplicitSizePaintedItemPrivate
{
    Q_DECLARE_PUBLIC(QDeclarativeTextInput)
public:
    QDeclarativeTextInputPrivate() : control(new QLineControl),
                 color((QRgb)0), style(QDeclarativeText::Normal),
                 styleColor((QRgb)0), hAlign(QDeclarativeTextInput::AlignLeft),
                 mouseSelectionMode(QDeclarativeTextInput::SelectCharacters),
                 hscroll(0), oldScroll(0), oldValidity(false), focused(false), focusOnPress(true),
                 showInputPanelOnFocus(true), clickCausedFocus(false), cursorVisible(false),
                 autoScroll(true), selectByMouse(false), canPaste(false), hAlignImplicit(true),
                 selectPressed(false)
    {

    }

    ~QDeclarativeTextInputPrivate()
    {
    }

    int xToPos(int x, QTextLine::CursorPosition betweenOrOn = QTextLine::CursorBetweenCharacters) const
    {
        Q_Q(const QDeclarativeTextInput);
        QRect cr = q->boundingRect().toRect();
        x-= cr.x() - hscroll;
        return control->xToPos(x, betweenOrOn);
    }

    void init();
    void startCreatingCursor();
    void focusChanged(bool hasFocus);
    void updateHorizontalScroll();
    bool determineHorizontalAlignment();
    bool setHAlign(QDeclarativeTextInput::HAlignment, bool forceAlign = false);
    void mirrorChange();
    int calculateTextWidth();

    QLineControl* control;

    QFont font;
    QFont sourceFont;
    QColor  color;
    QColor  selectionColor;
    QColor  selectedTextColor;
    QDeclarativeText::TextStyle style;
    QColor  styleColor;
    QDeclarativeTextInput::HAlignment hAlign;
    QDeclarativeTextInput::SelectionMode mouseSelectionMode;
    QPointer<QDeclarativeComponent> cursorComponent;
    QPointer<QDeclarativeItem> cursorItem;
    QPointF pressPos;

    int lastSelectionStart;
    int lastSelectionEnd;
    int oldHeight;
    int oldWidth;
    int hscroll;
    int oldScroll;
    bool oldValidity:1;
    bool focused:1;
    bool focusOnPress:1;
    bool showInputPanelOnFocus:1;
    bool clickCausedFocus:1;
    bool cursorVisible:1;
    bool autoScroll:1;
    bool selectByMouse:1;
    bool canPaste:1;
    bool hAlignImplicit:1;
    bool selectPressed:1;

    static inline QDeclarativeTextInputPrivate *get(QDeclarativeTextInput *t) {
        return t->d_func();
    }
};

QT_END_NAMESPACE

#endif // QT_NO_LINEEDIT

#endif

