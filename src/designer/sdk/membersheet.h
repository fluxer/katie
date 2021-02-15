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

#ifndef MEMBERSHEET_H
#define MEMBERSHEET_H

#include <QtDesigner/extension.h>

#include <QtCore/QList>
#include <QtCore/QByteArray>


QT_BEGIN_NAMESPACE

class QString;

class QDesignerMemberSheetExtension
{
public:
    virtual ~QDesignerMemberSheetExtension() {}

    virtual int count() const = 0;

    virtual int indexOf(const QString &name) const = 0;

    virtual QString memberName(int index) const = 0;
    virtual QString memberGroup(int index) const = 0;
    virtual void setMemberGroup(int index, const QString &group) = 0;

    virtual bool isVisible(int index) const = 0;
    virtual void setVisible(int index, bool b) = 0;

    virtual bool isSignal(int index) const = 0;
    virtual bool isSlot(int index) const = 0;

    virtual bool inheritedFromWidget(int index) const = 0;

    virtual QString declaredInClass(int index) const = 0;

    virtual QString signature(int index) const = 0;
    virtual QList<QByteArray> parameterTypes(int index) const = 0;
    virtual QList<QByteArray> parameterNames(int index) const = 0;
};

QT_END_NAMESPACE

Q_DECLARE_EXTENSION_INTERFACE(QDesignerMemberSheetExtension, "Katie.Designer.MemberSheet")


#endif // MEMBERSHEET_H
