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

#ifndef QDESIGNER_TASKMENU_H
#define QDESIGNER_TASKMENU_H

#include "extensionfactory_p.h"
#include <QtDesigner/taskmenu.h>

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QList>

QT_BEGIN_NAMESPACE

class QDesignerFormWindowInterface;
class QDesignerFormEditorInterface;

class QWidget;
class QSignalMapper;

namespace qdesigner_internal {
class QDesignerTaskMenuPrivate;

class Q_DESIGNER_EXPORT QDesignerTaskMenu: public QObject, public QDesignerTaskMenuExtension
{
    Q_OBJECT
    Q_INTERFACES(QDesignerTaskMenuExtension)
public:
    QDesignerTaskMenu(QWidget *widget, QObject *parent);
    virtual ~QDesignerTaskMenu();

    QWidget *widget() const;

    virtual QList<QAction*> taskActions() const;

    enum PropertyMode { CurrentWidgetMode, MultiSelectionMode };

    static bool isSlotNavigationEnabled(const QDesignerFormEditorInterface *core);
    static void navigateToSlot(QDesignerFormEditorInterface *core, QObject *o,
                               const QString &defaultSignal = QString());

protected:

    QDesignerFormWindowInterface *formWindow() const;
    void changeTextProperty(const QString &propertyName, const QString &windowTitle, PropertyMode pm, Qt::TextFormat desiredFormat);

    QAction *createSeparator();

    /* Retrieve the list of objects the task menu is supposed to act on. Note that a task menu can be invoked for
     * an unmanaged widget [as of 4.5], in which case it must not use the cursor selection,
     * but the unmanaged selection of the object inspector. */
    QObjectList applicableObjects(const QDesignerFormWindowInterface *fw, PropertyMode pm) const;
    QList<QWidget *> applicableWidgets(const QDesignerFormWindowInterface *fw, PropertyMode pm) const;

    void setProperty(QDesignerFormWindowInterface *fw, PropertyMode pm, const QString &name, const QVariant &newValue);

private slots:
    void changeObjectName();
    void changeToolTip();
    void changeWhatsThis();
    void changeStyleSheet();
    void createMenuBar();
    void addToolBar();
    void createStatusBar();
    void removeStatusBar();
    void changeScript();
    void containerFakeMethods();
    void slotNavigateToSlot();
    void applySize(QAction *a);
    void slotLayoutAlignment();

private:
    QDesignerTaskMenuPrivate *d;
};

typedef ExtensionFactory<QDesignerTaskMenuExtension, QWidget, QDesignerTaskMenu>  QDesignerTaskMenuFactory;

} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // QDESIGNER_TASKMENU_H
