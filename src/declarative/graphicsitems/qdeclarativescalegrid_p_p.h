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

#ifndef QDECLARATIVESCALEGRID_H
#define QDECLARATIVESCALEGRID_H

#include "qdeclarative.h"

#include <QtCore/QString>
#include <QtCore/QObject>

#include "qdeclarativeborderimage_p.h"
#include "qdeclarativepixmapcache_p.h"
#include "qdeclarativeglobal_p.h"


QT_BEGIN_NAMESPACE


class Q_DECLARATIVE_PRIVATE_EXPORT QDeclarativeScaleGrid : public QObject
{
    Q_OBJECT
    Q_ENUMS(TileRule)

    Q_PROPERTY(int left READ left WRITE setLeft NOTIFY borderChanged)
    Q_PROPERTY(int top READ top WRITE setTop NOTIFY borderChanged)
    Q_PROPERTY(int right READ right WRITE setRight NOTIFY borderChanged)
    Q_PROPERTY(int bottom READ bottom WRITE setBottom NOTIFY borderChanged)

public:
    QDeclarativeScaleGrid(QObject *parent=0);
    ~QDeclarativeScaleGrid();

    bool isNull() const;

    int left() const { return _left; }
    void setLeft(int);

    int top() const { return _top; }
    void setTop(int);

    int right() const { return _right; }
    void setRight(int);

    int  bottom() const { return _bottom; }
    void setBottom(int);

Q_SIGNALS:
    void borderChanged();

private:
    int _left;
    int _top;
    int _right;
    int _bottom;
};

class Q_DECLARATIVE_PRIVATE_EXPORT QDeclarativeGridScaledImage
{
public:
    QDeclarativeGridScaledImage();
    QDeclarativeGridScaledImage(const QDeclarativeGridScaledImage &);
    QDeclarativeGridScaledImage(QIODevice*);
    QDeclarativeGridScaledImage &operator=(const QDeclarativeGridScaledImage &);
    bool isValid() const;
    int gridLeft() const;
    int gridRight() const;
    int gridTop() const;
    int gridBottom() const;
    QDeclarativeBorderImage::TileMode horizontalTileRule() const { return _h; }
    QDeclarativeBorderImage::TileMode verticalTileRule() const { return _v; }

    QString pixmapUrl() const;

private:
    static QDeclarativeBorderImage::TileMode stringToRule(const QString &);

private:
    int _l;
    int _r;
    int _t;
    int _b;
    QDeclarativeBorderImage::TileMode _h;
    QDeclarativeBorderImage::TileMode _v;
    QString _pix;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeScaleGrid)


#endif // QDECLARATIVESCALEGRID_H
