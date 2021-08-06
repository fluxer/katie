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

#ifndef QERRORMESSAGE_H
#define QERRORMESSAGE_H

#include <QtGui/qdialog.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_ERRORMESSAGE

class QErrorMessagePrivate;

class Q_GUI_EXPORT QErrorMessage: public QDialog
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QErrorMessage)
public:
    explicit QErrorMessage(QWidget* parent = nullptr);
    ~QErrorMessage();

    static QErrorMessage * qtHandler();

public Q_SLOTS:
    void showMessage(const QString &message);
    void showMessage(const QString &message, const QString &type);

protected:
    void done(int);
    void changeEvent(QEvent *e);

private:
    Q_DISABLE_COPY(QErrorMessage)
};

#endif // QT_NO_ERRORMESSAGE

QT_END_NAMESPACE


#endif // QERRORMESSAGE_H
