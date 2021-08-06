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

#ifndef FORMWINDOWCURSOR_H
#define FORMWINDOWCURSOR_H

#include "formeditor_global.h"
#include "formwindow.h"
#include <QtDesigner/abstractformwindowcursor.h>

#include <QtCore/QObject>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

class QT_FORMEDITOR_EXPORT FormWindowCursor: public QObject, public QDesignerFormWindowCursorInterface
{
    Q_OBJECT
public:
    explicit FormWindowCursor(FormWindow *fw, QObject *parent = nullptr);
    virtual ~FormWindowCursor();

    virtual QDesignerFormWindowInterface *formWindow() const;

    virtual bool movePosition(MoveOperation op, MoveMode mode);

    virtual int position() const;
    virtual void setPosition(int pos, MoveMode mode);

    virtual QWidget *current() const;

    virtual int widgetCount() const;
    virtual QWidget *widget(int index) const;

    virtual bool hasSelection() const;
    virtual int selectedWidgetCount() const;
    virtual QWidget *selectedWidget(int index) const;

    virtual void setProperty(const QString &name, const QVariant &value);
    virtual void setWidgetProperty(QWidget *widget, const QString &name, const QVariant &value);
    virtual void resetWidgetProperty(QWidget *widget, const QString &name);

public slots:
    void update();

private:
    FormWindow *m_formWindow;
};

}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // FORMWINDOWCURSOR_H
