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

// designer
#include "qdesigner.h"
#include "qdesigner_actions.h"
#include "qdesigner_settings.h"
#include "qdesigner_workbench.h"
#include "mainwindow.h"

#include "qdesigner_propertysheet_p.h"

#include <QtGui/qevent.h>
#include <QtGui/QMessageBox>
#include <QtGui/QIcon>
#include <QtGui/QErrorMessage>
#include <QtCore/QMetaObject>
#include <QtCore/QFile>
#include <QtCore/QTimer>
#include <QtCore/QTranslator>
#include <QtCore/QFileInfo>
#include <QtCore/qdebug.h>

#include <QtDesigner/qdesigner_components.h>

QT_BEGIN_NAMESPACE

static const char *designerApplicationName = "Designer";
static const char *designerWarningPrefix = "Designer: ";

static void designerMessageHandler(QtMsgType type, const char *msg)
{
    // Only Designer warnings are displayed as box
    QDesigner *designerApp = qDesigner;
    if (type != QtWarningMsg || !designerApp || qstrncmp(designerWarningPrefix, msg, qstrlen(designerWarningPrefix))) {
        qInstallMsgHandler(0);
        qt_message_output(type, msg);
        qInstallMsgHandler (designerMessageHandler);
        return;
    }
    designerApp->showErrorMessage(msg);
}

QDesigner::QDesigner(int &argc, char **argv)
    : QApplication(argc, argv),
      m_workbench(0), m_suppressNewFormShow(false)
{
    setOrganizationName(QLatin1String("Katie"));
    setApplicationName(QLatin1String(designerApplicationName));
    QDesignerComponents::initializeResources();

    setWindowIcon(QIcon(QLatin1String(":/trolltech/designer/designer.png")));
    initialize();
}

QDesigner::~QDesigner()
{
    if (m_workbench)
        delete m_workbench;
}

void QDesigner::showErrorMessage(const char *message)
{
    // strip the prefix
    const QString qMessage = QString::fromUtf8(message + qstrlen(designerWarningPrefix));
    // If there is no main window yet, just store the message.
    // The QErrorMessage would otherwise be hidden by the main window.
    if (m_mainWindow) {
        showErrorMessageBox(qMessage);
    } else {
        qInstallMsgHandler(0);
        qt_message_output(QtWarningMsg, message); // just in case we crash
        qInstallMsgHandler (designerMessageHandler);
        m_initializationErrors += qMessage;
        m_initializationErrors += QLatin1Char('\n');
    }
}

void QDesigner::showErrorMessageBox(const QString &msg)
{
    // Manually suppress consecutive messages.
    // This happens if for example sth is wrong with custom widget creation.
    // The same warning will be displayed by Widget box D&D and form Drop
    // while trying to create instance.
    if (m_errorMessageDialog && m_lastErrorMessage == msg)
        return;

    if (!m_errorMessageDialog) {
        m_lastErrorMessage.clear();
        m_errorMessageDialog = new QErrorMessage(m_mainWindow);
        const QString title = QCoreApplication::translate("QDesigner", "%1 - warning").arg(QLatin1String(designerApplicationName));
        m_errorMessageDialog->setWindowTitle(title);
        m_errorMessageDialog->setMinimumSize(QSize(600, 250));
        m_errorMessageDialog->setWindowFlags(m_errorMessageDialog->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    }
    m_errorMessageDialog->showMessage(msg);
    m_lastErrorMessage = msg;
}

QDesignerWorkbench *QDesigner::workbench() const
{
    return m_workbench;
}

bool QDesigner::parseCommandLineArgs(QStringList &fileNames)
{
    const QStringList args = arguments();
    const QStringList::const_iterator acend = args.constEnd();
    QStringList::const_iterator it = args.constBegin();
    for (++it; it != acend; ++it) {
        const QString &argument = *it;
        do {
            // Arguments
            if (!argument.startsWith(QLatin1Char('-'))) {
                if (!fileNames.contains(argument))
                    fileNames.append(argument);
                break;
            }
            if (argument == QLatin1String("-enableinternaldynamicproperties")) {
                QDesignerPropertySheet::setInternalDynamicPropertiesEnabled(true);
                break;
            }
            const QString msg = QString::fromUtf8("** WARNING Unknown option %1").arg(argument);
            qWarning("%s", qPrintable(msg));
        } while (false);
    }
    return true;
}

void QDesigner::initialize()
{
    // initialize the sub components
    QStringList files;
    parseCommandLineArgs(files);

#ifndef QT_NO_TRANSLATION
    QTranslator *translator = new QTranslator(this);
    QTranslator *qtTranslator = new QTranslator(this);
    translator->load(QLatin1String("qt_tools"));
    qtTranslator->load(QLatin1String("qt"));
    installTranslator(translator);
    installTranslator(qtTranslator);
#endif

    m_workbench = new QDesignerWorkbench();

    emit initialized();
    qInstallMsgHandler(designerMessageHandler); // Warn when loading faulty forms

    m_suppressNewFormShow = m_workbench->readInBackup();

    if (!files.empty()) {
        const QStringList::const_iterator cend = files.constEnd();
        for (QStringList::const_iterator it = files.constBegin(); it != cend; ++it) {
            // Ensure absolute paths for recent file list to be unique
            QString fileName = *it;
            const QFileInfo fi(fileName);
            if (fi.exists() && fi.isRelative())
                fileName = fi.absoluteFilePath();
            m_workbench->readInForm(fileName);
        }
    }
    if ( m_workbench->formWindowCount())
        m_suppressNewFormShow = true;

    // Show up error box with parent now if something went wrong
    if (m_initializationErrors.isEmpty()) {
        if (!m_suppressNewFormShow && QDesignerSettings(m_workbench->core()).showNewFormOnStartup())
            QTimer::singleShot(100, this, SLOT(callCreateForm())); // won't show anything if suppressed
    } else {
        showErrorMessageBox(m_initializationErrors);
        m_initializationErrors.clear();
    }
}

bool QDesigner::event(QEvent *ev)
{
    switch (ev->type()) {
    case QEvent::Close: {
        bool eaten = true;
        QCloseEvent *closeEvent = static_cast<QCloseEvent *>(ev);
        closeEvent->setAccepted(m_workbench->handleClose());
        if (closeEvent->isAccepted()) {
            // We're going down, make sure that we don't get our settings saved twice.
            if (m_mainWindow)
                m_mainWindow->setCloseEventPolicy(MainWindowBase::AcceptCloseEvents);
            eaten = QApplication::event(ev);
        }
        return eaten;
    }
    default:
        return QApplication::event(ev);
    }
    Q_UNREACHABLE();
}

void QDesigner::setMainWindow(MainWindowBase *tw)
{
    m_mainWindow = tw;
}

MainWindowBase *QDesigner::mainWindow() const
{
    return m_mainWindow;
}

void QDesigner::callCreateForm()
{
    if (!m_suppressNewFormShow)
        m_workbench->actionManager()->createForm();
}

QT_END_NAMESPACE
#include "moc_qdesigner.h"
