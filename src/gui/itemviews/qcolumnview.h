/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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

#ifndef QCOLUMNVIEW_H
#define QCOLUMNVIEW_H

#include <QtGui/qabstractitemview.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_COLUMNVIEW

class QColumnViewPrivate;

class Q_GUI_EXPORT QColumnView : public QAbstractItemView {

Q_OBJECT
    Q_PROPERTY(bool resizeGripsVisible READ resizeGripsVisible WRITE setResizeGripsVisible)

Q_SIGNALS:
    void updatePreviewWidget(const QModelIndex &index);

public:
    explicit QColumnView(QWidget *parent = nullptr);
    ~QColumnView();

    // QAbstractItemView overloads
    QModelIndex indexAt(const QPoint &point) const;
    void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
    QSize sizeHint() const;
    QRect visualRect(const QModelIndex &index) const;
    void setModel(QAbstractItemModel *model);
    void setSelectionModel(QItemSelectionModel * selectionModel);
    void setRootIndex(const QModelIndex &index);
    void selectAll();

    // QColumnView functions
    void setResizeGripsVisible(bool visible);
    bool resizeGripsVisible() const;

    QWidget *previewWidget() const;
    void setPreviewWidget(QWidget *widget);

    void setColumnWidths(const QList<int> &list);
    QList<int> columnWidths() const;

protected:
    QColumnView(QColumnViewPrivate &dd, QWidget *parent = nullptr);

    // QAbstractItemView overloads
    bool isIndexHidden(const QModelIndex &index) const;
    QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);
    void resizeEvent(QResizeEvent *event);
    void setSelection(const QRect & rect, QItemSelectionModel::SelectionFlags command);
    QRegion visualRegionForSelection(const QItemSelection &selection) const;
    int horizontalOffset() const;
    int verticalOffset() const;
    void rowsInserted(const QModelIndex &parent, int start, int end);
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);

    // QColumnView functions
    void scrollContentsBy(int dx, int dy);
    virtual QAbstractItemView* createColumn(const QModelIndex &rootIndex);
    void initializeColumn(QAbstractItemView *column) const;

private:
    Q_DECLARE_PRIVATE(QColumnView)
    Q_DISABLE_COPY(QColumnView)
    Q_PRIVATE_SLOT(d_func(), void _q_gripMoved(int))
    Q_PRIVATE_SLOT(d_func(), void _q_changeCurrentColumn())
    Q_PRIVATE_SLOT(d_func(), void _q_clicked(const QModelIndex &))
};

#endif // QT_NO_COLUMNVIEW

QT_END_NAMESPACE


#endif // QCOLUMNVIEW_H

