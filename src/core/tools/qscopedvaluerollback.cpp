/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#include "qscopedvaluerollback.h"

QT_BEGIN_NAMESPACE

/*!
    \class QScopedValueRollback
    \brief The QScopedValueRollback class resets a variable to its previous value on destruction.
    \since 4.8
    \ingroup misc

    The QScopedAssignment class can be used to revert state when an
    exception is thrown without needing to write try-catch blocks.

    It can also be used to manage variables that are temporarily set,
    such as reentrancy guards. By using this class, the variable will
    be reset whether the function is exited normally, exited early by
    a return statement, or exited by an exception.

    The template can only be instantiated with a type that supports assignment.

    \sa QScopedPointer
*/

/*!
    \fn QScopedValueRollback::QScopedValueRollback(T &var)

    Stores the previous value of \a var internally, for revert on destruction.
*/

/*!
    \fn QScopedValueRollback::~QScopedValueRollback()

    Assigns the previous value to the managed variable.
    This is the value at construction time, or at the last call to commit()
*/

/*!
    \fn void QScopedValueRollback::commit()

    Updates the previous value of the managed variable to its current value.
*/

QT_END_NAMESPACE
