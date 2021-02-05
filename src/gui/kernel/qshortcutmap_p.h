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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QSHORTCUTMAP_P_H
#define QSHORTCUTMAP_P_H

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

#include "QtGui/qkeysequence.h"
#include "QtCore/qvector.h"
#include "QtCore/qscopedpointer.h"

QT_BEGIN_NAMESPACE

#ifndef QT_NO_SHORTCUT

// To enable dump output uncomment below
//#define Dump_QShortcutMap

class QKeyEvent;
struct QShortcutEntry;
class QShortcutMapPrivate;
class QGraphicsWidget;
class QWidget;
class QAction;
class QObject;

class QShortcutMap
{
    Q_DECLARE_PRIVATE(QShortcutMap)
public:
    QShortcutMap();
    ~QShortcutMap();

    int addShortcut(QObject *owner, const QKeySequence &key, Qt::ShortcutContext context);
    int removeShortcut(int id, QObject *owner);
    int setShortcutEnabled(bool enable, int id, QObject *owner);
    int setShortcutAutoRepeat(bool on, int id, QObject *owner);

    void resetState();
    QKeySequence::SequenceMatch nextState(QKeyEvent *e);
    void dispatchEvent(QKeyEvent *e);
    bool tryShortcutEvent(QObject *o, QKeyEvent *e);

#ifdef Dump_QShortcutMap
    void dumpMap() const;
#endif

    bool hasShortcutForKeySequence(const QKeySequence &seq) const;


private:
    bool correctWidgetContext(Qt::ShortcutContext context, QWidget *w, QWidget *active_window) const;
#ifndef QT_NO_GRAPHICSVIEW
    bool correctGraphicsWidgetContext(Qt::ShortcutContext context, QGraphicsWidget *w, QWidget *active_window) const;
#endif
#ifndef QT_NO_ACTION
    bool correctContext(Qt::ShortcutContext context,QAction *a, QWidget *active_window) const;
#endif
    QScopedPointer<QShortcutMapPrivate> d_ptr;

    QKeySequence::SequenceMatch find(QKeyEvent *e);
    QKeySequence::SequenceMatch matches(const QKeySequence &seq1, const QKeySequence &seq2) const;
    void createNewSequences(QKeyEvent *e, QVector<QKeySequence> &ksl);
    void clearSequence(QVector<QKeySequence> &ksl);
    bool correctContext(const QShortcutEntry &item) const;
};

#endif // QT_NO_SHORTCUT

QT_END_NAMESPACE

#endif // QSHORTCUTMAP_P_H
