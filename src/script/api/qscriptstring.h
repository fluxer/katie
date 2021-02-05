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

#ifndef QSCRIPTSTRING_H
#define QSCRIPTSTRING_H

#include <QtCore/qstring.h>
#include <QtCore/qsharedpointer.h>


QT_BEGIN_NAMESPACE


class QScriptStringPrivate;
class Q_SCRIPT_EXPORT QScriptString
{
public:
    QScriptString();
    QScriptString(const QScriptString &other);
    ~QScriptString();

    QScriptString &operator=(const QScriptString &other);

    bool isValid() const;

    bool operator==(const QScriptString &other) const;
    bool operator!=(const QScriptString &other) const;

    quint32 toArrayIndex(bool *ok = 0) const;

    QString toString() const;
    operator QString() const;

private:
    QExplicitlySharedDataPointer<QScriptStringPrivate> d_ptr;
    friend class QScriptValue;
    Q_DECLARE_PRIVATE(QScriptString)
};

Q_SCRIPT_EXPORT uint qHash(const QScriptString &key);

QT_END_NAMESPACE


#endif // QSCRIPTSTRING_H
