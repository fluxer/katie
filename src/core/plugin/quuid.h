/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
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

#ifndef QUUID_H
#define QUUID_H

#include <QtCore/qstring.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE


struct Q_CORE_EXPORT QUuid
{
    enum Variant {
        VarUnknown        = -1,
        NCS                = 0, // 0 - -
        DCE                = 2, // 1 0 -
        Microsoft        = 6, // 1 1 0
        Reserved        = 7  // 1 1 1
    };

    enum Version {
        VerUnknown        = -1,
        Time                = 1, // 0 0 0 1
        EmbeddedPOSIX        = 2, // 0 0 1 0
        Name                = 3, // 0 0 1 1
        Random                = 4  // 0 1 0 0
    };

    QUuid()
    {
        data1 = 0;
        data2 = 0;
        data3 = 0;
        for(int i = 0; i < 8; i++)
            data4[i] = 0;
    }
    QUuid(uint l, ushort w1, ushort w2, uchar b1, uchar b2, uchar b3, uchar b4, uchar b5, uchar b6, uchar b7, uchar b8)
    {
        data1 = l;
        data2 = w1;
        data3 = w2;
        data4[0] = b1;
        data4[1] = b2;
        data4[2] = b3;
        data4[3] = b4;
        data4[4] = b5;
        data4[5] = b6;
        data4[6] = b7;
        data4[7] = b8;
    }
#ifndef QT_NO_QUUID_STRING
    QUuid(const QString &);
    QUuid(const char *);
    QString toString() const;
    QUuid(const QByteArray &);
    QByteArray toByteArray() const;
#endif
    QByteArray toRfc4122() const;
    static QUuid fromRfc4122(const QByteArray &);
    bool isNull() const;

    bool operator==(const QUuid &orig) const
    {
        if (data1 != orig.data1 || data2 != orig.data2 ||
             data3 != orig.data3)
            return false;

        for(int i = 0; i < 8; i++)
            if (data4[i] != orig.data4[i])
                return false;

        return true;
    }

    bool operator!=(const QUuid &orig) const
    {
        return !(*this == orig);
    }

    bool operator<(const QUuid &other) const;
    bool operator>(const QUuid &other) const;

    static QUuid createUuid();
    QUuid::Variant variant() const;
    QUuid::Version version() const;

    uint    data1;
    ushort  data2;
    ushort  data3;
    uchar   data4[8];
};

#ifndef QT_NO_DATASTREAM
Q_CORE_EXPORT QDataStream &operator<<(QDataStream &, const QUuid &);
Q_CORE_EXPORT QDataStream &operator>>(QDataStream &, QUuid &);
#endif

Q_CORE_EXPORT uint qHash(const QUuid &uuid);

QT_END_NAMESPACE

QT_END_HEADER

#endif // QUUID_H
