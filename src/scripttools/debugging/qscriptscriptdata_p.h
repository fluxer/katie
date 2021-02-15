/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtSCriptTools module of the Katie Toolkit.
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

#ifndef QSCRIPTSCRIPTDATA_P_H
#define QSCRIPTSCRIPTDATA_P_H

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

#include "qobjectdefs.h"
#include "qscopedpointer_p.h"
#include "qdatetime.h"
#include "qmap.h"

QT_BEGIN_NAMESPACE

class QDataStream;
class QString;
class QStringList;

class QScriptScriptDataPrivate;
class Q_AUTOTEST_EXPORT QScriptScriptData
{
public:
    friend Q_AUTOTEST_EXPORT QDataStream &operator<<(QDataStream &, const QScriptScriptData &);
    friend Q_AUTOTEST_EXPORT QDataStream &operator>>(QDataStream &, QScriptScriptData &);

    QScriptScriptData();
    QScriptScriptData(const QString &contents, const QString &fileName,
                      int baseLineNumber, const QDateTime &timeStamp = QDateTime());
    QScriptScriptData(const QScriptScriptData &other);
    ~QScriptScriptData();

    QString contents() const;
    QStringList lines(int startLineNumber, int count) const;
    QString fileName() const;
    int baseLineNumber() const;
    QDateTime timeStamp() const;

    bool isValid() const;

    QScriptScriptData &operator=(const QScriptScriptData &other);

    bool operator==(const QScriptScriptData &other) const;
    bool operator!=(const QScriptScriptData &other) const;

private:
    QScopedSharedPointer<QScriptScriptDataPrivate> d_ptr;

    Q_DECLARE_PRIVATE(QScriptScriptData)
};

typedef QMap<qint64, QScriptScriptData> QScriptScriptMap;

Q_AUTOTEST_EXPORT QDataStream &operator<<(QDataStream &, const QScriptScriptData &);
Q_AUTOTEST_EXPORT QDataStream &operator>>(QDataStream &, QScriptScriptData &);

QT_END_NAMESPACE

#endif
