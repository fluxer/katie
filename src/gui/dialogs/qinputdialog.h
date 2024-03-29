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

#ifndef QINPUTDIALOG_H
#define QINPUTDIALOG_H

#include <QtGui/qdialog.h>
#include <QtCore/qstring.h>
#include <QtGui/qlineedit.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_INPUTDIALOG

class QInputDialogPrivate;

class Q_GUI_EXPORT QInputDialog : public QDialog
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QInputDialog)
//  Q_ENUMS(InputMode InputDialogOption)
    Q_PROPERTY(InputMode inputMode READ inputMode WRITE setInputMode)
    Q_PROPERTY(QString labelText READ labelText WRITE setLabelText)
    Q_PROPERTY(InputDialogOptions options READ options WRITE setOptions)
    Q_PROPERTY(QString textValue READ textValue WRITE setTextValue NOTIFY textValueChanged)
    Q_PROPERTY(int intValue READ intValue WRITE setIntValue NOTIFY intValueChanged)
    Q_PROPERTY(int doubleValue READ doubleValue WRITE setDoubleValue NOTIFY doubleValueChanged)
    Q_PROPERTY(QLineEdit::EchoMode textEchoMode READ textEchoMode WRITE setTextEchoMode)
    Q_PROPERTY(bool comboBoxEditable READ isComboBoxEditable WRITE setComboBoxEditable)
    Q_PROPERTY(QStringList comboBoxItems READ comboBoxItems WRITE setComboBoxItems)
    Q_PROPERTY(int intMinimum READ intMinimum WRITE setIntMinimum)
    Q_PROPERTY(int intMaximum READ intMaximum WRITE setIntMaximum)
    Q_PROPERTY(int intStep READ intStep WRITE setIntStep)
    Q_PROPERTY(double doubleMinimum READ doubleMinimum WRITE setDoubleMinimum)
    Q_PROPERTY(double doubleMaximum READ doubleMaximum WRITE setDoubleMaximum)
    Q_PROPERTY(int doubleDecimals READ doubleDecimals WRITE setDoubleDecimals)
    Q_PROPERTY(QString okButtonText READ okButtonText WRITE setOkButtonText)
    Q_PROPERTY(QString cancelButtonText READ cancelButtonText WRITE setCancelButtonText)

public:
    enum InputDialogOption {
        NoButtons                   = 0x00000001,
        UseListViewForComboBoxItems = 0x00000002
    };

    Q_DECLARE_FLAGS(InputDialogOptions, InputDialogOption)

    enum InputMode {
        TextInput,
        IntInput,
        DoubleInput
    };

    QInputDialog(QWidget *parent = nullptr, Qt::WindowFlags flags = 0);
    ~QInputDialog();

    void setInputMode(InputMode mode);
    InputMode inputMode() const;

    void setLabelText(const QString &text);
    QString labelText() const;

    void setOption(InputDialogOption option, bool on = true);
    bool testOption(InputDialogOption option) const;
    void setOptions(InputDialogOptions options);
    InputDialogOptions options() const;

    void setTextValue(const QString &text);
    QString textValue() const;

    void setTextEchoMode(QLineEdit::EchoMode mode);
    QLineEdit::EchoMode textEchoMode() const;

    void setComboBoxEditable(bool editable);
    bool isComboBoxEditable() const;

    void setComboBoxItems(const QStringList &items);
    QStringList comboBoxItems() const;

    void setIntValue(int value);
    int intValue() const;

    void setIntMinimum(int min);
    int intMinimum() const;

    void setIntMaximum(int max);
    int intMaximum() const;

    void setIntRange(int min, int max);

    void setIntStep(int step);
    int intStep() const;

    void setDoubleValue(double value);
    double doubleValue() const;

    void setDoubleMinimum(double min);
    double doubleMinimum() const;

    void setDoubleMaximum(double max);
    double doubleMaximum() const;

    void setDoubleRange(double min, double max);

    void setDoubleDecimals(int decimals);
    int doubleDecimals() const;

    void setOkButtonText(const QString &text);
    QString okButtonText() const;

    void setCancelButtonText(const QString &text);
    QString cancelButtonText() const;

#ifdef Q_NO_USING_KEYWORD
    void open() { QDialog::open(); }
#else
    using QDialog::open;
#endif
    void open(QObject *receiver, const char *member);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void setVisible(bool visible);

    static QString getText(QWidget *parent, const QString &title, const QString &label,
                           QLineEdit::EchoMode echo = QLineEdit::Normal,
                           const QString &text = QString(), bool *ok = 0,
                           Qt::WindowFlags flags = 0);
    static QString getItem(QWidget *parent, const QString &title, const QString &label,
                           const QStringList &items, int current = 0, bool editable = true,
                           bool *ok = 0, Qt::WindowFlags flags = 0);
    static int getInt(QWidget *parent, const QString &title, const QString &label, int value = 0,
                      int minValue = -2147483647, int maxValue = 2147483647,
                      int step = 1, bool *ok = 0, Qt::WindowFlags flags = 0);
    static double getDouble(QWidget *parent, const QString &title, const QString &label, double value = 0,
                            double minValue = -2147483647, double maxValue = 2147483647,
                            int decimals = 1, bool *ok = 0, Qt::WindowFlags flags = 0);

    // obsolete
    static inline int getInteger(QWidget *parent, const QString &title, const QString &label, int value = 0,
                          int minValue = -2147483647, int maxValue = 2147483647,
                          int step = 1, bool *ok = 0, Qt::WindowFlags flags = 0)
        { return getInt(parent, title, label, value, minValue, maxValue, step, ok, flags); };


Q_SIGNALS:
    // ### emit signals!
    void textValueChanged(const QString &text);
    void textValueSelected(const QString &text);
    void intValueChanged(int value);
    void intValueSelected(int value);
    void doubleValueChanged(double value);
    void doubleValueSelected(double value);


public:
    void done(int result); // ### Qt 5: Make protected.

private:
    Q_DISABLE_COPY(QInputDialog)
    Q_PRIVATE_SLOT(d_func(), void _q_textChanged(const QString&))
    Q_PRIVATE_SLOT(d_func(), void _q_currentRowChanged(const QModelIndex&, const QModelIndex&))
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QInputDialog::InputDialogOptions)

#endif // QT_NO_INPUTDIALOG

QT_END_NAMESPACE


#endif // QINPUTDIALOG_H
