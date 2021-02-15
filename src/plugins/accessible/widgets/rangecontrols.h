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

#ifndef RANGECONTROLS_H
#define RANGECONTROLS_H

#include <QtGui/qaccessiblewidget.h>
#include <QtGui/qaccessible2.h>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_ACCESSIBILITY

class QAbstractSpinBox;
class QAbstractSlider;
class QScrollBar;
class QSlider;
class QSpinBox;
class QDoubleSpinBox;
class QDial;

#ifndef QT_NO_SPINBOX
class QAccessibleAbstractSpinBox: public QAccessibleWidget, public QAccessibleValueInterface
{
    Q_ACCESSIBLE_OBJECT
public:
    explicit QAccessibleAbstractSpinBox(QWidget *w);

    enum SpinBoxElements {
        SpinBoxSelf        = 0,
        Editor,
        ValueUp,
        ValueDown
    };

    int childCount() const;
    QRect rect(int child) const;

    int navigate(RelationFlag rel, int entry, QAccessibleInterface **target) const;

    QString text(Text t, int child) const;
    Role role(int child) const;

    bool doAction(int action, int child, const QVariantList &params);

    QVariant invokeMethod(Method method, int child, const QVariantList &params);

    // QAccessibleValueInterface
    QVariant currentValue();
    void setCurrentValue(const QVariant &value);
    QVariant maximumValue();
    QVariant minimumValue();

protected:
    QAbstractSpinBox *abstractSpinBox() const;
};

class QAccessibleSpinBox : public QAccessibleAbstractSpinBox
{
public:
    explicit QAccessibleSpinBox(QWidget *w);

    State state(int child) const;

    bool doAction(int action, int child, const QVariantList &params);

protected:
    QSpinBox *spinBox() const;
};

class QAccessibleDoubleSpinBox : public QAccessibleWidget
{
public:
    explicit QAccessibleDoubleSpinBox(QWidget *widget);

    enum DoubleSpinBoxElements {
        SpinBoxSelf = 0,
        Editor,
        ValueUp,
        ValueDown
    };

    int childCount() const;
    QRect rect(int child) const;
    int navigate(RelationFlag rel, int entry, QAccessibleInterface **target) const;
    QVariant invokeMethod(QAccessible::Method method, int child, const QVariantList &params);
    QString text(Text t, int child) const;
    Role role(int child) const;
    State state(int child) const;

protected:
    QDoubleSpinBox *doubleSpinBox() const;
};
#endif // QT_NO_SPINBOX

class QAccessibleAbstractSlider: public QAccessibleWidget, public QAccessibleValueInterface
{
    Q_ACCESSIBLE_OBJECT
public:
    explicit QAccessibleAbstractSlider(QWidget *w, Role r = Slider);

    QVariant invokeMethod(Method method, int child, const QVariantList &params);

    // QAccessibleValueInterface
    QVariant currentValue();
    void setCurrentValue(const QVariant &value);
    QVariant maximumValue();
    QVariant minimumValue();

protected:
    QAbstractSlider *abstractSlider() const;
};

#ifndef QT_NO_SCROLLBAR
class QAccessibleScrollBar : public QAccessibleAbstractSlider
{
public:
    explicit QAccessibleScrollBar(QWidget *w);

    enum ScrollBarElements {
        ScrollBarSelf        = 0,
        LineUp,
        PageUp,
        Position,
        PageDown,
        LineDown
    };

    int childCount() const;

    QRect rect(int child) const;
    QString text(Text t, int child) const;
    Role role(int child) const;
    State state(int child) const;

protected:
    QScrollBar *scrollBar() const;
};
#endif // QT_NO_SCROLLBAR

#ifndef QT_NO_SLIDER
class QAccessibleSlider : public QAccessibleAbstractSlider
{
public:
    explicit QAccessibleSlider(QWidget *w);

    enum SliderElements {
        SliderSelf  = 0,
        PageLeft,
        Position,
        PageRight
    };

    int childCount() const;

    QRect rect(int child) const;
    QString text(Text t, int child) const;
    Role role(int child) const;
    State state(int child) const;

    int defaultAction(int child) const;
    QString actionText(int action, Text t, int child) const;

protected:
    QSlider *slider() const;
};
#endif // QT_NO_SLIDER

#ifndef QT_NO_DIAL
class QAccessibleDial : public QAccessibleWidget
{
public:
    explicit QAccessibleDial(QWidget *w);

    enum DialElements {
        Self  = 0,
        SpeedoMeter,
        SliderHandle
    };

    int childCount() const;
    QRect rect(int child) const;
    QString text(Text textType, int child) const;
    Role role(int child) const;
    State state(int child) const;
    QVariant invokeMethod(Method method, int child, const QVariantList &params);

protected:
    QDial *dial() const;
};
#endif // QT_NO_DIAL

#endif // QT_NO_ACCESSIBILITY

QT_END_NAMESPACE

#endif // RANGECONTROLS_H
