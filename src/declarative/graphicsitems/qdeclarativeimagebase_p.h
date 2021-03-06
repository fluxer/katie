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

#ifndef QDECLARATIVEIMAGEBASE_H
#define QDECLARATIVEIMAGEBASE_H

#include "qdeclarativeimplicitsizeitem_p.h"


QT_BEGIN_NAMESPACE

class QDeclarativeImageBasePrivate;
class Q_AUTOTEST_EXPORT QDeclarativeImageBase : public QDeclarativeImplicitSizeItem
{
    Q_OBJECT
    Q_ENUMS(Status)

    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(bool asynchronous READ asynchronous WRITE setAsynchronous NOTIFY asynchronousChanged)
    Q_PROPERTY(bool cache READ cache WRITE setCache NOTIFY cacheChanged REVISION 1)
    Q_PROPERTY(QSize sourceSize READ sourceSize WRITE setSourceSize RESET resetSourceSize NOTIFY sourceSizeChanged)
    Q_PROPERTY(bool mirror READ mirror WRITE setMirror NOTIFY mirrorChanged REVISION 1)

public:
    QDeclarativeImageBase(QDeclarativeItem *parent=0);
    ~QDeclarativeImageBase();
    enum Status { Null, Ready, Loading, Error };
    Status status() const;
    qreal progress() const;

    QUrl source() const;
    virtual void setSource(const QUrl &url);

    bool asynchronous() const;
    void setAsynchronous(bool);

    bool cache() const;
    void setCache(bool);

    virtual void setSourceSize(const QSize&);
    QSize sourceSize() const;
    void resetSourceSize();

    virtual void setMirror(bool mirror);
    bool mirror() const;

Q_SIGNALS:
    void sourceChanged(const QUrl &);
    void sourceSizeChanged();
    void statusChanged(QDeclarativeImageBase::Status);
    void progressChanged(qreal progress);
    void asynchronousChanged();
    Q_REVISION(1) void cacheChanged();
    Q_REVISION(1) void mirrorChanged();

protected:
    virtual void load();
    virtual void componentComplete();
    virtual void pixmapChange();
    QDeclarativeImageBase(QDeclarativeImageBasePrivate &dd, QDeclarativeItem *parent);

private Q_SLOTS:
    virtual void requestFinished();
    void requestProgress(qint64,qint64);

private:
    Q_DISABLE_COPY(QDeclarativeImageBase)
    Q_DECLARE_PRIVATE_D(QGraphicsItem::d_ptr.data(), QDeclarativeImageBase)
};

QT_END_NAMESPACE


#endif // QDECLARATIVEIMAGEBASE_H
