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

#ifndef PREVIEWACTIONGROUP_H
#define PREVIEWACTIONGROUP_H

#include <QtGui/QActionGroup>

QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;

namespace qdesigner_internal {

/* PreviewActionGroup: To be used as a submenu for 'Preview in...'
 * Offers a menu of styles and device profiles. */

class PreviewActionGroup : public QActionGroup
{
    Q_DISABLE_COPY(PreviewActionGroup)
    Q_OBJECT
public:
    explicit PreviewActionGroup(QDesignerFormEditorInterface *core, QObject *parent = nullptr);

signals:
    void preview(const QString &style, int deviceProfileIndex);

public slots:
    void updateDeviceProfiles();

private  slots:
    void slotTriggered(QAction *);

private:
    QDesignerFormEditorInterface *m_core;
};
}

QT_END_NAMESPACE

#endif // PREVIEWACTIONGROUP_H
