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

#ifndef QGUIPLATFORM_P_H
#define QGUIPLATFORM_P_H

#include <QtCore/qplugin.h>
#include <QtCore/qfactoryinterface.h>
#include <QtGui/qdialog.h>


QT_BEGIN_NAMESPACE

class QPalette;
class QIcon;
class QFileDialog;
class QColorDialog;
class QFileInfo;

struct Q_GUI_EXPORT QGuiPlatformPluginInterface  : public QFactoryInterface
{
};

QT_END_NAMESPACE

#define QGuiPlatformPluginInterface_iid "Katie.QGuiPlatformPluginInterface"
Q_DECLARE_INTERFACE(QGuiPlatformPluginInterface, QGuiPlatformPluginInterface_iid)

QT_BEGIN_NAMESPACE

class Q_GUI_EXPORT QGuiPlatformPlugin : public QObject, public QGuiPlatformPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(QGuiPlatformPluginInterface:QFactoryInterface)
    public:
        enum PlatformHint {
            PH_ToolButtonStyle,
            PH_ToolBarIconSize,
            PH_ItemView_ActivateItemOnSingleClick
        };

        explicit QGuiPlatformPlugin(QObject *parent = nullptr);
        ~QGuiPlatformPlugin();

        virtual QStringList keys() const;

        virtual QString styleName();
        virtual QPalette palette();
        virtual QString systemIconThemeName();
        virtual QStringList iconThemeSearchPaths();
        virtual QIcon systemIcon(const QString &name);
        virtual QIcon fileSystemIcon(const QFileInfo &name);

        virtual int platformHint(PlatformHint hint);

        virtual void fileDialogDelete(QFileDialog *);
        virtual bool fileDialogSetVisible(QFileDialog *, bool);
        virtual QDialog::DialogCode fileDialogResultCode(QFileDialog *);
        virtual void fileDialogSetDirectory(QFileDialog *, const QString &);
        virtual QString fileDialogDirectory(const QFileDialog *) const;
        virtual void fileDialogSelectFile(QFileDialog *, const QString &);
        virtual QStringList fileDialogSelectedFiles(const QFileDialog *) const;
        virtual void fileDialogSetFilter(QFileDialog *);
        virtual void fileDialogSetNameFilters(QFileDialog *, const QStringList &);
        virtual void fileDialogSelectNameFilter(QFileDialog *, const QString &);
        virtual QString fileDialogSelectedNameFilter(const QFileDialog *) const;

        virtual void colorDialogDelete(QColorDialog *);
        virtual bool colorDialogSetVisible(QColorDialog *, bool);
        virtual void colorDialogSetCurrentColor(QColorDialog *, const QColor &);
};

// internal
QGuiPlatformPlugin *qt_guiPlatformPlugin();

QT_END_NAMESPACE


#endif // QGUIPLATFORMPLUGIN_H
