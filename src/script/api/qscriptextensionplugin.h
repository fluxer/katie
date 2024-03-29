/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtScript module of the Katie Toolkit.
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

#ifndef QSCRIPTEXTENSIONPLUGIN_H
#define QSCRIPTEXTENSIONPLUGIN_H

#include <QtCore/qplugin.h>
#include <QtCore/qstringlist.h>
#include <QtScript/qscriptextensioninterface.h>

QT_BEGIN_NAMESPACE

class QScriptValue;

class Q_SCRIPT_EXPORT QScriptExtensionPlugin : public QObject,
                                               public QScriptExtensionInterface
{
    Q_OBJECT
public:
    explicit QScriptExtensionPlugin(QObject *parent = nullptr);
    ~QScriptExtensionPlugin();

    virtual QStringList keys() const = 0;
    virtual void initialize(const QString &key, QScriptEngine *engine) = 0;

    QScriptValue setupPackage(const QString &key, QScriptEngine *engine) const;
};

QT_END_NAMESPACE


#endif // QSCRIPTEXTENSIONPLUGIN_H
