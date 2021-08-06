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

#ifndef QDESIGNER_FORMWINDOMANAGER_H
#define QDESIGNER_FORMWINDOMANAGER_H

#include <QtDesigner/abstractformwindowmanager.h>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

class PreviewManager;

//
// Convenience methods to manage form previews (ultimately forwarded to PreviewManager).
//
class Q_DESIGNER_EXPORT QDesignerFormWindowManager
    : public QDesignerFormWindowManagerInterface
{
    Q_OBJECT
public:
    explicit QDesignerFormWindowManager(QObject *parent = nullptr);
    virtual ~QDesignerFormWindowManager();

    virtual QAction *actionDefaultPreview() const;
    virtual QActionGroup *actionGroupPreviewInStyle() const;
    virtual QAction *actionShowFormWindowSettingsDialog() const;

    virtual QPixmap createPreviewPixmap(QString *errorMessage) = 0;

    virtual PreviewManager *previewManager() const = 0;

Q_SIGNALS:
    void formWindowSettingsChanged(QDesignerFormWindowInterface *fw);

public Q_SLOTS:
    virtual void closeAllPreviews() = 0;
    void aboutPlugins();
};

} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // QDESIGNER_FORMWINDOMANAGER_H
