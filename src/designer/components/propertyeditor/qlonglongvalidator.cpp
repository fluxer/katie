/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the Katie Designer of the Katie Toolkit.
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

#include "qlonglongvalidator.h"

QT_BEGIN_NAMESPACE

using namespace qdesigner_internal;

// ----------------------------------------------------------------------------
QLongLongValidator::QLongLongValidator(QObject * parent)
    : QValidator(parent),
      b(Q_UINT64_C(0x8000000000000000)), t(Q_UINT64_C(0x7FFFFFFFFFFFFFFF))
{
}

QLongLongValidator::QLongLongValidator(qlonglong minimum, qlonglong maximum,
                              QObject * parent)
    : QValidator(parent), b(minimum), t(maximum)
{
}

QLongLongValidator::~QLongLongValidator()
{
    // nothing
}

QValidator::State QLongLongValidator::validate(QString & input, int &) const
{
    if (input.contains(QLatin1Char(' ')))
        return Invalid;
    if (input.isEmpty() || (b < 0 && input == QString(QLatin1Char('-'))))
        return Intermediate;
    bool ok;
    qlonglong entered = input.toLongLong(&ok);
    if (!ok || (entered < 0 && b >= 0)) {
        return Invalid;
    } else if (entered >= b && entered <= t) {
        return Acceptable;
    } else {
        if (entered >= 0)
            return (entered > t) ? Invalid : Intermediate;
        else
            return (entered < b) ? Invalid : Intermediate;
    }
}

void QLongLongValidator::setRange(qlonglong bottom, qlonglong top)
{
    b = bottom;
    t = top;
}

void QLongLongValidator::setBottom(qlonglong bottom)
{
    setRange(bottom, top());
}

void QLongLongValidator::setTop(qlonglong top)
{
    setRange(bottom(), top);
}


// ----------------------------------------------------------------------------
QULongLongValidator::QULongLongValidator(QObject * parent)
    : QValidator(parent),
      b(0), t(Q_UINT64_C(0xFFFFFFFFFFFFFFFF))
{
}

QULongLongValidator::QULongLongValidator(qulonglong minimum, qulonglong maximum,
                              QObject * parent)
    : QValidator(parent), b(minimum), t(maximum)
{
}

QULongLongValidator::~QULongLongValidator()
{
    // nothing
}

QValidator::State QULongLongValidator::validate(QString & input, int &) const
{
    if (input.isEmpty())
        return Intermediate;

    bool ok;
    qulonglong entered = input.toULongLong(&ok);
    if (input.contains(QLatin1Char(' ')) || input.contains(QLatin1Char('-')) || !ok)
        return Invalid;

    if (entered >= b && entered <= t)
        return Acceptable;

    return Invalid;
}

void QULongLongValidator::setRange(qulonglong bottom, qulonglong top)
{
    b = bottom;
    t = top;
}

void QULongLongValidator::setBottom(qulonglong bottom)
{
    setRange(bottom, top());
}

void QULongLongValidator::setTop(qulonglong top)
{
    setRange(bottom(), top);
}

QT_END_NAMESPACE
#include "moc_qlonglongvalidator.h"
