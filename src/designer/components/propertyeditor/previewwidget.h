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

#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include "ui_previewwidget.h"

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

class PreviewWidget: public QWidget
{
    Q_OBJECT
public:
    explicit PreviewWidget(QWidget *parent);
    virtual ~PreviewWidget();

private:
    Ui_PreviewWidget ui;
};

}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // PREVIEWWIDGET_H
