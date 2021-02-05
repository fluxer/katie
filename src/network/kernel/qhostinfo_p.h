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

#ifndef QHOSTINFO_P_H
#define QHOSTINFO_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of the QHostInfo class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include "QtCore/qcoreapplication.h"
#include "qcoreapplication_p.h"
#include "QtNetwork/qhostinfo.h"
#include "QtCore/qmutex.h"
#include "QtCore/qobject.h"
#include "QtCore/qpointer.h"
#include "QtCore/qlist.h"
#include <QElapsedTimer>
#include <QCache>

#include <QNetworkSession>
#include <QSharedPointer>



QT_BEGIN_NAMESPACE


class QHostInfoResult : public QObject
{
    Q_OBJECT
public Q_SLOTS:
    inline void emitResultsReady(const QHostInfo &info)
    {
        emit resultsReady(info);
    }

Q_SIGNALS:
    void resultsReady(const QHostInfo &info);
};

class QHostInfoAgent
{
    Q_DECLARE_TR_FUNCTIONS(QHostInfoAgent)
public:
    static QHostInfo fromName(const QString &hostName);
};

class QHostInfoPrivate
{
public:
    inline QHostInfoPrivate()
        : err(QHostInfo::NoError),
          errorStr(QLatin1String(QT_TRANSLATE_NOOP("QHostInfo", "Unknown error"))),
          lookupId(0)
    {
    }
    QHostInfo::HostInfoError err;
    QString errorStr;
    QList<QHostAddress> addrs;
    QString hostName;
    int lookupId;
};

// These functions are outside of the QHostInfo class and strictly internal.
// Do NOT use them outside of QAbstractSocket.
void Q_AUTOTEST_EXPORT qt_qhostinfo_clear_cache();
void Q_AUTOTEST_EXPORT qt_qhostinfo_enable_cache(bool e);

class QHostInfoCache
{
public:
    QHostInfoCache();
    const int max_age; // seconds

    QHostInfo get(const QString &name, bool *valid) const;
    void put(const QString &name, const QHostInfo &info);
    void clear();

    bool isEnabled() const;
    void setEnabled(bool e);
private:
    bool enabled;
    struct QHostInfoCacheElement {
        QHostInfo info;
        QElapsedTimer age;
    };
    QCache<QString,QHostInfoCacheElement> cache;
    QMutex mutex;
};

QT_END_NAMESPACE

#endif // QHOSTINFO_P_H
