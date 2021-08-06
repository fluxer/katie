/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the tools applications of the Katie Toolkit.
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

#ifndef TEXTEDITFINDWIDGET_H
#define TEXTEDITFINDWIDGET_H

#include "abstractfindwidget.h"

QT_BEGIN_NAMESPACE

class QTextEdit;

class TextEditFindWidget : public AbstractFindWidget
{
    Q_OBJECT

public:
    explicit TextEditFindWidget(FindFlags flags = FindFlags(), QWidget *parent = nullptr);

    QTextEdit *textEdit() const
    { return m_textEdit; }

    void setTextEdit(QTextEdit *textEdit);

protected:
    virtual void deactivate();
    virtual void find(const QString &textToFind, bool skipCurrent, bool backward, bool *found, bool *wrapped);

private:
    QTextEdit *m_textEdit;
};

QT_END_NAMESPACE

#endif  // TEXTEDITFINDWIDGET_H
