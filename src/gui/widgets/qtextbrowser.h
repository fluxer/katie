/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QTEXTBROWSER_H
#define QTEXTBROWSER_H

#include <QtGui/qtextedit.h>
#include <QtCore/qurl.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_TEXTBROWSER

class QTextBrowserPrivate;

class Q_GUI_EXPORT QTextBrowser : public QTextEdit
{
    Q_OBJECT

    Q_PROPERTY(QUrl source READ source WRITE setSource)
    Q_OVERRIDE(bool modified SCRIPTABLE false)
    Q_OVERRIDE(bool readOnly DESIGNABLE false SCRIPTABLE false)
    Q_OVERRIDE(bool undoRedoEnabled DESIGNABLE false SCRIPTABLE false)
    Q_PROPERTY(QStringList searchPaths READ searchPaths WRITE setSearchPaths)
    Q_PROPERTY(bool openExternalLinks READ openExternalLinks WRITE setOpenExternalLinks)
    Q_PROPERTY(bool openLinks READ openLinks WRITE setOpenLinks)

public:
    explicit QTextBrowser(QWidget* parent = Q_NULLPTR);
    virtual ~QTextBrowser();

    QUrl source() const;

    QStringList searchPaths() const;
    void setSearchPaths(const QStringList &paths);

    virtual QVariant loadResource(int type, const QUrl &name);

    bool isBackwardAvailable() const;
    bool isForwardAvailable() const;
    void clearHistory();
    QString historyTitle(int) const;
    QUrl historyUrl(int) const;
    int backwardHistoryCount() const;
    int forwardHistoryCount() const;

    bool openExternalLinks() const;
    void setOpenExternalLinks(bool open);

    bool openLinks() const;
    void setOpenLinks(bool open);

public Q_SLOTS:
    virtual void setSource(const QUrl &name);
    virtual void backward();
    virtual void forward();
    virtual void home();
    virtual void reload();

Q_SIGNALS:
    void backwardAvailable(bool);
    void forwardAvailable(bool);
    void historyChanged();
    void sourceChanged(const QUrl &);
    void highlighted(const QUrl &);
    void highlighted(const QString &);
    void anchorClicked(const QUrl &);

protected:
    virtual void keyPressEvent(QKeyEvent *ev);
    virtual void focusOutEvent(QFocusEvent *ev);
    virtual bool focusNextPrevChild(bool next);
    virtual void paintEvent(QPaintEvent *e);


private:
    Q_DISABLE_COPY(QTextBrowser)
    Q_DECLARE_PRIVATE(QTextBrowser)
    Q_PRIVATE_SLOT(d_func(), void _q_documentModified())
    Q_PRIVATE_SLOT(d_func(), void _q_activateAnchor(const QString &))
    Q_PRIVATE_SLOT(d_func(), void _q_highlightLink(const QString &))
};

#endif // QT_NO_TEXTBROWSER

QT_END_NAMESPACE


#endif // QTEXTBROWSER_H
