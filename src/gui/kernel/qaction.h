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

#ifndef QACTION_H
#define QACTION_H

#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtGui/qkeysequence.h>
#include <QtGui/qwidget.h>
#include <QtGui/qicon.h>


QT_BEGIN_NAMESPACE

#ifndef QT_NO_ACTION

class QMenu;
class QActionGroup;
class QActionPrivate;
class QGraphicsWidget;

class Q_GUI_EXPORT QAction : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QAction)

    Q_ENUMS(MenuRole)
    Q_ENUMS(Priority)
    Q_PROPERTY(bool checkable READ isCheckable WRITE setCheckable NOTIFY changed)
    Q_PROPERTY(bool checked READ isChecked WRITE setChecked DESIGNABLE isCheckable NOTIFY toggled)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY changed)
    Q_PROPERTY(QIcon icon READ icon WRITE setIcon NOTIFY changed)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY changed)
    Q_PROPERTY(QString iconText READ iconText WRITE setIconText NOTIFY changed)
    Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip NOTIFY changed)
    Q_PROPERTY(QString statusTip READ statusTip WRITE setStatusTip NOTIFY changed)
    Q_PROPERTY(QString whatsThis READ whatsThis WRITE setWhatsThis NOTIFY changed)
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY changed)
#ifndef QT_NO_SHORTCUT
    Q_PROPERTY(QKeySequence shortcut READ shortcut WRITE setShortcut NOTIFY changed)
    Q_PROPERTY(Qt::ShortcutContext shortcutContext READ shortcutContext WRITE setShortcutContext NOTIFY changed)
    Q_PROPERTY(bool autoRepeat READ autoRepeat WRITE setAutoRepeat NOTIFY changed)
#endif
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY changed)
    Q_PROPERTY(MenuRole menuRole READ menuRole WRITE setMenuRole NOTIFY changed)
    Q_PROPERTY(bool iconVisibleInMenu READ isIconVisibleInMenu WRITE setIconVisibleInMenu NOTIFY changed)
    Q_PROPERTY(Priority priority READ priority WRITE setPriority)

public:
    enum MenuRole { NoRole, TextHeuristicRole, ApplicationSpecificRole, AboutQtRole,
                    AboutRole, PreferencesRole, QuitRole };
    enum Priority { LowPriority = 0,
                    NormalPriority = 128,
                    HighPriority = 256};
    explicit QAction(QObject* parent);
    QAction(const QString &text, QObject* parent);
    QAction(const QIcon &icon, const QString &text, QObject* parent);

    ~QAction();

    void setActionGroup(QActionGroup *group);
    QActionGroup *actionGroup() const;
    void setIcon(const QIcon &icon);
    QIcon icon() const;

    void setText(const QString &text);
    QString text() const;

    void setIconText(const QString &text);
    QString iconText() const;

    void setToolTip(const QString &tip);
    QString toolTip() const;

    void setStatusTip(const QString &statusTip);
    QString statusTip() const;

    void setWhatsThis(const QString &what);
    QString whatsThis() const;

    void setPriority(Priority priority);
    Priority priority() const;

#ifndef QT_NO_MENU
    QMenu *menu() const;
    void setMenu(QMenu *menu);
#endif

    void setSeparator(bool b);
    bool isSeparator() const;

#ifndef QT_NO_SHORTCUT
    void setShortcut(const QKeySequence &shortcut);
    QKeySequence shortcut() const;

    void setShortcuts(const QList<QKeySequence> &shortcuts);
    void setShortcuts(QKeySequence::StandardKey);
    QList<QKeySequence> shortcuts() const;

    void setShortcutContext(Qt::ShortcutContext context);
    Qt::ShortcutContext shortcutContext() const;

    void setAutoRepeat(bool);
    bool autoRepeat() const;
#endif

    void setFont(const QFont &font);
    QFont font() const;

    void setCheckable(bool);
    bool isCheckable() const;

    QVariant data() const;
    void setData(const QVariant &var);

    bool isChecked() const;
    bool isEnabled() const;
    bool isVisible() const;

    enum ActionEvent { Trigger, Hover };
    void activate(ActionEvent event);
    bool showStatusText(QWidget *widget=0);

    void setMenuRole(MenuRole menuRole);
    MenuRole menuRole() const;

    void setIconVisibleInMenu(bool visible);
    bool isIconVisibleInMenu() const;


    QWidget *parentWidget() const;

    QList<QWidget *> associatedWidgets() const;
#ifndef QT_NO_GRAPHICSVIEW
    QList<QGraphicsWidget *> associatedGraphicsWidgets() const; // ### suboptimal
#endif

protected:
    bool event(QEvent *);
    QAction(QActionPrivate &dd, QObject *parent);

public Q_SLOTS:
    void trigger() { activate(Trigger); }
    void hover() { activate(Hover); }
    void setChecked(bool);
    void toggle();
    void setEnabled(bool);
    inline void setDisabled(bool b) { setEnabled(!b); }
    void setVisible(bool);

Q_SIGNALS:
    void changed();
    void triggered(bool checked = false);
    void hovered();
    void toggled(bool);

private:
    Q_DISABLE_COPY(QAction)

    friend class QGraphicsWidget;
    friend class QWidget;
    friend class QActionGroup;
    friend class QMenu;
    friend class QMenuPrivate;
    friend class QMenuBar;
    friend class QShortcutMap;
    friend class QToolButton;
};

#endif // QT_NO_ACTION

QT_END_NAMESPACE


#include <QtGui/qactiongroup.h>

#endif // QACTION_H
