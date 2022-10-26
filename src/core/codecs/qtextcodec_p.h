/****************************************************************************
**
** Copyright (C) 2019 Ivailo Monev
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

#include "QtCore/qtextcodec.h"

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

#include <unicode/ucnv.h>

QT_BEGIN_NAMESPACE

class QTextCodecPrivate
{
public:
    QTextCodecPrivate(const QByteArray &name);
    QTextCodecPrivate(const int mib);

    static QList<QByteArray> allCodecs();
    static QList<int> allMibs();

    static QString convertTo(const char *data, int len, const char* const codec);
    static QByteArray convertFrom(const QChar *unicode, int len, const char* const codec);

    QByteArray name;
private:
    Q_DISABLE_COPY(QTextCodecPrivate);
};

class QTextConverterPrivate
{
public:
    QTextConverterPrivate(const QByteArray &name);
    QTextConverterPrivate(const int mib);
    ~QTextConverterPrivate();

    UConverter* getConverter();
    void invalidChars(int length) const;

    QByteArray name;
    QTextConverter::ConversionFlags flags;
    UConverter* conv;
    mutable int invalidchars;
private:
    Q_DISABLE_COPY(QTextConverterPrivate);
};

QT_END_NAMESPACE

#endif // QTEXTCODEC_P_H
