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

#include "stringlisteditorbutton.h"
#include "stringlisteditor.h"

#include <QtCore/qdebug.h>

QT_BEGIN_NAMESPACE

using namespace qdesigner_internal;

StringListEditorButton::StringListEditorButton(
    const QStringList &stringList, QWidget *parent)
    : QToolButton(parent), m_stringList(stringList)
{
    setFocusPolicy(Qt::NoFocus);
    setText(tr("Change String List"));
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));

    connect(this, SIGNAL(clicked()), this, SLOT(showStringListEditor()));
}

StringListEditorButton::~StringListEditorButton()
{
}

void StringListEditorButton::setStringList(const QStringList &stringList)
{
    m_stringList = stringList;
}

void StringListEditorButton::showStringListEditor()
{
    int result;
    QStringList lst = StringListEditor::getStringList(0, m_stringList, &result);
    if (result == QDialog::Accepted) {
        m_stringList = lst;
        emit stringListChanged(m_stringList);
    }
}

QT_END_NAMESPACE
#include "moc_stringlisteditorbutton.h"
