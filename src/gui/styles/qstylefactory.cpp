/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qstylefactory.h"
#include "qstyleplugin.h"
#include "qfactoryloader_p.h"
#include "qmutex.h"

#include "qapplication.h"
#include "qwindowsstyle.h"
#include "qmotifstyle.h"
#ifndef QT_NO_STYLE_PLASTIQUE
#include "qplastiquestyle.h"
#endif
#ifndef QT_NO_STYLE_CLEANLOOKS
#include "qcleanlooksstyle.h"
#endif

QT_BEGIN_NAMESPACE

#ifndef QT_NO_LIBRARY
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, stylesloader,
    (QStyleFactoryInterface_iid, QLatin1String("/styles"), Qt::CaseInsensitive))
#endif

/*!
    \class QStyleFactory
    \brief The QStyleFactory class creates QStyle objects.

    \ingroup appearance

    The QStyle class is an abstract base class that encapsulates the
    look and feel of a GUI. QStyleFactory creates a QStyle object
    using the create() function and a key identifying the style. The
    styles are either built-in or dynamically loaded from a style
    plugin (see QStylePlugin).

    The valid keys can be retrieved using the keys()
    function. Typically they include "windows", "motif",
    "plastique" and "cleanlooks".  Note that keys are case
    insensitive.

    \sa QStyle
*/

/*!
    Creates and returns a QStyle object that matches the given \a key, or
    returns 0 if no matching style is found.

    Both built-in styles and styles from style plugins are queried for a
    matching style.

    \note The keys used are case insensitive.

    \sa keys()
*/
QStyle *QStyleFactory::create(const QString& key)
{
    QStyle *ret = Q_NULLPTR;
    QString style = key.toLower();
#ifndef QT_NO_STYLE_WINDOWS
    if (style == QLatin1String("windows"))
        ret = new QWindowsStyle;
    else
#endif
#ifndef QT_NO_STYLE_MOTIF
    if (style == QLatin1String("motif"))
        ret = new QMotifStyle;
    else
#endif
#ifndef QT_NO_STYLE_PLASTIQUE
    if (style == QLatin1String("plastique"))
        ret = new QPlastiqueStyle;
    else
#endif
#ifndef QT_NO_STYLE_CLEANLOOKS
    if (style == QLatin1String("cleanlooks"))
        ret = new QCleanlooksStyle;
    else
#endif
    { } // Keep these here - they make the #ifdefery above work
#if !defined(QT_NO_LIBRARY)
    if(!ret) {
        if (QStyleFactoryInterface *factory = qobject_cast<QStyleFactoryInterface*>(stylesloader()->instance(style)))
            ret = factory->create(style);
    }
#endif
    if(ret)
        ret->setObjectName(style);
    return ret;
}

/*!
    Returns the list of valid keys, i.e. the keys this factory can
    create styles for.

    \sa create()
*/
QStringList QStyleFactory::keys()
{
#if !defined(QT_NO_LIBRARY)
    QStringList list = stylesloader()->keys();
#else
    QStringList list;
#endif
#ifndef QT_NO_STYLE_WINDOWS
    if (!list.contains(QLatin1String("Windows")))
        list << QLatin1String("Windows");
#endif
#ifndef QT_NO_STYLE_MOTIF
    if (!list.contains(QLatin1String("Motif")))
        list << QLatin1String("Motif");
#endif
#ifndef QT_NO_STYLE_PLASTIQUE
    if (!list.contains(QLatin1String("Plastique")))
        list << QLatin1String("Plastique");
#endif
#ifndef QT_NO_STYLE_CLEANLOOKS
    if (!list.contains(QLatin1String("Cleanlooks")))
        list << QLatin1String("Cleanlooks");
#endif
    return list;
}

QT_END_NAMESPACE




