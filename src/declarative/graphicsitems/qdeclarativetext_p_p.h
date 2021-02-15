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

#ifndef QDECLARATIVETEXT_P_H
#define QDECLARATIVETEXT_P_H

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

#include "qdeclarativeitem.h"
#include "qdeclarativeimplicitsizeitem_p_p.h"
#include "qdeclarative.h"

#include <QtGui/qtextlayout.h>

QT_BEGIN_NAMESPACE

class QTextDocumentWithImageResources;

class Q_AUTOTEST_EXPORT QDeclarativeTextPrivate : public QDeclarativeImplicitSizeItemPrivate
{
    Q_DECLARE_PUBLIC(QDeclarativeText)
public:
    QDeclarativeTextPrivate();

    ~QDeclarativeTextPrivate();

    void updateSize();
    void updateLayout();
    bool determineHorizontalAlignment();
    bool setHAlign(QDeclarativeText::HAlignment, bool forceAlign = false);
    void mirrorChange();
    QTextDocument *textDocument();

    QString text;
    QFont font;
    QFont sourceFont;
    QColor  color;
    QDeclarativeText::TextStyle style;
    QColor  styleColor;
    QString activeLink;
    QDeclarativeText::HAlignment hAlign;
    QDeclarativeText::VAlignment vAlign;
    QDeclarativeText::TextElideMode elideMode;
    QDeclarativeText::TextFormat format;
    QDeclarativeText::WrapMode wrapMode;
    qreal lineHeight;
    QDeclarativeText::LineHeightMode lineHeightMode;
    int lineCount;
    bool truncated;
    int maximumLineCount;
    int maximumLineCountValid;
    QPointF elidePos;

    static QString elideChar;

    void invalidateImageCache();
    void checkImageCache();
    QPixmap imageCache;

    bool imageCacheDirty:1;
    bool updateOnComponentComplete:1;
    bool richText:1;
    bool singleline:1;
    bool cacheAllTextAsImage:1;
    bool internalWidthUpdate:1;
    bool requireImplicitWidth:1;
    bool hAlignImplicit:1;
    bool rightToLeftText:1;
    bool layoutTextElided:1;

    QRect layedOutTextRect;
    QSize paintedSize;
    qreal naturalWidth;
    virtual qreal implicitWidth() const;
    void ensureDoc();
    QPixmap textDocumentImage(bool drawStyle);
    QTextDocumentWithImageResources *doc;

    QRect setupTextLayout();
    QPixmap textLayoutImage(bool drawStyle);
    void drawTextLayout(QPainter *p, const QPointF &pos, bool drawStyle);
    QTextLayout layout;

    static QPixmap drawOutline(const QPixmap &source, const QPixmap &styleSource);
    static QPixmap drawOutline(const QPixmap &source, const QPixmap &styleSource, int yOffset);

    static inline QDeclarativeTextPrivate *get(QDeclarativeText *t) {
        return t->d_func();
    }
};

QT_END_NAMESPACE
#endif
