/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtNetwork module of the Katie Toolkit.
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

#ifndef QNETWORKCONFIGURATIONPRIVATE_H
#define QNETWORKCONFIGURATIONPRIVATE_H

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

#include "qnetworkconfiguration.h"

#include <QtCore/qshareddata.h>
#include <QtCore/qmutex.h>
#include <QtCore/qmap.h>

QT_BEGIN_NAMESPACE

typedef QExplicitlySharedDataPointer<QNetworkConfigurationPrivate> QNetworkConfigurationPrivatePointer;
class QNetworkConfigurationPrivate : public QSharedData
{
public:
    QNetworkConfigurationPrivate() :
        mutex(QMutex::Recursive),
        type(QNetworkConfiguration::Invalid),
        purpose(QNetworkConfiguration::UnknownPurpose),
        bearerType(QNetworkConfiguration::BearerUnknown),
        isValid(false), roamingSupported(false)
    {}
    virtual ~QNetworkConfigurationPrivate()
    {
        //release pointers to member configurations
        serviceNetworkMembers.clear();
    }

    virtual QString bearerTypeName() const
    {
        return QLatin1String("Unknown");
    }

    QMap<unsigned int, QNetworkConfigurationPrivatePointer> serviceNetworkMembers;

    mutable QMutex mutex;

    QString name;
    QString id;

    QNetworkConfiguration::StateFlags state;
    QNetworkConfiguration::Type type;
    QNetworkConfiguration::Purpose purpose;
    QNetworkConfiguration::BearerType bearerType;

    bool isValid;
    bool roamingSupported;

private:
    Q_DISABLE_COPY(QNetworkConfigurationPrivate)
};

QT_END_NAMESPACE

#endif // QNETWORKCONFIGURATIONPRIVATE_H
