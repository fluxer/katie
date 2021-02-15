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

#ifndef SCRIPT_H
#define SCRIPT_H

#include <QtDesigner/extension.h>
#include <QtCore/QVariant>


QT_BEGIN_NAMESPACE

class QString;

class Q_DESIGNER_EXPORT QDesignerScriptExtension
{
public:
    virtual ~QDesignerScriptExtension();

    virtual QVariantMap data() const = 0;
    virtual void setData(const QVariantMap &data) = 0;

    virtual QString script() const = 0;

};

QT_END_NAMESPACE

Q_DECLARE_EXTENSION_INTERFACE(QDesignerScriptExtension, "Katie.Designer.Script")


#endif // SCRIPT_H
