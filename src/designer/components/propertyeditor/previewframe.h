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

#ifndef PREVIEWFRAME_H
#define PREVIEWFRAME_H

#include <QtGui/QFrame>
#include <QtCore/QPointer>

QT_BEGIN_NAMESPACE

class QMdiArea;
class QMdiSubWindow;

namespace qdesigner_internal {

class PreviewFrame: public QFrame
{
    Q_OBJECT
public:
    explicit PreviewFrame(QWidget *parent);

    void setPreviewPalette(const QPalette &palette);
    void setSubWindowActive(bool active);
    
private:
    // The user can on some platforms close the mdi child by invoking the system menu.
    // Ensure a child is present.
    QMdiSubWindow *ensureMdiSubWindow();
    QMdiArea *m_mdiArea;
    QPointer<QMdiSubWindow> m_mdiSubWindow;
};

}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif
