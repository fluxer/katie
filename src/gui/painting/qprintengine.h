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

#ifndef QPRINTENGINE_H
#define QPRINTENGINE_H

#include <QtCore/qvariant.h>
#include <QtGui/qprinter.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_PRINTER

class Q_GUI_EXPORT QPrintEngine
{
public:
    virtual ~QPrintEngine() {}
    enum PrintEnginePropertyKey {
        PPK_CollateCopies,
        PPK_ColorMode,
        PPK_Creator,
        PPK_DocumentName,
        PPK_FullPage,
        PPK_NumberOfCopies,
        PPK_Orientation,
        PPK_OutputFileName,
        PPK_PageOrder,
        PPK_PageRect,
        PPK_PageSize,
        PPK_PaperRect,
        PPK_PaperSource,
        PPK_PrinterName,
        PPK_Resolution,
        PPK_SelectionOption,
        PPK_SupportedResolutions,

        PPK_FontEmbedding,
        PPK_SuppressSystemPrintStatus,

        PPK_Duplex,

        PPK_PaperSources,
        PPK_CustomPaperSize,
        PPK_PageMargins,
        PPK_CopyCount,
        PPK_SupportsMultipleCopies,
        PPK_PaperSize = PPK_PageSize,

        PPK_CustomBase = 0xff00
    };

    virtual void setProperty(PrintEnginePropertyKey key, const QVariant &value) = 0;
    virtual QVariant property(PrintEnginePropertyKey key) const = 0;

    virtual bool newPage() = 0;
    virtual bool abort() = 0;

    virtual int metric(QPaintDevice::PaintDeviceMetric) const = 0;

    virtual QPrinter::PrinterState printerState() const = 0;
};

#endif // QT_NO_PRINTER

QT_END_NAMESPACE


#endif // QPRINTENGINE_H
