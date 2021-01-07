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

#ifndef QPRINTERINFO_H
#define QPRINTERINFO_H

#include <QtCore/QList>

#include <QtGui/QPrinter>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_PRINTER
class QPrinterInfoPrivate;
class QPrinterInfoPrivateDeleter;
class Q_GUI_EXPORT QPrinterInfo
{
public:
    QPrinterInfo();
    QPrinterInfo(const QPrinterInfo &other);
    QPrinterInfo(const QPrinter &printer);
    ~QPrinterInfo();

    QPrinterInfo &operator=(const QPrinterInfo &other);

    QString printerName() const;
    bool isNull() const;
    bool isDefault() const;
    QList<QPrinter::PaperSize> supportedPaperSizes() const;

    static QList<QPrinterInfo> availablePrinters();
    static QPrinterInfo defaultPrinter();

private:
    QPrinterInfo(const QString &name);

private:
    Q_DECLARE_PRIVATE(QPrinterInfo)
    QScopedPointer<QPrinterInfoPrivate, QPrinterInfoPrivateDeleter> d_ptr;
};

#endif // QT_NO_PRINTER

QT_END_NAMESPACE


#endif // QPRINTERINFO_H
