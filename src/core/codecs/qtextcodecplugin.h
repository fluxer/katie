/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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

#ifndef QTEXTCODECPLUGIN_H
#define QTEXTCODECPLUGIN_H

#include <QtCore/qplugin.h>
#include <QtCore/qfactoryinterface.h>


QT_BEGIN_NAMESPACE

#ifndef QT_NO_TEXTCODECPLUGIN

class QTextCodec;

struct Q_CORE_EXPORT QTextCodecFactoryInterface : public QFactoryInterface
{
    virtual QTextCodec *create(const QString &key) = 0;
};

QT_END_NAMESPACE

#define QTextCodecFactoryInterface_iid "Katie.QTextCodecFactoryInterface"
Q_DECLARE_INTERFACE(QTextCodecFactoryInterface, QTextCodecFactoryInterface_iid)

QT_BEGIN_NAMESPACE

class Q_CORE_EXPORT QTextCodecPlugin : public QObject, public QTextCodecFactoryInterface
{
    Q_OBJECT
    Q_INTERFACES(QTextCodecFactoryInterface:QFactoryInterface)
public:
    explicit QTextCodecPlugin(QObject *parent = Q_NULLPTR);
    ~QTextCodecPlugin();

    virtual QList<QByteArray> names() const = 0;
    virtual QList<QByteArray> aliases() const = 0;
    virtual QTextCodec *createForName(const QByteArray &name) = 0;

    virtual QList<int> mibEnums() const = 0;
    virtual QTextCodec *createForMib(int mib) = 0;

private:
    QStringList keys() const;
    QTextCodec *create(const QString &name);
};

#endif // QT_NO_TEXTCODECPLUGIN

QT_END_NAMESPACE


#endif // QTEXTCODECPLUGIN_H
