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

#ifndef QACTION_P_H
#define QACTION_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "QtGui/qaction.h"
#include "QtGui/qmenu.h"
#include "qgraphicswidget_p.h"
#include "qobject_p.h"

QT_BEGIN_NAMESPACE

#ifndef QT_NO_ACTION


class QShortcutMap;

class Q_AUTOTEST_EXPORT QActionPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QAction)
public:
    QActionPrivate();
    ~QActionPrivate();

    bool showStatusText(QWidget *w, const QString &str);

    QPointer<QActionGroup> group;
    QString text;
    QString iconText;
    QIcon icon;
    QString tooltip;
    QString statustip;
    QString whatsthis;
#ifndef QT_NO_SHORTCUT
    QKeySequence shortcut;
    QList<QKeySequence> alternateShortcuts;
#endif
    QVariant userData;
#ifndef QT_NO_SHORTCUT
    int shortcutId;
    QList<int> alternateShortcutIds;
    Qt::ShortcutContext shortcutContext;
    bool autorepeat;
#endif
    bool enabled;
    bool visible;
    bool checkable;
    bool checked;
    bool separator;

    QFont font;
    QPointer<QMenu> menu;
    QAction::MenuRole menuRole;
    QAction::Priority priority;

    QList<QWidget *> widgets;
#ifndef QT_NO_GRAPHICSVIEW
    QList<QGraphicsWidget *> graphicsWidgets;
#endif
#ifndef QT_NO_SHORTCUT
    void redoGrab(QShortcutMap &map);
    void redoGrabAlternate(QShortcutMap &map);
    void setShortcutEnabled(bool enable, QShortcutMap &map);
#endif // QT_NO_SHORTCUT

    void sendDataChanged();
};

#endif // QT_NO_ACTION

QT_END_NAMESPACE

#endif // QACTION_P_H
