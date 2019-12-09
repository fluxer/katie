/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
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
// This file is not part of the Qt API.  It exists purely as an
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
    explicit QIcuCodec(const char *name);
    explicit QIcuCodec(const int mib);
    ~QIcuCodec();

    QString convertToUnicode(const char *data, int len, ConverterState *state) const;
    QByteArray convertFromUnicode(const QChar *unicode, int len, ConverterState *state) const;

    QByteArray name() const;
    QList<QByteArray> aliases() const;
    int mibEnum() const;

#ifndef QT_NO_TEXTCODEC
    static QList<QByteArray> allCodecs();
    static QList<int> allMibs();
    static QTextCodec* codecForUtf(const QByteArray &text, QTextCodec *defaultCodec);
#endif

private:
    UConverter *getConverter(QTextCodec::ConverterState *state) const;

    const char *m_name;
};

QT_END_NAMESPACE

#endif
