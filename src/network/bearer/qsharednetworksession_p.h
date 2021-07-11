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

#ifndef QSHAREDNETWORKSESSIONPRIVATE_H
#define QSHAREDNETWORKSESSIONPRIVATE_H

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

#include "qnetworksession.h"
#include "qnetworkconfiguration.h"
#include <QHash>
#include <QSharedPointer>

#ifndef QT_NO_BEARERMANAGEMENT

QT_BEGIN_NAMESPACE

uint qHash(const QNetworkConfiguration& config);

class QSharedNetworkSessionManager
{
public:
    static QSharedPointer<QNetworkSession> getSession(const QNetworkConfiguration &config);
    static void setSession(QNetworkConfiguration config, QSharedPointer<QNetworkSession> session);
private:
    QHash<QNetworkConfiguration, QWeakPointer<QNetworkSession> > sessions;
};

QT_END_NAMESPACE

#endif // QT_NO_BEARERMANAGEMENT

#endif //QSHAREDNETWORKSESSIONPRIVATE_H

