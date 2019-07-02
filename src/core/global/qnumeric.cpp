/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
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

#include "qnumeric.h"

QT_BEGIN_NAMESPACE

/*!
    Returns the bit pattern for an infinite number as a double.
*/
static const union { unsigned char c[8]; double d; } qt_be_inf_bytes = { { 0x7f, 0xf0, 0, 0, 0, 0, 0, 0 } };
static const union { unsigned char c[8]; double d; } qt_le_inf_bytes = { { 0, 0, 0, 0, 0, 0, 0xf0, 0x7f } };
#ifdef QT_ARMFPA
static const union { unsigned char c[8]; double d; } qt_armfpa_inf_bytes = { { 0, 0, 0xf0, 0x7f, 0, 0, 0, 0 } };
#endif
Q_CORE_EXPORT double qInf()
{
#ifdef QT_ARMFPA
    return qt_armfpa_inf_bytes.d;
#else
    return (QSysInfo::ByteOrder == QSysInfo::BigEndian
            ? qt_be_inf_bytes.d
            : qt_le_inf_bytes.d);
#endif
}

/*!
    Returns the bit pattern of a signalling NaN as a double.
*/
static const union { unsigned char c[8]; double d; } qt_be_snan_bytes = { { 0x7f, 0xf8, 0, 0, 0, 0, 0, 0 } };
static const union { unsigned char c[8]; double d; } qt_le_snan_bytes = { { 0, 0, 0, 0, 0, 0, 0xf8, 0x7f } };
#ifdef QT_ARMFPA
static const union { unsigned char c[8]; double d; } qt_armfpa_snan_bytes = { { 0, 0, 0xf8, 0x7f, 0, 0, 0, 0 } };
#endif
Q_CORE_EXPORT double qSNaN()
{
#ifdef QT_ARMFPA
    return qt_armfpa_snan_bytes.d;
#else
    return (QSysInfo::ByteOrder == QSysInfo::BigEndian
            ? qt_be_snan_bytes.d
            : qt_le_snan_bytes.d);
#endif
}

/*!
    Returns the bit pattern of a quiet NaN as a double.
*/
static const union { unsigned char c[8]; double d; } qt_be_qnan_bytes = { { 0xff, 0xf8, 0, 0, 0, 0, 0, 0 } };
static const union { unsigned char c[8]; double d; } qt_le_qnan_bytes = { { 0, 0, 0, 0, 0, 0, 0xf8, 0xff } };
#ifdef QT_ARMFPA
static const union { unsigned char c[8]; double d; } qt_armfpa_qnan_bytes = { { 0, 0, 0xf8, 0xff, 0, 0, 0, 0 } };
#endif
Q_CORE_EXPORT double qQNaN()
{
#ifdef QT_ARMFPA
    return qt_armfpa_qnan_bytes.d;
#else
    return (QSysInfo::ByteOrder == QSysInfo::BigEndian
            ? qt_be_qnan_bytes.d
            : qt_le_qnan_bytes.d);
#endif
}

QT_END_NAMESPACE
