/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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

#ifndef QDIALOG_H
#define QDIALOG_H

#include <QtGui/qwidget.h>


QT_BEGIN_NAMESPACE


class QPushButton;
class QDialogPrivate;

class Q_GUI_EXPORT QDialog : public QWidget
{
    Q_OBJECT
    friend class QPushButton;

    Q_PROPERTY(bool sizeGripEnabled READ isSizeGripEnabled WRITE setSizeGripEnabled)
    Q_PROPERTY(bool modal READ isModal WRITE setModal)

public:
    explicit QDialog(QWidget *parent = nullptr, Qt::WindowFlags f = 0);
    ~QDialog();

    enum DialogCode { Rejected, Accepted };

    int result() const;

    void setVisible(bool visible);

    void setSizeGripEnabled(bool);
    bool isSizeGripEnabled() const;

    void setModal(bool modal);
    void setResult(int r);

Q_SIGNALS:
    void finished(int result);
    void accepted();
    void rejected();

public Q_SLOTS:
    void open();
    int exec();
    virtual void done(int);
    virtual void accept();
    virtual void reject();

protected:
    QDialog(QDialogPrivate &, QWidget *parent, Qt::WindowFlags f = 0);

    void keyPressEvent(QKeyEvent *);
    void closeEvent(QCloseEvent *);
    void showEvent(QShowEvent *);
    void resizeEvent(QResizeEvent *);
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *);
#endif
    bool eventFilter(QObject *, QEvent *);
    void adjustPosition(QWidget*);
private:
    Q_DECLARE_PRIVATE(QDialog)
    Q_DISABLE_COPY(QDialog)
};

QT_END_NAMESPACE


#endif // QDIALOG_H
