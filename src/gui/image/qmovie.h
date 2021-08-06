/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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

#ifndef QMOVIE_H
#define QMOVIE_H

#include <QtCore/qobject.h>

#ifndef QT_NO_MOVIE

#include <QtCore/qbytearray.h>
#include <QtCore/qlist.h>
#include <QtGui/qimagereader.h>



QT_BEGIN_NAMESPACE

class QByteArray;
class QColor;
class QIODevice;
class QImage;
class QPixmap;
class QRect;
class QSize;

class QMoviePrivate;
class Q_GUI_EXPORT QMovie : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMovie)
    Q_ENUMS(MovieState CacheMode)
    Q_PROPERTY(int speed READ speed WRITE setSpeed)
    Q_PROPERTY(CacheMode cacheMode READ cacheMode WRITE setCacheMode)
public:
    enum MovieState {
        NotRunning,
        Paused,
        Running
    };
    enum CacheMode {
        CacheNone,
        CacheAll
    };

    QMovie(QObject *parent = nullptr);
    explicit QMovie(QIODevice *device, const QByteArray &format = QByteArray(), QObject *parent = nullptr);
    explicit QMovie(const QString &fileName, const QByteArray &format = QByteArray(), QObject *parent = nullptr);
    ~QMovie();

    static QList<QByteArray> supportedFormats();

    void setDevice(QIODevice *device);
    QIODevice *device() const;

    void setFileName(const QString &fileName);
    QString fileName() const;

    void setFormat(const QByteArray &format);
    QByteArray format() const;

    void setBackgroundColor(const QColor &color);
    QColor backgroundColor() const;

    MovieState state() const;

    QRect frameRect() const;
    QImage currentImage() const;
    QPixmap currentPixmap() const;

    bool isValid() const;

    bool jumpToFrame(int frameNumber);
    int loopCount() const;
    int frameCount() const;
    int nextFrameDelay() const;
    int currentFrameNumber() const;

    int speed() const;

    QSize scaledSize() const;
    void setScaledSize(const QSize &size);

    CacheMode cacheMode() const;
    void setCacheMode(CacheMode mode);

Q_SIGNALS:
    void started();
    void resized(const QSize &size);
    void updated(const QRect &rect);
    void stateChanged(QMovie::MovieState state);
    void error(QImageReader::ImageReaderError error);
    void finished();
    void frameChanged(int frameNumber);

public Q_SLOTS:
    void start();
    bool jumpToNextFrame();
    void setPaused(bool paused);
    void stop();
    void setSpeed(int percentSpeed);

private:
    Q_DISABLE_COPY(QMovie)
    Q_PRIVATE_SLOT(d_func(), void _q_loadNextFrame())

};

QT_END_NAMESPACE


#endif // QT_NO_MOVIE

#endif // QMOVIE_H
