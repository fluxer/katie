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

#ifndef QPRINTPREVIEWDIALOG_H
#define QPRINTPREVIEWDIALOG_H

#include <QtGui/qdialog.h>

#ifndef QT_NO_PRINTPREVIEWDIALOG


QT_BEGIN_NAMESPACE

class QPrinter;
class QGraphicsView;
class QPrintPreviewDialogPrivate;

class Q_GUI_EXPORT QPrintPreviewDialog : public QDialog
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QPrintPreviewDialog)

public:
    explicit QPrintPreviewDialog(QWidget *parent = nullptr, Qt::WindowFlags flags = 0);
    explicit QPrintPreviewDialog(QPrinter *printer, QWidget *parent = nullptr, Qt::WindowFlags flags = 0);
    ~QPrintPreviewDialog();

#ifdef Q_NO_USING_KEYWORD
    void open() { QDialog::open(); }
#else
    using QDialog::open;
#endif
    void open(QObject *receiver, const char *member);

    QPrinter *printer();

    void setVisible(bool visible);
    void done(int result);

Q_SIGNALS:
    void paintRequested(QPrinter *printer);

private:
    Q_PRIVATE_SLOT(d_func(), void _q_fit(QAction *action))
    Q_PRIVATE_SLOT(d_func(), void _q_zoomIn())
    Q_PRIVATE_SLOT(d_func(), void _q_zoomOut())
    Q_PRIVATE_SLOT(d_func(), void _q_navigate(QAction *action))
    Q_PRIVATE_SLOT(d_func(), void _q_setMode(QAction *action))
    Q_PRIVATE_SLOT(d_func(), void _q_pageNumEdited())
    Q_PRIVATE_SLOT(d_func(), void _q_print())
    Q_PRIVATE_SLOT(d_func(), void _q_pageSetup())
    Q_PRIVATE_SLOT(d_func(), void _q_previewChanged())
    Q_PRIVATE_SLOT(d_func(), void _q_zoomFactorChanged())
};


QT_END_NAMESPACE


#endif // QT_NO_PRINTPREVIEWDIALOG

#endif // QPRINTPREVIEWDIALOG_H
