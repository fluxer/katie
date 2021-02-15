/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtDeclarative module of the Katie Toolkit.
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

#ifndef QDECLARATIVESTRINGCONVERTERS_P_H
#define QDECLARATIVESTRINGCONVERTERS_P_H

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

#include <QtCore/qglobal.h>
#include <QtCore/qvariant.h>

#include "qdeclarativeglobal_p.h"

QT_BEGIN_NAMESPACE

class QColor;
class QPointF;
class QSizeF;
class QRectF;
class QString;
class QByteArray;
class QVector3D;

// XXX - Bauhaus currently uses these methods which is why they're exported
namespace QDeclarativeStringConverters
{
    QVariant Q_DECLARATIVE_PRIVATE_EXPORT variantFromString(const QString &);
    QVariant Q_DECLARATIVE_PRIVATE_EXPORT variantFromString(const QString &, int preferredType, bool *ok = 0);

    QColor Q_DECLARATIVE_PRIVATE_EXPORT colorFromString(const QString &, bool *ok = 0);
#ifndef QT_NO_DATESTRING
    QDate Q_DECLARATIVE_PRIVATE_EXPORT dateFromString(const QString &, bool *ok = 0); 
    QTime Q_DECLARATIVE_PRIVATE_EXPORT timeFromString(const QString &, bool *ok = 0);
    QDateTime Q_DECLARATIVE_PRIVATE_EXPORT dateTimeFromString(const QString &, bool *ok = 0);
#endif
    QPointF Q_DECLARATIVE_PRIVATE_EXPORT pointFFromString(const QString &, bool *ok = 0);
    QSizeF Q_DECLARATIVE_PRIVATE_EXPORT sizeFFromString(const QString &, bool *ok = 0);
    QRectF Q_DECLARATIVE_PRIVATE_EXPORT rectFFromString(const QString &, bool *ok = 0);
    QVector3D Q_DECLARATIVE_PRIVATE_EXPORT vector3DFromString(const QString &, bool *ok = 0);
}

QT_END_NAMESPACE

#endif // QDECLARATIVESTRINGCONVERTERS_P_H
