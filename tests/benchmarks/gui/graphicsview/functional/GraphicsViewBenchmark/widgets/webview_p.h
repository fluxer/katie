/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the test suite of the Katie Toolkit.
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

#ifndef WEBVIEW_P_H
#define WEBVIEW_P_H

#include "webview.h"
#include <QtWebKit/qgraphicswebview.h>
#include <QtWebKit/qwebpage.h>
#include <QtWebKit/qwebframe.h>
#include <QGraphicsEffect>
#include <QPainter>
#include <QPixmapCache>
#include <QTimer>
#include <QDebug>

class WebViewCache;

class WebViewPrivate {
public:

    WebViewPrivate(WebView *w);
    void adjustSize();
    void _q_loadStarted();
    void _q_loadProgress(int);
    void _q_loadFinished(bool);
    void _q_viewportChanged(QGraphicsWidget*);
    void _q_motionEnded();

    WebView *q;
    QGraphicsWebView *web;
    WebViewCache *cache;
    QTimer motionTimer;
};

class WebViewCache : public QGraphicsEffect 
{
    Q_OBJECT

public:

    WebViewCache(QGraphicsWebView *webView);
    virtual ~WebViewCache();

public:

    void refresh();

    void draw(QPainter * painter, QGraphicsEffectSource * source);

private:

    QVector<QRectF> m_tileRects;
    QVector<QPixmapCache::Key> m_tilePixmaps;
    QSizeF m_itemSize;
    QGraphicsWebView *m_webView;

    friend class WebViewPrivate;
};

#endif // WEBVIEW_P_H
