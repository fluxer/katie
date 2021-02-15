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

#ifndef ABSTRACTPROPERTYEDITOR_H
#define ABSTRACTPROPERTYEDITOR_H

#include <QtGui/QWidget>


QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;
class QString;
class QVariant;

class Q_DESIGNER_EXPORT QDesignerPropertyEditorInterface: public QWidget
{
    Q_OBJECT
public:
    QDesignerPropertyEditorInterface(QWidget *parent, Qt::WindowFlags flags = 0);
    virtual ~QDesignerPropertyEditorInterface();

    virtual QDesignerFormEditorInterface *core() const;

    virtual bool isReadOnly() const = 0;
    virtual QObject *object() const = 0;

    virtual QString currentPropertyName() const = 0;

Q_SIGNALS:
    void propertyChanged(const QString &name, const QVariant &value);

public Q_SLOTS:
    virtual void setObject(QObject *object) = 0;
    virtual void setPropertyValue(const QString &name, const QVariant &value, bool changed = true) = 0;
    virtual void setReadOnly(bool readOnly) = 0;
};

QT_END_NAMESPACE


#endif // ABSTRACTPROPERTYEDITOR_H
