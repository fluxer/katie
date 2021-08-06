/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtSCriptTools module of the Katie Toolkit.
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

#ifndef QSCRIPTBREAKPOINTSMODEL_P_H
#define QSCRIPTBREAKPOINTSMODEL_P_H

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

#include <QtCore/qabstractitemmodel.h>

#include "qscriptbreakpointdata_p.h"

QT_BEGIN_NAMESPACE

class QScriptDebuggerJobSchedulerInterface;
class QScriptDebuggerCommandSchedulerInterface;

class QScriptBreakpointsModelPrivate;
class Q_AUTOTEST_EXPORT QScriptBreakpointsModel
    : public QAbstractItemModel
{
    Q_OBJECT
public:
    QScriptBreakpointsModel(QScriptDebuggerJobSchedulerInterface *jobScheduler,
                            QScriptDebuggerCommandSchedulerInterface *commandScheduler,
                            QObject *parent = nullptr);
    ~QScriptBreakpointsModel();

    void setBreakpoint(const QScriptBreakpointData &data);
    void setBreakpointData(int id, const QScriptBreakpointData &data);
    void deleteBreakpoint(int id);

    void addBreakpoint(int id, const QScriptBreakpointData &data);
    void modifyBreakpoint(int id, const QScriptBreakpointData &data);
    void removeBreakpoint(int id);

    int breakpointIdAt(int row) const;
    QScriptBreakpointData breakpointDataAt(int row) const;
    QScriptBreakpointData breakpointData(int id) const;

    int resolveBreakpoint(qint64 scriptId, int lineNumber) const;
    int resolveBreakpoint(const QString &fileName, int lineNumber) const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    Q_DECLARE_PRIVATE(QScriptBreakpointsModel)
    Q_DISABLE_COPY(QScriptBreakpointsModel)
};

QT_END_NAMESPACE

#endif
