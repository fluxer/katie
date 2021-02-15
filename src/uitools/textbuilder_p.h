/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the Katie Designer of the Katie Toolkit.
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

#ifndef TEXTBUILDER_H
#define TEXTBUILDER_H

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

#include <QtCore/QList>
#include <QtCore/QString>

QT_BEGIN_NAMESPACE

class QVariant;

#ifdef QFORMINTERNAL_NAMESPACE
namespace QFormInternal
{
#endif

class DomProperty;
class DomString;

class Q_UITOOLS_EXPORT QTextBuilder
{
public:
    QTextBuilder();
    virtual ~QTextBuilder();

    virtual QVariant loadText(const DomProperty *property) const;

    virtual QVariant toNativeValue(const QVariant &value) const;

    virtual DomProperty *saveText(const QVariant &value) const;
};


#ifdef QFORMINTERNAL_NAMESPACE
}
#endif

QT_END_NAMESPACE

#endif // TEXTBUILDER_H
