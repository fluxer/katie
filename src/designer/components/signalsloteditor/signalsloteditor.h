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

#ifndef SIGNALSLOTEDITOR_H
#define SIGNALSLOTEDITOR_H

#include "signalsloteditor_global.h"
#include "connectionedit_p.h"

#include <QtXml/qdom.h>

QT_BEGIN_NAMESPACE

class DomConnections;

namespace qdesigner_internal {

class SignalSlotConnection;

class QT_SIGNALSLOTEDITOR_EXPORT SignalSlotEditor : public ConnectionEdit
{
    Q_OBJECT

public:
    SignalSlotEditor(QDesignerFormWindowInterface *form_window, QWidget *parent);

    virtual void setSignal(SignalSlotConnection *con, const QString &member);
    virtual void setSlot(SignalSlotConnection *con, const QString &member);
    virtual void setSource(Connection *con, const QString &obj_name);
    virtual void setTarget(Connection *con, const QString &obj_name);

    DomConnections *toUi() const;
    void fromUi(const DomConnections *connections, QWidget *parent);

    QDesignerFormWindowInterface *formWindow() const { return m_form_window; }

    QObject *objectByName(QWidget *topLevel, const QString &name) const;

    void addEmptyConnection();

protected:
    virtual QWidget *widgetAt(const QPoint &pos) const;

private:
    virtual Connection *createConnection(QWidget *source, QWidget *destination);
    virtual void modifyConnection(Connection *con);

    QDesignerFormWindowInterface *m_form_window;
    bool m_showAllSignalsSlots;

    friend class SetMemberCommand;
};

} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // SIGNALSLOTEDITOR_H
