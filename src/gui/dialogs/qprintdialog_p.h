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

#ifndef QPRINTDIALOG_P_H
#define QPRINTDIALOG_P_H

#include "qdialogbuttonbox.h"
#include "qabstractpagesetupdialog_p.h"
#include "ui_qprintsettingsoutput.h"

QT_BEGIN_NAMESPACE

class QUnixPrintWidget;
class QCUPSSupport;

class QPrintDialogPrivate : public QAbstractPrintDialogPrivate
{
    Q_DECLARE_PUBLIC(QPrintDialog)
    Q_DECLARE_TR_FUNCTIONS(QPrintDialog)
public:
    QPrintDialogPrivate();
    ~QPrintDialogPrivate();

    void init();
    /// copy printer properties to the widget
    void applyPrinterProperties(QPrinter *p);

#if !defined(QT_NO_CUPS)
    void selectPrinter(QCUPSSupport *cups);
#endif

    void _q_chbPrintLastFirstToggled(bool);
#ifndef QT_NO_MESSAGEBOX
    void _q_checkFields();
#endif
    void _q_collapseOrExpandDialog();

    void setupPrinter();
    void updateWidgets();

    virtual void setTabs(const QList<QWidget*> &tabs);

    Ui::QPrintSettingsOutput options;
    QUnixPrintWidget *top;
    QWidget *bottom;
    QDialogButtonBox *buttons;
    QPushButton *collapseButton;
};

QT_END_NAMESPACE

#endif // QPRINTDIALOG_P_H
