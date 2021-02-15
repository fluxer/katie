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

#ifndef QLAYOUT_H
#define QLAYOUT_H

#include <QtGui/qlayoutitem.h>
#include <QtCore/qmargins.h>


QT_BEGIN_NAMESPACE

class QSize;

class QLayoutPrivate;

class Q_GUI_EXPORT QLayout : public QObject, public QLayoutItem
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QLayout)

    Q_ENUMS(SizeConstraint)
    Q_PROPERTY(int margin READ margin WRITE setMargin)
    Q_PROPERTY(int spacing READ spacing WRITE setSpacing)
    Q_PROPERTY(SizeConstraint sizeConstraint READ sizeConstraint WRITE setSizeConstraint)
public:
    enum SizeConstraint {
        SetDefaultConstraint,
        SetNoConstraint,
        SetMinimumSize,
        SetFixedSize,
        SetMaximumSize,
        SetMinAndMaxSize
    };

    QLayout(QWidget *parent);
    QLayout();
    ~QLayout();

    int margin() const;
    int spacing() const;

    void setMargin(int);
    void setSpacing(int);

    void setContentsMargins(int left, int top, int right, int bottom);
    void setContentsMargins(const QMargins &margins);
    void getContentsMargins(int *left, int *top, int *right, int *bottom) const;
    QMargins contentsMargins() const;
    QRect contentsRect() const;

    bool setAlignment(QWidget *w, Qt::Alignment alignment);
    bool setAlignment(QLayout *l, Qt::Alignment alignment);
#ifdef Q_NO_USING_KEYWORD
    inline void setAlignment(Qt::Alignment alignment) { QLayoutItem::setAlignment(alignment); }
#else
    using QLayoutItem::setAlignment;
#endif

    void setSizeConstraint(SizeConstraint);
    SizeConstraint sizeConstraint() const;
    void setMenuBar(QWidget *w);
    QWidget *menuBar() const;

    QWidget *parentWidget() const;

    void invalidate();
    QRect geometry() const;
    bool activate();
    void update();

    void addWidget(QWidget *w);
    virtual void addItem(QLayoutItem *) = 0;

    void removeWidget(QWidget *w);
    void removeItem(QLayoutItem *);

    Qt::Orientations expandingDirections() const;
    QSize minimumSize() const;
    QSize maximumSize() const;
    virtual void setGeometry(const QRect&);
    virtual QLayoutItem *itemAt(int index) const = 0;
    virtual QLayoutItem *takeAt(int index) = 0;
    virtual int indexOf(QWidget *) const;
    virtual int count() const = 0;
    bool isEmpty() const;
    QSizePolicy::ControlTypes controlTypes() const;

    int totalHeightForWidth(int w) const;
    QSize totalMinimumSize() const;
    QSize totalMaximumSize() const;
    QSize totalSizeHint() const;
    QLayout *layout();

    void setEnabled(bool);
    bool isEnabled() const;


    static QSize closestAcceptableSize(const QWidget *w, const QSize &s);

protected:
    void widgetEvent(QEvent *);
    void childEvent(QChildEvent *e);
    void addChildLayout(QLayout *l);
    void addChildWidget(QWidget *w);
    bool adoptLayout(QLayout *layout);

    QRect alignmentRect(const QRect&) const;
protected:
    QLayout(QLayoutPrivate &d, QLayout*, QWidget*);

private:
    Q_DISABLE_COPY(QLayout)

    static void activateRecursiveHelper(QLayoutItem *item);

    friend class QApplicationPrivate;
    friend class QWidget;
};

QT_END_NAMESPACE

// legacy includes
#include <QtGui/qboxlayout.h>
#include <QtGui/qgridlayout.h>


#endif // QLAYOUT_H
