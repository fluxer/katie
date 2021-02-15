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

#ifndef FORMLAYOUTMENU
#define FORMLAYOUTMENU

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

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QPointer>

QT_BEGIN_NAMESPACE

class QDesignerFormWindowInterface;

class QAction;
class QWidget;

namespace qdesigner_internal {

// Task menu to be used for form layouts. Offers an options "Add row" which
// pops up a dialog in which the user can specify label name, text and buddy.
class Q_DESIGNER_EXPORT FormLayoutMenu : public QObject
{
    Q_DISABLE_COPY(FormLayoutMenu)
    Q_OBJECT
public:
    typedef QList<QAction *> ActionList;

    explicit FormLayoutMenu(QObject *parent);

    // Populate a list of actions with the form layout actions.
    void populate(QWidget *w, QDesignerFormWindowInterface *fw, ActionList &actions);
    // For implementing QDesignerTaskMenuExtension::preferredEditAction():
    // Return appropriate action for double clicking.
    QAction *preferredEditAction(QWidget *w, QDesignerFormWindowInterface *fw);

private slots:
    void slotAddRow();

private:
    QAction *m_separator1;
    QAction *m_populateFormAction;
    QAction *m_separator2;
    QPointer<QWidget> m_widget;
};
}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // FORMLAYOUTMENU
