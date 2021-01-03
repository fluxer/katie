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

#include "qrgb.h"
#include "qstringlist.h"

QT_BEGIN_NAMESPACE

static inline int h2i(char hex)
{
    if (hex >= '0' && hex <= '9')
        return hex - '0';
    if (hex >= 'a' && hex <= 'f')
        return hex - 'a' + 10;
    if (hex >= 'A' && hex <= 'F')
        return hex - 'A' + 10;
    return -1;
}

static inline int hex2int(const char *s)
{
    return (h2i(s[0]) << 4) | h2i(s[1]);
}

static inline int hex2int(char s)
{
    int h = h2i(s);
    return (h << 4) | h;
}

bool qt_get_hex_rgb(const char *name, const int length, QRgb *rgb)
{
    Q_ASSERT(name[0] == '#');
    name++;
    const int len = (length - 1);
    int r, g, b;
    if (len == 12) {
        r = hex2int(name);
        g = hex2int(name + 4);
        b = hex2int(name + 8);
    } else if (len == 9) {
        r = hex2int(name);
        g = hex2int(name + 3);
        b = hex2int(name + 6);
    } else if (len == 6) {
        r = hex2int(name);
        g = hex2int(name + 2);
        b = hex2int(name + 4);
    } else if (len == 3) {
        r = hex2int(name[0]);
        g = hex2int(name[1]);
        b = hex2int(name[2]);
    } else {
        r = g = b = -1;
    }
    if ((uint)r > 255 || (uint)g > 255 || (uint)b > 255) {
        *rgb = 0;
        return false;
    }
    *rgb = qRgb(r, g ,b);
    return true;
}

QT_END_NAMESPACE


