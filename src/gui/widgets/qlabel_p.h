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

#ifndef QLABEL_P_H
#define QLABEL_P_H

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

#include "qlabel.h"

#include "qtextdocumentlayout_p.h"
#include "qtextcontrol_p.h"
#include "qtextdocumentfragment.h"
#include "qframe_p.h"
#include "qtextdocument.h"
#include "qmovie.h"
#include "qimage.h"
#include "qbitmap.h"
#include "qmenu.h"

QT_BEGIN_NAMESPACE

class QLabelPrivate : public QFramePrivate
{
    Q_DECLARE_PUBLIC(QLabel)
public:
    QLabelPrivate() {}

    void init();
    void clearContents();
    void updateLabel();
    QSize sizeForWidth(int w) const;

    mutable QSize sh;
    mutable QSize msh;
    mutable bool valid_hints;
    mutable QSizePolicy sizePolicy;
    int margin;
    QString text;
    QPixmap  *pixmap;
    QPixmap *scaledpixmap;
    QImage *cachedimage;
#ifndef QT_NO_MOVIE
    QPointer<QMovie> movie;
    void _q_movieUpdated(const QRect&);
    void _q_movieResized(const QSize&);
#endif
#ifndef QT_NO_SHORTCUT
    void updateShortcut();
#endif
#ifndef QT_NO_SHORTCUT
    QPointer<QWidget> buddy;
    int shortcutId;
#endif
    ushort align;
    short indent;
    bool scaledcontents;
    mutable bool textLayoutDirty;
    mutable bool textDirty;
    bool isRichText;
    bool isTextLabel;
    bool hasShortcut;
    Qt::TextFormat textformat;
    QTextControl *control;
    mutable QTextCursor shortcutCursor;
    Qt::TextInteractionFlags textInteractionFlags;

    inline bool needTextControl() const {
        return isTextLabel
               && (isRichText
                   || (!isRichText && (textInteractionFlags & (Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard))));
    }

    void ensureTextPopulated() const;
    void ensureTextLayouted() const;
    void ensureTextControl();
    void sendControlEvent(QEvent *e);

    void _q_linkHovered(const QString &link);

    QRectF layoutRect() const;
    QRect documentRect() const;
    QPoint layoutPoint(const QPoint& p) const;
    Qt::LayoutDirection textDirection() const;
#ifndef QT_NO_CONTEXTMENU
    QMenu *createStandardContextMenu(const QPoint &pos);
#endif

    bool openExternalLinks;

#ifndef QT_NO_CURSOR
    bool validCursor;
    bool onAnchor;
    QCursor cursor;
#endif

    friend class QMessageBoxPrivate;
};

QT_END_NAMESPACE

#endif // QLABEL_P_H
