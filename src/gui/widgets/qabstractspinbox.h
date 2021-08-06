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

#ifndef QABSTRACTSPINBOX_H
#define QABSTRACTSPINBOX_H

#include <QtGui/qwidget.h>
#include <QtGui/qvalidator.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_SPINBOX

class QLineEdit;
class QAbstractSpinBoxPrivate;
class QStyleOptionSpinBox;

class Q_GUI_EXPORT QAbstractSpinBox : public QWidget
{
    Q_OBJECT

    Q_ENUMS(ButtonSymbols)
    Q_ENUMS(CorrectionMode)
    Q_PROPERTY(bool wrapping READ wrapping WRITE setWrapping)
    Q_PROPERTY(bool frame READ hasFrame WRITE setFrame)
    Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment)
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
    Q_PROPERTY(ButtonSymbols buttonSymbols READ buttonSymbols WRITE setButtonSymbols)
    Q_PROPERTY(QString specialValueText READ specialValueText WRITE setSpecialValueText)
    Q_PROPERTY(QString text READ text)
    Q_PROPERTY(bool accelerated READ isAccelerated WRITE setAccelerated)
    Q_PROPERTY(CorrectionMode correctionMode READ correctionMode WRITE setCorrectionMode)
    Q_PROPERTY(bool acceptableInput READ hasAcceptableInput)
    Q_PROPERTY(bool keyboardTracking READ keyboardTracking WRITE setKeyboardTracking)
public:
    explicit QAbstractSpinBox(QWidget *parent = nullptr);
    ~QAbstractSpinBox();

    enum StepEnabledFlag { StepNone = 0x00, StepUpEnabled = 0x01,
                           StepDownEnabled = 0x02 };
    Q_DECLARE_FLAGS(StepEnabled, StepEnabledFlag)

    enum ButtonSymbols { UpDownArrows, PlusMinus, NoButtons };

    ButtonSymbols buttonSymbols() const;
    void setButtonSymbols(ButtonSymbols bs);

    enum CorrectionMode  { CorrectToPreviousValue, CorrectToNearestValue };

    void setCorrectionMode(CorrectionMode cm);
    CorrectionMode correctionMode() const;

    bool hasAcceptableInput() const;
    QString text() const;

    QString specialValueText() const;
    void setSpecialValueText(const QString &txt);

    bool wrapping() const;
    void setWrapping(bool w);

    void setReadOnly(bool r);
    bool isReadOnly() const;

    void setKeyboardTracking(bool kt);
    bool keyboardTracking() const;

    void setAlignment(Qt::Alignment flag);
    Qt::Alignment alignment() const;

    void setFrame(bool);
    bool hasFrame() const;

    void setAccelerated(bool on);
    bool isAccelerated() const;

    QSize sizeHint() const;
    QSize minimumSizeHint() const;
    void interpretText();
    bool event(QEvent *event);

    virtual QValidator::State validate(QString &input, int &pos) const;
    virtual void fixup(QString &input) const;

    virtual void stepBy(int steps);
public Q_SLOTS:
    void stepUp();
    void stepDown();
    void selectAll();
    virtual void clear();
protected:
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event);
#endif
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void changeEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);
    void hideEvent(QHideEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void timerEvent(QTimerEvent *event);
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);
    void initStyleOption(QStyleOptionSpinBox *option) const;

    QLineEdit *lineEdit() const;
    void setLineEdit(QLineEdit *edit);

    virtual StepEnabled stepEnabled() const;
Q_SIGNALS:
    void editingFinished();
protected:
    QAbstractSpinBox(QAbstractSpinBoxPrivate &dd, QWidget *parent = nullptr);

private:
    Q_PRIVATE_SLOT(d_func(), void _q_editorTextChanged(const QString &))
    Q_PRIVATE_SLOT(d_func(), void _q_editorCursorPositionChanged(int, int))

    Q_DECLARE_PRIVATE(QAbstractSpinBox)
    Q_DISABLE_COPY(QAbstractSpinBox)
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QAbstractSpinBox::StepEnabled)

#endif // QT_NO_SPINBOX

QT_END_NAMESPACE


#endif // QABSTRACTSPINBOX_H
