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

#ifndef QLAYOUT_P_H
#define QLAYOUT_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of qlayout*.cpp, and qabstractlayout.cpp.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#include "qobject_p.h"
#include "qstyle.h"
#include "qsizepolicy.h"

QT_BEGIN_NAMESPACE

class QWidgetItem;
class QSpacerItem;

class Q_GUI_EXPORT QLayoutPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QLayout)

public:
    typedef QWidgetItem * (*QWidgetItemFactoryMethod)(const QLayout *layout, QWidget *widget);
    typedef QSpacerItem * (*QSpacerItemFactoryMethod)(const QLayout *layout, int w, int h, QSizePolicy::Policy hPolicy, QSizePolicy::Policy);

    QLayoutPrivate();

    void getMargin(int *result, int userMargin, QStyle::PixelMetric pm) const;
    void doResize(const QSize &);
    void reparentChildWidgets(QWidget *mw);

    static QWidgetItem *createWidgetItem(const QLayout *layout, QWidget *widget);
    static QSpacerItem *createSpacerItem(const QLayout *layout, int w, int h, QSizePolicy::Policy hPolicy = QSizePolicy::Minimum, QSizePolicy::Policy vPolicy = QSizePolicy::Minimum);

    static QWidgetItemFactoryMethod widgetItemFactoryMethod;
    static QSpacerItemFactoryMethod spacerItemFactoryMethod;

    int insideSpacing;
    int userLeftMargin;
    int userTopMargin;
    int userRightMargin;
    int userBottomMargin;
    bool topLevel;
    bool enabled;
    bool activated;
    QLayout::SizeConstraint constraint;
    QRect rect;
    QWidget *menubar;
};

QT_END_NAMESPACE

#endif // QLAYOUT_P_H
