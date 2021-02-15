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

#ifndef QDECLARATIVETEXTEDIT_P_H
#define QDECLARATIVETEXTEDIT_P_H

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

#include "qdeclarativeitem.h"
#include "qdeclarativeimplicitsizeitem_p_p.h"

#include "qdeclarative.h"

QT_BEGIN_NAMESPACE
class QTextLayout;
class QTextDocument;
class QTextControl;
class QDeclarativeTextEditPrivate : public QDeclarativeImplicitSizePaintedItemPrivate
{
    Q_DECLARE_PUBLIC(QDeclarativeTextEdit)

public:
    QDeclarativeTextEditPrivate()
      : color("black"), hAlign(QDeclarativeTextEdit::AlignLeft), vAlign(QDeclarativeTextEdit::AlignTop),
      dirty(false), richText(false), cursorVisible(false), focusOnPress(true),
      showInputPanelOnFocus(true), clickCausedFocus(false), persistentSelection(true), requireImplicitWidth(false),
      hAlignImplicit(true), rightToLeftText(false), textMargin(0.0), lastSelectionStart(0), lastSelectionEnd(0),
      cursorComponent(0), cursor(0), format(Qt::AutoText), document(0), wrapMode(QDeclarativeTextEdit::NoWrap),
      mouseSelectionMode(QDeclarativeTextEdit::SelectCharacters), lineCount(0), selectByMouse(false), canPaste(false),
      yoff(0)
    {
    }

    void init();

    void updateDefaultTextOption();
    void relayoutDocument();
    void updateSelection();
    bool determineHorizontalAlignment();
    bool setHAlign(QDeclarativeTextEdit::HAlignment, bool forceAlign = false);
    void mirrorChange();
    qreal implicitWidth() const;
    void focusChanged(bool);

    QString text;
    QFont font;
    QFont sourceFont;
    QColor  color;
    QColor  selectionColor;
    QColor  selectedTextColor;
    QDeclarativeTextEdit::HAlignment hAlign;
    QDeclarativeTextEdit::VAlignment vAlign;
    bool dirty;
    bool richText;
    bool cursorVisible;
    bool focusOnPress;
    bool showInputPanelOnFocus;
    bool clickCausedFocus;
    bool persistentSelection;
    bool requireImplicitWidth;
    bool hAlignImplicit;
    bool rightToLeftText;
    qreal textMargin;
    int lastSelectionStart;
    int lastSelectionEnd;
    QDeclarativeComponent* cursorComponent;
    QDeclarativeItem* cursor;
    Qt::TextFormat format;
    QTextDocument *document;
    QTextControl *control;
    QDeclarativeTextEdit::WrapMode wrapMode;
    QDeclarativeTextEdit::SelectionMode mouseSelectionMode;
    int lineCount;
    bool selectByMouse;
    bool canPaste;
    int yoff;
    QSize paintedSize;
};

QT_END_NAMESPACE
#endif
