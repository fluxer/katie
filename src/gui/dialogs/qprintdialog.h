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

#ifndef QPRINTDIALOG_H
#define QPRINTDIALOG_H

#include <QtGui/qabstractprintdialog.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_PRINTDIALOG

class QPrintDialogPrivate;
class QPrinter;

class Q_GUI_EXPORT QPrintDialog : public QAbstractPrintDialog
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QPrintDialog)
    Q_ENUMS(PrintDialogOption)
    Q_PROPERTY(PrintDialogOptions options READ options WRITE setOptions)

public:
    explicit QPrintDialog(QPrinter *printer, QWidget *parent = nullptr);
    explicit QPrintDialog(QWidget *parent = nullptr);
    ~QPrintDialog();

    int exec();
    virtual void accept();
    void done(int result);


    void setOption(PrintDialogOption option, bool on = true);
    bool testOption(PrintDialogOption option) const;
    void setOptions(PrintDialogOptions options);
    PrintDialogOptions options() const;

    void setVisible(bool visible);

#ifdef Q_NO_USING_KEYWORD
    void open() { QDialog::open(); }
#else
    using QDialog::open;
#endif
    void open(QObject *receiver, const char *member);

#ifdef Q_NO_USING_KEYWORD
    void accepted() { QDialog::accepted(); }
#else
    using QDialog::accepted;
#endif

Q_SIGNALS:
    void accepted(QPrinter *printer);

private:
    Q_PRIVATE_SLOT(d_func(), void _q_chbPrintLastFirstToggled(bool))
    Q_PRIVATE_SLOT(d_func(), void _q_collapseOrExpandDialog())
#if !defined(QT_NO_MESSAGEBOX)
    Q_PRIVATE_SLOT(d_func(), void _q_checkFields())
#endif
    friend class QUnixPrintWidget;
};

#endif // QT_NO_PRINTDIALOG

QT_END_NAMESPACE


#endif // QPRINTDIALOG_H
