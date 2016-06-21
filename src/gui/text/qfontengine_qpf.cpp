/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtGui module of the Qt Toolkit.
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

#include "qfontengine_qpf_p.h"

#include "qpaintengine_raster_p.h"
#include <QtCore/qlibraryinfo.h>
#include <QtCore/qfileinfo.h>

#include <QtCore/qfile.h>
#include <QtCore/qdir.h>
#include <QtCore/qbuffer.h>
#if !defined(QT_NO_FREETYPE)
#include "qfontengine_ft_p.h"
#endif
#include "qcore_unix_p.h" // overrides QT_OPEN

// for mmap
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

QT_BEGIN_NAMESPACE


QString qws_fontCacheDir()
{
    QString dir = QDir::tempPath();
    dir.append(QLatin1String("/fonts/"));
    QDir qd(dir);
    if (!qd.exists() && !qd.mkpath(dir))
        dir = QDir::tempPath();
    return dir;
}


/*
    Creates a new multi qws engine.

    This function takes ownership of the QFontEngine, increasing it's refcount.
*/
QFontEngineMultiQWS::QFontEngineMultiQWS(QFontEngine *fe, int _script, const QStringList &fallbacks)
    : QFontEngineMulti(fallbacks.size() + 1),
      fallbackFamilies(fallbacks), script(_script)
{
    engines[0] = fe;
    fe->ref.ref();
    fontDef = engines[0]->fontDef;
}

void QFontEngineMultiQWS::loadEngine(int at)
{
    Q_ASSERT(at < engines.size());
    Q_ASSERT(engines.at(at) == 0);

    QFontDef request = fontDef;
    request.styleStrategy |= QFont::NoFontMerging;
    request.family = fallbackFamilies.at(at-1);
    engines[at] = QFontDatabase::findFont(script,
                                          /*fontprivate*/0,
                                          request);
    Q_ASSERT(engines[at]);
    engines[at]->ref.ref();
    engines[at]->fontDef = request;
}

void QFontEngineMultiQWS::draw(QPaintEngine */*p*/, qreal /*x*/, qreal /*y*/, const QTextItemInt &/*si*/)
{
    qFatal("QFontEngineMultiQWS::draw should never be called!");
}

QT_END_NAMESPACE
