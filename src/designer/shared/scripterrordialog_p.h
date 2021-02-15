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

#ifndef SCRIPTERRORDIALOG_H
#define SCRIPTERRORDIALOG_H

#include "formscriptrunner_p.h"

#include <QtGui/QDialog>

QT_BEGIN_NAMESPACE

class QTextEdit;

namespace qdesigner_internal {

    // Dialog for showing script errors
    class  Q_DESIGNER_EXPORT ScriptErrorDialog : public QDialog {
        Q_OBJECT

    public:
        typedef  QFormScriptRunner::Errors Errors;
        ScriptErrorDialog(const Errors& errors, QWidget *parent);

    private:
        QTextEdit *m_textEdit;

    };
} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // SCRIPTERRORDIALOG_H
