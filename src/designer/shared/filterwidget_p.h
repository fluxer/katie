/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the Katie Designer of the Katie Toolkit.
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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of Katie Designer.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QColor>
#include <QtGui/QToolButton>

QT_BEGIN_NAMESPACE

class QToolButton;

namespace qdesigner_internal {

/* This widget should never have initial focus
 * (ie, be the first widget of a dialog, else, the hint cannot be displayed.
 * For situations, where it is the only focusable control (widget box),
 * there is a special "refuseFocus()" mode, in which it clears the focus
 * policy and focusses explicitly on click (note that setting Qt::ClickFocus
 * is not sufficient for that as an ActivationFocus will occur). */

#define ICONBUTTON_SIZE 16

class Q_DESIGNER_EXPORT HintLineEdit : public QLineEdit {
    Q_OBJECT
public:
    explicit HintLineEdit(QWidget *parent = nullptr);

    bool refuseFocus() const;
    void setRefuseFocus(bool v);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void focusInEvent(QFocusEvent *e);

private:
    const Qt::FocusPolicy m_defaultFocusPolicy;
    bool m_refuseFocus;
};

// IconButton: This is a simple helper class that represents clickable icons

class IconButton: public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(float fader READ fader WRITE setFader)
public:
    IconButton(QWidget *parent);
    void paintEvent(QPaintEvent *event);
    float fader() { return m_fader; }
    void setFader(float value) { m_fader = value; update(); }
    void animateShow(bool visible);

private:
    float m_fader;
};

// FilterWidget: For filtering item views, with reset button Uses HintLineEdit.

class  Q_DESIGNER_EXPORT FilterWidget : public QWidget
{
    Q_OBJECT
public:
    enum LayoutMode {
        // For use in toolbars: Expand to the right
        LayoutAlignRight,
        // No special alignment
        LayoutAlignNone
    };

    explicit FilterWidget(QWidget *parent = nullptr, LayoutMode lm = LayoutAlignRight);

    QString text() const;
    void resizeEvent(QResizeEvent *);
    bool refuseFocus() const; // see HintLineEdit
    void setRefuseFocus(bool v);

signals:
    void filterChanged(const QString &);

public slots:
    void reset();

private slots:
    void checkButton(const QString &text);

private:
    HintLineEdit *m_editor;
    IconButton *m_button;
    QString m_oldText;
};
} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif
