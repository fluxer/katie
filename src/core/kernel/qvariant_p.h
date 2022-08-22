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

#ifndef QVARIANT_P_H
#define QVARIANT_P_H

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

#include <QtCore/qvariant.h>

QT_BEGIN_NAMESPACE

class QVariantPrivate
{
public:
    QVariantPrivate();
    QVariantPrivate(const int vtype, const void* copy);
    QVariantPrivate(const QVariantPrivate &other);
    ~QVariantPrivate();

    int type;
    bool is_null;
    void *ptr;
    QAtomicInt ref;
};

template <typename T>
inline const T *v_cast(const QVariantPrivate *d)
{
    Q_ASSERT(d->type != QVariant::Invalid);
    return static_cast<const T *>(static_cast<const void *>(d->ptr));
}

template <typename T>
inline T *v_cast(QVariantPrivate *d)
{
    Q_ASSERT(d->type != QVariant::Invalid);
    return static_cast<T *>(static_cast<void *>(d->ptr));
}

Q_CORE_EXPORT const QVariant::Handler *qcoreVariantHandler();

QT_END_NAMESPACE

#endif // QVARIANT_P_H
