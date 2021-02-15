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

#ifndef PROMOTIONMODEL_H
#define PROMOTIONMODEL_H

#include <QtGui/QStandardItemModel>
#include <QtCore/QSet>

QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;
class QDesignerWidgetDataBaseItemInterface;

namespace qdesigner_internal {

    // Item model representing the promoted widgets.
    class PromotionModel : public QStandardItemModel {
        Q_OBJECT

    public:
        explicit PromotionModel(QDesignerFormEditorInterface *core);

        void updateFromWidgetDatabase();

        // Return item at model index or 0.
        QDesignerWidgetDataBaseItemInterface *databaseItemAt(const QModelIndex &, bool *referenced) const;

        QModelIndex indexOfClass(const QString &className) const;

   signals:
        void includeFileChanged(QDesignerWidgetDataBaseItemInterface *, const QString &includeFile);
        void classNameChanged(QDesignerWidgetDataBaseItemInterface *, const QString &newName);

    private slots:
        void slotItemChanged(QStandardItem * item);

    private:
        void initializeHeaders();
        // Retrieve data base item of item or return 0.
        QDesignerWidgetDataBaseItemInterface *databaseItem(const QStandardItem * item, bool *referenced) const;

        QDesignerFormEditorInterface *m_core;
    };
} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // PROMOTIONMODEL_H
