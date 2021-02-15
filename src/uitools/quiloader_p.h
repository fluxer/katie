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

#ifndef QUILOADER_P_H
#define QUILOADER_P_H

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

#include <QtCore/QByteArray>
#include <QtCore/QMetaType>

// This file is here for use by the form preview in Linguist. If you change anything
// here or in the code which uses it, remember to adapt Linguist accordingly.

#define PROP_GENERIC_PREFIX "_q_notr_"
#define PROP_TOOLITEMTEXT "_q_toolItemText_notr"
#define PROP_TOOLITEMTOOLTIP "_q_toolItemToolTip_notr"
#define PROP_TABPAGETEXT "_q_tabPageText_notr"
#define PROP_TABPAGETOOLTIP "_q_tabPageToolTip_notr"
#define PROP_TABPAGEWHATSTHIS "_q_tabPageWhatsThis_notr"


QT_BEGIN_NAMESPACE

class QUiTranslatableStringValue
{
public:
    QByteArray value() const { return m_value; }
    void setValue(const QByteArray &value) { m_value = value; }
    QByteArray comment() const { return m_comment; }
    void setComment(const QByteArray &comment) { m_comment = comment; }

private:
    QByteArray m_value;
    QByteArray m_comment;
};

struct QUiItemRolePair {
    int realRole;
    int shadowRole;
};

QT_END_NAMESPACE


Q_DECLARE_METATYPE(QUiTranslatableStringValue)


#endif // QUILOADER_P_H
