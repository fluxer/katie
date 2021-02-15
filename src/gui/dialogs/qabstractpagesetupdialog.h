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

#ifndef QABSTRACTPAGESETUPDIALOG_H
#define QABSTRACTPAGESETUPDIALOG_H

#include <QtGui/qdialog.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_PRINTDIALOG

class QAbstractPageSetupDialogPrivate;
class QPrinter;

// ### Qt 5: Remove this class
class Q_GUI_EXPORT QAbstractPageSetupDialog : public QDialog
{
    Q_DECLARE_PRIVATE(QAbstractPageSetupDialog)
    Q_OBJECT

public:
    explicit QAbstractPageSetupDialog(QPrinter *printer, QWidget *parent = Q_NULLPTR);
    QAbstractPageSetupDialog(QAbstractPageSetupDialogPrivate &ptr,
                             QPrinter *printer, QWidget *parent = Q_NULLPTR);
    ~QAbstractPageSetupDialog();

    virtual int exec() = 0;
    void done(int result);

    QPrinter *printer();
};

#endif // QT_NO_PRINTDIALOG

QT_END_NAMESPACE


#endif // QABSTRACTPAGESETUPDIALOG_H
