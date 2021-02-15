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

#ifndef QFACTORYINTERFACE_H
#define QFACTORYINTERFACE_H

#include <QtCore/qobject.h>
#include <QtCore/qstringlist.h>


QT_BEGIN_NAMESPACE


struct Q_CORE_EXPORT QFactoryInterface
{
    virtual ~QFactoryInterface() {}
    virtual QStringList keys() const = 0;
};

QT_END_NAMESPACE

Q_DECLARE_INTERFACE(QFactoryInterface, "Katie.QFactoryInterface")


#endif // QFACTORYINTERFACE_H
