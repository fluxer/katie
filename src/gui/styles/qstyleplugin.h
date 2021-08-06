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

#ifndef QSTYLEPLUGIN_H
#define QSTYLEPLUGIN_H

#include <QtCore/qplugin.h>
#include <QtCore/qfactoryinterface.h>


QT_BEGIN_NAMESPACE

class QStyle;

struct Q_GUI_EXPORT QStyleFactoryInterface : public QFactoryInterface
{
    virtual QStyle *create(const QString &key) = 0;
};

QT_END_NAMESPACE

#define QStyleFactoryInterface_iid "Katie.QStyleFactoryInterface"
Q_DECLARE_INTERFACE(QStyleFactoryInterface, QStyleFactoryInterface_iid)

QT_BEGIN_NAMESPACE

class Q_GUI_EXPORT QStylePlugin : public QObject, public QStyleFactoryInterface
{
    Q_OBJECT
    Q_INTERFACES(QStyleFactoryInterface:QFactoryInterface)
public:
    explicit QStylePlugin(QObject *parent = nullptr);
    ~QStylePlugin();

    virtual QStringList keys() const = 0;
    virtual QStyle *create(const QString &key) = 0;
};

QT_END_NAMESPACE


#endif // QSTYLEPLUGIN_H
