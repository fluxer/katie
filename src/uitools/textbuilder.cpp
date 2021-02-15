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

#include "textbuilder_p.h"
#include "ui4.h"
#include <QtCore/QVariant>

QT_BEGIN_NAMESPACE

#ifdef QFORMINTERNAL_NAMESPACE
namespace QFormInternal {
#endif

QTextBuilder::QTextBuilder()
{

}

QTextBuilder::~QTextBuilder()
{

}

QVariant QTextBuilder::loadText(const DomProperty *property) const
{
    if (property->kind() == DomProperty::String)
        return property->elementString()->text();
    return QVariant();
}

QVariant QTextBuilder::toNativeValue(const QVariant &value) const
{
    return value;
}

DomProperty *QTextBuilder::saveText(const QVariant &value) const
{
    Q_UNUSED(value)
    return 0;
}

#ifdef QFORMINTERNAL_NAMESPACE
} // namespace QFormInternal
#endif

QT_END_NAMESPACE
