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

#ifndef PREVIEWFRAME_H
#define PREVIEWFRAME_H

#include <QMdiArea>

QT_BEGIN_NAMESPACE

class PreviewFrame;
class Workspace : public QMdiArea
{
    Q_OBJECT

public:
    Workspace(PreviewFrame *parent = 0);
    ~Workspace() {}

protected:
    void paintEvent(QPaintEvent *);
private:
    PreviewFrame *previewFrame;
};

class PreviewWidget;
class PreviewFrame : public QFrame
{
    Q_OBJECT

public:
    PreviewFrame(QWidget *parent = nullptr);
    void setPreviewPalette(const QPalette &pal);
    void setPreviewVisible(bool val);
    QString previewText() const;
    PreviewWidget *widget() const { return previewWidget; }
private:
    Workspace *workspace;
    PreviewWidget *previewWidget;
    QString m_previewWindowText;
};

QT_END_NAMESPACE

#endif
