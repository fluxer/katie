/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QPAGESETUPDIALOG_H
#define QPAGESETUPDIALOG_H

#include <QtGui/qabstractpagesetupdialog.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE


#ifndef QT_NO_PRINTDIALOG

class QPageSetupDialogPrivate;

class Q_GUI_EXPORT QPageSetupDialog : public QAbstractPageSetupDialog
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QPageSetupDialog)
    Q_ENUMS(PageSetupDialogOption)
    Q_PROPERTY(PageSetupDialogOptions options READ options WRITE setOptions)

public:
    enum PageSetupDialogOption {
        None                    = 0x00000000, // internal
        DontUseSheet            = 0x00000001,
        OwnsPrinter             = 0x80000000  // internal
    };

    Q_DECLARE_FLAGS(PageSetupDialogOptions, PageSetupDialogOption)

    explicit QPageSetupDialog(QPrinter *printer, QWidget *parent = Q_NULLPTR);
    explicit QPageSetupDialog(QWidget *parent = Q_NULLPTR);

    // obsolete
    void addEnabledOption(PageSetupDialogOption option);
    void setEnabledOptions(PageSetupDialogOptions options);
    PageSetupDialogOptions enabledOptions() const;
    bool isOptionEnabled(PageSetupDialogOption option) const;

    void setOption(PageSetupDialogOption option, bool on = true);
    bool testOption(PageSetupDialogOption option) const;
    void setOptions(PageSetupDialogOptions options);
    PageSetupDialogOptions options() const;

    virtual int exec();

#ifdef Q_NO_USING_KEYWORD
    void open() { QDialog::open(); }
#else
    using QDialog::open;
#endif
    void open(QObject *receiver, const char *member);

};

#endif // QT_NO_PRINTDIALOG

QT_END_NAMESPACE

QT_END_HEADER

#endif // QPAGESETUPDIALOG_H
