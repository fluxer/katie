/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the plugins of the Katie Toolkit.
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

#ifndef SIMPLEWIDGETS_H
#define SIMPLEWIDGETS_H

#include <QtCore/qcoreapplication.h>
#include <QtGui/qaccessible2.h>
#include <QtGui/qaccessiblewidget.h>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_ACCESSIBILITY

class QAbstractButton;
class QLineEdit;
class QToolButton;
class QGroupBox;
class QProgressBar;

class QAccessibleButton : public QAccessibleWidget, public QAccessibleActionInterface
{
    Q_ACCESSIBLE_OBJECT
    Q_DECLARE_TR_FUNCTIONS(QAccessibleButton)
public:
    QAccessibleButton(QWidget *w, Role r);

    QString text(Text t, int child) const;
    State state(int child) const;

    QString actionText(int action, Text text, int child) const;
    bool doAction(int action, int child, const QVariantList &params);

    // QAccessibleActionInterface
    int actionCount();
    void doAction(int actionIndex);
    QString description(int actionIndex);
    QString name(int actionIndex);
    QString localizedName(int actionIndex);
    QStringList keyBindings(int actionIndex);

protected:
    QAbstractButton *button() const;
};

#ifndef QT_NO_TOOLBUTTON
class QAccessibleToolButton : public QAccessibleButton
{
public:
    QAccessibleToolButton(QWidget *w, Role role);

    enum ToolButtonElements {
        ToolButtonSelf        = 0,
        ButtonExecute,
        ButtonDropMenu
    };

    Role role(int child) const;
    State state(int child) const;

    int childCount() const;
    QRect rect(int child) const;

    QString text(Text t, int child) const;

    QString actionText(int action, Text text, int child) const;
    bool doAction(int action, int child, const QVariantList &params);

protected:
    QToolButton *toolButton() const;

    bool isSplitButton() const;
};
#endif // QT_NO_TOOLBUTTON

class QAccessibleDisplay : public QAccessibleWidget, public QAccessibleImageInterface
{
    Q_ACCESSIBLE_OBJECT
public:
    explicit QAccessibleDisplay(QWidget *w, Role role = StaticText);

    QString text(Text t, int child) const;
    Role role(int child) const;

    Relation relationTo(int child, const QAccessibleInterface *other, int otherChild) const;
    int navigate(RelationFlag, int entry, QAccessibleInterface **target) const;

    // QAccessibleImageInterface
    QString imageDescription();
    QSize imageSize();
    QRect imagePosition(QAccessible2::CoordinateType coordType);
};

#ifndef QT_NO_GROUPBOX
class QAccessibleGroupBox : public QAccessibleWidget,
                            public QAccessibleActionInterface
{
    Q_ACCESSIBLE_OBJECT
public:
    explicit QAccessibleGroupBox(QWidget *w);

    State state(int child) const;
    Role role(int child) const;
    QString text(Text t, int child) const;

    Relation relationTo(int child, const QAccessibleInterface *other, int otherChild) const;
    int navigate(RelationFlag rel, int entry, QAccessibleInterface** target) const;

    //QAccessibleActionInterface
    int actionCount();
    QString description(int actionIndex);
    void doAction(int actionIndex);
    QString name(int actionIndex);
    QString localizedName(int actionIndex);
    QStringList keyBindings(int actionIndex);

private:
    QGroupBox *groupBox() const;
};
#endif

#ifndef QT_NO_LINEEDIT
class QAccessibleLineEdit : public QAccessibleWidget, public QAccessibleTextInterface,
                            public QAccessibleSimpleEditableTextInterface
{
    Q_ACCESSIBLE_OBJECT
public:
    explicit QAccessibleLineEdit(QWidget *o, const QString &name = QString());

    QString text(Text t, int child) const;
    void setText(Text t, int control, const QString &text);
    State state(int child) const;
    QVariant invokeMethod(QAccessible::Method method, int child, const QVariantList &params);

    // QAccessibleTextInterface
    void addSelection(int startOffset, int endOffset);
    QString attributes(int offset, int *startOffset, int *endOffset);
    int cursorPosition();
    QRect characterRect(int offset, QAccessible2::CoordinateType coordType);
    int selectionCount();
    int offsetAtPoint(const QPoint &point, QAccessible2::CoordinateType coordType);
    void selection(int selectionIndex, int *startOffset, int *endOffset);
    QString text(int startOffset, int endOffset);
    QString textBeforeOffset (int offset, QAccessible2::BoundaryType boundaryType,
            int *startOffset, int *endOffset);
    QString textAfterOffset(int offset, QAccessible2::BoundaryType boundaryType,
            int *startOffset, int *endOffset);
    QString textAtOffset(int offset, QAccessible2::BoundaryType boundaryType,
            int *startOffset, int *endOffset);
    void removeSelection(int selectionIndex);
    void setCursorPosition(int position);
    void setSelection(int selectionIndex, int startOffset, int endOffset);
    int characterCount();
    void scrollToSubstring(int startIndex, int endIndex);

protected:
    QLineEdit *lineEdit() const;
};
#endif // QT_NO_LINEEDIT

#ifndef QT_NO_PROGRESSBAR
class QAccessibleProgressBar : public QAccessibleDisplay, public QAccessibleValueInterface
{
    Q_ACCESSIBLE_OBJECT
public:
    explicit QAccessibleProgressBar(QWidget *o);

    // QAccessibleValueInterface
    QVariant currentValue();
    QVariant maximumValue();
    QVariant minimumValue();
    inline void setCurrentValue(const QVariant &) {}

protected:
    QProgressBar *progressBar() const;
};
#endif

#endif // QT_NO_ACCESSIBILITY

QT_END_NAMESPACE

#endif // SIMPLEWIDGETS_H
