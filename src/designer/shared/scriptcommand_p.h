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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of Katie Designer.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef QDESIGNER_SCRIPTCOMMAND_H
#define QDESIGNER_SCRIPTCOMMAND_H

#include "qdesigner_formwindowcommand_p.h"

#include <QtCore/QPair>
#include <QtCore/QList>
#include <QtCore/QPointer>

QT_BEGIN_NAMESPACE

class QDesignerFormWindowInterface;

namespace qdesigner_internal {

class Q_DESIGNER_EXPORT ScriptCommand: public QDesignerFormWindowCommand
{
    ScriptCommand(const ScriptCommand &);
    ScriptCommand& operator=(const ScriptCommand &);

public:
    explicit ScriptCommand(QDesignerFormWindowInterface *formWindow);

    typedef QList<QObject *> ObjectList;
    bool init(const ObjectList &list, const QString &script);

    virtual void redo();
    virtual void undo();

private:
    typedef QPair<QPointer<QObject>, QString> ObjectScriptPair;
    typedef QList<ObjectScriptPair> ObjectScriptList;
    ObjectScriptList m_oldValues;
    QString m_script;
};

} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // QDESIGNER_SCRIPTCOMMAND_H
