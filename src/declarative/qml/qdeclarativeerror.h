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

#ifndef QDECLARATIVEERROR_H
#define QDECLARATIVEERROR_H

#include <QtCore/qurl.h>
#include <QtCore/qstring.h>


QT_BEGIN_NAMESPACE


class QDebug;
class QDeclarativeErrorPrivate;
class Q_DECLARATIVE_EXPORT QDeclarativeError
{
public:
    QDeclarativeError();
    QDeclarativeError(const QDeclarativeError &);
    QDeclarativeError &operator=(const QDeclarativeError &);
    ~QDeclarativeError();

    bool isValid() const;

    QUrl url() const;
    void setUrl(const QUrl &);
    QString description() const;
    void setDescription(const QString &);
    int line() const;
    void setLine(int);
    int column() const;
    void setColumn(int);

    QString toString() const;
private:
    QDeclarativeErrorPrivate *d;
};

QDebug Q_DECLARATIVE_EXPORT operator<<(QDebug debug, const QDeclarativeError &error);

QT_END_NAMESPACE


#endif // QDECLARATIVEERROR_H
