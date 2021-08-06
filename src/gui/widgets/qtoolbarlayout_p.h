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

#ifndef QTOOLBARLAYOUT_P_H
#define QTOOLBARLAYOUT_P_H

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

#include <QtGui/qlayout.h>
#include "qlayoutengine_p.h"
#include <QVector>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_TOOLBAR

class QAction;
class QToolBarExtension;
class QMenu;

class QToolBarItem : public QWidgetItem
{
public:
    QToolBarItem(QWidget *widget);
    bool isEmpty() const;

    QAction *action;
    bool customWidget;
};

class QToolBarLayout : public QLayout
{
    Q_OBJECT

public:
    QToolBarLayout(QWidget *parent = nullptr);
    ~QToolBarLayout();

    void addItem(QLayoutItem *item);
    QLayoutItem *itemAt(int index) const;
    QLayoutItem *takeAt(int index);
    int count() const;

    bool isEmpty() const;
    void invalidate();
    Qt::Orientations expandingDirections() const;

    void setGeometry(const QRect &r);
    QSize minimumSize() const;
    QSize sizeHint() const;

    void insertAction(int index, QAction *action);
    int indexOf(QAction *action) const;
    int indexOf(QWidget *widget) const { return QLayout::indexOf(widget); }

    bool layoutActions(const QSize &size);
    QSize expandedSize(const QSize &size) const;
    bool expanded, animating;

    void setUsePopupMenu(bool set); // Yeah, there's no getter, but it's internal.
    void checkUsePopupMenu();

    bool movable() const;
    void updateMarginAndSpacing();
    bool hasExpandFlag() const;

public Q_SLOTS:
    void setExpanded(bool b);

private:
    QList<QToolBarItem*> items;
    QSize hint, minSize;
    bool dirty, expanding, empty, expandFlag;
    QVector<QLayoutStruct> geomArray;
    QRect handRect;
    QToolBarExtension *extension;

    void updateGeomArray() const;
    QToolBarItem *createItem(QAction *action);
    QMenu *popupMenu;
};

#endif // QT_NO_TOOLBAR

QT_END_NAMESPACE

#endif // QTOOLBARLAYOUT_P_H
