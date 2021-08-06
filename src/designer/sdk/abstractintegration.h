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

#ifndef ABSTRACTINTEGRATION_H
#define ABSTRACTINTEGRATION_H

#include <QtCore/QObject>
#include <QtCore/QString>


QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;

class Q_DESIGNER_EXPORT QDesignerIntegrationInterface: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString headerSuffix READ headerSuffix WRITE setHeaderSuffix)
    Q_PROPERTY(bool headerLowercase READ isHeaderLowercase WRITE setHeaderLowercase)

public:
    QDesignerIntegrationInterface(QDesignerFormEditorInterface *core, QObject *parent = nullptr);

    inline QDesignerFormEditorInterface *core() const;

    virtual QWidget *containerWindow(QWidget *widget) const = 0;

    QString headerSuffix() const;
    void setHeaderSuffix(const QString &headerSuffix);

    bool isHeaderLowercase() const;
    void setHeaderLowercase(bool headerLowerCase);

private:
    QDesignerFormEditorInterface *m_core;
};

inline QDesignerFormEditorInterface *QDesignerIntegrationInterface::core() const
{ return m_core; }

QT_END_NAMESPACE


#endif // ABSTRACTINTEGRATION_H
