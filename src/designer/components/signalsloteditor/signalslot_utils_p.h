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

#ifndef SIGNALSLOTUTILS_P_H
#define SIGNALSLOTUTILS_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/QString>
#include <QtCore/QStringList>

QT_BEGIN_NAMESPACE

class QDesignerFormWindowInterface;
class QDesignerFormEditorInterface;

namespace qdesigner_internal {

enum MemberType { SignalMember, SlotMember };

// member to class name
QMap<QString, QString> getSignals(QDesignerFormEditorInterface *core, QObject *object, bool showAll);
QMap<QString, QString> getMatchingSlots(QDesignerFormEditorInterface *core, QObject *object,
            const QString &signalSignature, bool showAll);

bool memberFunctionListContains(QDesignerFormEditorInterface *core, QObject *object, MemberType type, const QString &signature);

// Members functions listed by class they were inherited from
struct ClassMemberFunctions
{
    ClassMemberFunctions() {}
    ClassMemberFunctions(const QString &_class_name);

    QString m_className;
    QStringList m_memberList;
};

typedef QList<ClassMemberFunctions> ClassesMemberFunctions;

// Return classes and members in reverse class order to
// populate of the combo of the ToolWindow.

ClassesMemberFunctions reverseClassesMemberFunctions(const QString &obj_name, MemberType member_type,
                                                     const QString &peer, QDesignerFormWindowInterface *form);

bool signalMatchesSlot(QDesignerFormEditorInterface *core, const QString &signal, const QString &slot);

QString realObjectName(QDesignerFormEditorInterface *core, QObject *object);

} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // SIGNALSLOTUTILS_P_H
