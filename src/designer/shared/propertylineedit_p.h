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

#ifndef PROPERTYLINEEDIT_H
#define PROPERTYLINEEDIT_H

#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

    // A line edit with a special context menu allowing for adding (escaped) new  lines
    class PropertyLineEdit : public QLineEdit {
        Q_OBJECT
    public:
        explicit PropertyLineEdit(QWidget *parent);
        void setWantNewLine(bool nl) {  m_wantNewLine = nl; }
        bool wantNewLine() const { return m_wantNewLine; }

        bool event(QEvent *e);
    protected:
        void contextMenuEvent (QContextMenuEvent *event );
    private slots:
        void insertNewLine();
    private:
        void insertText(const QString &);
        bool m_wantNewLine;
    };
}

QT_END_NAMESPACE

#endif // PROPERTYLINEEDIT_H
