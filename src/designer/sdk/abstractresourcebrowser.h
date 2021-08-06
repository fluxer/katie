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

#ifndef ABSTRACTRESOURCEBROWSER_H
#define ABSTRACTRESOURCEBROWSER_H

#include <QtGui/QWidget>


QT_BEGIN_NAMESPACE

class QWidget;

class Q_DESIGNER_EXPORT QDesignerResourceBrowserInterface: public QWidget
{
    Q_OBJECT
public:
    QDesignerResourceBrowserInterface(QWidget *parent = nullptr);
    virtual ~QDesignerResourceBrowserInterface();

    virtual void setCurrentPath(const QString &filePath) = 0;
    virtual QString currentPath() const = 0;

Q_SIGNALS:
    void currentPathChanged(const QString &filePath);
    void pathActivated(const QString &filePath);
};

QT_END_NAMESPACE


#endif // ABSTRACTFORMEDITOR_H

