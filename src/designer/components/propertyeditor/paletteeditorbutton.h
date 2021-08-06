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

#ifndef PALETTEEDITORBUTTON_H
#define PALETTEEDITORBUTTON_H

#include "propertyeditor_global.h"

#include <QtGui/QPalette>
#include <QtGui/QToolButton>

#include "abstractformeditor.h"

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

class QT_PROPERTYEDITOR_EXPORT PaletteEditorButton: public QToolButton
{
    Q_OBJECT
public:
    PaletteEditorButton(const QPalette &palette, QWidget *parent = nullptr);
    virtual ~PaletteEditorButton();

    void setSuperPalette(const QPalette &palette);
    inline QPalette palette() const
    { return m_palette; }

signals:
    void paletteChanged(const QPalette &palette);

public slots:
    void setPalette(const QPalette &palette);

private slots:
    void showPaletteEditor();

private:
    QPalette m_palette;
    QPalette m_superPalette;
};

}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // PALETTEEDITORBUTTON_H
