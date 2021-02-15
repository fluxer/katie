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

#ifndef GRADIENTUTILS_H
#define GRADIENTUTILS_H

#include <QtGui/qbrush.h>
#include <QtGui/QPainter>

QT_BEGIN_NAMESPACE

class QtGradientManager;

class QtGradientUtils
{
public:
    static QString styleSheetCode(const QGradient &gradient);
    // utils methods, they could be outside of this class
    static QString saveState(const QtGradientManager *manager);
    static void restoreState(QtGradientManager *manager, const QString &state);

    static QPixmap gradientPixmap(const QGradient &gradient, const QSize &size = QSize(64, 64), bool checkeredBackground = false);

};

QT_END_NAMESPACE

#endif
