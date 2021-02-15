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

#include "qscriptvalueproperty_p.h"

#include <QtCore/qshareddata.h>
#include <QtCore/qstring.h>

QT_BEGIN_NAMESPACE

class QScriptValuePropertyPrivate : public QSharedData
{
public:
    QScriptValuePropertyPrivate();
    ~QScriptValuePropertyPrivate();

    QString name;
    QScriptValue value;
    QScriptValue::PropertyFlags flags;
};

QScriptValuePropertyPrivate::QScriptValuePropertyPrivate()
{
}

QScriptValuePropertyPrivate::~QScriptValuePropertyPrivate()
{
}

/*!
  Constructs an invalid QScriptValueProperty.
*/
QScriptValueProperty::QScriptValueProperty()
    : d_ptr(0)
{
}

/*!
  Constructs a QScriptValueProperty with the given \a name,
  \a value and \a flags.
*/
QScriptValueProperty::QScriptValueProperty(const QString &name,
                                           const QScriptValue &value,
                                           QScriptValue::PropertyFlags flags)
    : d_ptr(new QScriptValuePropertyPrivate)
{
    d_ptr->name = name;
    d_ptr->value = value;
    d_ptr->flags = flags;
    d_ptr->ref.ref();
}

/*!
  Constructs a QScriptValueProperty that is a copy of the \a other property.
*/
QScriptValueProperty::QScriptValueProperty(const QScriptValueProperty &other)
    : d_ptr(other.d_ptr.data())
{
    if (d_ptr)
        d_ptr->ref.ref();
}

/*!
  Destroys this QScriptValueProperty.
*/
QScriptValueProperty::~QScriptValueProperty()
{
}

/*!
  Assigns the \a other property to this QScriptValueProperty.
*/
QScriptValueProperty &QScriptValueProperty::operator=(const QScriptValueProperty &other)
{
    d_ptr.assign(other.d_ptr.data());
    return *this;
}

/*!
  Returns the name of this QScriptValueProperty.
*/
QString QScriptValueProperty::name() const
{
    Q_D(const QScriptValueProperty);
    if (!d)
        return QString();
    return d->name;
}

/*!
  Returns the value of this QScriptValueProperty.
*/
QScriptValue QScriptValueProperty::value() const
{
    Q_D(const QScriptValueProperty);
    if (!d)
        return QScriptValue();
    return d->value;
}

/*!
  Returns the flags of this QScriptValueProperty.
*/
QScriptValue::PropertyFlags QScriptValueProperty::flags() const
{
    Q_D(const QScriptValueProperty);
    if (!d)
        return 0;
    return d->flags;
}

/*!
  Returns true if this QScriptValueProperty is valid, otherwise
  returns false.
*/
bool QScriptValueProperty::isValid() const
{
    Q_D(const QScriptValueProperty);
    return (d != 0);
}

QT_END_NAMESPACE
