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

#ifndef QWIZARD_CONTAINER_H
#define QWIZARD_CONTAINER_H

#include <QtDesigner/container.h>

#include "qdesigner_propertysheet_p.h"
#include <extensionfactory_p.h>

#include <QtGui/QWizard>
#include <QtGui/qwizard.h>

QT_BEGIN_NAMESPACE

class QWizardPage;

namespace qdesigner_internal {

// Container for QWizard. Care must be taken to position
// the  QWizard at some valid page after removal/insertion
// as it is not used to having its pages ripped out.
class QWizardContainer: public QObject, public QDesignerContainerExtension
{
    Q_OBJECT
    Q_INTERFACES(QDesignerContainerExtension)
public:
    explicit QWizardContainer(QWizard *widget, QObject *parent = nullptr);

    virtual int count() const;
    virtual QWidget *widget(int index) const;
    virtual int currentIndex() const;
    virtual void setCurrentIndex(int index);
    virtual void addWidget(QWidget *widget);
    virtual void insertWidget(int index, QWidget *widget);
    virtual void remove(int index);

private:
    QWizard *m_wizard;
};

// QWizardPagePropertySheet: Introduces a attribute string fake property
// "pageId" that allows for specifying enumeration values (uic only).
// This breaks the pattern of having a "currentSth" property for the
// container, but was deemed to make sense here since the Page has
// its own "title" properties.
class QWizardPagePropertySheet: public QDesignerPropertySheet
{
    Q_OBJECT
public:
    explicit QWizardPagePropertySheet(QWizardPage *object, QObject *parent = nullptr);

    virtual bool reset(int index);

    static const char *pageIdProperty;

private:
    const int m_pageIdIndex;
};

// QWizardPropertySheet: Hides the "startId" property. It cannot be used
// as QWizard cannot handle setting it as a property before the actual
// page is added.

class QWizardPropertySheet: public QDesignerPropertySheet
{
    Q_OBJECT
public:
    explicit QWizardPropertySheet(QWizard *object, QObject *parent = nullptr);
    virtual bool isVisible(int index) const;

private:
    const QString m_startId;
};

// Factories
typedef QDesignerPropertySheetFactory<QWizard, QWizardPropertySheet>  QWizardPropertySheetFactory;
typedef QDesignerPropertySheetFactory<QWizardPage, QWizardPagePropertySheet>  QWizardPagePropertySheetFactory;
typedef ExtensionFactory<QDesignerContainerExtension,  QWizard,  QWizardContainer> QWizardContainerFactory;
}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // QWIZARD_CONTAINER_H
