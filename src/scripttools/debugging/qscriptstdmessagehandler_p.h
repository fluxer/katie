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

#ifndef QSCRIPTSTDMESSAGEHANDLER_P_H
#define QSCRIPTSTDMESSAGEHANDLER_P_H

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

#include <QtCore/qscopedpointer.h>

#include "qscriptmessagehandlerinterface_p.h"

QT_BEGIN_NAMESPACE

class QScriptStdMessageHandlerPrivate;
class Q_AUTOTEST_EXPORT QScriptStdMessageHandler
    : public QScriptMessageHandlerInterface
{
public:
    QScriptStdMessageHandler();
    ~QScriptStdMessageHandler();

    void message(QtMsgType type, const QString &text,
                 const QString &fileName = QString(),
                 int lineNumber = -1, int columnNumber = -1,
                 const QVariant &data = QVariant());

private:
    QScopedPointer<QScriptStdMessageHandlerPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(QScriptStdMessageHandler)
    Q_DISABLE_COPY(QScriptStdMessageHandler)
};

QT_END_NAMESPACE

#endif
