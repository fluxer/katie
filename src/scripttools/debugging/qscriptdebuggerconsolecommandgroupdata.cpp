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

#include "qscriptdebuggerconsolecommandgroupdata_p.h"

#include <QtCore/qstring.h>
#include <QtCore/qshareddata.h>

QT_BEGIN_NAMESPACE

/*!
  \since 4.5
  \class QScriptDebuggerConsoleCommandGroupData
  \internal

  \brief The QScriptDebuggerConsoleCommandGroupData class holds data associated with a console command group.
*/

class QScriptDebuggerConsoleCommandGroupDataPrivate : public QSharedData
{
public:
    QScriptDebuggerConsoleCommandGroupDataPrivate();
    ~QScriptDebuggerConsoleCommandGroupDataPrivate();

    QString shortDescription;
    QString longDescription;
};

QScriptDebuggerConsoleCommandGroupDataPrivate::QScriptDebuggerConsoleCommandGroupDataPrivate()
{
}

QScriptDebuggerConsoleCommandGroupDataPrivate::~QScriptDebuggerConsoleCommandGroupDataPrivate()
{
}

QScriptDebuggerConsoleCommandGroupData::QScriptDebuggerConsoleCommandGroupData()
    : d_ptr(0)
{
}

QScriptDebuggerConsoleCommandGroupData::QScriptDebuggerConsoleCommandGroupData(
    const QString &shortDescription, const QString &longDescription)
    : d_ptr(new QScriptDebuggerConsoleCommandGroupDataPrivate)
{
    d_ptr->shortDescription = shortDescription;
    d_ptr->longDescription = longDescription;
    d_ptr->ref.ref();
}

QScriptDebuggerConsoleCommandGroupData::QScriptDebuggerConsoleCommandGroupData(
    const QScriptDebuggerConsoleCommandGroupData &other)
    : d_ptr(other.d_ptr.data())
{
    if (d_ptr)
        d_ptr->ref.ref();
}

QScriptDebuggerConsoleCommandGroupData::~QScriptDebuggerConsoleCommandGroupData()
{
}

QScriptDebuggerConsoleCommandGroupData &QScriptDebuggerConsoleCommandGroupData::operator=(
    const QScriptDebuggerConsoleCommandGroupData &other)
{
    d_ptr.assign(other.d_ptr.data());
    return *this;
}

QString QScriptDebuggerConsoleCommandGroupData::shortDescription() const
{
    Q_D(const QScriptDebuggerConsoleCommandGroupData);
    return d->shortDescription;
}

QString QScriptDebuggerConsoleCommandGroupData::longDescription() const
{
    Q_D(const QScriptDebuggerConsoleCommandGroupData);
    return d->longDescription;
}

bool QScriptDebuggerConsoleCommandGroupData::isValid() const
{
    Q_D(const QScriptDebuggerConsoleCommandGroupData);
    return (d != 0);
}

QT_END_NAMESPACE
