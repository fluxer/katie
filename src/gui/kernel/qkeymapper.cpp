/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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


#include "qapplication.h"

#include "qobject_p.h"
#include "qkeymapper_p.h"
#include "qwidget.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKeyMapper
    \since 4.2
    \internal

    \sa QObject
*/

/*!
    Constructs a new key mapper.
*/
QKeyMapper::QKeyMapper()
    : QObject(*new QKeyMapperPrivate, 0)
{
}

/*!
    Destroys the key mapper.
*/
QKeyMapper::~QKeyMapper()
{
}

QList<int> QKeyMapper::possibleKeys(QKeyEvent *e)
{
    QList<int> result;

    if (e->key() != Qt::Key_unknown) {
        result << int(e->key() + e->modifiers());
    } else if (!e->text().isEmpty()) {
        result << int(e->text().at(0).unicode() + e->modifiers());
    }

    return result;
}

extern bool qt_sendSpontaneousEvent(QObject *receiver, QEvent *event); // in qapplication_*.cpp
void QKeyMapper::changeKeyboard()
{
    instance()->d_func()->clearMappings();

    // inform all toplevel widgets of the change
    QEvent e(QEvent::KeyboardLayoutChange);
    foreach (QWidget *w, QApplication::topLevelWidgets()) {
        qt_sendSpontaneousEvent(w, &e);
    }
}

Q_GLOBAL_STATIC(QKeyMapper, keymapper)
/*!
    Returns the pointer to the single instance of QKeyMapper in the application.
    If none yet exists, the function ensures that one is created.
*/
QKeyMapper *QKeyMapper::instance()
{
    return keymapper();
}

QKeyMapperPrivate *qt_keymapper_private()
{
    return QKeyMapper::instance()->d_func();
}

QT_END_NAMESPACE


#include "moc_qkeymapper_p.h"
