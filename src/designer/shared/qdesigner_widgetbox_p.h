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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of Katie Designer.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef QDESIGNER_WIDGETBOX_H
#define QDESIGNER_WIDGETBOX_H

#include <QtDesigner/abstractwidgetbox.h>

QT_BEGIN_NAMESPACE

class DomUI;

namespace qdesigner_internal {

// A widget box with a load mode that allows for updating custom widgets.

class Q_DESIGNER_EXPORT QDesignerWidgetBox : public QDesignerWidgetBoxInterface
{
    Q_OBJECT
public:
    enum LoadMode { LoadMerge, LoadReplace, LoadCustomWidgetsOnly };

    explicit QDesignerWidgetBox(QWidget *parent = nullptr, Qt::WindowFlags flags = 0);

    LoadMode loadMode() const;
    void setLoadMode(LoadMode lm);

    virtual bool loadContents(const QString &contents) = 0;

    // Convenience to access the widget box icon of a widget. Empty category
    // matches all
    virtual QIcon iconForWidget(const QString &className,
                                const QString &category = QString()) const = 0;

    // Convenience to find a widget by class name. Empty category matches all
    static bool findWidget(const QDesignerWidgetBoxInterface *wbox,
                           const QString &className,
                           const QString &category /* = QString()  */,
                           Widget *widgetData);
    // Convenience functions to create a DomWidget from widget box xml.
    static DomUI *xmlToUi(const QString &name, const QString &xml, bool insertFakeTopLevel, QString *errorMessage);
    static DomUI *xmlToUi(const QString &name, const QString &xml, bool insertFakeTopLevel);

private:
    LoadMode m_loadMode;
};
}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // QDESIGNER_WIDGETBOX_H
