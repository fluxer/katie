/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the Katie Designer of the Katie Toolkit.
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

#include "abstractsettings_p.h"
#include "previewmanager_p.h"
#include "qdesigner_formbuilder_p.h"
#include "shared_settings_p.h"
#include "zoomwidget_p.h"
#include "formwindowbase_p.h"
#include "widgetfactory_p.h"

#include <QtDesigner/abstractformwindow.h>
#include <QtDesigner/abstractformeditor.h>
#include <QtDesigner/abstractformwindowmanager.h>

#include <QtGui/QWidget>
#include <QtGui/qevent.h>
#include <QtGui/QDesktopWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QDockWidget>
#include <QtGui/QApplication>
#include <QtGui/QPixmap>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialog>
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QCursor>
#include <QtGui/QMatrix>

#include <QtCore/QMap>
#include <QtCore/QDebug>
#include <QtCore/QSharedData>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

static inline int compare(const qdesigner_internal::PreviewConfiguration &pc1, const qdesigner_internal::PreviewConfiguration &pc2)
{
    int rc = pc1.style().compare(pc2.style());
    if (rc)
        return rc;
    return pc1.applicationStyleSheet().compare(pc2.applicationStyleSheet());
}

// ------ PreviewData (data associated with a preview window)
struct PreviewData {
    PreviewData(const QPointer<QWidget> &widget, const  QDesignerFormWindowInterface *formWindow, const qdesigner_internal::PreviewConfiguration &pc);
    QPointer<QWidget> m_widget;
    const QDesignerFormWindowInterface *m_formWindow;
    qdesigner_internal::PreviewConfiguration m_configuration;
};

PreviewData::PreviewData(const QPointer<QWidget>& widget,
                         const QDesignerFormWindowInterface *formWindow,
                         const qdesigner_internal::PreviewConfiguration &pc) :
    m_widget(widget),
    m_formWindow(formWindow),
    m_configuration(pc)
{
}

/* In designer, we have the situation that laid-out maincontainers have
 * a geometry set (which might differ from their sizeHint()). The QGraphicsItem
 * should return that in its size hint, else such cases won't work */

class DesignerZoomProxyWidget : public ZoomProxyWidget  {
    Q_DISABLE_COPY(DesignerZoomProxyWidget)
public:
    DesignerZoomProxyWidget(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
protected:
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF & constraint = QSizeF() ) const;
};

DesignerZoomProxyWidget::DesignerZoomProxyWidget(QGraphicsItem *parent, Qt::WindowFlags wFlags) :
    ZoomProxyWidget(parent, wFlags)
{
}

QSizeF DesignerZoomProxyWidget::sizeHint(Qt::SizeHint which, const QSizeF & constraint) const
{
    if (const QWidget *w = widget())
            return QSizeF(w->size());
    return ZoomProxyWidget::sizeHint(which, constraint);
}

// DesignerZoomWidget which returns DesignerZoomProxyWidget in its factory function
class DesignerZoomWidget : public ZoomWidget {
    Q_DISABLE_COPY(DesignerZoomWidget)
public:
    DesignerZoomWidget(QWidget *parent = nullptr);
private:
    virtual QGraphicsProxyWidget *createProxyWidget(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0) const;
};

DesignerZoomWidget::DesignerZoomWidget(QWidget *parent) :
    ZoomWidget(parent)
{
}

QGraphicsProxyWidget *DesignerZoomWidget::createProxyWidget(QGraphicsItem *parent, Qt::WindowFlags wFlags) const
{
    return new DesignerZoomProxyWidget(parent, wFlags);
}

// ------------ PreviewConfigurationPrivate
class PreviewConfigurationData : public QSharedData {
public:
    PreviewConfigurationData() {}
    explicit PreviewConfigurationData(const QString &style, const QString &applicationStyleSheet);

    QString m_style;
    // Style sheet to prepend (to simulate the effect od QApplication::setSyleSheet()).
    QString m_applicationStyleSheet;
};

PreviewConfigurationData::PreviewConfigurationData(const QString &style, const QString &applicationStyleSheet) :
    m_style(style),
    m_applicationStyleSheet(applicationStyleSheet)
{
}

// ------------- PreviewConfiguration

static const char *styleKey = "Style";
static const char *appStyleSheetKey = "AppStyleSheet";

PreviewConfiguration::PreviewConfiguration() :
    m_d(new PreviewConfigurationData)
{
}

PreviewConfiguration::PreviewConfiguration(const QString &sty, const QString &applicationSheet) :
    m_d(new PreviewConfigurationData(sty, applicationSheet))
{
}

PreviewConfiguration::PreviewConfiguration(const PreviewConfiguration &o) :
    m_d(o.m_d)
{
}

PreviewConfiguration &PreviewConfiguration::operator=(const PreviewConfiguration &o)
{
    m_d.operator=(o.m_d);
    return *this;
}

PreviewConfiguration::~PreviewConfiguration()
{
}

void PreviewConfiguration::clear()
{
    PreviewConfigurationData &d = *m_d;
    d.m_style.clear();
    d.m_applicationStyleSheet.clear();
}

QString PreviewConfiguration::style() const
{
    return m_d->m_style;
}

void PreviewConfiguration::setStyle(const QString &s)
{
    m_d->m_style = s;
}

// Style sheet to prepend (to simulate the effect od QApplication::setSyleSheet()).
QString PreviewConfiguration::applicationStyleSheet() const
{
    return m_d->m_applicationStyleSheet;
}

void PreviewConfiguration::setApplicationStyleSheet(const QString &as)
{
    m_d->m_applicationStyleSheet = as;
}

void PreviewConfiguration::toSettings(const QString &prefix, QDesignerSettingsInterface *settings) const
{
    const PreviewConfigurationData &d = *m_d;
    settings->beginGroup(prefix);
    settings->setValue(QLatin1String(styleKey),  d.m_style);
    settings->setValue(QLatin1String(appStyleSheetKey), d.m_applicationStyleSheet);
    settings->endGroup();
}

void PreviewConfiguration::fromSettings(const QString &prefix, const QDesignerSettingsInterface *settings)
{
    clear();
    QString key = prefix;
    key += QLatin1Char('/');
    const int prefixSize = key.size();

    PreviewConfigurationData &d = *m_d;

    const QVariant emptyString = QVariant(QString());

    key += QLatin1String(styleKey);
    d.m_style = settings->value(key, emptyString).toString();

    key.replace(prefixSize, key.size() - prefixSize, QLatin1String(appStyleSheetKey));
    d.m_applicationStyleSheet = settings->value(key, emptyString).toString();
}


Q_DESIGNER_EXPORT bool operator<(const PreviewConfiguration &pc1, const PreviewConfiguration &pc2)
{
    return compare(pc1, pc2) < 0;
}

Q_DESIGNER_EXPORT bool operator==(const PreviewConfiguration &pc1, const PreviewConfiguration &pc2)
{
    return compare(pc1, pc2) == 0;
}

Q_DESIGNER_EXPORT bool operator!=(const PreviewConfiguration &pc1, const PreviewConfiguration &pc2)
{
    return compare(pc1, pc2) != 0;
}

// ------------- PreviewManagerPrivate
class PreviewManagerPrivate {
public:
    PreviewManagerPrivate(PreviewManager::PreviewMode mode);

    const PreviewManager::PreviewMode m_mode;

    QPointer<QWidget> m_activePreview;

    typedef QList<PreviewData> PreviewDataList;

    PreviewDataList m_previews;

    QDesignerFormEditorInterface *m_core;
    bool m_updateBlocked;
};

PreviewManagerPrivate::PreviewManagerPrivate(PreviewManager::PreviewMode mode) :
    m_mode(mode),
    m_core(0),
    m_updateBlocked(false)
{
}

// ------------- PreviewManager

PreviewManager::PreviewManager(PreviewMode mode, QObject *parent) :
   QObject(parent),
   d(new PreviewManagerPrivate(mode))
{
}

PreviewManager:: ~PreviewManager()
{
    delete d;
}

// Some widgets might require fake containers

static QWidget *fakeContainer(QWidget *w)
{
    // Prevent a dock widget from trying to dock to Designer's main window
    // (which can be found in the parent hierarchy in MDI mode) by
    // providing a fake mainwindow
    if (QDockWidget *dock = qobject_cast<QDockWidget *>(w)) {
        // Reparent: Clear modality, propagate title and resize outer container
        const QSize size = w->size();
        w->setWindowModality(Qt::NonModal);
        dock->setFeatures(dock->features() & ~(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetClosable));
        dock->setAllowedAreas(Qt::LeftDockWidgetArea);
        QMainWindow *mw = new QMainWindow;
        int leftMargin, topMargin, rightMargin, bottomMargin;
        mw->getContentsMargins(&leftMargin, &topMargin, &rightMargin, &bottomMargin);
        mw->addDockWidget(Qt::LeftDockWidgetArea, dock);
        mw->resize(size + QSize(leftMargin + rightMargin, topMargin + bottomMargin));
        return mw;
    }
    return w;
}

static PreviewConfiguration configurationFromSettings(QDesignerFormEditorInterface *core, const QString &style)
{
    qdesigner_internal::PreviewConfiguration pc;
    const QDesignerSharedSettings settings(core);
    if (settings.isCustomPreviewConfigurationEnabled())
        pc = settings.customPreviewConfiguration();
    if (!style.isEmpty())
        pc.setStyle(style);
    return pc;
}

QWidget *PreviewManager::showPreview(const QDesignerFormWindowInterface *fw, const QString &style, int deviceProfileIndex, QString *errorMessage)
{
    return showPreview(fw, configurationFromSettings(fw->core(), style), deviceProfileIndex, errorMessage);
}

QWidget *PreviewManager::showPreview(const QDesignerFormWindowInterface *fw, const QString &style, QString *errorMessage)
{
    return showPreview(fw, style, -1, errorMessage);
}

QWidget *PreviewManager::createPreview(const QDesignerFormWindowInterface *fw,
                                       const PreviewConfiguration &pc,
                                       int deviceProfileIndex,
                                       QString *errorMessage,
                                       int initialZoom)
{
    if (!d->m_core)
        d->m_core = fw->core();

    const bool zoomable = initialZoom > 0;
    // Figure out which profile to apply
    DeviceProfile deviceProfile;
    if (deviceProfileIndex >= 0) {
        deviceProfile = QDesignerSharedSettings(fw->core()).deviceProfileAt(deviceProfileIndex);
    } else {
        if (const FormWindowBase *fwb = qobject_cast<const FormWindowBase *>(fw))
            deviceProfile = fwb->deviceProfile();
    }
    // Create
    QWidget *formWidget = QDesignerFormBuilder::createPreview(fw, pc.style(), pc.applicationStyleSheet(), deviceProfile, errorMessage);
    if (!formWidget)
        return 0;

    const QString title = tr("%1 - [Preview]").arg(formWidget->windowTitle());
    formWidget = fakeContainer(formWidget);
    formWidget->setWindowTitle(title);

    // Clear any modality settings, child widget modalities must not be higher than parent's
    formWidget->setWindowModality(Qt::NonModal);
    // Embed into ZoomWidget
    if (zoomable) {
        ZoomWidget *zw = new DesignerZoomWidget;
        connect(zw->zoomMenu(), SIGNAL(zoomChanged(int)), this, SLOT(slotZoomChanged(int)));
        zw->setWindowTitle(title);
        zw->setWidget(formWidget);
        // Keep any widgets' context menus working, do not use global menu
        zw->setWidgetZoomContextMenuEnabled(true);
        zw->setParent(fw->window(), Qt::Dialog);
        // Make preview close when Widget closes (Dialog/accept, etc)
        formWidget->setAttribute(Qt::WA_DeleteOnClose, true);
        connect(formWidget, SIGNAL(destroyed()), zw, SLOT(close()));
        zw->setZoom(initialZoom);
        return zw;
    }
    formWidget->setParent(fw->window(), Qt::Dialog);
    return formWidget;
}

QWidget *PreviewManager::showPreview(const QDesignerFormWindowInterface *fw,
                                     const PreviewConfiguration &pc,
                                     int deviceProfileIndex,
                                     QString *errorMessage)
{
    enum { Spacing = 10 };
    if (QWidget *existingPreviewWidget = raise(fw, pc))
        return existingPreviewWidget;

    const QDesignerSharedSettings settings(fw->core());
    const int initialZoom = settings.zoomEnabled() ? settings.zoom() : -1;

    QWidget *widget = createPreview(fw, pc, deviceProfileIndex, errorMessage, initialZoom);
    if (!widget)
        return 0;
    // Install filter for Escape key
    widget->setAttribute(Qt::WA_DeleteOnClose, true);
    widget->installEventFilter(this);

    switch (d->m_mode) {
    case ApplicationModalPreview:
        // Cannot do this on the Mac as the dialog would have no close button
        widget->setWindowModality(Qt::ApplicationModal);
        break;
    case SingleFormNonModalPreview:
    case MultipleFormNonModalPreview:
        widget->setWindowModality(Qt::NonModal);
        connect(fw, SIGNAL(changed()), widget, SLOT(close()));
        connect(fw, SIGNAL(destroyed()), widget, SLOT(close()));
        if (d->m_mode == SingleFormNonModalPreview)
            connect(fw->core()->formWindowManager(), SIGNAL(activeFormWindowChanged(QDesignerFormWindowInterface*)), widget, SLOT(close()));
        break;
    }
    // Semi-smart algorithm to position previews:
    // If its the first one, position relative to form.
    // 2nd, attempt to tile right (for comparing styles) or cascade
    const QSize size = widget->size();
    const bool firstPreview = d->m_previews.empty();
    if (firstPreview) {
        widget->move(fw->mapToGlobal(QPoint(Spacing, Spacing)));
    } else {
        if (QWidget *lastPreview = d->m_previews.back().m_widget) {
            QDesktopWidget *desktop = qApp->desktop();
            const QRect lastPreviewGeometry = lastPreview->frameGeometry();
            const QRect availGeometry = desktop->availableGeometry(desktop->screenNumber(lastPreview));
            const QPoint newPos = lastPreviewGeometry.topRight() + QPoint(Spacing, 0);
            if (newPos.x() +  size.width() < availGeometry.right())
                widget->move(newPos);
            else
                widget->move(lastPreviewGeometry.topLeft() + QPoint(Spacing, Spacing));
        }

    }
    d->m_previews.push_back(PreviewData(widget, fw, pc));
    widget->show();
    if (firstPreview)
        emit firstPreviewOpened();
    return widget;
}

QWidget *PreviewManager::raise(const QDesignerFormWindowInterface *fw, const PreviewConfiguration &pc)
{
    typedef PreviewManagerPrivate::PreviewDataList PreviewDataList;
    if (d->m_previews.empty())
        return 0;

    // find matching window
    const PreviewDataList::const_iterator cend =  d->m_previews.constEnd();
    for (PreviewDataList::const_iterator it = d->m_previews.constBegin(); it !=  cend ;++it) {
        QWidget * w = it->m_widget;
        if (w && it->m_formWindow == fw && it->m_configuration == pc) {
            w->raise();
            w->activateWindow();
            return w;
        }
    }
    return 0;
}

void PreviewManager::closeAllPreviews()
{
    typedef PreviewManagerPrivate::PreviewDataList PreviewDataList;
    if (!d->m_previews.empty()) {
        d->m_updateBlocked = true;
        d->m_activePreview = 0;
        const PreviewDataList::iterator cend =  d->m_previews.end();
        for (PreviewDataList::iterator it = d->m_previews.begin(); it !=  cend ;++it) {
            if (it->m_widget)
                it->m_widget->close();
        }
        d->m_previews.clear();
        d->m_updateBlocked = false;
        emit lastPreviewClosed();
    }
}

void PreviewManager::updatePreviewClosed(QWidget *w)
{
    typedef PreviewManagerPrivate::PreviewDataList PreviewDataList;
    if (d->m_updateBlocked)
        return;
    // Purge out all 0 or widgets to be deleted
    for (PreviewDataList::iterator it = d->m_previews.begin(); it != d->m_previews.end() ; ) {
        QWidget *iw = it->m_widget; // Might be 0 when catching QEvent::Destroyed
        if (iw == 0 || iw == w) {
            it = d->m_previews.erase(it);
        } else {
            ++it;
        }
    }
    if (d->m_previews.empty())
        emit lastPreviewClosed();
}

bool PreviewManager::eventFilter(QObject *watched, QEvent *event)
{
    // Courtesy of designer
    do {
        if (!watched->isWidgetType())
            break;
        QWidget *previewWindow = qobject_cast<QWidget *>(watched);
        if (!previewWindow || !previewWindow->isWindow())
            break;

        switch (event->type()) {
        case QEvent::KeyPress:
        case QEvent::ShortcutOverride:        {
            const  QKeyEvent *keyEvent = static_cast<const QKeyEvent *>(event);
            const int key = keyEvent->key();
            if (key == Qt::Key_Escape) {
                previewWindow->close();
                return true;
            }
        }
            break;
        case QEvent::WindowActivate:
            d->m_activePreview = previewWindow;
            break;
        case  QEvent::Destroy: // We don't get QEvent::Close if someone accepts a QDialog.
            updatePreviewClosed(previewWindow);
            break;
        case  QEvent::Close:
            updatePreviewClosed(previewWindow);
            previewWindow->removeEventFilter (this);
            break;
        default:
            break;
        }
    } while(false);
    return QObject::eventFilter(watched, event);
}

int PreviewManager::previewCount() const
{
    return  d->m_previews.size();
}

QPixmap PreviewManager::createPreviewPixmap(const QDesignerFormWindowInterface *fw, const QString &style, int deviceProfileIndex, QString *errorMessage)
{
    return createPreviewPixmap(fw, configurationFromSettings(fw->core(), style), deviceProfileIndex, errorMessage);
}

QPixmap PreviewManager::createPreviewPixmap(const QDesignerFormWindowInterface *fw, const QString &style, QString *errorMessage)
{
    return createPreviewPixmap(fw, style, -1, errorMessage);
}

QPixmap PreviewManager::createPreviewPixmap(const QDesignerFormWindowInterface *fw,
                                            const PreviewConfiguration &pc,
                                            int deviceProfileIndex,
                                            QString *errorMessage)
{
    QWidget *widget = createPreview(fw, pc, deviceProfileIndex, errorMessage);
    if (!widget)
        return QPixmap();
    const QPixmap rc = QPixmap::grabWidget(widget);
    widget->deleteLater();
    return rc;
}

void PreviewManager::slotZoomChanged(int z)
{
    if (d->m_core) { // Save the last zoom chosen by the user.
        QDesignerSharedSettings settings(d->m_core);
        settings.setZoom(z);
    }
}
}

QT_END_NAMESPACE

#include "moc_previewmanager_p.h"
