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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QWHATSTHIS_H
#define QWHATSTHIS_H

#include <QtCore/qobject.h>
#include <QtGui/qcursor.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_WHATSTHIS

class QAction;

class Q_GUI_EXPORT QWhatsThis
{
    QWhatsThis();

public:
    static void enterWhatsThisMode();
    static bool inWhatsThisMode();
    static void leaveWhatsThisMode();

    static void showText(const QPoint &pos, const QString &text, QWidget *w = 0);
    static void hideText();

    static QAction *createAction(QObject *parent = nullptr);

};

#endif // QT_NO_WHATSTHIS

QT_END_NAMESPACE


#endif // QWHATSTHIS_H
