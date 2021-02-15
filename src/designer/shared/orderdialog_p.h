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
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#ifndef ORDERDIALOG_P_H
#define ORDERDIALOG_P_H

#include <QtGui/QDialog>
#include <QtCore/QMap>

QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;

namespace qdesigner_internal {

class Ui_OrderDialog;

class Q_DESIGNER_EXPORT OrderDialog: public QDialog
{
    Q_OBJECT
public:
    OrderDialog(QWidget *parent);
    virtual ~OrderDialog();

    static QWidgetList pagesOfContainer(const QDesignerFormEditorInterface *core, QWidget *container);

    void setPageList(const QWidgetList &pages);
    QWidgetList pageList() const;

    void setDescription(const QString &d);

    enum Format {        // Display format
        PageOrderFormat, // Container pages, ranging 0..[n-1]
        TabOrderFormat   // List of widgets,  ranging 1..1
    };

    void setFormat(Format f)  { m_format = f; }
    Format format() const     { return m_format; }

private slots:
    void on_upButton_clicked();
    void on_downButton_clicked();
    void on_pageList_currentRowChanged(int row);
    void slotEnableButtonsAfterDnD();
    void slotReset();

private:
    void buildList();
    void enableButtons(int r);

    typedef QMap<int, QWidget*> OrderMap;
    OrderMap m_orderMap;
    Ui_OrderDialog* m_ui;
    Format m_format;
};

}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // ORDERDIALOG_P_H
