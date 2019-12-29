/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
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

#ifndef QFONTENGINEGLYPHCACHE_P_H
#define QFONTENGINEGLYPHCACHE_P_H

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


#include "QtCore/qglobal.h"
#include "QtCore/qatomic.h"
#include "qfont_p.h"

QT_BEGIN_NAMESPACE

class QFontEngineGlyphCache: public QSharedData
{
public:
    enum Type {
        Raster_RGBMask,
        Raster_A8,
        Raster_Mono
    };

    QFontEngineGlyphCache(const QTransform &matrix, Type type) : m_transform(matrix), m_type(type) { }

    virtual ~QFontEngineGlyphCache() { }

    Type cacheType() const { return m_type; }

    QTransform m_transform;
    QFontEngineGlyphCache::Type m_type;
};
typedef QHash<void *, QList<QFontEngineGlyphCache *> > GlyphPointerHash;
typedef QHash<int, QList<QFontEngineGlyphCache *> > GlyphIntHash;

QT_END_NAMESPACE

#endif
