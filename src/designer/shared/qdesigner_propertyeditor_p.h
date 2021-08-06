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


#ifndef DESIGNERPROPERTYEDITOR_H
#define DESIGNERPROPERTYEDITOR_H

#include "shared_enums_p.h"
#include <QtDesigner/abstractpropertyeditor.h>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

// Extends the QDesignerPropertyEditorInterface by property comment handling and
// a signal for resetproperty.

class Q_DESIGNER_EXPORT QDesignerPropertyEditor: public QDesignerPropertyEditorInterface
{
    Q_OBJECT
public:
    explicit QDesignerPropertyEditor(QWidget *parent = nullptr, Qt::WindowFlags flags = 0);

    // A pair <ValidationMode, bool isTranslatable>.
    typedef QPair<TextPropertyValidationMode, bool> StringPropertyParameters;

    // Return a pair of validation mode and flag indicating whether property is translatable
    // for textual properties.
    static StringPropertyParameters textPropertyValidationMode(QDesignerFormEditorInterface *core,
                const QObject *object, const QString &propertyName, bool isMainContainer);

Q_SIGNALS:
    void propertyValueChanged(const QString &name, const QVariant &value, bool enableSubPropertyHandling);
    void resetProperty(const QString &name);
    void addDynamicProperty(const QString &name, const QVariant &value);
    void removeDynamicProperty(const QString &name);
    void editorOpened();
    void editorClosed();

public Q_SLOTS:
    /* Quick update that assumes the actual count of properties has not changed
     * (as opposed to setObject()). N/A when for example executing a
     * layout command and margin properties appear. */
    virtual void updatePropertySheet() = 0;
    virtual void reloadResourceProperties() = 0;

private Q_SLOTS:
    void slotPropertyChanged(const QString &name, const QVariant &value);

protected:
    void emitPropertyValueChanged(const QString &name, const QVariant &value, bool enableSubPropertyHandling);

private:
    bool m_propertyChangedForwardingBlocked;

};

}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // DESIGNERPROPERTYEDITOR_H
