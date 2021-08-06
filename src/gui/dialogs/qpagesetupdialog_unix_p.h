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
#ifndef QPAGESETUPWIDGET_H
#define QPAGESETUPWIDGET_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// to version without notice, or even be removed.
//
// We mean it.
//
//

#include "qglobal.h"

#ifndef QT_NO_PRINTDIALOG

#include <ui_qpagesetupwidget.h>

QT_BEGIN_NAMESPACE

class QPrinter;
class QPagePreview;
class QCUPSSupport;

class QPageSetupWidget : public QWidget {
    Q_OBJECT
public:
    QPageSetupWidget(QWidget *parent = nullptr);
    QPageSetupWidget(QPrinter *printer, QWidget *parent = nullptr);
    void setPrinter(QPrinter *printer);
    /// copy information from the widget and apply that to the printer.
    void setupPrinter() const;
    void selectPrinter(QCUPSSupport *m_cups);
    void selectPdfPsPrinter(const QPrinter *p);

private slots:
    void _q_pageOrientationChanged();
    void _q_paperSizeChanged();
    void unitChanged(int item);
    void setTopMargin(double newValue);
    void setBottomMargin(double newValue);
    void setLeftMargin(double newValue);
    void setRightMargin(double newValue);

private:
    Ui::QPageSetupWidget widget;
    QPagePreview *m_pagePreview;
    QPrinter *m_printer;
    qreal m_leftMargin;
    qreal m_topMargin;
    qreal m_rightMargin;
    qreal m_bottomMargin;
    QSizeF m_paperSize;
    qreal m_currentMultiplier;
    bool m_blockSignals;
    QCUPSSupport *m_cups;
};

QT_END_NAMESPACE

#endif // QT_NO_PRINTDIALOG
#endif
