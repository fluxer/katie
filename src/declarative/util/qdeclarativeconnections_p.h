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

#ifndef QDECLARATIVECONNECTIONS_H
#define QDECLARATIVECONNECTIONS_H

#include "qdeclarative.h"
#include "qdeclarativescriptstring.h"
#include "qdeclarativecustomparser_p.h"

#include <QtCore/qobject.h>
#include <QtCore/qstring.h>


QT_BEGIN_NAMESPACE


class QDeclarativeBoundSignal;
class QDeclarativeContext;
class QDeclarativeConnectionsPrivate;
class Q_AUTOTEST_EXPORT QDeclarativeConnections : public QObject, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativeConnections)

    Q_INTERFACES(QDeclarativeParserStatus)
    Q_PROPERTY(QObject *target READ target WRITE setTarget NOTIFY targetChanged)
    Q_PROPERTY(bool ignoreUnknownSignals READ ignoreUnknownSignals WRITE setIgnoreUnknownSignals)

public:
    QDeclarativeConnections(QObject *parent=0);
    ~QDeclarativeConnections();

    QObject *target() const;
    void setTarget(QObject *);

    bool ignoreUnknownSignals() const;
    void setIgnoreUnknownSignals(bool ignore);

Q_SIGNALS:
    void targetChanged();

private:
    void connectSignals();
    void classBegin();
    void componentComplete();
};

class QDeclarativeConnectionsParser : public QDeclarativeCustomParser
{
public:
    virtual QByteArray compile(const QList<QDeclarativeCustomParserProperty> &);
    virtual void setCustomData(QObject *, const QByteArray &);
};


QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeConnections)


#endif
