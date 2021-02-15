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

#include "qlineedit.h"
#include "qlineedit_p.h"

#ifndef QT_NO_LINEEDIT

#include "qabstractitemview.h"
#include "qclipboard.h"
#ifndef QT_NO_ACCESSIBILITY
#include "qaccessible.h"
#endif

QT_BEGIN_NAMESPACE

const int QLineEditPrivate::verticalMargin(1);
const int QLineEditPrivate::horizontalMargin(2);

QRect QLineEditPrivate::adjustedControlRect(const QRect &rect) const
{
    QRect widgetRect = !rect.isEmpty() ? rect : q_func()->rect();
    QRect cr = adjustedContentsRect();
    int cix = cr.x() - hscroll + horizontalMargin;
    return widgetRect.translated(QPoint(cix, vscroll));
}

int QLineEditPrivate::xToPos(int x, QTextLine::CursorPosition betweenOrOn) const
{
    QRect cr = adjustedContentsRect();
    x-= cr.x() - hscroll + horizontalMargin;
    return control->xToPos(x, betweenOrOn);
}

QRect QLineEditPrivate::cursorRect() const
{
    return adjustedControlRect(control->cursorRect());
}

#ifndef QT_NO_COMPLETER
void QLineEditPrivate::_q_completionHighlighted(const QString &newText)
{
    Q_Q(QLineEdit);
    if (control->completer()->completionMode() != QCompleter::InlineCompletion) {
        q->setText(newText);
    } else {
        int c = control->cursor();
        QString text = control->text();
        q->setText(text.left(c) + newText.mid(c));
        control->moveCursor(control->end(), false);
        control->moveCursor(c, true);
    }
}
#endif // QT_NO_COMPLETER

void QLineEditPrivate::_q_handleWindowActivate()
{
    Q_Q(QLineEdit);
    if (!q->hasFocus() && control->hasSelectedText())
        control->deselect();
}

void QLineEditPrivate::_q_textEdited(const QString &text)
{
    Q_Q(QLineEdit);
    emit q->textEdited(text);
#ifndef QT_NO_COMPLETER
    if (control->completer()
        && control->completer()->completionMode() != QCompleter::InlineCompletion)
        control->complete(-1); // update the popup on cut/paste/del
#endif
}

void QLineEditPrivate::_q_cursorPositionChanged(int from, int to)
{
    Q_Q(QLineEdit);
    q->update();
    emit q->cursorPositionChanged(from, to);
}


void QLineEditPrivate::_q_selectionChanged()
{
    Q_Q(QLineEdit);
    if (control->preeditAreaText().isEmpty()) {
        QStyleOptionFrameV2 opt;
        q->initStyleOption(&opt);
        bool showCursor = control->hasSelectedText() ?
                          q->style()->styleHint(QStyle::SH_BlinkCursorWhenTextSelected, &opt, q):
                          q->hasFocus();
        setCursorVisible(showCursor);
    }

    emit q->selectionChanged();
#ifndef QT_NO_ACCESSIBILITY
    QAccessible::updateAccessibility(q, 0, QAccessible::TextSelectionChanged);
#endif
}

void QLineEditPrivate::_q_updateNeeded(const QRect &rect)
{
    q_func()->update(adjustedControlRect(rect));
}

void QLineEditPrivate::init(const QString& txt)
{
    Q_Q(QLineEdit);
    control = new QLineControl(txt);
    control->setParent(q);
    control->setFont(q->font());
    QObject::connect(control, SIGNAL(textChanged(QString)),
            q, SIGNAL(textChanged(QString)));
    QObject::connect(control, SIGNAL(textEdited(QString)),
            q, SLOT(_q_textEdited(QString)));
    QObject::connect(control, SIGNAL(cursorPositionChanged(int,int)),
            q, SLOT(_q_cursorPositionChanged(int,int)));
    QObject::connect(control, SIGNAL(selectionChanged()),
            q, SLOT(_q_selectionChanged()));
    QObject::connect(control, SIGNAL(accepted()),
            q, SIGNAL(returnPressed()));
    QObject::connect(control, SIGNAL(editingFinished()),
            q, SIGNAL(editingFinished()));
    // for now, going completely overboard with updates.
    QObject::connect(control, SIGNAL(selectionChanged()),
            q, SLOT(update()));

    QObject::connect(control, SIGNAL(displayTextChanged(QString)),
            q, SLOT(update()));

    QObject::connect(control, SIGNAL(updateNeeded(QRect)),
            q, SLOT(_q_updateNeeded(QRect)));

    QStyleOptionFrameV2 opt;
    q->initStyleOption(&opt);
    control->setPasswordCharacter(q->style()->styleHint(QStyle::SH_LineEdit_PasswordCharacter, &opt, q));
#ifndef QT_NO_CURSOR
    q->setCursor(Qt::IBeamCursor);
#endif
    q->setFocusPolicy(Qt::StrongFocus);
    //   Specifies that this widget can use more, but is able to survive on
    //   less, horizontal space; and is fixed vertically.
    q->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed, QSizePolicy::LineEdit));
    q->setBackgroundRole(QPalette::Base);
    q->setMouseTracking(true);
    q->setAcceptDrops(true);
}

QRect QLineEditPrivate::adjustedContentsRect() const
{
    Q_Q(const QLineEdit);
    QStyleOptionFrameV2 opt;
    q->initStyleOption(&opt);
    QRect r = q->style()->subElementRect(QStyle::SE_LineEditContents, &opt, q);
    r.setX(r.x() + leftTextMargin);
    r.setY(r.y() + topTextMargin);
    r.setRight(r.right() - rightTextMargin);
    r.setBottom(r.bottom() - bottomTextMargin);
    return r;
}

void QLineEditPrivate::setCursorVisible(bool visible)
{
    Q_Q(QLineEdit);
    if (cursorVisible == visible)
        return;
    cursorVisible = visible;
    if (control->inputMask().isEmpty())
        q->update(cursorRect());
    else
        q->update();
}

void QLineEditPrivate::updatePasswordEchoEditing(bool editing)
{
    control->updatePasswordEchoEditing(editing);
}

#ifndef QT_NO_DRAGANDDROP
void QLineEditPrivate::drag()
{
    Q_Q(QLineEdit);
    dndTimer.stop();
    QMimeData *data = new QMimeData;
    data->setText(control->selectedText());
    QDrag *drag = new QDrag(q);
    drag->setMimeData(data);
    Qt::DropAction action = drag->start();
    if (action == Qt::MoveAction && !control->isReadOnly() && drag->target() != q)
        control->removeSelection();
}

#endif // QT_NO_DRAGANDDROP

QT_END_NAMESPACE

#endif


