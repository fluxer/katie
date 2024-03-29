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

#ifndef QDECLARATIVEITEM_P_H
#define QDECLARATIVEITEM_P_H

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

#include "qdeclarativeanchors_p.h"
#include "qdeclarativeanchors_p_p.h"
#include "qdeclarativeitemchangelistener_p.h"
#include "qpodvector_p.h"
#include "qdeclarativestate_p.h"
#include "qdeclarativenullablevalue_p_p.h"
#include "qdeclarativenotifier_p.h"
#include "qdeclarativeglobal_p.h"
#include "qdeclarative.h"
#include "qdeclarativecontext.h"
#include "qlist.h"
#include "qdebug.h"
#include "qelapsedtimer.h"
#include "qgraphicsitem_p.h"

QT_BEGIN_NAMESPACE

class QDeclarativeItemKeyFilter;
class QDeclarativeLayoutMirroringAttached;

//### merge into private?
class QDeclarativeContents : public QObject, public QDeclarativeItemChangeListener
{
    Q_OBJECT
public:
    QDeclarativeContents(QDeclarativeItem *item);
    ~QDeclarativeContents();

    QRectF rectF() const;

    void childRemoved(QDeclarativeItem *item);
    void childAdded(QDeclarativeItem *item);

    void calcGeometry() { calcWidth(); calcHeight(); }
    void complete();

Q_SIGNALS:
    void rectChanged(QRectF);

protected:
    void itemGeometryChanged(QDeclarativeItem *item, const QRectF &newGeometry, const QRectF &oldGeometry);
    void itemDestroyed(QDeclarativeItem *item);
    //void itemVisibilityChanged(QDeclarativeItem *item)

private:
    void calcHeight(QDeclarativeItem *changed = 0);
    void calcWidth(QDeclarativeItem *changed = 0);

    QDeclarativeItem *m_item;
    qreal m_x;
    qreal m_y;
    qreal m_width;
    qreal m_height;
};

class Q_DECLARATIVE_EXPORT QDeclarativeItemPrivate : public QGraphicsItemPrivate
{
    Q_DECLARE_PUBLIC(QDeclarativeItem)

public:
    QDeclarativeItemPrivate()
    : _anchors(0), _contents(0),
      baselineOffset(0),
      _anchorLines(0),
      _stateGroup(0), origin(QDeclarativeItem::Center),
      widthValid(false), heightValid(false),
      componentComplete(true), keepMouse(false),
      smooth(false), transformOriginDirty(true), doneEventPreHandler(false),
      inheritedLayoutMirror(false), effectiveLayoutMirror(false), isMirrorImplicit(true),
      inheritMirrorFromParent(false), inheritMirrorFromItem(false), hadFocus(false), hadActiveFocus(false), keyHandler(0),
      mWidth(0), mHeight(0), mImplicitWidth(0), mImplicitHeight(0), attachedLayoutDirection(0)
    {
        QGraphicsItemPrivate::acceptedMouseButtons = 0;
        isDeclarativeItem = 1;
        QGraphicsItemPrivate::flags = QGraphicsItem::GraphicsItemFlags(
                                      QGraphicsItem::ItemHasNoContents
                                      | QGraphicsItem::ItemIsFocusable
                                      | QGraphicsItem::ItemNegativeZStacksBehindParent);
    }

    void init(QDeclarativeItem *parent)
    {
        Q_Q(QDeclarativeItem);
        if (parent) {
            QDeclarative_setParent_noEvent(q, parent);
            q->setParentItem(parent);
            QDeclarativeItemPrivate *parentPrivate = QDeclarativeItemPrivate::get(parent);
            setImplicitLayoutMirror(parentPrivate->inheritedLayoutMirror, parentPrivate->inheritMirrorFromParent);
        }
        baselineOffset.invalidate();
        mouseSetsFocus = false;
    }

    bool isMirrored() const {
        return effectiveLayoutMirror;
    }

    // Private Properties
    qreal width() const;
    void setWidth(qreal);
    void resetWidth();

    qreal height() const;
    void setHeight(qreal);
    void resetHeight();

    virtual qreal implicitWidth() const;
    virtual qreal implicitHeight() const;
    virtual void implicitWidthChanged();
    virtual void implicitHeightChanged();

    void resolveLayoutMirror();
    void setImplicitLayoutMirror(bool mirror, bool inherit);
    void setLayoutMirror(bool mirror);

    QDeclarativeListProperty<QObject> data();
    QDeclarativeListProperty<QObject> resources();

    QDeclarativeListProperty<QDeclarativeState> states();
    QDeclarativeListProperty<QDeclarativeTransition> transitions();

    QString state() const;
    void setState(const QString &);

    QDeclarativeAnchorLine left() const;
    QDeclarativeAnchorLine right() const;
    QDeclarativeAnchorLine horizontalCenter() const;
    QDeclarativeAnchorLine top() const;
    QDeclarativeAnchorLine bottom() const;
    QDeclarativeAnchorLine verticalCenter() const;
    QDeclarativeAnchorLine baseline() const;

    // data property
    static void data_append(QDeclarativeListProperty<QObject> *, QObject *);
    static int data_count(QDeclarativeListProperty<QObject> *);
    static QObject *data_at(QDeclarativeListProperty<QObject> *, int);
    static void data_clear(QDeclarativeListProperty<QObject> *);

    // resources property
    static QObject *resources_at(QDeclarativeListProperty<QObject> *, int);
    static void resources_append(QDeclarativeListProperty<QObject> *, QObject *);
    static int resources_count(QDeclarativeListProperty<QObject> *);
    static void resources_clear(QDeclarativeListProperty<QObject> *);

    // transform property
    static int transform_count(QDeclarativeListProperty<QGraphicsTransform> *list);
    static void transform_append(QDeclarativeListProperty<QGraphicsTransform> *list, QGraphicsTransform *);
    static QGraphicsTransform *transform_at(QDeclarativeListProperty<QGraphicsTransform> *list, int);
    static void transform_clear(QDeclarativeListProperty<QGraphicsTransform> *list);

    static QDeclarativeItemPrivate* get(QDeclarativeItem *item)
    {
        return item->d_func();
    }

    // Accelerated property accessors
    QDeclarativeNotifier parentNotifier;
    static void parentProperty(QObject *o, void *rv, QDeclarativeNotifierEndpoint *e);

    QDeclarativeAnchors *anchors() {
        if (!_anchors) {
            Q_Q(QDeclarativeItem);
            _anchors = new QDeclarativeAnchors(q);
            if (!componentComplete)
                _anchors->classBegin();
        }
        return _anchors;
    }
    QDeclarativeAnchors *_anchors;
    QDeclarativeContents *_contents;

    QDeclarativeNullableValue<qreal> baselineOffset;

    struct AnchorLines {
        AnchorLines(QGraphicsObject *);
        QDeclarativeAnchorLine left;
        QDeclarativeAnchorLine right;
        QDeclarativeAnchorLine hCenter;
        QDeclarativeAnchorLine top;
        QDeclarativeAnchorLine bottom;
        QDeclarativeAnchorLine vCenter;
        QDeclarativeAnchorLine baseline;
    };
    mutable AnchorLines *_anchorLines;
    AnchorLines *anchorLines() const {
        Q_Q(const QDeclarativeItem);
        if (!_anchorLines) _anchorLines =
            new AnchorLines(const_cast<QDeclarativeItem *>(q));
        return _anchorLines;
    }

    enum ChangeType {
        Geometry = 0x01,
        SiblingOrder = 0x02,
        Visibility = 0x04,
        Opacity = 0x08,
        Destroyed = 0x10
    };

    Q_DECLARE_FLAGS(ChangeTypes, ChangeType)

    struct ChangeListener {
        ChangeListener(QDeclarativeItemChangeListener *l, QDeclarativeItemPrivate::ChangeTypes t) : listener(l), types(t) {}
        QDeclarativeItemChangeListener *listener;
        QDeclarativeItemPrivate::ChangeTypes types;
        bool operator==(const ChangeListener &other) const { return listener == other.listener && types == other.types; }
    };

    void addItemChangeListener(QDeclarativeItemChangeListener *listener, ChangeTypes types) {
        changeListeners.append(ChangeListener(listener, types));
    }
    void removeItemChangeListener(QDeclarativeItemChangeListener *, ChangeTypes types);
    QPODVector<ChangeListener,4> changeListeners;

    QDeclarativeStateGroup *_states();
    QDeclarativeStateGroup *_stateGroup;

    QDeclarativeItem::TransformOrigin origin;
    bool widthValid;
    bool heightValid;
    bool componentComplete;
    bool keepMouse;
    bool smooth;
    bool transformOriginDirty;
    bool doneEventPreHandler;
    bool inheritedLayoutMirror;
    bool effectiveLayoutMirror;
    bool isMirrorImplicit;
    bool inheritMirrorFromParent;
    bool inheritMirrorFromItem;
    bool hadFocus;
    bool hadActiveFocus;

    QDeclarativeItemKeyFilter *keyHandler;

    qreal mWidth;
    qreal mHeight;
    qreal mImplicitWidth;
    qreal mImplicitHeight;

    QDeclarativeLayoutMirroringAttached* attachedLayoutDirection;


    QPointF computeTransformOrigin() const;

    virtual void setPosHelper(const QPointF &pos)
    {
        Q_Q(QDeclarativeItem);
        QRectF oldGeometry(this->pos.x(), this->pos.y(), mWidth, mHeight);
        QGraphicsItemPrivate::setPosHelper(pos);
        q->geometryChanged(QRectF(this->pos.x(), this->pos.y(), mWidth, mHeight), oldGeometry);
    }

    // Reimplemented from QGraphicsItemPrivate
    virtual void focusScopeItemChange(bool isSubFocusItem)
    {
        if (hadFocus != isSubFocusItem) {
            hadFocus = isSubFocusItem;
            emit q_func()->focusChanged(isSubFocusItem);
        }
    }

    // Reimplemented from QGraphicsItemPrivate
    virtual void siblingOrderChange()
    {
        Q_Q(QDeclarativeItem);
        for(int ii = 0; ii < changeListeners.count(); ++ii) {
            const QDeclarativeItemPrivate::ChangeListener &change = changeListeners.at(ii);
            if (change.types & QDeclarativeItemPrivate::SiblingOrder) {
                change.listener->itemSiblingOrderChanged(q);
            }
        }
    }

    // Reimplemented from QGraphicsItemPrivate
    virtual void transformChanged();

    virtual void focusChanged(bool);

    virtual void mirrorChange() {};
};

/*
    Key filters can be installed on a QDeclarativeItem, but not removed.  Currently they
    are only used by attached objects (which are only destroyed on Item
    destruction), so this isn't a problem.  If in future this becomes any form
    of public API, they will have to support removal too.
*/
class QDeclarativeItemKeyFilter
{
public:
    QDeclarativeItemKeyFilter(QDeclarativeItem * = 0);
    virtual ~QDeclarativeItemKeyFilter();

    virtual void keyPressed(QKeyEvent *event, bool post);
    virtual void keyReleased(QKeyEvent *event, bool post);
    virtual void componentComplete();

    bool m_processPost;

private:
    QDeclarativeItemKeyFilter *m_next;
};

class QDeclarativeKeyNavigationAttachedPrivate : public QObjectPrivate
{
public:
    QDeclarativeKeyNavigationAttachedPrivate()
        : QObjectPrivate(), left(0), right(0), up(0), down(0), tab(0), backtab(0) {}

    QDeclarativeItem *left;
    QDeclarativeItem *right;
    QDeclarativeItem *up;
    QDeclarativeItem *down;
    QDeclarativeItem *tab;
    QDeclarativeItem *backtab;
};

class QDeclarativeKeyNavigationAttached : public QObject, public QDeclarativeItemKeyFilter
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativeKeyNavigationAttached)

    Q_PROPERTY(QDeclarativeItem *left READ left WRITE setLeft NOTIFY leftChanged)
    Q_PROPERTY(QDeclarativeItem *right READ right WRITE setRight NOTIFY rightChanged)
    Q_PROPERTY(QDeclarativeItem *up READ up WRITE setUp NOTIFY upChanged)
    Q_PROPERTY(QDeclarativeItem *down READ down WRITE setDown NOTIFY downChanged)
    Q_PROPERTY(QDeclarativeItem *tab READ tab WRITE setTab NOTIFY tabChanged)
    Q_PROPERTY(QDeclarativeItem *backtab READ backtab WRITE setBacktab NOTIFY backtabChanged)
    Q_PROPERTY(Priority priority READ priority WRITE setPriority NOTIFY priorityChanged)

    Q_ENUMS(Priority)

public:
    QDeclarativeKeyNavigationAttached(QObject * = 0);

    QDeclarativeItem *left() const;
    void setLeft(QDeclarativeItem *);
    QDeclarativeItem *right() const;
    void setRight(QDeclarativeItem *);
    QDeclarativeItem *up() const;
    void setUp(QDeclarativeItem *);
    QDeclarativeItem *down() const;
    void setDown(QDeclarativeItem *);
    QDeclarativeItem *tab() const;
    void setTab(QDeclarativeItem *);
    QDeclarativeItem *backtab() const;
    void setBacktab(QDeclarativeItem *);

    enum Priority { BeforeItem, AfterItem };
    Priority priority() const;
    void setPriority(Priority);

    static QDeclarativeKeyNavigationAttached *qmlAttachedProperties(QObject *);

Q_SIGNALS:
    void leftChanged();
    void rightChanged();
    void upChanged();
    void downChanged();
    void tabChanged();
    void backtabChanged();
    void priorityChanged();

private:
    virtual void keyPressed(QKeyEvent *event, bool post);
    virtual void keyReleased(QKeyEvent *event, bool post);
    void setFocusNavigation(QDeclarativeItem *currentItem, const char *dir);
};

class QDeclarativeLayoutMirroringAttached : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled RESET resetEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool childrenInherit READ childrenInherit WRITE setChildrenInherit NOTIFY childrenInheritChanged)

public:
    explicit QDeclarativeLayoutMirroringAttached(QObject *parent = nullptr);

    bool enabled() const;
    void setEnabled(bool);
    void resetEnabled();

    bool childrenInherit() const;
    void setChildrenInherit(bool);

    static QDeclarativeLayoutMirroringAttached *qmlAttachedProperties(QObject *);
Q_SIGNALS:
    void enabledChanged();
    void childrenInheritChanged();
private:
    friend class QDeclarativeItemPrivate;
    QDeclarativeItemPrivate *itemPrivate;
};

class QDeclarativeKeysAttachedPrivate : public QObjectPrivate
{
public:
    QDeclarativeKeysAttachedPrivate()
        : QObjectPrivate(), inPress(false), inRelease(false)
        , enabled(true), item(nullptr)
    {}

    QGraphicsItem *finalFocusProxy(QGraphicsItem *item) const
    {
        QGraphicsItem *fp;
        while ((fp = item->focusProxy()))
            item = fp;
        return item;
    }

    //loop detection
    bool inPress;
    bool inRelease;

    bool enabled;

    QList<QDeclarativeItem *> targets;
    QDeclarativeItem *item;
};

class QDeclarativeKeysAttached : public QObject, public QDeclarativeItemKeyFilter
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativeKeysAttached)

    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QDeclarativeListProperty<QDeclarativeItem> forwardTo READ forwardTo)
    Q_PROPERTY(Priority priority READ priority WRITE setPriority NOTIFY priorityChanged)

    Q_ENUMS(Priority)

public:
    QDeclarativeKeysAttached(QObject *parent=0);
    ~QDeclarativeKeysAttached();

    bool enabled() const { Q_D(const QDeclarativeKeysAttached); return d->enabled; }
    void setEnabled(bool enabled) {
        Q_D(QDeclarativeKeysAttached);
        if (enabled != d->enabled) {
            d->enabled = enabled;
            emit enabledChanged();
        }
    }

    enum Priority { BeforeItem, AfterItem};
    Priority priority() const;
    void setPriority(Priority);

    QDeclarativeListProperty<QDeclarativeItem> forwardTo() {
        Q_D(QDeclarativeKeysAttached);
        return QDeclarativeListProperty<QDeclarativeItem>(this, d->targets);
    }

    static QDeclarativeKeysAttached *qmlAttachedProperties(QObject *);

Q_SIGNALS:
    void enabledChanged();
    void priorityChanged();
    void pressed(QDeclarativeKeyEvent *event);
    void released(QDeclarativeKeyEvent *event);
    void digit0Pressed(QDeclarativeKeyEvent *event);
    void digit1Pressed(QDeclarativeKeyEvent *event);
    void digit2Pressed(QDeclarativeKeyEvent *event);
    void digit3Pressed(QDeclarativeKeyEvent *event);
    void digit4Pressed(QDeclarativeKeyEvent *event);
    void digit5Pressed(QDeclarativeKeyEvent *event);
    void digit6Pressed(QDeclarativeKeyEvent *event);
    void digit7Pressed(QDeclarativeKeyEvent *event);
    void digit8Pressed(QDeclarativeKeyEvent *event);
    void digit9Pressed(QDeclarativeKeyEvent *event);

    void leftPressed(QDeclarativeKeyEvent *event);
    void rightPressed(QDeclarativeKeyEvent *event);
    void upPressed(QDeclarativeKeyEvent *event);
    void downPressed(QDeclarativeKeyEvent *event);
    void tabPressed(QDeclarativeKeyEvent *event);
    void backtabPressed(QDeclarativeKeyEvent *event);

    void asteriskPressed(QDeclarativeKeyEvent *event);
    void numberSignPressed(QDeclarativeKeyEvent *event);
    void escapePressed(QDeclarativeKeyEvent *event);
    void returnPressed(QDeclarativeKeyEvent *event);
    void enterPressed(QDeclarativeKeyEvent *event);
    void deletePressed(QDeclarativeKeyEvent *event);
    void spacePressed(QDeclarativeKeyEvent *event);
    void cancelPressed(QDeclarativeKeyEvent *event);
    void selectPressed(QDeclarativeKeyEvent *event);
    void menuPressed(QDeclarativeKeyEvent *event);

private:
    virtual void keyPressed(QKeyEvent *event, bool post);
    virtual void keyReleased(QKeyEvent *event, bool post);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QDeclarativeItemPrivate::ChangeTypes)

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeKeysAttached)
QML_DECLARE_TYPEINFO(QDeclarativeKeysAttached, QML_HAS_ATTACHED_PROPERTIES)
QML_DECLARE_TYPE(QDeclarativeKeyNavigationAttached)
QML_DECLARE_TYPEINFO(QDeclarativeKeyNavigationAttached, QML_HAS_ATTACHED_PROPERTIES)
QML_DECLARE_TYPE(QDeclarativeLayoutMirroringAttached)
QML_DECLARE_TYPEINFO(QDeclarativeLayoutMirroringAttached, QML_HAS_ATTACHED_PROPERTIES)

#endif // QDECLARATIVEITEM_P_H
