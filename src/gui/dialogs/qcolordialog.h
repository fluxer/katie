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

#ifndef QCOLORDIALOG_H
#define QCOLORDIALOG_H

#include <QtGui/qdialog.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_COLORDIALOG

class QColorDialogPrivate;

class Q_GUI_EXPORT QColorDialog : public QDialog
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QColorDialog)
    Q_ENUMS(ColorDialogOption)
    Q_PROPERTY(QColor currentColor READ currentColor WRITE setCurrentColor
               NOTIFY currentColorChanged)
    Q_PROPERTY(ColorDialogOptions options READ options WRITE setOptions)

public:
    enum ColorDialogOption {
        ShowAlphaChannel    = 0x00000001,
        NoButtons           = 0x00000002
    };

    Q_DECLARE_FLAGS(ColorDialogOptions, ColorDialogOption)

    explicit QColorDialog(QWidget *parent = nullptr);
    explicit QColorDialog(const QColor &initial, QWidget *parent = nullptr);
    ~QColorDialog();

    void setCurrentColor(const QColor &color);
    QColor currentColor() const;

    QColor selectedColor() const;

    void setOption(ColorDialogOption option, bool on = true);
    bool testOption(ColorDialogOption option) const;
    void setOptions(ColorDialogOptions options);
    ColorDialogOptions options() const;

#ifdef Q_NO_USING_KEYWORD
    void open() { QDialog::open(); }
#else
    using QDialog::open;
#endif
    void open(QObject *receiver, const char *member);

    void setVisible(bool visible);

    static QColor getColor(const QColor &initial = Qt::white, QWidget *parent = nullptr,
                            const QString &title = QString(), ColorDialogOptions options = 0);

    // obsolete
    static QRgb getRgba(QRgb rgba = 0xffffffff, bool *ok = 0, QWidget *parent = nullptr);

    // ### Qt 5: use QColor in signatures
    static int customCount();
    static QRgb customColor(int index);
    static void setCustomColor(int index, QRgb color);
    static void setStandardColor(int index, QRgb color);


Q_SIGNALS:
    void currentColorChanged(const QColor &color);
    void colorSelected(const QColor &color);

protected:
    void changeEvent(QEvent *event);
    void done(int result);

private:
    Q_DISABLE_COPY(QColorDialog)

    Q_PRIVATE_SLOT(d_func(), void _q_addCustom())
    Q_PRIVATE_SLOT(d_func(), void _q_newHsv(int h, int s, int v))
    Q_PRIVATE_SLOT(d_func(), void _q_newColorTypedIn(QRgb rgb))
    Q_PRIVATE_SLOT(d_func(), void _q_newCustom(int, int))
    Q_PRIVATE_SLOT(d_func(), void _q_newStandard(int, int))

    friend class QColorShower;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QColorDialog::ColorDialogOptions)

#endif // QT_NO_COLORDIALOG

QT_END_NAMESPACE


#endif // QCOLORDIALOG_H
