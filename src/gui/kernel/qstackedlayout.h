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

#ifndef QSTACKEDLAYOUT_H
#define QSTACKEDLAYOUT_H

#include <QtGui/qlayout.h>


QT_BEGIN_NAMESPACE


class QStackedLayoutPrivate;

class Q_GUI_EXPORT QStackedLayout : public QLayout
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QStackedLayout)
    Q_ENUMS(StackingMode)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentChanged)
    Q_PROPERTY(StackingMode stackingMode READ stackingMode WRITE setStackingMode)
    Q_PROPERTY(int count READ count)

public:
    enum StackingMode {
        StackOne,
        StackAll
    };

    QStackedLayout();
    explicit QStackedLayout(QWidget *parent);
    explicit QStackedLayout(QLayout *parentLayout);
    ~QStackedLayout();

    int addWidget(QWidget *w);
    int insertWidget(int index, QWidget *w);

    QWidget *currentWidget() const;
    int currentIndex() const;
#ifdef Q_NO_USING_KEYWORD
    inline QWidget *widget() { return QLayout::widget(); }
#else
    using QLayout::widget;
#endif
    QWidget *widget(int) const;
    int count() const;

    StackingMode stackingMode() const;
    void setStackingMode(StackingMode stackingMode);

    // abstract virtual functions:
    void addItem(QLayoutItem *item);
    QSize sizeHint() const;
    QSize minimumSize() const;
    QLayoutItem *itemAt(int) const;
    QLayoutItem *takeAt(int);
    void setGeometry(const QRect &rect);
    bool hasHeightForWidth() const;
    int heightForWidth(int width) const;

Q_SIGNALS:
    void widgetRemoved(int index);
    void currentChanged(int index);

public Q_SLOTS:
    void setCurrentIndex(int index);
    void setCurrentWidget(QWidget *w);

private:
    Q_DISABLE_COPY(QStackedLayout)
};

QT_END_NAMESPACE


#endif // QSTACKEDLAYOUT_H
