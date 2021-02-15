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

#ifndef LISTWIDGETEDITOR_H
#define LISTWIDGETEDITOR_H

#include "itemlisteditor.h"
#include "qdesigner_command_p.h"

#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class QListWidget;
class QComboBox;
class QDesignerFormWindowInterface;

namespace qdesigner_internal {

class ListWidgetEditor: public QDialog
{
    Q_OBJECT

public:
    ListWidgetEditor(QDesignerFormWindowInterface *form,
                     QWidget *parent);

    ListContents fillContentsFromListWidget(QListWidget *listWidget);
    ListContents fillContentsFromComboBox(QComboBox *comboBox);
    ListContents contents() const;

private:
    ItemListEditor *m_itemsEditor;
};

}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // LISTWIDGETEDITOR_H
