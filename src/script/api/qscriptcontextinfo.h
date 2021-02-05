/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtScript module of the Katie Toolkit.
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

#ifndef QSCRIPTCONTEXTINFO_H
#define QSCRIPTCONTEXTINFO_H

#include <QtCore/qobjectdefs.h>

#include <QtCore/qlist.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qsharedpointer.h>


QT_BEGIN_NAMESPACE


class QScriptContext;
#ifndef QT_NO_DATASTREAM
class QDataStream;
#endif

class QScriptContextInfoPrivate;
class Q_SCRIPT_EXPORT QScriptContextInfo
{
public:
#ifndef QT_NO_DATASTREAM
    friend Q_SCRIPT_EXPORT QDataStream &operator<<(QDataStream &, const QScriptContextInfo &);
    friend Q_SCRIPT_EXPORT QDataStream &operator>>(QDataStream &, QScriptContextInfo &);
#endif

    enum FunctionType {
        ScriptFunction,
        QtFunction,
        QtPropertyFunction,
        NativeFunction
    };

    QScriptContextInfo(const QScriptContext *context);
    QScriptContextInfo(const QScriptContextInfo &other);
    QScriptContextInfo();
    ~QScriptContextInfo();

    QScriptContextInfo &operator=(const QScriptContextInfo &other);

    bool isNull() const;

    qint64 scriptId() const;
    QString fileName() const;
    int lineNumber() const;
#ifdef QT_DEPRECATED
    QT_DEPRECATED int columnNumber() const;
#endif

    QString functionName() const;
    FunctionType functionType() const;

    QStringList functionParameterNames() const;

    int functionStartLineNumber() const;
    int functionEndLineNumber() const;

    int functionMetaIndex() const;

    bool operator==(const QScriptContextInfo &other) const;
    bool operator!=(const QScriptContextInfo &other) const;

private:
    QExplicitlySharedDataPointer<QScriptContextInfoPrivate> d_ptr;

    Q_DECLARE_PRIVATE(QScriptContextInfo)
};

typedef QList<QScriptContextInfo> QScriptContextInfoList;

#ifndef QT_NO_DATASTREAM
Q_SCRIPT_EXPORT QDataStream &operator<<(QDataStream &, const QScriptContextInfo &);
Q_SCRIPT_EXPORT QDataStream &operator>>(QDataStream &, QScriptContextInfo &);
#endif

QT_END_NAMESPACE


#endif
