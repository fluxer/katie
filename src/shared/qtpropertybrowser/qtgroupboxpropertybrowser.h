/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the tools applications of the Katie Toolkit.
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

#ifndef QTGROUPBOXPROPERTYBROWSER_H
#define QTGROUPBOXPROPERTYBROWSER_H

#include "qtpropertybrowser.h"

QT_BEGIN_NAMESPACE

class QtGroupBoxPropertyBrowserPrivate;

class QtGroupBoxPropertyBrowser : public QtAbstractPropertyBrowser
{
    Q_OBJECT
public:

    QtGroupBoxPropertyBrowser(QWidget *parent = nullptr);
    ~QtGroupBoxPropertyBrowser();

protected:
    virtual void itemInserted(QtBrowserItem *item, QtBrowserItem *afterItem);
    virtual void itemRemoved(QtBrowserItem *item);
    virtual void itemChanged(QtBrowserItem *item);

private:

    QScopedPointer<QtGroupBoxPropertyBrowserPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtGroupBoxPropertyBrowser)
    Q_DISABLE_COPY(QtGroupBoxPropertyBrowser)
    Q_PRIVATE_SLOT(d_func(), void slotUpdate())
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed())

};

QT_END_NAMESPACE

#endif
