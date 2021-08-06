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

#ifndef METADATABASE_H
#define METADATABASE_H

#include <QtDesigner/abstractmetadatabase.h>

#include <QtCore/QHash>
#include <QtCore/QStringList>
#include <QtGui/QCursor>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

class Q_DESIGNER_EXPORT MetaDataBaseItem: public QDesignerMetaDataBaseItemInterface
{
public:
    explicit MetaDataBaseItem(QObject *object);
    virtual ~MetaDataBaseItem();

    virtual QString name() const;
    virtual void setName(const QString &name);

    typedef QList<QWidget*> TabOrder;
    virtual TabOrder tabOrder() const;
    virtual void setTabOrder(const TabOrder &tabOrder);

    virtual bool enabled() const;
    virtual void setEnabled(bool b);

    QString customClassName() const;
    void setCustomClassName(const QString &customClassName);

    QString script() const;
    void setScript(const QString &script);

    QStringList fakeSlots() const;
    void setFakeSlots(const QStringList &);

    QStringList fakeSignals() const;
    void setFakeSignals(const QStringList &);

private:
    QObject *m_object;
    TabOrder m_tabOrder;
    bool m_enabled;
    QString m_customClassName;
    QString m_script;
    QStringList m_fakeSlots;
    QStringList m_fakeSignals;
};

class Q_DESIGNER_EXPORT MetaDataBase: public QDesignerMetaDataBaseInterface
{
    Q_OBJECT
public:
    explicit MetaDataBase(QDesignerFormEditorInterface *core, QObject *parent = nullptr);
    virtual ~MetaDataBase();

    virtual QDesignerFormEditorInterface *core() const;

    virtual QDesignerMetaDataBaseItemInterface *item(QObject *object) const { return metaDataBaseItem(object); }
    virtual MetaDataBaseItem *metaDataBaseItem(QObject *object) const;
    virtual void add(QObject *object);
    virtual void remove(QObject *object);

    virtual QList<QObject*> objects() const;

private slots:
    void slotDestroyed(QObject *object);

private:
    QDesignerFormEditorInterface *m_core;
    typedef QHash<QObject *, MetaDataBaseItem*> ItemMap;
    ItemMap m_items;
};

    // promotion convenience
    Q_DESIGNER_EXPORT bool promoteWidget(QDesignerFormEditorInterface *core,QWidget *widget,const QString &customClassName);
    Q_DESIGNER_EXPORT void demoteWidget(QDesignerFormEditorInterface *core,QWidget *widget); 
    Q_DESIGNER_EXPORT bool isPromoted(QDesignerFormEditorInterface *core, QWidget* w);
    Q_DESIGNER_EXPORT QString promotedCustomClassName(QDesignerFormEditorInterface *core, QWidget* w);
    Q_DESIGNER_EXPORT QString promotedExtends(QDesignerFormEditorInterface *core, QWidget* w);

} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // METADATABASE_H
