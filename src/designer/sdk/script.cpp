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

#include "script_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QDesignerScriptExtension
    \brief The QDesignerScriptExtension class allows you to generate a
    per-widget \l{QtScript} {Qt Script} snippet to be executed while
    building the form.

    \internal
    \inmodule QtDesigner
    \since 4.3

    On saving the form, the extension is queried for a script snippet
    to be associated with the widget while saving the UI file.
    This script is then run after creating the widget by \l uic or
    QUiLoader.

    As opposed to \l QDesignerCustomWidgetInterface::codeTemplate(),
    it allows for applying an internal state of the widget
    that can be manipulated using \QD.

    Such a state might for example be the contents of a custom item view widget,
    for which an editor is provided by the QDesignerTaskMenuExtension.

    While saving the form, the state is serialized as a QVariantMap of
    \QD-supported properties, which is stored in the UI file. This is
    handled by data() and setData().

    For item view contents, there might be for example a key that determines
    the number of items and other keys that contain the actual items following
    a naming scheme (\c numItems, \c item1, \c item2, ...).

    On saving, script() is invoked, which should return a script snippet that
    applies the state to the widget while building the form.

    \sa {Creating Custom Widgets for Katie Designer#Using Qt Script to Aid in Building Forms}{Creating Custom Widgets for Katie Designer}, QtScript
*/

/*!
    Destroys the extension.
*/

QDesignerScriptExtension::~QDesignerScriptExtension()
{
}

/*!
    \fn virtual QString QDesignerScriptExtension::script() const

    Returns a script snippet to be associated with the widget.
*/

/*!
    \fn virtual QVariantMap QDesignerScriptExtension::data() const

    Returns a map of variants describing the internal state to be
    stored in the UI file.
*/

/*!
    \fn virtual void QDesignerScriptExtension::setData(const QVariantMap &data)

    Applies the internal state stored in \a data to the widget while loading a form.
*/

QT_END_NAMESPACE
