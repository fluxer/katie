/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtDeclarative module of the Katie Toolkit.
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

#ifndef QDECLARATIVEANIMATEDIMAGE_P_H
#define QDECLARATIVEANIMATEDIMAGE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qdeclarativeimage_p_p.h"

#ifndef QT_NO_MOVIE

QT_BEGIN_NAMESPACE

class QMovie;
class QNetworkReply;

class QDeclarativeAnimatedImagePrivate : public QDeclarativeImagePrivate
{
    Q_DECLARE_PUBLIC(QDeclarativeAnimatedImage)

public:
    QDeclarativeAnimatedImagePrivate()
      : playing(true), paused(false), preset_currentframe(0), _movie(0), reply(0), redirectCount(0)
    {
    }

    bool playing;
    bool paused;
    int preset_currentframe;
    QMovie *_movie;
    QNetworkReply *reply;
    int redirectCount;
};

QT_END_NAMESPACE

#endif // QT_NO_MOVIE

#endif // QDECLARATIVEANIMATEDIMAGE_P_H
