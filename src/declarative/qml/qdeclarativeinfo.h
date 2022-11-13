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

#ifndef QDECLARATIVEINFO_H
#define QDECLARATIVEINFO_H

#include <QtDeclarative/qdeclarativeerror.h>

QT_BEGIN_NAMESPACE

class QDeclarativeInfoPrivate;

class Q_DECLARATIVE_EXPORT QDeclarativeInfo
{
public:
    QDeclarativeInfo(const QDeclarativeInfo &);
    ~QDeclarativeInfo();

    QDeclarativeInfo &operator<<(const char* t);
    QDeclarativeInfo &operator<<(const QString & t);
    QDeclarativeInfo &operator<<(const QByteArray & t);

private:
    friend Q_DECLARATIVE_EXPORT QDeclarativeInfo qmlInfo(const QObject *me);
    friend Q_DECLARATIVE_EXPORT QDeclarativeInfo qmlInfo(const QObject *me, const QDeclarativeError &error);
    friend Q_DECLARATIVE_EXPORT QDeclarativeInfo qmlInfo(const QObject *me, const QList<QDeclarativeError> &errors);

    QDeclarativeInfo(QDeclarativeInfoPrivate *);
    QDeclarativeInfoPrivate *d;
};

Q_DECLARATIVE_EXPORT QDeclarativeInfo qmlInfo(const QObject *me);
Q_DECLARATIVE_EXPORT QDeclarativeInfo qmlInfo(const QObject *me, const QDeclarativeError &error);
Q_DECLARATIVE_EXPORT QDeclarativeInfo qmlInfo(const QObject *me, const QList<QDeclarativeError> &errors);

QT_END_NAMESPACE

#endif // QDECLARATIVEINFO_H
