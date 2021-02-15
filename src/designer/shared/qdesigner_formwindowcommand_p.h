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

#ifndef QDESIGNER_FORMWINDOWCOMMAND_H
#define QDESIGNER_FORMWINDOWCOMMAND_H

#include <QtCore/QPointer>
#include <QtGui/qundostack.h>

QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;
class QDesignerFormWindowInterface;
class QDesignerPropertySheetExtension;

namespace qdesigner_internal {

class Q_DESIGNER_EXPORT QDesignerFormWindowCommand: public QUndoCommand
{

public:
    QDesignerFormWindowCommand(const QString &description,
                               QDesignerFormWindowInterface *formWindow,
                               QUndoCommand *parent = 0);

    virtual void undo();
    virtual void redo();

    static void updateBuddies(QDesignerFormWindowInterface *form,
                              const QString &old_name, const QString &new_name);
protected:
    QDesignerFormWindowInterface *formWindow() const;
    QDesignerFormEditorInterface *core() const;
    QDesignerPropertySheetExtension* propertySheet(QObject *object) const;

    void cheapUpdate();

    void selectUnmanagedObject(QObject *unmanagedObject);
private:
    QPointer<QDesignerFormWindowInterface> m_formWindow;
};

} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // QDESIGNER_COMMAND_H
