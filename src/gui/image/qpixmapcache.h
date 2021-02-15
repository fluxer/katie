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

#ifndef QPIXMAPCACHE_H
#define QPIXMAPCACHE_H

#include <QtGui/qpixmap.h>


QT_BEGIN_NAMESPACE


class Q_GUI_EXPORT QPixmapCache
{
public:
    typedef QString Key;

    static int cacheLimit();
    static void setCacheLimit(int);
    static QPixmap *find(const QString &key);
    static bool find(const QString &key, QPixmap *pixmap);
    // ### get rid of this function
    static inline bool find(const QString &key, QPixmap &pixmap)
        { return find(key, &pixmap); };
    static bool insert(const QString &key, const QPixmap &pixmap);
    static Key insert(const QPixmap &pixmap);
    static bool replace(const Key &key, const QPixmap &pixmap);
    static void remove(const QString &key);
    static void clear();
};

QT_END_NAMESPACE


#endif // QPIXMAPCACHE_H
