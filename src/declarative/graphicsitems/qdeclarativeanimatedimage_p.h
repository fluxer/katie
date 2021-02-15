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

#ifndef QDECLARATIVEANIMATEDIMAGE_H
#define QDECLARATIVEANIMATEDIMAGE_H

#include "qdeclarativeimage_p.h"

#ifndef QT_NO_MOVIE


QT_BEGIN_NAMESPACE


class QMovie;
class QDeclarativeAnimatedImagePrivate;

class Q_AUTOTEST_EXPORT QDeclarativeAnimatedImage : public QDeclarativeImage
{
    Q_OBJECT

    Q_PROPERTY(bool playing READ isPlaying WRITE setPlaying NOTIFY playingChanged)
    Q_PROPERTY(bool paused READ isPaused WRITE setPaused NOTIFY pausedChanged)
    Q_PROPERTY(int currentFrame READ currentFrame WRITE setCurrentFrame NOTIFY frameChanged)
    Q_PROPERTY(int frameCount READ frameCount)

    // read-only for AnimatedImage
    Q_PROPERTY(QSize sourceSize READ sourceSize NOTIFY sourceSizeChanged)

public:
    QDeclarativeAnimatedImage(QDeclarativeItem *parent=0);
    ~QDeclarativeAnimatedImage();

    bool isPlaying() const;
    void setPlaying(bool play);

    bool isPaused() const;
    void setPaused(bool pause);

    int currentFrame() const;
    void setCurrentFrame(int frame);

    int frameCount() const;

    // Extends QDeclarativeImage's src property*/
    virtual void setSource(const QUrl&);

Q_SIGNALS:
    void playingChanged();
    void pausedChanged();
    void frameChanged();
    void sourceSizeChanged();

private Q_SLOTS:
    void movieUpdate();
    void movieRequestFinished();
    void playingStatusChanged();

protected:
    virtual void load();
    void componentComplete();

private:
    Q_DISABLE_COPY(QDeclarativeAnimatedImage)
    Q_DECLARE_PRIVATE_D(QGraphicsItem::d_ptr.data(), QDeclarativeAnimatedImage)
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeAnimatedImage)


#endif // QT_NO_MOVIE

#endif
