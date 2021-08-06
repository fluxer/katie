/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#ifndef QTEXTCODEC_P_H
#define QTEXTCODEC_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of the QTextCodec class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include "qtextcodec.h"

QT_BEGIN_NAMESPACE

#ifdef QT_NO_TEXTCODEC

class QTextCodec
{
public:
    enum ConversionFlag {
        DefaultConversion,
        ConvertInvalidToNull = 0x80000000,
        IgnoreHeader = 0x1
    };
    Q_DECLARE_FLAGS(ConversionFlags, ConversionFlag)

    struct ConverterState {
        ConverterState(ConversionFlags f = DefaultConversion)
            : flags(f), invalidChars(0), d(nullptr) { }
        ~ConverterState() { }
        ConversionFlags flags;
        int invalidChars;
    private:
        void *d;

        friend class QIcuCodec;
        friend class QTextStreamPrivate;
        friend class QTextStream;

        ConverterState(const ConverterState &other);
        ConverterState& operator=(const ConverterState &other);
    };
};

#endif //QT_NO_TEXTCODEC

QT_END_NAMESPACE

#endif
