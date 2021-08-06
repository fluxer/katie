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

#ifndef QDESIGNERUNDOSTACK_H
#define QDESIGNERUNDOSTACK_H

#include <QtCore/QObject>

QT_BEGIN_NAMESPACE
class QUndoStack;
class QUndoCommand;

namespace qdesigner_internal {

/* QDesignerUndoStack: A QUndoStack extended by a way of setting it to
 * "dirty" indepently of commands (by modifications without commands
 * such as resizing). Accomplished via bool m_fakeDirty flag. The
 * lifecycle of the QUndoStack is managed by the QUndoGroup. */
class QDesignerUndoStack : public QObject
{
    Q_DISABLE_COPY(QDesignerUndoStack)
    Q_OBJECT
public:
    explicit QDesignerUndoStack(QObject *parent = nullptr);
    virtual ~QDesignerUndoStack();

    void push(QUndoCommand * cmd);
    void beginMacro(const QString &text);
    void endMacro();
    int  index() const;

    const QUndoStack *qundoStack() const;
    QUndoStack *qundoStack();

    bool isDirty() const;

signals:
    void changed();

public slots:
    void setDirty(bool);

private:
    QUndoStack *m_undoStack;
    bool m_fakeDirty;
};

} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // QDESIGNERUNDOSTACK_H
