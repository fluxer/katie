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

#ifndef QCOLORDIALOG_P_H
#define QCOLORDIALOG_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// to version without notice, or even be removed.
//
// We mean it.
//
//

#include "qdialog_p.h"
#include "qcolordialog.h"

#ifndef QT_NO_COLORDIALOG

QT_BEGIN_NAMESPACE

class QColorLuminancePicker;
class QColorPicker;
class QColorShower;
class QDialogButtonBox;
class QLabel;
class QVBoxLayout;
class QPushButton;
class QWellArray;

class QColorDialogPrivate : public QDialogPrivate
{
    Q_DECLARE_PUBLIC(QColorDialog)

public:
    void init(const QColor &initial);
    QRgb currentColor() const;
    QColor currentQColor() const;
    void setCurrentColor(QRgb rgb);
    void setCurrentQColor(const QColor &color);
    bool selectColor(const QColor &color);

    int currentAlpha() const;
    void setCurrentAlpha(int a);
    void showAlpha(bool b);
    bool isAlphaVisible() const;
    void retranslateStrings();

    void _q_addCustom();

    void _q_newHsv(int h, int s, int v);
    void _q_newColorTypedIn(QRgb rgb);
    void _q_newCustom(int, int);
    void _q_newStandard(int, int);

    QWellArray *custom;
    QWellArray *standard;

    QDialogButtonBox *buttons;
    QVBoxLayout *leftLay;
    QColorPicker *cp;
    QColorLuminancePicker *lp;
    QColorShower *cs;
    QLabel *lblBasicColors;
    QLabel *lblCustomColors;
    QPushButton *ok;
    QPushButton *cancel;
    QPushButton *addCusBt;
    QColor selectedQColor;
    int nextCust;
    bool smallDisplay;
    QColorDialog::ColorDialogOptions opts;
    QPointer<QObject> receiverToDisconnectOnClose;
    QByteArray memberToDisconnectOnClose;
    bool nativeDialogInUse;
};

#endif // QT_NO_COLORDIALOG

QT_END_NAMESPACE

#endif // QCOLORDIALOG_P_H
