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

#ifndef ABSTRACTMETADATABASE_H
#define ABSTRACTMETADATABASE_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QHash>


QT_BEGIN_NAMESPACE

class QCursor;
class QWidget;

class QDesignerFormEditorInterface;

class QDesignerMetaDataBaseItemInterface
{
public:
    virtual ~QDesignerMetaDataBaseItemInterface() {}

    virtual QString name() const = 0;
    virtual void setName(const QString &name) = 0;
    
    virtual QList<QWidget*> tabOrder() const = 0;
    virtual void setTabOrder(const QList<QWidget*> &tabOrder) = 0;
  
    virtual bool enabled() const = 0;
    virtual void setEnabled(bool b) = 0;    
};


class Q_DESIGNER_EXPORT QDesignerMetaDataBaseInterface: public QObject
{
    Q_OBJECT
public:
    QDesignerMetaDataBaseInterface(QObject *parent = nullptr);
    virtual ~QDesignerMetaDataBaseInterface();

    virtual QDesignerMetaDataBaseItemInterface *item(QObject *object) const = 0;
    virtual void add(QObject *object) = 0;
    virtual void remove(QObject *object) = 0;

    virtual QList<QObject*> objects() const = 0;

    virtual QDesignerFormEditorInterface *core() const = 0;
    
Q_SIGNALS:
    void changed();
};

QT_END_NAMESPACE


#endif // ABSTRACTMETADATABASE_H
