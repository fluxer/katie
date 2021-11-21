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

#include "qprinterinfo.h"
#include "qprinterinfo_p.h"

#include "qfile.h"
#include "qdir.h"
#include "qprintdialog.h"
#include "qtextstream.h"

#if !defined(QT_NO_CUPS)
#  include "qcups_p.h"
#  include <cups/cups.h>
#  include "qpdf_p.h"
#endif

#include "qprinterinfo_unix_p.h"

QT_BEGIN_NAMESPACE

#ifndef QT_NO_PRINTER

#if !defined(QT_NO_CUPS)
// preserver names in ascending order for the binary search
static const struct NamedPaperSizeData {
    const char *const name;
    const QPrinter::PaperSize size;
} NamedPaperSizeTbl[QPrinter::NPageSize] = {
    { "A0", QPrinter::A0 },
    { "A1", QPrinter::A1 },
    { "A2", QPrinter::A2 },
    { "A3", QPrinter::A3 },
    { "A4", QPrinter::A4 },
    { "A5", QPrinter::A5 },
    { "A6", QPrinter::A6 },
    { "A7", QPrinter::A7 },
    { "A8", QPrinter::A8 },
    { "A9", QPrinter::A9 },
    { "B0", QPrinter::B0 },
    { "B1", QPrinter::B1 },
    { "B10", QPrinter::B10 },
    { "B2", QPrinter::B2 },
    { "B4", QPrinter::B4 },
    { "B5", QPrinter::B5 },
    { "B6", QPrinter::B6 },
    { "B7", QPrinter::B7 },
    { "B8", QPrinter::B8 },
    { "B9", QPrinter::B9 },
    { "C5E", QPrinter::C5E },
    { "Comm10E", QPrinter::Comm10E },
    { "Custom", QPrinter::Custom },
    { "DLE", QPrinter::DLE },
    { "Executive", QPrinter::Executive },
    { "Folio", QPrinter::Folio },
    { "Ledger", QPrinter::Ledger },
    { "Legal", QPrinter::Legal },
    { "Letter", QPrinter::Letter },
    { "Tabloid", QPrinter::Tabloid }
};
static const qint16 NamedPaperSizeTblSize = sizeof(NamedPaperSizeTbl) / sizeof(NamedPaperSizeData);

static inline QPrinter::PaperSize string2PaperSize(const char *name)
{
    for (qint16 i = 0; i < NamedPaperSizeTblSize; i++) {
        if (qstrcmp(name, NamedPaperSizeTbl[i].name) == 0)
            return NamedPaperSizeTbl[i].size;
    }
    return QPrinter::Custom;
}
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

QList<QPrinterInfo> QPrinterInfo::availablePrinters()
{
    QList<QPrinterInfo> printers;

#if !defined(QT_NO_CUPS)
    if (QCUPSSupport::isAvailable()) {
        QCUPSSupport cups;
        int cupsPrinterCount = cups.availablePrintersCount();
        const cups_dest_t* cupsPrinters = cups.availablePrinters();
        for (int i = 0; i < cupsPrinterCount; ++i) {
            QString printerName(QString::fromLocal8Bit(cupsPrinters[i].name));
            if (cupsPrinters[i].instance)
                printerName += QLatin1Char('/') + QString::fromLocal8Bit(cupsPrinters[i].instance);

            QPrinterInfo printerInfo(printerName);
            if (cupsPrinters[i].is_default)
                printerInfo.d_ptr->isDefault = true;
            printerInfo.d_ptr->cupsPrinterIndex = i;
            printers.append(printerInfo);
        }
    }
#endif

    return printers;
}

QPrinterInfo QPrinterInfo::defaultPrinter()
{
    QList<QPrinterInfo> printers = availablePrinters();
    foreach (const QPrinterInfo &printerInfo, printers) {
        if (printerInfo.isDefault())
            return printerInfo;
    }

    return printers.value(0);
}

QList<QPrinter::PaperSize> QPrinterInfo::supportedPaperSizes() const
{
#if !defined(QT_NO_CUPS)
    Q_D(const QPrinterInfo);

    if (isNull())
        return d->paperSizes;

    if (!d->hasPaperSizes) {
        d->hasPaperSizes = true;

        if (QCUPSSupport::isAvailable()) {
            // Find paper sizes from CUPS.
            QCUPSSupport cups;
            cups.setCurrentPrinter(d->cupsPrinterIndex);
            const ppd_option_t* sizes = cups.pageSizes();
            if (sizes) {
                for (int j = 0; j < sizes->num_choices; ++j)
                    d->paperSizes.append(string2PaperSize(sizes->choices[j].choice));
            }
        }
    }

    return d->paperSizes;
#else
    return QList<QPrinter::PaperSize>();
#endif
}

#endif // QT_NO_PRINTER

QT_END_NAMESPACE




