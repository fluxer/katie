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

#ifndef QICUCODEC_P_H
#define QICUCODEC_P_H

#include "QtCore/qtextcodec.h"

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qtextcodec_p.h"

#include <unicode/ucnv.h>

QT_BEGIN_NAMESPACE

class QIcuCodec : public QTextCodec
{
public:
    explicit QIcuCodec(const QByteArray &name);
    explicit QIcuCodec(const int mib);
    ~QIcuCodec();

    QString convertToUnicode(const char *data, int len, ConverterState *state) const;
    QByteArray convertFromUnicode(const QChar *unicode, int len, ConverterState *state) const;

#ifndef QT_NO_TEXTCODEC
    QByteArray name() const;
    QList<QByteArray> aliases() const;
    int mibEnum() const;

    static QList<QByteArray> allCodecs();
    static QList<int> allMibs();
    static QTextCodec* codecForUtf(const QByteArray &text, QTextCodec *defaultCodec);
    static QTextCodec* codecForData(const QByteArray &text, QTextCodec *defaultCodec);
#endif

    static QString convertTo(const char *data, int len, const char* const codec);
    static QByteArray convertFrom(const QChar *unicode, int len, const char* const codec);

    void invalidChars(int length) const;

private:
    UConverter *getConverter(QTextCodec::ConverterState *state) const;

    QByteArray m_name;
    mutable QTextCodec::ConverterState *m_callbackstate;
};

QT_END_NAMESPACE

#endif
