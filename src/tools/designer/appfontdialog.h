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

#ifndef QDESIGNER_APPFONTWIDGET_H
#define QDESIGNER_APPFONTWIDGET_H

#include <QtGui/QGroupBox>
#include <QtGui/QDialog>

QT_BEGIN_NAMESPACE

class AppFontModel;

class QTreeView;
class QToolButton;
class QItemSelection;
class QDesignerSettingsInterface;

// AppFontWidget: Manages application fonts which the user can load and 
// provides API for saving/restoring them.

class AppFontWidget : public QGroupBox
{
    Q_DISABLE_COPY(AppFontWidget)
    Q_OBJECT
public:
    explicit AppFontWidget(QWidget *parent = nullptr);

    QStringList fontFiles() const;

    static void save(QDesignerSettingsInterface *s, const QString &prefix);
    static void restore(const QDesignerSettingsInterface *s, const QString &prefix);

private slots:
    void addFiles();
    void slotRemoveFiles();
    void slotRemoveAll();
    void selectionChanged(const QItemSelection & selected, const QItemSelection & deselected);

private:
    QTreeView *m_view;
    QToolButton *m_addButton;
    QToolButton *m_removeButton;
    QToolButton *m_removeAllButton;
    AppFontModel *m_model;
};

// AppFontDialog: Non modal dialog for AppFontWidget which has Qt::WA_DeleteOnClose set.

class AppFontDialog : public QDialog
{
    Q_DISABLE_COPY(AppFontDialog)
    Q_OBJECT
public:
    explicit AppFontDialog(QWidget *parent = nullptr);

private:
    AppFontWidget *m_appFontWidget;
};

QT_END_NAMESPACE

#endif // QDESIGNER_APPFONTWIDGET_H
