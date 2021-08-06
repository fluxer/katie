/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the tools applications of the Katie Toolkit.
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

#ifndef GRADIENTVIEW_H
#define GRADIENTVIEW_H

#include <QtGui/QWidget>
#include <QtCore/QMap>
#include "ui_qtgradientview.h"

QT_BEGIN_NAMESPACE

class QtGradientManager;
class QListViewItem;
class QAction;

class QtGradientView : public QWidget
{
    Q_OBJECT
public:
    QtGradientView(QWidget *parent = nullptr);

    void setGradientManager(QtGradientManager *manager);
    QtGradientManager *gradientManager() const;

    void setCurrentGradient(const QString &id);
    QString currentGradient() const;

signals:
    void currentGradientChanged(const QString &id);
    void gradientActivated(const QString &id);

private slots:
    void slotGradientAdded(const QString &id, const QGradient &gradient);
    void slotGradientRenamed(const QString &id, const QString &newId);
    void slotGradientChanged(const QString &id, const QGradient &newGradient);
    void slotGradientRemoved(const QString &id);
    void slotNewGradient();
    void slotEditGradient();
    void slotRemoveGradient();
    void slotRenameGradient();
    void slotRenameGradient(QListWidgetItem *item);
    void slotCurrentItemChanged(QListWidgetItem *item);
    void slotGradientActivated(QListWidgetItem *item);

private:
    QMap<QString, QListWidgetItem *> m_idToItem;
    QMap<QListWidgetItem *, QString> m_itemToId;

    QAction *m_newAction;
    QAction *m_editAction;
    QAction *m_renameAction;
    QAction *m_removeAction;

    QtGradientManager *m_manager;
    Ui_QtGradientView m_ui;
};

QT_END_NAMESPACE

#endif
