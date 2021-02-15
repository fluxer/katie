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

#ifndef QCOMBOBOX_P_H
#define QCOMBOBOX_P_H

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

#include "QtGui/qcombobox.h"

#ifndef QT_NO_COMBOBOX
#include "QtGui/qabstractslider.h"
#include "QtGui/qapplication.h"
#include "QtGui/qstyleditemdelegate.h"
#include "QtGui/qstandarditemmodel.h"
#include "QtGui/qlineedit.h"
#include "QtGui/qlistview.h"
#include "QtGui/qpainter.h"
#include "QtGui/qstyle.h"
#include "QtGui/qstyleoption.h"
#include "QtCore/qhash.h"
#include "QtCore/qpair.h"
#include "QtCore/qtimer.h"
#include "qwidget_p.h"
#include "QtCore/qpointer.h"
#include "QtGui/qcompleter.h"
#include "QtGui/qevent.h"
#include "QtCore/qdebug.h"
#include "QtCore/qbasictimer.h"

#include <limits.h>

QT_BEGIN_NAMESPACE

class QAction;

class QComboBoxListView : public QListView
{
    Q_OBJECT
public:
    QComboBoxListView(QComboBox *cmb = 0) : combo(cmb) {}

protected:
    void resizeEvent(QResizeEvent *event)
    {
        resizeContents(viewport()->width(), contentsSize().height());
        QListView::resizeEvent(event);
    }

    QStyleOptionViewItem viewOptions() const
    {
        QStyleOptionViewItem option = QListView::viewOptions();
        option.showDecorationSelected = true;
        if (combo)
            option.font = combo->font();
        return option;
    }

private:
    QComboBox *combo;
};


class QStandardItemModel;

class Q_AUTOTEST_EXPORT QComboBoxPrivateScroller : public QWidget
{
    Q_OBJECT

public:
    QComboBoxPrivateScroller(QAbstractSlider::SliderAction action, QWidget *parent)
        : QWidget(parent), sliderAction(action)
    {
        setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        setAttribute(Qt::WA_NoMousePropagation);
    }
    QSize sizeHint() const {
        return QSize(20, style()->pixelMetric(QStyle::PM_MenuScrollerHeight));
    }

protected:
    inline void stopTimer() {
        timer.stop();
    }

    inline void startTimer() {
        timer.start(100, this);
    }

    void enterEvent(QEvent *) {
        startTimer();
    }

    void leaveEvent(QEvent *) {
        stopTimer();
    }
    void timerEvent(QTimerEvent *e) {
        if (e->timerId() == timer.timerId()) {
            emit doScroll(sliderAction);
        }
    }
    void hideEvent(QHideEvent *) {
        stopTimer();
    }

    void paintEvent(QPaintEvent *) {
        QPainter p(this);
        QStyleOptionMenuItem menuOpt;
        menuOpt.init(this);
        menuOpt.checkType = QStyleOptionMenuItem::NotCheckable;
        menuOpt.menuRect = rect();
        menuOpt.maxIconWidth = 0;
        menuOpt.tabWidth = 0;
        menuOpt.menuItemType = QStyleOptionMenuItem::Scroller;
        if (sliderAction == QAbstractSlider::SliderSingleStepAdd)
            menuOpt.state |= QStyle::State_DownArrow;
        p.eraseRect(rect());
        style()->drawControl(QStyle::CE_MenuScroller, &menuOpt, &p);
    }

Q_SIGNALS:
    void doScroll(int action);

private:
    QAbstractSlider::SliderAction sliderAction;
    QBasicTimer timer;
};

class Q_AUTOTEST_EXPORT QComboBoxPrivateContainer : public QFrame
{
    Q_OBJECT

public:
    QComboBoxPrivateContainer(QAbstractItemView *itemView, QComboBox *parent);
    QAbstractItemView *itemView() const;
    void setItemView(QAbstractItemView *itemView);
    int spacing() const;
    void updateTopBottomMargin();

    QTimer blockMouseReleaseTimer;
    QBasicTimer adjustSizeTimer;
    QPoint initialClickPosition;

public Q_SLOTS:
    void scrollItemView(int action);
    void updateScrollers();
    void viewDestroyed();

protected:
    void changeEvent(QEvent *e);
    bool eventFilter(QObject *o, QEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
    void timerEvent(QTimerEvent *timerEvent);
    void resizeEvent(QResizeEvent *e);
    QStyleOptionComboBox comboStyleOption() const;

Q_SIGNALS:
    void itemSelected(const QModelIndex &);
    void resetButton();

private:
    QComboBox *combo;
    QAbstractItemView *view;
    QComboBoxPrivateScroller *top;
    QComboBoxPrivateScroller *bottom;
};

class QComboMenuDelegate : public QAbstractItemDelegate
{ Q_OBJECT
public:
    QComboMenuDelegate(QObject *parent, QComboBox *cmb) : QAbstractItemDelegate(parent), mCombo(cmb) {}

protected:
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const {
        QStyleOptionMenuItem opt = getStyleOption(option, index);
        painter->fillRect(option.rect, opt.palette.background());
        mCombo->style()->drawControl(QStyle::CE_MenuItem, &opt, painter, mCombo);
    }
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const {
        QStyleOptionMenuItem opt = getStyleOption(option, index);
        return mCombo->style()->sizeFromContents(
            QStyle::CT_MenuItem, &opt, option.rect.size(), mCombo);
    }

private:
    QStyleOptionMenuItem getStyleOption(const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const;
    QComboBox *mCombo;
};

class QComboBoxDelegate : public QStyledItemDelegate
{ Q_OBJECT
public:
    QComboBoxDelegate(QObject *parent, QComboBox *cmb) : QStyledItemDelegate(parent), mCombo(cmb) {}

    static bool isSeparator(const QModelIndex &index) {
        return index.data(Qt::AccessibleDescriptionRole).toString() == QLatin1String("separator");
    }
    static void setSeparator(QAbstractItemModel *model, const QModelIndex &index) {
        model->setData(index, QString::fromLatin1("separator"), Qt::AccessibleDescriptionRole);
        if (QStandardItemModel *m = qobject_cast<QStandardItemModel*>(model))
            if (QStandardItem *item = m->itemFromIndex(index))
                item->setFlags(item->flags() & ~(Qt::ItemIsSelectable|Qt::ItemIsEnabled));
    }

protected:
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const {
        if (isSeparator(index)) {
            QRect rect = option.rect;
            if (const QAbstractItemView *view = qobject_cast<const QAbstractItemView*>(option.widget))
                rect.setWidth(view->viewport()->width());
            QStyleOption opt;
            opt.rect = rect;
            mCombo->style()->drawPrimitive(QStyle::PE_IndicatorToolBarSeparator, &opt, painter, mCombo);
        } else {
            QStyledItemDelegate::paint(painter, option, index);
        }
    }

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const {
        if (isSeparator(index)) {
            int pm = mCombo->style()->pixelMetric(QStyle::PM_DefaultFrameWidth, 0, mCombo);
            return QSize(pm, pm);
        }
        return QStyledItemDelegate::sizeHint(option, index);
    }
private:
    QComboBox *mCombo;
};

class Q_AUTOTEST_EXPORT QComboBoxPrivate : public QWidgetPrivate
{
    Q_DECLARE_PUBLIC(QComboBox)
public:
    QComboBoxPrivate();
    ~QComboBoxPrivate() {}
    void init();
    QComboBoxPrivateContainer* viewContainer();
    void updateLineEditGeometry();
    Qt::MatchFlags matchFlags() const;
    void _q_editingFinished();
    void _q_returnPressed();
    void _q_complete();
    void _q_itemSelected(const QModelIndex &item);
    bool contains(const QString &text, int role);
    void emitActivated(const QModelIndex&);
    void _q_emitHighlighted(const QModelIndex&);
    void _q_emitCurrentIndexChanged(const QModelIndex &index);
    void _q_modelDestroyed();
    void _q_modelReset();
    void _q_resetButton();
    void _q_dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void _q_updateIndexBeforeChange();
    void _q_rowsInserted(const QModelIndex & parent, int start, int end);
    void _q_rowsRemoved(const QModelIndex & parent, int start, int end);
    void updateArrow(QStyle::StateFlag state);
    bool updateHoverControl(const QPoint &pos);
    QRect popupGeometry(int screen = -1) const;
    QStyle::SubControl newHoverControl(const QPoint &pos);
    int computeWidthHint() const;
    QSize recomputeSizeHint(QSize &sh) const;
    void adjustComboBoxSize();
    QString itemText(const QModelIndex &index) const;
    QIcon itemIcon(const QModelIndex &index) const;
    int itemRole() const;
    void updateLayoutDirection();
    void setCurrentIndex(const QModelIndex &index);
    void updateDelegate(bool force = false);
    void keyboardSearchString(const QString &text);
    void modelChanged();
    void updateViewContainerPaletteAndOpacity();

    QAbstractItemModel *model;
    QLineEdit *lineEdit;
    QComboBoxPrivateContainer *container;
    QComboBox::InsertPolicy insertPolicy;
    QComboBox::SizeAdjustPolicy sizeAdjustPolicy;
    int minimumContentsLength;
    QSize iconSize;
    bool shownOnce;
    bool autoCompletion;
    bool duplicatesEnabled;
    bool frame;
    int maxVisibleItems;
    int maxCount;
    int modelColumn;
    bool inserting;
    mutable QSize minimumSizeHint;
    mutable QSize sizeHint;
    QStyle::StateFlag arrowState;
    QStyle::SubControl hoverControl;
    QRect hoverRect;
    QPersistentModelIndex currentIndex;
    QPersistentModelIndex root;
    Qt::CaseSensitivity autoCompletionCaseSensitivity;
    int indexBeforeChange;
#ifndef QT_NO_COMPLETER
    QPointer<QCompleter> completer;
#endif
    static QPalette viewContainerPalette(QComboBox *cmb)
    { return cmb->d_func()->viewContainer()->palette(); }
};

QT_END_NAMESPACE

#endif // QT_NO_COMBOBOX

#endif // QCOMBOBOX_P_H
