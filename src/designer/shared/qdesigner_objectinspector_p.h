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

#ifndef DESIGNEROBJECTINSPECTOR_H
#define DESIGNEROBJECTINSPECTOR_H

#include <QtDesigner/abstractobjectinspector.h>
#include <QtCore/QList>

QT_BEGIN_NAMESPACE

class QDesignerDnDItemInterface;

namespace qdesigner_internal {

struct Q_DESIGNER_EXPORT Selection {
    bool empty() const;
    void clear();

    // Merge all lists
    QObjectList selection() const;

    // Selection in cursor (managed widgets)
    QWidgetList managed;
    // Unmanaged widgets
    QWidgetList unmanaged;
    // Remaining selected objects (non-widgets)
    QObjectList objects;
};

// Extends the QDesignerObjectInspectorInterface by functionality
// to access the selection

class Q_DESIGNER_EXPORT QDesignerObjectInspector: public QDesignerObjectInspectorInterface
{
    Q_OBJECT
public:
    explicit QDesignerObjectInspector(QWidget *parent = nullptr, Qt::WindowFlags flags = 0);

    // Select a qobject unmanaged by form window
    virtual bool selectObject(QObject *o) = 0;
    virtual void getSelection(Selection &s) const = 0;
    virtual void clearSelection() = 0;

public slots:
    virtual void mainContainerChanged();
};

}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // DESIGNEROBJECTINSPECTOR_H
