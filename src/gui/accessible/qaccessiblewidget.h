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

#ifndef QACCESSIBLEWIDGET_H
#define QACCESSIBLEWIDGET_H

#include <QtGui/qaccessibleobject.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_ACCESSIBILITY

class QAccessibleWidgetPrivate;

class Q_GUI_EXPORT QAccessibleWidget : public QAccessibleObject
{
public:
    explicit QAccessibleWidget(QWidget *o, Role r = Client, const QString& name = QString());

    int childCount() const;
    int indexOfChild(const QAccessibleInterface *child) const;
    Relation relationTo(int child, const QAccessibleInterface *other, int otherChild) const;

    int childAt(int x, int y) const;
    QRect rect(int child) const;
    int navigate(RelationFlag rel, int entry, QAccessibleInterface **target) const;

    QString text(Text t, int child) const;
    Role role(int child) const;
    State state(int child) const;

#ifndef QT_NO_ACTION
    int userActionCount(int child) const;
    QString actionText(int action, Text t, int child) const;
    bool doAction(int action, int child, const QVariantList &params);
#endif

    QVariant invokeMethod(Method method, int child, const QVariantList &params);

protected:
    ~QAccessibleWidget();
    QWidget *widget() const;
    QObject *parentObject() const;

    void addControllingSignal(const QString &signal);
    void setValue(const QString &value);
    void setDescription(const QString &desc);
    void setHelp(const QString &help);
    void setAccelerator(const QString &accel);

private:
    QAccessibleWidgetPrivate *d;
    Q_DISABLE_COPY(QAccessibleWidget)
};
typedef QAccessibleWidget QAccessibleWidgetEx;

#endif // QT_NO_ACCESSIBILITY

QT_END_NAMESPACE


#endif // QACCESSIBLEWIDGET_H
