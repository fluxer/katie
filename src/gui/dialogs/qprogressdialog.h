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

#ifndef QPROGRESSDIALOG_H
#define QPROGRESSDIALOG_H

#include <QtGui/qdialog.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_PROGRESSDIALOG

class QPushButton;
class QLabel;
class QProgressBar;
class QTimer;
class QProgressDialogPrivate;

class Q_GUI_EXPORT QProgressDialog : public QDialog
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QProgressDialog)
    Q_PROPERTY(bool wasCanceled READ wasCanceled)
    Q_PROPERTY(int minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(int maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(int value READ value WRITE setValue)
    Q_PROPERTY(bool autoReset READ autoReset WRITE setAutoReset)
    Q_PROPERTY(bool autoClose READ autoClose WRITE setAutoClose)
    Q_PROPERTY(int minimumDuration READ minimumDuration WRITE setMinimumDuration)
    Q_PROPERTY(QString labelText READ labelText WRITE setLabelText)

public:
    explicit QProgressDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = 0);
    QProgressDialog(const QString &labelText, const QString &cancelButtonText,
                    int minimum, int maximum, QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = 0);
    ~QProgressDialog();

    void setLabel(QLabel *label);
    void setCancelButton(QPushButton *button);
    void setBar(QProgressBar *bar);

    bool wasCanceled() const;

    int minimum() const;
    int maximum() const;

    int value() const;

    QSize sizeHint() const;

    QString labelText() const;
    int minimumDuration() const;

    void setAutoReset(bool reset);
    bool autoReset() const;
    void setAutoClose(bool close);
    bool autoClose() const;

#ifdef Q_NO_USING_KEYWORD
    void open() { QDialog::open(); }
#else
    using QDialog::open;
#endif
    void open(QObject *receiver, const char *member);

public Q_SLOTS:
    void cancel();
    void reset();
    void setMaximum(int maximum);
    void setMinimum(int minimum);
    void setRange(int minimum, int maximum);
    void setValue(int progress);
    void setLabelText(const QString &text);
    void setCancelButtonText(const QString &text);
    void setMinimumDuration(int ms);

Q_SIGNALS:
    void canceled();

protected:
    void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *event);
    void showEvent(QShowEvent *event);

protected Q_SLOTS:
    void forceShow();

private:
    Q_DISABLE_COPY(QProgressDialog)

    Q_PRIVATE_SLOT(d_func(), void _q_disconnectOnClose())
};

#endif // QT_NO_PROGRESSDIALOG

QT_END_NAMESPACE


#endif // QPROGRESSDIALOG_H
