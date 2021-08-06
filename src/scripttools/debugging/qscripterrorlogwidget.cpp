/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtSCriptTools module of the Katie Toolkit.
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

#include "qscripterrorlogwidget_p.h"
#include "qscripterrorlogwidgetinterface_p_p.h"

#include <QtCore/qdatetime.h>
#include <QtGui/qboxlayout.h>
#include <QtGui/qtextedit.h>
#include <QtGui/qscrollbar.h>
#include <QtCore/qdebug.h>

QT_BEGIN_NAMESPACE

class QScriptErrorLogWidgetOutputEdit : public QTextEdit
{
public:
    QScriptErrorLogWidgetOutputEdit(QWidget *parent = nullptr)
        : QTextEdit(parent)
    {
        setReadOnly(true);
//        setFocusPolicy(Qt::NoFocus);
        document()->setMaximumBlockCount(255);
    }

    void scrollToBottom()
    {
        QScrollBar *bar = verticalScrollBar();
        bar->setValue(bar->maximum());
    }
};

class QScriptErrorLogWidgetPrivate
    : public QScriptErrorLogWidgetInterfacePrivate
{
    Q_DECLARE_PUBLIC(QScriptErrorLogWidget)
public:
    QScriptErrorLogWidgetPrivate();
    ~QScriptErrorLogWidgetPrivate();

    QScriptErrorLogWidgetOutputEdit *outputEdit;
};

QScriptErrorLogWidgetPrivate::QScriptErrorLogWidgetPrivate()
{
}

QScriptErrorLogWidgetPrivate::~QScriptErrorLogWidgetPrivate()
{
}

QScriptErrorLogWidget::QScriptErrorLogWidget(QWidget *parent)
    : QScriptErrorLogWidgetInterface(*new QScriptErrorLogWidgetPrivate, parent, 0)
{
    Q_D(QScriptErrorLogWidget);
    d->outputEdit = new QScriptErrorLogWidgetOutputEdit();
    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->setMargin(0);
    vbox->setSpacing(0);
    vbox->addWidget(d->outputEdit);

//    QString sheet = QString::fromLatin1("font-size: 14px; font-family: \"Monospace\";");
//    setStyleSheet(sheet);
}

QScriptErrorLogWidget::~QScriptErrorLogWidget()
{
}

void QScriptErrorLogWidget::message(
    QtMsgType type, const QString &text, const QString &fileName,
    int lineNumber, int columnNumber, const QVariant &/*data*/)
{
    // ### we need the error message rather than Error.toString()
    Q_UNUSED(type);
    Q_UNUSED(fileName);
    Q_UNUSED(lineNumber);
    Q_UNUSED(columnNumber);
    Q_D(QScriptErrorLogWidget);
    QString html;
    html.append(QString::fromLatin1("<b>%0</b> %1<br>")
                .arg(QDateTime::currentDateTime().toString()).arg(Qt::escape(text)));
    d->outputEdit->insertHtml(html);
    d->outputEdit->scrollToBottom();
}

void QScriptErrorLogWidget::clear()
{
    Q_D(QScriptErrorLogWidget);
    d->outputEdit->clear();
}

QT_END_NAMESPACE

#include "moc_qscripterrorlogwidget_p.h"
