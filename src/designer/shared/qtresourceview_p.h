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
// This file is not part of the Katie API.  It exists for the convenience
// of Katie Designer.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef QTRESOURCEVIEW_H
#define QTRESOURCEVIEW_H

#include <QtGui/QWidget>
#include <QtGui/QDialog>

QT_BEGIN_NAMESPACE

class QtResourceModel;
class QtResourceSet;
class QDesignerFormEditorInterface;
class QMimeData;

class Q_DESIGNER_EXPORT QtResourceView : public QWidget
{
    Q_OBJECT
public:
    explicit QtResourceView(QDesignerFormEditorInterface *core, QWidget *parent = nullptr);
    ~QtResourceView();

    void setDragEnabled(bool dragEnabled);
    bool dragEnabled() const;

    QtResourceModel *model() const;
    void setResourceModel(QtResourceModel *model);

    QString selectedResource() const;
    void selectResource(const QString &resource);

    QString settingsKey() const;
    void setSettingsKey(const QString &key);

    bool isResourceEditingEnabled() const;
    void setResourceEditingEnabled(bool enable);

    // Helpers for handling the drag originating in QtResourceView (Xml/text)
    enum ResourceType { ResourceImage, ResourceStyleSheet, ResourceOther };
    static QString encodeMimeData(ResourceType resourceType, const QString &path);

    static bool decodeMimeData(const QMimeData *md, ResourceType *t = 0, QString *file = 0);
    static bool decodeMimeData(const QString &text, ResourceType *t = 0, QString *file = 0);

signals:
    void resourceSelected(const QString &resource);
    void resourceActivated(const QString &resource);

protected:
    bool event(QEvent *event);

private:

    QScopedPointer<class QtResourceViewPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtResourceView)
    Q_DISABLE_COPY(QtResourceView)
    Q_PRIVATE_SLOT(d_func(), void slotResourceSetActivated(QtResourceSet *))
    Q_PRIVATE_SLOT(d_func(), void slotCurrentPathChanged(QTreeWidgetItem *))
    Q_PRIVATE_SLOT(d_func(), void slotCurrentResourceChanged(QListWidgetItem *))
    Q_PRIVATE_SLOT(d_func(), void slotResourceActivated(QListWidgetItem *))
    Q_PRIVATE_SLOT(d_func(), void slotEditResources())
    Q_PRIVATE_SLOT(d_func(), void slotReloadResources())
    Q_PRIVATE_SLOT(d_func(), void slotCopyResourcePath())
    Q_PRIVATE_SLOT(d_func(), void slotListWidgetContextMenuRequested(const QPoint &pos))
    Q_PRIVATE_SLOT(d_func(), void slotFilterChanged(const QString &pattern))
};

class Q_DESIGNER_EXPORT  QtResourceViewDialog : public QDialog
{
    Q_OBJECT
public:
    explicit QtResourceViewDialog(QDesignerFormEditorInterface *core, QWidget *parent = nullptr);
    virtual ~QtResourceViewDialog();

    QString selectedResource() const;
    void selectResource(const QString &path);

    bool isResourceEditingEnabled() const;
    void setResourceEditingEnabled(bool enable);

private:
    QScopedPointer<class QtResourceViewDialogPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtResourceViewDialog)
    Q_DISABLE_COPY(QtResourceViewDialog)
    Q_PRIVATE_SLOT(d_func(), void slotResourceSelected(const QString &))
};

QT_END_NAMESPACE

#endif
