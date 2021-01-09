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

#ifndef QFONTDIALOG_H
#define QFONTDIALOG_H

#include <QtGui/qwindowdefs.h>
#include <QtGui/qdialog.h>
#include <QtGui/qfont.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_FONTDIALOG

class QFontDialogPrivate;

class Q_GUI_EXPORT QFontDialog : public QDialog
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QFontDialog)
    Q_ENUMS(FontDialogOption)
    Q_PROPERTY(QFont currentFont READ currentFont WRITE setCurrentFont NOTIFY currentFontChanged)
    Q_PROPERTY(FontDialogOptions options READ options WRITE setOptions)

public:
    enum FontDialogOption {
        NoButtons           = 0x00000001,
    };

    Q_DECLARE_FLAGS(FontDialogOptions, FontDialogOption)

    explicit QFontDialog(QWidget *parent = Q_NULLPTR);
    explicit QFontDialog(const QFont &initial, QWidget *parent = Q_NULLPTR);
    ~QFontDialog();

    void setCurrentFont(const QFont &font);
    QFont currentFont() const;

    QFont selectedFont() const;

    void setOption(FontDialogOption option, bool on = true);
    bool testOption(FontDialogOption option) const;
    void setOptions(FontDialogOptions options);
    FontDialogOptions options() const;

#ifdef Q_NO_USING_KEYWORD
    void open() { QDialog::open(); }
#else
    using QDialog::open;
#endif
    void open(QObject *receiver, const char *member);

    static QFont getFont(bool *ok, const QFont &initial, QWidget *parent = Q_NULLPTR,
                        const QString &title = QString(), FontDialogOptions options = 0);
    static QFont getFont(bool *ok, QWidget *parent = Q_NULLPTR);


Q_SIGNALS:
    void currentFontChanged(const QFont &font);
    void fontSelected(const QFont &font);

protected:
    void changeEvent(QEvent *event);
    void done(int result);

private:
    // ### Qt 5: make protected
    bool eventFilter(QObject *object, QEvent *event);

    Q_DISABLE_COPY(QFontDialog)

    Q_PRIVATE_SLOT(d_func(), void _q_sizeChanged(const QString &))
    Q_PRIVATE_SLOT(d_func(), void _q_familyHighlighted(int))
    Q_PRIVATE_SLOT(d_func(), void _q_styleHighlighted(int))
    Q_PRIVATE_SLOT(d_func(), void _q_sizeHighlighted(int))
    Q_PRIVATE_SLOT(d_func(), void _q_updateSample())
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QFontDialog::FontDialogOptions)

#endif // QT_NO_FONTDIALOG

QT_END_NAMESPACE


#endif // QFONTDIALOG_H
