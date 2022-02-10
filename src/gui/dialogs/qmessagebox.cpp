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

#include <QtGui/qmessagebox.h>

#ifndef QT_NO_MESSAGEBOX

#include <QtGui/qdialogbuttonbox.h>
#include "qlabel_p.h"
#include "qapplication_p.h"
#include <QtCore/qlist.h>
#include <QtCore/qdebug.h>
#include <QtGui/qstyle.h>
#include <QtGui/qstyleoption.h>
#include <QtGui/qgridlayout.h>
#include <QtGui/qdesktopwidget.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qicon.h>
#include <QtGui/qtextdocument.h>
#include <QtGui/qapplication.h>
#include <QtGui/qtextedit.h>
#include <QtGui/qtextbrowser.h>
#include <QtGui/qmenu.h>
#include "qdialog_p.h"
#include <QtGui/qfont.h>
#include <QtGui/qfontmetrics.h>
#include <QtGui/qclipboard.h>

QT_BEGIN_NAMESPACE

static const uchar* qrc_katie()
{
    static const uchar static_qrc_data[] = {
        0x89, 0x50, 0x4e, 0x47, 0xd, 0xa, 0x1a, 0xa, 0x0, 0x0, 0x0, 0xd, 0x49, 0x48, 0x44, 0x52, 0x0, 0x0, 0x0, 0x40,
        0x0, 0x0, 0x0, 0x40, 0x8, 0x4, 0x0, 0x0, 0x0, 0x0, 0x60, 0xb9, 0x55, 0x0, 0x0, 0x0, 0x4, 0x67, 0x41, 0x4d,
        0x41, 0x0, 0x0, 0xb1, 0x8f, 0xb, 0xfc, 0x61, 0x5, 0x0, 0x0, 0x0, 0x20, 0x63, 0x48, 0x52, 0x4d, 0x0, 0x0, 0x7a,
        0x26, 0x0, 0x0, 0x80, 0x84, 0x0, 0x0, 0xfa, 0x0, 0x0, 0x0, 0x80, 0xe8, 0x0, 0x0, 0x75, 0x30, 0x0, 0x0, 0xea,
        0x60, 0x0, 0x0, 0x3a, 0x98, 0x0, 0x0, 0x17, 0x70, 0x9c, 0xba, 0x51, 0x3c, 0x0, 0x0, 0x0, 0x2, 0x62, 0x4b, 0x47,
        0x44, 0x0, 0x0, 0xaa, 0x8d, 0x23, 0x32, 0x0, 0x0, 0x0, 0x9, 0x70, 0x48, 0x59, 0x73, 0x0, 0x0, 0xe, 0xc4, 0x0,
        0x0, 0xe, 0xc4, 0x1, 0x95, 0x2b, 0xe, 0x1b, 0x0, 0x0, 0x0, 0x7, 0x74, 0x49, 0x4d, 0x45, 0x7, 0xe3, 0xb, 0x1d,
        0x2, 0x3b, 0x3, 0x2f, 0x98, 0x17, 0x33, 0x0, 0x0, 0x4, 0xd, 0x49, 0x44, 0x41, 0x54, 0x68, 0xde, 0xc5, 0x98, 0x5b,
        0x6c, 0x14, 0x55, 0x18, 0xc7, 0x7f, 0xdb, 0xd2, 0x92, 0x2c, 0xd0, 0xae, 0x5c, 0x34, 0x95, 0x36, 0x68, 0x83, 0x1a, 0x20,
        0xc5, 0x44, 0xc2, 0x45, 0x1, 0x25, 0xc6, 0x7, 0xa3, 0x1, 0x41, 0x51, 0x91, 0x90, 0x48, 0x42, 0x2, 0x89, 0xd7, 0x98,
        0x48, 0xe0, 0x41, 0x8, 0x88, 0x97, 0xc0, 0xb, 0x1a, 0x5e, 0x30, 0x5e, 0x1a, 0x13, 0xd, 0x11, 0x45, 0xd2, 0x6a, 0x42,
        0x2c, 0xd1, 0x20, 0x4, 0x88, 0x4, 0x4c, 0xa0, 0x60, 0x42, 0xa0, 0x9, 0x97, 0x56, 0x8c, 0xb4, 0x5c, 0xb6, 0xd0, 0x16,
        0xba, 0x74, 0x78, 0x98, 0xdd, 0x6f, 0xbf, 0xd9, 0x99, 0xd9, 0x9d, 0xd9, 0xd9, 0x99, 0xfd, 0xe6, 0x61, 0xff, 0x33, 0xdf,
        0x99, 0xf3, 0xff, 0xcd, 0x99, 0xb3, 0x67, 0xce, 0x39, 0x10, 0x34, 0x86, 0xb3, 0x86, 0x7f, 0xb8, 0xcd, 0x4d, 0x2e, 0xd0,
        0xc6, 0x7, 0x4c, 0xe, 0x5c, 0xa3, 0xaf, 0x18, 0xc9, 0x1, 0x8c, 0x9c, 0xa3, 0x8d, 0xa6, 0xe8, 0x0, 0x76, 0xd9, 0xec,
        0xd, 0xc, 0x6e, 0xb1, 0x3c, 0x1a, 0xfb, 0x5, 0x8e, 0xf6, 0x6, 0x6, 0x77, 0x58, 0x1c, 0x5, 0xc0, 0x41, 0xf5, 0xcc,
        0xb7, 0x72, 0x10, 0xae, 0x72, 0x5f, 0xd8, 0xf6, 0x13, 0x18, 0x12, 0xbb, 0x85, 0xc0, 0x68, 0x16, 0xb2, 0x5f, 0x21, 0x6c,
        0xd, 0x1b, 0x60, 0xb9, 0x58, 0xed, 0x93, 0x6b, 0x31, 0xbe, 0x91, 0xab, 0x57, 0xa8, 0xe, 0x17, 0xe0, 0x6b, 0xb1, 0x7a,
        0x4f, 0x5d, 0xad, 0xa1, 0x47, 0xae, 0xcf, 0x29, 0x5c, 0x49, 0x45, 0x0, 0x80, 0x27, 0x45, 0xed, 0x51, 0x57, 0x93, 0xb4,
        0x8a, 0x9e, 0x15, 0x26, 0x40, 0x1d, 0x13, 0xd3, 0xea, 0x7f, 0x4e, 0x5b, 0x32, 0x87, 0x44, 0x35, 0x86, 0x9, 0x30, 0x4f,
        0xd4, 0x7e, 0xc, 0x4b, 0xe6, 0x8c, 0xa8, 0xf1, 0x61, 0x2, 0x3c, 0xab, 0x0, 0xac, 0x71, 0x5d, 0x54, 0x3c, 0x3c, 0x80,
        0x51, 0x3c, 0xe7, 0xa, 0x90, 0x12, 0x55, 0x15, 0x1e, 0xc0, 0x87, 0x8c, 0x4d, 0xab, 0xe, 0x4e, 0xe4, 0xe4, 0xb2, 0xd,
        0x7f, 0x2d, 0x2c, 0x80, 0x57, 0x78, 0x57, 0xf4, 0x97, 0x39, 0x3d, 0x40, 0xf7, 0xfd, 0x8b, 0x45, 0x3e, 0x5e, 0x81, 0x98,
        0x4e, 0x9f, 0x1a, 0x70, 0xc7, 0xd9, 0xf2, 0x27, 0x24, 0xbb, 0x22, 0xc, 0xfb, 0x7a, 0xba, 0xd4, 0x70, 0xbb, 0xcc, 0x96,
        0x9f, 0xa9, 0xb2, 0x1e, 0xfe, 0x86, 0x7e, 0x23, 0xce, 0x31, 0x65, 0xb0, 0xc5, 0xa1, 0xc4, 0x57, 0x92, 0x3d, 0x5e, 0x7a,
        0xfb, 0x18, 0x3f, 0x29, 0xfb, 0xdd, 0xe, 0x3d, 0xa8, 0x86, 0x5e, 0xc9, 0xbf, 0xed, 0xa5, 0xca, 0x61, 0xbe, 0x0, 0xd6,
        0xf3, 0x92, 0xe8, 0xbf, 0x59, 0xc6, 0x90, 0xad, 0xc4, 0x6b, 0x8c, 0x4c, 0x2b, 0x83, 0x3e, 0x5e, 0x96, 0xeb, 0x29, 0x2e,
        0xd3, 0xc9, 0xb9, 0x60, 0xcf, 0xbf, 0x48, 0x7d, 0x7e, 0xbb, 0x5c, 0x46, 0x39, 0xfd, 0x82, 0xec, 0xc7, 0x59, 0x36, 0x91,
        0x28, 0xd6, 0xbe, 0x49, 0x35, 0xee, 0x20, 0x73, 0x1d, 0xcb, 0x4c, 0xcb, 0x6b, 0x6f, 0x1e, 0xdd, 0xaa, 0x15, 0x7d, 0xc4,
        0x38, 0x3a, 0x54, 0x25, 0xef, 0xbb, 0x94, 0xfa, 0xc8, 0x3, 0x80, 0xc1, 0x10, 0xab, 0xfd, 0xda, 0x27, 0x2c, 0x4d, 0xbb,
        0x8b, 0x98, 0x4b, 0xb9, 0xbd, 0x9e, 0x0, 0xc, 0x86, 0xb2, 0x7f, 0x5f, 0x5d, 0xd5, 0x28, 0xe6, 0x52, 0x47, 0xa5, 0x43,
        0xc5, 0x6f, 0xa9, 0x89, 0x76, 0x3b, 0x4f, 0x70, 0xc3, 0x5, 0x60, 0x1d, 0xf5, 0x2e, 0x99, 0xa, 0xea, 0x78, 0x4a, 0x3a,
        0x28, 0xf4, 0x33, 0x89, 0xf3, 0xb9, 0x26, 0x49, 0xf, 0xec, 0x5d, 0x4c, 0x28, 0xe6, 0xd, 0x2, 0x50, 0xcb, 0x76, 0x55,
        0xd3, 0xb7, 0xd6, 0xe4, 0x6a, 0x4f, 0x4d, 0xd7, 0x13, 0x78, 0xc9, 0xb1, 0x45, 0xea, 0xea, 0x57, 0xed, 0x41, 0x3, 0x3,
        0x1e, 0xec, 0xff, 0x2d, 0xc1, 0x8a, 0xa7, 0x8a, 0x33, 0x52, 0xdf, 0x2, 0xf3, 0xdd, 0x0, 0xcc, 0x67, 0x78, 0xc1, 0x5b,
        0x7f, 0x67, 0x16, 0xed, 0x81, 0x1, 0x6, 0xd9, 0x29, 0xba, 0x11, 0x32, 0x23, 0x61, 0x7d, 0xde, 0x9b, 0xbe, 0xe3, 0x3c,
        0x47, 0xf8, 0xc5, 0xf6, 0xd9, 0x2d, 0x2e, 0x3a, 0x45, 0x8d, 0xc9, 0x2, 0x54, 0xe6, 0xbd, 0x65, 0x3b, 0x7, 0x4b, 0x62,
        0x6d, 0x46, 0x83, 0xa8, 0x24, 0x4, 0x9b, 0x96, 0x17, 0x17, 0x4f, 0x8b, 0xba, 0x50, 0xe, 0x80, 0xc9, 0xcc, 0x14, 0x7d,
        0x38, 0x7a, 0x80, 0x4a, 0x3e, 0x13, 0xfd, 0x97, 0xd9, 0x2, 0xfe, 0x3e, 0xc7, 0xc1, 0x22, 0xc1, 0x6f, 0xcc, 0x90, 0xb3,
        0x6d, 0xe6, 0x4f, 0x94, 0x2d, 0x70, 0x4d, 0x2d, 0x59, 0x8e, 0xb3, 0x23, 0x7a, 0x0, 0x68, 0x16, 0x35, 0x98, 0x99, 0xcc,
        0x44, 0xb, 0x70, 0x48, 0xe6, 0x50, 0x99, 0x75, 0x65, 0xc4, 0x0, 0xfd, 0xdc, 0x4c, 0x2b, 0xf9, 0xe, 0x44, 0xb, 0x30,
        0x42, 0x8c, 0x93, 0xe5, 0x1, 0x78, 0x46, 0xe6, 0x1f, 0xd2, 0x1d, 0xa3, 0x4, 0x88, 0xb1, 0x56, 0xb4, 0xc, 0xee, 0x51,
        0x2, 0x6c, 0x52, 0xab, 0xc6, 0x1f, 0xca, 0x1, 0x90, 0xdd, 0xd2, 0x39, 0xc0, 0x91, 0x72, 0x0, 0xfc, 0x28, 0x4a, 0x2d,
        0xda, 0xa2, 0x4, 0xc8, 0x4e, 0x42, 0x6b, 0xcb, 0x3, 0x70, 0x8f, 0xa8, 0xec, 0x26, 0x4e, 0xa4, 0x0, 0x8f, 0x8b, 0xfa,
        0xaf, 0x1c, 0x0, 0x9, 0xb5, 0x54, 0x3d, 0x9c, 0xf, 0xa0, 0x95, 0x17, 0x78, 0x9d, 0x8e, 0x12, 0xdb, 0x57, 0xb0, 0x8d,
        0xd1, 0x69, 0x7d, 0x45, 0xed, 0x24, 0xa6, 0x63, 0xb3, 0xda, 0xf5, 0x35, 0xc7, 0xaa, 0x6, 0xb5, 0x14, 0x9d, 0x5d, 0x2,
        0x80, 0x2f, 0xd4, 0xf4, 0xfe, 0x53, 0x2b, 0x99, 0x35, 0x5a, 0xd2, 0x73, 0xdf, 0x8b, 0x1c, 0x2d, 0x69, 0xb, 0x64, 0x77,
        0x92, 0x2e, 0x59, 0xf7, 0x55, 0x72, 0x1, 0x32, 0xbb, 0x3a, 0x95, 0x1, 0x96, 0x60, 0x4e, 0x71, 0x59, 0xd4, 0x46, 0xae,
        0xe6, 0x3, 0x58, 0xc9, 0x22, 0xa0, 0x9a, 0xcf, 0x79, 0xb0, 0xa4, 0x0, 0x23, 0x44, 0xe5, 0xf4, 0x2e, 0x73, 0x4e, 0x38,
        0x20, 0xe7, 0xd5, 0xfc, 0x4c, 0x37, 0x71, 0xcb, 0x26, 0xeb, 0x4e, 0xae, 0x73, 0x89, 0x93, 0xfc, 0xc9, 0x5e, 0x7a, 0x8b,
        0x4, 0x78, 0x58, 0x54, 0xb7, 0x53, 0xfa, 0x55, 0x8f, 0xeb, 0xfa, 0x1, 0x5a, 0x59, 0xaa, 0x9e, 0xc6, 0x6b, 0xdc, 0x4f,
        0x4a, 0x6a, 0x70, 0xbc, 0x3b, 0x4e, 0xa7, 0x47, 0x4, 0x3, 0x83, 0x5e, 0xbe, 0x67, 0xb1, 0x2f, 0x8c, 0x66, 0xb9, 0xf7,
        0xf, 0xb7, 0x22, 0xb3, 0x3d, 0xed, 0xe, 0xe8, 0xa3, 0x8f, 0x16, 0x56, 0xf1, 0x48, 0x41, 0xf3, 0x61, 0x7c, 0xac, 0xee,
        0x5a, 0xe9, 0x5e, 0x70, 0x12, 0xbb, 0xe9, 0xf7, 0x9, 0x61, 0x2e, 0xda, 0x77, 0xb0, 0x96, 0xf9, 0x34, 0xea, 0xf5, 0x7e,
        0xda, 0xba, 0x89, 0x77, 0x38, 0xa5, 0xca, 0x76, 0xd8, 0xf7, 0xcf, 0x63, 0x39, 0x67, 0x9, 0x47, 0xb8, 0x38, 0x8f, 0x31,
        0x8f, 0x17, 0x79, 0xa0, 0xc0, 0xd3, 0xa6, 0x54, 0x27, 0xad, 0xb2, 0x1, 0xa5, 0x78, 0x9e, 0x36, 0x1f, 0x2f, 0xce, 0x21,
        0xa6, 0xf1, 0x9, 0xa7, 0x8b, 0x68, 0x25, 0xf3, 0x78, 0x23, 0x98, 0x79, 0x36, 0xa6, 0xb0, 0x81, 0xa3, 0x3e, 0xcd, 0x93,
        0x2c, 0x29, 0x95, 0x7d, 0x26, 0x1e, 0xe2, 0x4d, 0x5a, 0x3c, 0x75, 0xdd, 0xdb, 0x34, 0xbb, 0xef, 0x9b, 0xc7, 0xfc, 0x78,
        0x3a, 0x44, 0x15, 0x8f, 0x32, 0x95, 0xa9, 0x34, 0x71, 0x2f, 0xb5, 0xd4, 0x52, 0x23, 0x99, 0x14, 0x3d, 0xf4, 0xd0, 0xce,
        0x3e, 0x7e, 0x55, 0xbb, 0x22, 0xb6, 0xb8, 0xb, 0x80, 0x9c, 0x7f, 0x17, 0xe4, 0xd1, 0x5b, 0x68, 0x0, 0x0, 0x0, 0x25,
        0x74, 0x45, 0x58, 0x74, 0x64, 0x61, 0x74, 0x65, 0x3a, 0x63, 0x72, 0x65, 0x61, 0x74, 0x65, 0x0, 0x32, 0x30, 0x31, 0x39,
        0x2d, 0x31, 0x31, 0x2d, 0x32, 0x39, 0x54, 0x30, 0x32, 0x3a, 0x35, 0x39, 0x3a, 0x30, 0x33, 0x2b, 0x30, 0x30, 0x3a, 0x30,
        0x30, 0xb3, 0x26, 0x10, 0x56, 0x0, 0x0, 0x0, 0x25, 0x74, 0x45, 0x58, 0x74, 0x64, 0x61, 0x74, 0x65, 0x3a, 0x6d, 0x6f,
        0x64, 0x69, 0x66, 0x79, 0x0, 0x32, 0x30, 0x31, 0x39, 0x2d, 0x31, 0x31, 0x2d, 0x32, 0x39, 0x54, 0x30, 0x32, 0x3a, 0x35,
        0x39, 0x3a, 0x30, 0x33, 0x2b, 0x30, 0x30, 0x3a, 0x30, 0x30, 0xc2, 0x7b, 0xa8, 0xea, 0x0, 0x0, 0x0, 0x19, 0x74, 0x45,
        0x58, 0x74, 0x53, 0x6f, 0x66, 0x74, 0x77, 0x61, 0x72, 0x65, 0x0, 0x77, 0x77, 0x77, 0x2e, 0x69, 0x6e, 0x6b, 0x73, 0x63,
        0x61, 0x70, 0x65, 0x2e, 0x6f, 0x72, 0x67, 0x9b, 0xee, 0x3c, 0x1a, 0x0, 0x0, 0x0, 0x0, 0x49, 0x45, 0x4e, 0x44, 0xae,
        0x42, 0x60, 0x82
    };
    return static_qrc_data;
};
static const qint64 qrc_katie_size = 1343;

enum DetailButtonLabel { ShowLabel = 0, HideLabel = 1 };
#ifndef QT_NO_TEXTEDIT
class QMessageBoxDetailsText : public QWidget
{
public:
    class TextEdit : public QTextEdit
    {
    public:
        TextEdit(QWidget *parent=0) : QTextEdit(parent) { }
        void contextMenuEvent(QContextMenuEvent * e)
        {
#ifndef QT_NO_CONTEXTMENU
            QMenu *menu = createStandardContextMenu();
            menu->setAttribute(Qt::WA_DeleteOnClose);
            menu->popup(e->globalPos());
#else
            Q_UNUSED(e);
#endif
        }
    };

    QMessageBoxDetailsText(QWidget *parent=0)
        : QWidget(parent)
    {
        QVBoxLayout *layout = new QVBoxLayout;
        layout->setMargin(0);
        QFrame *line = new QFrame(this);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        layout->addWidget(line);
        textEdit = new TextEdit();
        textEdit->setFixedHeight(100);
        textEdit->setFocusPolicy(Qt::NoFocus);
        textEdit->setReadOnly(true);
        layout->addWidget(textEdit);
        setLayout(layout);
    }
    void setText(const QString &text) { textEdit->setPlainText(text); }
    QString text() const { return textEdit->toPlainText(); }
private:
    TextEdit *textEdit;
};
#endif // QT_NO_TEXTEDIT

class DetailButton : public QPushButton
{
public:
    DetailButton(QWidget *parent) : QPushButton(label(ShowLabel), parent)
    {
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }

    QString label(DetailButtonLabel label) const
    { return label == ShowLabel ? QMessageBox::tr("Show Details...") : QMessageBox::tr("Hide Details..."); }

    void setLabel(DetailButtonLabel lbl)
    { setText(label(lbl)); }

    QSize sizeHint() const
    {
        ensurePolished();
        QStyleOptionButton opt;
        initStyleOption(&opt);
        const QFontMetrics fm = fontMetrics();
        opt.text = label(ShowLabel);
        QSize sz = fm.size(Qt::TextShowMnemonic, opt.text);
        QSize ret = style()->sizeFromContents(QStyle::CT_PushButton, &opt, sz, this).
                      expandedTo(QApplication::globalStrut());
        opt.text = label(HideLabel);
        sz = fm.size(Qt::TextShowMnemonic, opt.text);
        ret = ret.expandedTo(style()->sizeFromContents(QStyle::CT_PushButton, &opt, sz, this).
                      expandedTo(QApplication::globalStrut()));
        return ret;
    }
};


class QMessageBoxPrivate : public QDialogPrivate
{
    Q_DECLARE_PUBLIC(QMessageBox)

public:
    QMessageBoxPrivate() : escapeButton(0), defaultButton(0), clickedButton(0), detailsButton(0),
#ifndef QT_NO_TEXTEDIT
                           detailsText(0),
#endif
                           autoAddOkButton(true),
                           detectedEscapeButton(0), informativeLabel(0) { }

    void init(const QString &title = QString(), const QString &text = QString());
    void _q_buttonClicked(QAbstractButton *);

    int execReturnCode(QAbstractButton *button);

    void detectEscapeButton();
    void updateSize();
    int layoutMinimumWidth();
    void retranslateStrings();

    static QMessageBox::StandardButton showNewMessageBox(QWidget *parent,
                QMessageBox::Icon icon, const QString& title, const QString& text,
                QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton);

    static QPixmap standardIcon(QMessageBox::Icon icon, QMessageBox *mb);

    QLabel *label;
    QMessageBox::Icon icon;
    QLabel *iconLabel;
    QDialogButtonBox *buttonBox;
    QList<QAbstractButton *> customButtonList;
    QAbstractButton *escapeButton;
    QPushButton *defaultButton;
    QAbstractButton *clickedButton;
    DetailButton *detailsButton;
#ifndef QT_NO_TEXTEDIT
    QMessageBoxDetailsText *detailsText;
#endif
    bool autoAddOkButton;
    QAbstractButton *detectedEscapeButton;
    QLabel *informativeLabel;
    QPointer<QObject> receiverToDisconnectOnClose;
    QByteArray memberToDisconnectOnClose;
    QByteArray signalToDisconnectOnClose;
};

void QMessageBoxPrivate::init(const QString &title, const QString &text)
{
    Q_Q(QMessageBox);

    label = new QLabel;
    label->setObjectName(QLatin1String("qt_msgbox_label"));
    label->setTextInteractionFlags(Qt::TextInteractionFlags(q->style()->styleHint(QStyle::SH_MessageBox_TextInteractionFlags, 0, q)));
    label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    label->setOpenExternalLinks(true);
    label->setContentsMargins(2, 0, 0, 0);
    label->setIndent(9);
    icon = QMessageBox::NoIcon;
    iconLabel = new QLabel;
    iconLabel->setObjectName(QLatin1String("qt_msgboxex_icon_label"));
    iconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    buttonBox = new QDialogButtonBox;
    buttonBox->setObjectName(QLatin1String("qt_msgbox_buttonbox"));
    buttonBox->setCenterButtons(q->style()->styleHint(QStyle::SH_MessageBox_CenterButtons, 0, q));
    QObject::connect(buttonBox, SIGNAL(clicked(QAbstractButton*)),
                     q, SLOT(_q_buttonClicked(QAbstractButton*)));

    QGridLayout *grid = new QGridLayout;
    const int preferredIconColumn = 0;
    const int preferredTextColumn = 1;
    grid->addWidget(iconLabel, 0, preferredIconColumn, 2, 1, Qt::AlignTop);
    grid->addWidget(label, 0, preferredTextColumn, 1, 1);
    // -- leave space for information label --
    grid->addWidget(buttonBox, 2, 0, 1, 2);

    grid->setSizeConstraint(QLayout::SetNoConstraint);
    q->setLayout(grid);

    if (!title.isEmpty() || !text.isEmpty()) {
        q->setWindowTitle(title);
        q->setText(text);
    }
    q->setModal(true);

    retranslateStrings();
}

int QMessageBoxPrivate::layoutMinimumWidth()
{
    layout->activate();
    return layout->totalMinimumSize().width();
}

void QMessageBoxPrivate::updateSize()
{
    Q_Q(QMessageBox);

    if (!q->isVisible())
        return;

    const QSize screenSize = QApplication::desktop()->availableGeometry(QCursor::pos()).size();
    int hardLimit = qMin(screenSize.width() - 480, 1000); // can never get bigger than this
    // on small screens allows the messagebox be the same size as the screen
    if (screenSize.width() <= 1024)
        hardLimit = screenSize.width();
    const int softLimit = qMin(screenSize.width()/2, 500);

    if (informativeLabel)
        informativeLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    label->setWordWrap(false); // makes the label return min size
    int width = layoutMinimumWidth();

    if (width > softLimit) {
        label->setWordWrap(true);
        width = qMax(softLimit, layoutMinimumWidth());

        if (width > hardLimit) {
            label->d_func()->ensureTextControl();
            if (QTextControl *control = label->d_func()->control) {
                QTextOption opt = control->document()->defaultTextOption();
                opt.setWrapMode(QTextOption::WrapAnywhere);
                control->document()->setDefaultTextOption(opt);
            }
            width = hardLimit;
        }
    }

    if (informativeLabel) {
        label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        QSizePolicy policy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        policy.setHeightForWidth(true);
        informativeLabel->setSizePolicy(policy);
        width = qMax(width, layoutMinimumWidth());
        if (width > hardLimit) { // longest word is really big, so wrap anywhere
            informativeLabel->d_func()->ensureTextControl();
            if (QTextControl *control = informativeLabel->d_func()->control) {
                QTextOption opt = control->document()->defaultTextOption();
                opt.setWrapMode(QTextOption::WrapAnywhere);
                control->document()->setDefaultTextOption(opt);
            }
            width = hardLimit;
        }
        policy.setHeightForWidth(label->wordWrap());
        label->setSizePolicy(policy);
    }

    QFontMetrics fm(QApplication::font("QWorkspaceTitleBar"));
    int windowTitleWidth = qMin(fm.width(q->windowTitle()) + 50, hardLimit);
    if (windowTitleWidth > width)
        width = windowTitleWidth;

    layout->activate();
    int height = (layout->hasHeightForWidth())
                     ? layout->totalHeightForWidth(width)
                     : layout->totalMinimumSize().height();

    q->setFixedSize(width, height);
    QCoreApplication::removePostedEvents(q, QEvent::LayoutRequest);
}

int QMessageBoxPrivate::execReturnCode(QAbstractButton *button)
{
    int ret = buttonBox->standardButton(button);
    if (ret == QMessageBox::NoButton) {
        ret = customButtonList.indexOf(button); // if button == 0, correctly sets ret = -1
    }
    return ret;
}

void QMessageBoxPrivate::_q_buttonClicked(QAbstractButton *button)
{
    Q_Q(QMessageBox);
#ifndef QT_NO_TEXTEDIT
    if (detailsButton && detailsText && button == detailsButton) {
        detailsButton->setLabel(detailsText->isHidden() ? HideLabel : ShowLabel);
        detailsText->setHidden(!detailsText->isHidden());
        updateSize();
    } else
#endif
    {
        clickedButton = button;
        q->done(execReturnCode(button)); // does not trigger closeEvent
        emit q->buttonClicked(button);

        if (receiverToDisconnectOnClose) {
            QObject::disconnect(q, signalToDisconnectOnClose, receiverToDisconnectOnClose,
                                memberToDisconnectOnClose);
            receiverToDisconnectOnClose = 0;
        }
        signalToDisconnectOnClose.clear();
        memberToDisconnectOnClose.clear();
    }
}

/*!
    \class QMessageBox

    \brief The QMessageBox class provides a modal dialog for informing
    the user or for asking the user a question and receiving an answer.

    \ingroup standard-dialogs


    A message box displays a primary \l{QMessageBox::text}{text} to
    alert the user to a situation, an \l{QMessageBox::informativeText}
    {informative text} to further explain the alert or to ask the user
    a question, and an optional \l{QMessageBox::detailedText}
    {detailed text} to provide even more data if the user requests
    it. A message box can also display an \l{QMessageBox::icon} {icon}
    and \l{QMessageBox::standardButtons} {standard buttons} for
    accepting a user response.

    Two APIs for using QMessageBox are provided, the property-based
    API, and the static functions. Calling one of the static functions
    is the simpler approach, but it is less flexible than using the
    property-based API, and the result is less informative. Using the
    property-based API is recommended.

    \section1 The Property-based API

    To use the property-based API, construct an instance of
    QMessageBox, set the desired properties, and call exec() to show
    the message. The simplest configuration is to set only the
    \l{QMessageBox::text} {message text} property.

    \snippet doc/src/snippets/code/src_gui_dialogs_qmessagebox.cpp 5

    The user must click the \gui{OK} button to dismiss the message
    box. The rest of the GUI is blocked until the message box is
    dismissed.

    \image msgbox1.png

    A better approach than just alerting the user to an event is to
    also ask the user what to do about it. Store the question in the
    \l{QMessageBox::informativeText} {informative text} property, and
    set the \l{QMessageBox::standardButtons} {standard buttons}
    property to the set of buttons you want as the set of user
    responses. The buttons are specified by combining values from
    StandardButtons using the bitwise OR operator. The display order
    for the buttons is platform-dependent. For example, on Windows,
    \gui{Save} is displayed to the left of \gui{Cancel}, whereas on
    Mac OS, the order is reversed.

    Mark one of your standard buttons to be your
    \l{QMessageBox::defaultButton()} {default button}.

    \snippet doc/src/snippets/code/src_gui_dialogs_qmessagebox.cpp 6

    This is the approach recommended in the
    \l{http://developer.apple.com/library/mac/documentation/UserExperience/Conceptual/AppleHIGuidelines/Windows/Windows.html#//apple_ref/doc/uid/20000961-BABCAJID}
    {Mac OS X Guidelines}. Similar guidelines apply for the other
    platforms, but note the different ways the
    \l{QMessageBox::informativeText} {informative text} is handled for
    different platforms.

    \image msgbox2.png

    The exec() slot returns the StandardButtons value of the button
    that was clicked.

    \snippet doc/src/snippets/code/src_gui_dialogs_qmessagebox.cpp 7

    To give the user more information to help him answer the question,
    set the \l{QMessageBox::detailedText} {detailed text} property. If
    the \l{QMessageBox::detailedText} {detailed text} property is set,
    the \gui{Show Details...} button will be shown.

    \image msgbox3.png

    Clicking the \gui{Show Details...} button displays the detailed text.

    \image msgbox4.png

    \section2 Rich Text and the Text Format Property

    The \l{QMessageBox::detailedText} {detailed text} property is
    always interpreted as plain text. The \l{QMessageBox::text} {main
    text} and \l{QMessageBox::informativeText} {informative text}
    properties can be either plain text or rich text. These strings
    are interpreted according to the setting of the
    \l{QMessageBox::textFormat} {text format} property. The default
    setting is \l{Qt::AutoText} {auto-text}.

    Note that for some plain text strings containing XML
    meta-characters, the auto-text \l{Qt::mightBeRichText()} {rich
    text detection test} may fail causing your plain text string to be
    interpreted incorrectly as rich text. In these rare cases, use
    Qt::convertFromPlainText() to convert your plain text string to a
    visually equivalent rich text string, or set the
    \l{QMessageBox::textFormat} {text format} property explicitly with
    setTextFormat().

    \section2 Severity Levels and the Icon and Pixmap Properties

    QMessageBox supports four predefined message severity levels, or message
    types, which really only differ in the predefined icon they each show.
    Specify one of the four predefined message types by setting the
    \l{QMessageBox::icon}{icon} property to one of the
    \l{QMessageBox::Icon}{predefined icons}. The following rules are
    guidelines:

    \table
    \row
    \o \img qmessagebox-quest.png
    \o \l Question
    \o For asking a question during normal operations.
    \row
    \o \img qmessagebox-info.png
    \o \l Information
    \o For reporting information about normal operations.
    \row
    \o \img qmessagebox-warn.png
    \o \l Warning
    \o For reporting non-critical errors.
    \row
    \o \img qmessagebox-crit.png
    \o \l Critical
    \o For reporting critical errors.
    \endtable

    \l{QMessageBox::Icon}{Predefined icons} are not defined by QMessageBox, but
    provided by the style. The default value is \l{QMessageBox::NoIcon}
    {No Icon}. The message boxes are otherwise the same for all cases. When
    using a standard icon, use the one recommended in the table, or use the
    one recommended by the style guidelines for your platform. If none of the
    standard icons is right for your message box, you can use a custom icon by
    setting the \l{QMessageBox::iconPixmap}{icon pixmap} property instead of
    setting the \l{QMessageBox::icon}{icon} property.

    In summary, to set an icon, use \e{either} setIcon() for one of the
    standard icons, \e{or} setIconPixmap() for a custom icon.

    \section1 The Static Functions API

    Building message boxes with the static functions API, although
    convenient, is less flexible than using the property-based API,
    because the static function signatures lack parameters for setting
    the \l{QMessageBox::informativeText} {informative text} and
    \l{QMessageBox::detailedText} {detailed text} properties. One
    work-around for this has been to use the \c{title} parameter as
    the message box main text and the \c{text} parameter as the
    message box informative text. Because this has the obvious
    drawback of making a less readable message box, platform
    guidelines do not recommend it. The \e{Microsoft Windows User
    Interface Guidelines} recommend using the
    \l{QCoreApplication::applicationName} {application name} as the
    \l{QMessageBox::setWindowTitle()} {window's title}, which means
    that if you have an informative text in addition to your main
    text, you must concatenate it to the \c{text} parameter.

    Note that the static function signatures have changed with respect
    to their button parameters, which are now used to set the
    \l{QMessageBox::standardButtons} {standard buttons} and the
    \l{QMessageBox::defaultButton()} {default button}.

    Static functions are available for creating information(),
    question(), warning(), and critical() message boxes.

    \snippet doc/src/snippets/code/src_gui_dialogs_qmessagebox.cpp 0

    The \l{dialogs/standarddialogs}{Standard Dialogs} example shows
    how to use QMessageBox and the other built-in Qt dialogs.

    \section1 Advanced Usage

    If the \l{QMessageBox::StandardButtons} {standard buttons} are not
    flexible enough for your message box, you can use the addButton()
    overload that takes a text and a ButtonRoleto to add custom
    buttons. The ButtonRole is used by QMessageBox to determine the
    ordering of the buttons on screen (which varies according to the
    platform). You can test the value of clickedButton() after calling
    exec(). For example,

    \snippet doc/src/snippets/code/src_gui_dialogs_qmessagebox.cpp 2

    \section1 Default and Escape Keys

    The default button (i.e., the button activated when \key Enter is
    pressed) can be specified using setDefaultButton(). If a default
    button is not specified, QMessageBox tries to find one based on
    the \l{ButtonRole} {button roles} of the buttons used in the
    message box.

    The escape button (the button activated when \key Esc is pressed)
    can be specified using setEscapeButton().  If an escape button is
    not specified, QMessageBox tries to find one using these rules:

    \list 1

    \o If there is only one button, it is the button activated when
    \key Esc is pressed.

    \o If there is a \l Cancel button, it is the button activated when
    \key Esc is pressed.

    \o If there is exactly one button having either
       \l{QMessageBox::RejectRole} {the Reject role} or the
       \l{QMessageBox::NoRole} {the No role}, it is the button
       activated when \key Esc is pressed.

    \endlist

    When an escape button can't be determined using these rules,
    pressing \key Esc has no effect.

    \sa QDialogButtonBox, {fowler}{GUI Design Handbook: Message Box}, {Standard Dialogs Example}, {Application Example}
*/

/*!
    \enum QMessageBox::StandardButton
    \since 4.2

    These enums describe flags for standard buttons. Each button has a
    defined \l ButtonRole.

    \value Ok An "OK" button defined with the \l AcceptRole.
    \value Open A "Open" button defined with the \l AcceptRole.
    \value Save A "Save" button defined with the \l AcceptRole.
    \value Cancel A "Cancel" button defined with the \l RejectRole.
    \value Close A "Close" button defined with the \l RejectRole.
    \value Discard A "Discard" or "Don't Save" button, depending on the platform,
                    defined with the \l DestructiveRole.
    \value Apply An "Apply" button defined with the \l ApplyRole.
    \value Reset A "Reset" button defined with the \l ResetRole.
    \value RestoreDefaults A "Restore Defaults" button defined with the \l ResetRole.
    \value Help A "Help" button defined with the \l HelpRole.
    \value SaveAll A "Save All" button defined with the \l AcceptRole.
    \value Yes A "Yes" button defined with the \l YesRole.
    \value YesToAll A "Yes to All" button defined with the \l YesRole.
    \value No A "No" button defined with the \l NoRole.
    \value NoToAll A "No to All" button defined with the \l NoRole.
    \value Abort An "Abort" button defined with the \l RejectRole.
    \value Retry A "Retry" button defined with the \l AcceptRole.
    \value Ignore An "Ignore" button defined with the \l AcceptRole.

    \value NoButton An invalid button.

    \omitvalue FirstButton
    \omitvalue LastButton

    The following values are obsolete:

    \value YesAll Use YesToAll instead.
    \value NoAll Use NoToAll instead.
    \value Default Use the \c defaultButton argument of
           information(), warning(), etc. instead, or call
           setDefaultButton().
    \value Escape Call setEscapeButton() instead.
    \value FlagMask
    \value ButtonMask

    \sa ButtonRole, standardButtons
*/

/*!
    \fn void QMessageBox::buttonClicked(QAbstractButton *button)

    This signal is emitted whenever a button is clicked inside the QMessageBox.
    The button that was clicked in returned in \a button.
*/

/*!
    Constructs a message box with no text and no buttons. \a parent is
    passed to the QDialog constructor.

    On Mac OS X, if you want your message box to appear
    as a Qt::Sheet of its \a parent, set the message box's
    \l{setWindowModality()} {window modality} to Qt::WindowModal or use open().
    Otherwise, the message box will be a standard dialog.

*/
QMessageBox::QMessageBox(QWidget *parent)
    : QDialog(*new QMessageBoxPrivate, parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint)
{
    Q_D(QMessageBox);
    d->init();
}

/*!
    Constructs a message box with the given \a icon, \a title, \a
    text, and standard \a buttons. Standard or custom buttons can be
    added at any time using addButton(). The \a parent and \a f
    arguments are passed to the QDialog constructor.

    The message box is an \l{Qt::ApplicationModal} {application modal}
    dialog box.

    On Mac OS X, if \a parent is not 0 and you want your message box
    to appear as a Qt::Sheet of that parent, set the message box's
    \l{setWindowModality()} {window modality} to Qt::WindowModal
    (default). Otherwise, the message box will be a standard dialog.

    \sa setWindowTitle(), setText(), setIcon(), setStandardButtons()
*/
QMessageBox::QMessageBox(Icon icon, const QString &title, const QString &text,
                         StandardButtons buttons, QWidget *parent,
                         Qt::WindowFlags f)
: QDialog(*new QMessageBoxPrivate, parent, f | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint)
{
    Q_D(QMessageBox);
    d->init(title, text);
    setIcon(icon);
    if (buttons != NoButton)
        setStandardButtons(buttons);
}

/*!
    Destroys the message box.
*/
QMessageBox::~QMessageBox()
{
}

/*!
    \since 4.2

    Adds the given \a button to the message box with the specified \a
    role.

    \sa removeButton(), button(), setStandardButtons()
*/
void QMessageBox::addButton(QAbstractButton *button, ButtonRole role)
{
    Q_D(QMessageBox);
    if (!button)
        return;
    removeButton(button);
    d->buttonBox->addButton(button, (QDialogButtonBox::ButtonRole)role);
    d->customButtonList.append(button);
    d->autoAddOkButton = false;
}

/*!
    \since 4.2
    \overload

    Creates a button with the given \a text, adds it to the message box for the
    specified \a role, and returns it.
*/
QPushButton *QMessageBox::addButton(const QString& text, ButtonRole role)
{
    Q_D(QMessageBox);
    QPushButton *pushButton = new QPushButton(text);
    addButton(pushButton, role);
    d->updateSize();
    return pushButton;
}

/*!
    \since 4.2
    \overload

    Adds a standard \a button to the message box if it is valid to do so, and
    returns the push button.

    \sa setStandardButtons()
*/
QPushButton *QMessageBox::addButton(StandardButton button)
{
    Q_D(QMessageBox);
    QPushButton *pushButton = d->buttonBox->addButton((QDialogButtonBox::StandardButton)button);
    if (pushButton)
        d->autoAddOkButton = false;
    return pushButton;
}

/*!
    \since 4.2

    Removes \a button from the button box without deleting it.

    \sa addButton(), setStandardButtons()
*/
void QMessageBox::removeButton(QAbstractButton *button)
{
    Q_D(QMessageBox);
    d->customButtonList.removeAll(button);
    if (d->escapeButton == button)
        d->escapeButton = 0;
    if (d->defaultButton == button)
        d->defaultButton = 0;
    d->buttonBox->removeButton(button);
    d->updateSize();
}

/*!
    \property QMessageBox::standardButtons
    \brief collection of standard buttons in the message box
    \since 4.2

    This property controls which standard buttons are used by the message box.

    By default, this property contains no standard buttons.

    \sa addButton()
*/
void QMessageBox::setStandardButtons(StandardButtons buttons)
{
    Q_D(QMessageBox);
    d->buttonBox->setStandardButtons(QDialogButtonBox::StandardButtons(int(buttons)));

    QList<QAbstractButton *> buttonList = d->buttonBox->buttons();
    if (!buttonList.contains(d->escapeButton))
        d->escapeButton = 0;
    if (!buttonList.contains(d->defaultButton))
        d->defaultButton = 0;
    d->autoAddOkButton = false;
    d->updateSize();
}

QMessageBox::StandardButtons QMessageBox::standardButtons() const
{
    Q_D(const QMessageBox);
    return QMessageBox::StandardButtons(int(d->buttonBox->standardButtons()));
}

/*!
    \since 4.2

    Returns the standard button enum value corresponding to the given \a button,
    or NoButton if the given \a button isn't a standard button.

    \sa button(), standardButtons()
*/
QMessageBox::StandardButton QMessageBox::standardButton(QAbstractButton *button) const
{
    Q_D(const QMessageBox);
    return (QMessageBox::StandardButton)d->buttonBox->standardButton(button);
}

/*!
    \since 4.2

    Returns a pointer corresponding to the standard button \a which,
    or 0 if the standard button doesn't exist in this message box.

    \sa standardButtons, standardButton()
*/
QAbstractButton *QMessageBox::button(StandardButton which) const
{
    Q_D(const QMessageBox);
    return d->buttonBox->button(QDialogButtonBox::StandardButton(which));
}

/*!
    \since 4.2

    Returns the button that is activated when escape is pressed.

    By default, QMessageBox attempts to automatically detect an
    escape button as follows:

    \list 1
    \o If there is only one button, it is made the escape button.
    \o If there is a \l Cancel button, it is made the escape button.
    \o On Mac OS X only, if there is exactly one button with the role
       QMessageBox::RejectRole, it is made the escape button.
    \endlist

    When an escape button could not be automatically detected, pressing
    \key Esc has no effect.

    \sa addButton()
*/
QAbstractButton *QMessageBox::escapeButton() const
{
    Q_D(const QMessageBox);
    return d->escapeButton;
}

/*!
    \since 4.2

    Sets the button that gets activated when the \key Escape key is
    pressed to \a button.

    \sa addButton(), clickedButton()
*/
void QMessageBox::setEscapeButton(QAbstractButton *button)
{
    Q_D(QMessageBox);
    if (d->buttonBox->buttons().contains(button))
        d->escapeButton = button;
}

/*!
    \since 4.3

    Sets the buttons that gets activated when the \key Escape key is
    pressed to \a button.

    \sa addButton(), clickedButton()
*/
void QMessageBox::setEscapeButton(QMessageBox::StandardButton button)
{
    Q_D(QMessageBox);
    setEscapeButton(d->buttonBox->button(QDialogButtonBox::StandardButton(button)));
}

void QMessageBoxPrivate::detectEscapeButton()
{
    if (escapeButton) { // escape button explicitly set
        detectedEscapeButton = escapeButton;
        return;
    }

    // Cancel button automatically becomes escape button
    detectedEscapeButton = buttonBox->button(QDialogButtonBox::Cancel);
    if (detectedEscapeButton)
        return;

    // If there is only one button, make it the escape button
    const QList<QAbstractButton *> buttons = buttonBox->buttons();
    if (buttons.count() == 1) {
        detectedEscapeButton = buttons.first();
        return;
    }

    // if the message box has one RejectRole button, make it the escape button
    for (int i = 0; i < buttons.count(); i++) {
        if (buttonBox->buttonRole(buttons.at(i)) == QDialogButtonBox::RejectRole) {
            if (detectedEscapeButton) { // already detected!
                detectedEscapeButton = 0;
                break;
            }
            detectedEscapeButton = buttons.at(i);
        }
    }
    if (detectedEscapeButton)
        return;

    // if the message box has one NoRole button, make it the escape button
    for (int i = 0; i < buttons.count(); i++) {
        if (buttonBox->buttonRole(buttons.at(i)) == QDialogButtonBox::NoRole) {
            if (detectedEscapeButton) { // already detected!
                detectedEscapeButton = 0;
                break;
            }
            detectedEscapeButton = buttons.at(i);
        }
    }
}

/*!
    \since 4.2

    Returns the button that was clicked by the user,
    or 0 if the user hit the \key Esc key and
    no \l{setEscapeButton()}{escape button} was set.

    If exec() hasn't been called yet, returns 0.

    Example:

    \snippet doc/src/snippets/code/src_gui_dialogs_qmessagebox.cpp 3

    \sa standardButton(), button()
*/
QAbstractButton *QMessageBox::clickedButton() const
{
    Q_D(const QMessageBox);
    return d->clickedButton;
}

/*!
    \since 4.2

    Returns the button that should be the message box's
    \l{QPushButton::setDefault()}{default button}. Returns 0
    if no default button was set.

    \sa addButton(), QPushButton::setDefault()
*/
QPushButton *QMessageBox::defaultButton() const
{
    Q_D(const QMessageBox);
    return d->defaultButton;
}

/*!
    \since 4.2

    Sets the message box's \l{QPushButton::setDefault()}{default button}
    to \a button.

    \sa addButton(), QPushButton::setDefault()
*/
void QMessageBox::setDefaultButton(QPushButton *button)
{
    Q_D(QMessageBox);
    if (!d->buttonBox->buttons().contains(button))
        return;
    d->defaultButton = button;
    button->setDefault(true);
    button->setFocus();
}

/*!
    \since 4.3

    Sets the message box's \l{QPushButton::setDefault()}{default button}
    to \a button.

    \sa addButton(), QPushButton::setDefault()
*/
void QMessageBox::setDefaultButton(QMessageBox::StandardButton button)
{
    Q_D(QMessageBox);
    setDefaultButton(d->buttonBox->button(QDialogButtonBox::StandardButton(button)));
}

/*!
  \property QMessageBox::text
  \brief the message box text to be displayed.

  The text will be interpreted either as a plain text or as rich text,
  depending on the text format setting (\l QMessageBox::textFormat).
  The default setting is Qt::AutoText, i.e., the message box will try
  to auto-detect the format of the text.

  The default value of this property is an empty string.

  \sa textFormat, QMessageBox::informativeText, QMessageBox::detailedText
*/
QString QMessageBox::text() const
{
    Q_D(const QMessageBox);
    return d->label->text();
}

void QMessageBox::setText(const QString &text)
{
    Q_D(QMessageBox);
    d->label->setText(text);
    d->label->setWordWrap(d->label->textFormat() == Qt::RichText
        || (d->label->textFormat() == Qt::AutoText && Qt::mightBeRichText(text)));
    d->updateSize();
}

/*!
    \enum QMessageBox::Icon

    This enum has the following values:

    \value NoIcon the message box does not have any icon.

    \value Question an icon indicating that
    the message is asking a question.

    \value Information an icon indicating that
    the message is nothing out of the ordinary.

    \value Warning an icon indicating that the
    message is a warning, but can be dealt with.

    \value Critical an icon indicating that
    the message represents a critical problem.

*/

/*!
    \property QMessageBox::icon
    \brief the message box's icon

    The icon of the message box can be specified with one of the
    values:

    \list
    \o QMessageBox::NoIcon
    \o QMessageBox::Question
    \o QMessageBox::Information
    \o QMessageBox::Warning
    \o QMessageBox::Critical
    \endlist

    The default is QMessageBox::NoIcon.

    The pixmap used to display the actual icon depends on the current
    \l{QWidget::style()} {GUI style}. You can also set a custom pixmap
    for the icon by setting the \l{QMessageBox::iconPixmap} {icon
    pixmap} property.

    \sa iconPixmap
*/
QMessageBox::Icon QMessageBox::icon() const
{
    Q_D(const QMessageBox);
    return d->icon;
}

void QMessageBox::setIcon(Icon icon)
{
    Q_D(QMessageBox);
    setIconPixmap(QMessageBoxPrivate::standardIcon((QMessageBox::Icon)icon,
                                                   this));
    d->icon = icon;
}

/*!
    \property QMessageBox::iconPixmap
    \brief the current icon

    The icon currently used by the message box. Note that it's often
    hard to draw one pixmap that looks appropriate in all GUI styles;
    you may want to supply a different pixmap for each platform.

    By default, this property is undefined.

    \sa icon
*/
QPixmap QMessageBox::iconPixmap() const
{
    Q_D(const QMessageBox);
    if (d->iconLabel && d->iconLabel->pixmap())
        return *d->iconLabel->pixmap();
    return QPixmap();
}

void QMessageBox::setIconPixmap(const QPixmap &pixmap)
{
    Q_D(QMessageBox);
    d->iconLabel->setPixmap(pixmap);
    d->updateSize();
    d->icon = NoIcon;
}

/*!
    \property QMessageBox::textFormat
    \brief the format of the text displayed by the message box

    The current text format used by the message box. See the \l
    Qt::TextFormat enum for an explanation of the possible options.

    The default format is Qt::AutoText.

    \sa setText()
*/
Qt::TextFormat QMessageBox::textFormat() const
{
    Q_D(const QMessageBox);
    return d->label->textFormat();
}

void QMessageBox::setTextFormat(Qt::TextFormat format)
{
    Q_D(QMessageBox);
    d->label->setTextFormat(format);
    d->label->setWordWrap(format == Qt::RichText
                    || (format == Qt::AutoText && Qt::mightBeRichText(d->label->text())));
    d->updateSize();
}

/*!
    \reimp
*/
bool QMessageBox::event(QEvent *e)
{
    bool result =QDialog::event(e);
    switch (e->type()) {
        case QEvent::LayoutRequest:
            d_func()->updateSize();
            break;
        case QEvent::LanguageChange:
            d_func()->retranslateStrings();
            break;
        default:
            break;
    }
    return result;
}

/*!
    \reimp
*/
void QMessageBox::closeEvent(QCloseEvent *e)
{
    Q_D(QMessageBox);
    if (!d->detectedEscapeButton) {
        e->ignore();
        return;
    }
    QDialog::closeEvent(e);
    d->clickedButton = d->detectedEscapeButton;
    setResult(d->execReturnCode(d->detectedEscapeButton));
}

/*!
    \reimp
*/
void QMessageBox::changeEvent(QEvent *ev)
{
    Q_D(QMessageBox);
    switch (ev->type()) {
    case QEvent::StyleChange:
    {
        if (d->icon != NoIcon)
            setIcon(d->icon);
        Qt::TextInteractionFlags flags(style()->styleHint(QStyle::SH_MessageBox_TextInteractionFlags, 0, this));
        d->label->setTextInteractionFlags(flags);
        d->buttonBox->setCenterButtons(style()->styleHint(QStyle::SH_MessageBox_CenterButtons, 0, this));
        if (d->informativeLabel)
            d->informativeLabel->setTextInteractionFlags(flags);
        // intentional fall through
    }
    case QEvent::FontChange:
    case QEvent::ApplicationFontChange:
    default:
        break;
    }
    QDialog::changeEvent(ev);
}

/*!
    \reimp
*/
void QMessageBox::keyPressEvent(QKeyEvent *e)
{
    Q_D(QMessageBox);
    if (e->key() == Qt::Key_Escape) {
        if (d->detectedEscapeButton) {
            d->detectedEscapeButton->click();
        }
        return;
    }

#ifndef QT_NO_SHORTCUT
    if (!(e->modifiers() & Qt::AltModifier)) {
        int key = e->key() & ~Qt::MODIFIER_MASK;
        if (key) {
            const QList<QAbstractButton *> buttons = d->buttonBox->buttons();
            for (int i = 0; i < buttons.count(); ++i) {
                QAbstractButton *pb = buttons.at(i);
                QKeySequence shortcut = pb->shortcut();
                if (!shortcut.isEmpty() && key == (shortcut[0] & ~Qt::MODIFIER_MASK)) {
                    pb->animateClick();
                    return;
                }
            }
        }
    }
#endif
    QDialog::keyPressEvent(e);
}

/*!
    \overload

    Opens the dialog and connects its finished() or buttonClicked() signal to
    the slot specified by \a receiver and \a member. If the slot in \a member
    has a pointer for its first parameter the connection is to buttonClicked(),
    otherwise the connection is to finished().

    The signal will be disconnected from the slot when the dialog is closed.
*/
void QMessageBox::open(QObject *receiver, const char *member)
{
    Q_D(QMessageBox);
    const char *signal = member && strchr(member, '*') ? SIGNAL(buttonClicked(QAbstractButton*))
                                                       : SIGNAL(finished(int));
    connect(this, signal, receiver, member);
    d->signalToDisconnectOnClose = signal;
    d->receiverToDisconnectOnClose = receiver;
    d->memberToDisconnectOnClose = member;
    QDialog::open();
}

/*!
    \since 4.5

    Returns a list of all the buttons that have been added to the message box.

    \sa buttonRole(), addButton(), removeButton()
*/
QList<QAbstractButton *> QMessageBox::buttons() const
{
    Q_D(const QMessageBox);
    return d->buttonBox->buttons();
}

/*!
    \since 4.5

    Returns the button role for the specified \a button. This function returns
    \l InvalidRole if \a button is 0 or has not been added to the message box.

    \sa buttons(), addButton()
*/
QMessageBox::ButtonRole QMessageBox::buttonRole(QAbstractButton *button) const
{
    Q_D(const QMessageBox);
    return QMessageBox::ButtonRole(d->buttonBox->buttonRole(button));
}

/*!
    \reimp
*/
void QMessageBox::showEvent(QShowEvent *e)
{
    Q_D(QMessageBox);
    if (d->autoAddOkButton) {
        addButton(Ok);
    }
    if (d->detailsButton)
        addButton(d->detailsButton, QMessageBox::ActionRole);
    d->detectEscapeButton();
    d->updateSize();

    QDialog::showEvent(e);
}


static QMessageBox::StandardButton showNewMessageBox(QWidget *parent,
    QMessageBox::Icon icon,
    const QString& title, const QString& text,
    QMessageBox::StandardButtons buttons,
    QMessageBox::StandardButton defaultButton)
{
    QMessageBox msgBox(icon, title, text, QMessageBox::NoButton, parent);
    QDialogButtonBox *buttonBox = msgBox.findChild<QDialogButtonBox*>();
    Q_ASSERT(buttonBox != 0);

    uint mask = QMessageBox::FirstButton;
    while (mask <= QMessageBox::LastButton) {
        uint sb = buttons & mask;
        mask <<= 1;
        if (!sb)
            continue;
        QPushButton *button = msgBox.addButton((QMessageBox::StandardButton)sb);
        // Choose the first accept role as the default
        if (msgBox.defaultButton())
            continue;
        if ((defaultButton == QMessageBox::NoButton && buttonBox->buttonRole(button) == QDialogButtonBox::AcceptRole)
            || (defaultButton != QMessageBox::NoButton && sb == uint(defaultButton)))
            msgBox.setDefaultButton(button);
    }
    if (msgBox.exec() == -1)
        return QMessageBox::Cancel;
    return msgBox.standardButton(msgBox.clickedButton());
}

/*!
    \since 4.2

    Opens an information message box with the given \a title and
    \a text in front of the specified \a parent widget.

    The standard \a buttons are added to the message box.
    \a defaultButton specifies the button used when \key Enter is pressed.
    \a defaultButton must refer to a button that was given in \a buttons.
    If \a defaultButton is QMessageBox::NoButton, QMessageBox
    chooses a suitable default automatically.

    Returns the identity of the standard button that was clicked. If
    \key Esc was pressed instead, the \l{Default and Escape Keys}
    {escape button} is returned.

    The message box is an \l{Qt::ApplicationModal}{application modal}
    dialog box.

    \warning Do not delete \a parent during the execution of the dialog.
             If you want to do this, you should create the dialog
             yourself using one of the QMessageBox constructors.

    \sa question(), warning(), critical()
*/
QMessageBox::StandardButton QMessageBox::information(QWidget *parent, const QString &title,
                               const QString& text, StandardButtons buttons,
                               StandardButton defaultButton)
{
    return showNewMessageBox(parent, Information, title, text, buttons,
                             defaultButton);
}


/*!
    \since 4.2

    Opens a question message box with the given \a title and \a
    text in front of the specified \a parent widget.

    The standard \a buttons are added to the message box. \a
    defaultButton specifies the button used when \key Enter is
    pressed. \a defaultButton must refer to a button that was given in \a buttons.
    If \a defaultButton is QMessageBox::NoButton, QMessageBox
    chooses a suitable default automatically.

    Returns the identity of the standard button that was clicked. If
    \key Esc was pressed instead, the \l{Default and Escape Keys}
    {escape button} is returned.

    The message box is an \l{Qt::ApplicationModal} {application modal}
    dialog box.

    \warning Do not delete \a parent during the execution of the dialog.
             If you want to do this, you should create the dialog
             yourself using one of the QMessageBox constructors.

    \sa information(), warning(), critical()
*/
QMessageBox::StandardButton QMessageBox::question(QWidget *parent, const QString &title,
                            const QString& text, StandardButtons buttons,
                            StandardButton defaultButton)
{
    return showNewMessageBox(parent, Question, title, text, buttons, defaultButton);
}

/*!
    \since 4.2

    Opens a warning message box with the given \a title and \a
    text in front of the specified \a parent widget.

    The standard \a buttons are added to the message box. \a
    defaultButton specifies the button used when \key Enter is
    pressed. \a defaultButton must refer to a button that was given in \a buttons.
    If \a defaultButton is QMessageBox::NoButton, QMessageBox
    chooses a suitable default automatically.

    Returns the identity of the standard button that was clicked. If
    \key Esc was pressed instead, the \l{Default and Escape Keys}
    {escape button} is returned.

    The message box is an \l{Qt::ApplicationModal} {application modal}
    dialog box.

    \warning Do not delete \a parent during the execution of the dialog.
             If you want to do this, you should create the dialog
             yourself using one of the QMessageBox constructors.

    \sa question(), information(), critical()
*/
QMessageBox::StandardButton QMessageBox::warning(QWidget *parent, const QString &title,
                        const QString& text, StandardButtons buttons,
                        StandardButton defaultButton)
{
    return showNewMessageBox(parent, Warning, title, text, buttons, defaultButton);
}

/*!
    \since 4.2

    Opens a critical message box with the given \a title and \a
    text in front of the specified \a parent widget.

    The standard \a buttons are added to the message box. \a
    defaultButton specifies the button used when \key Enter is
    pressed. \a defaultButton must refer to a button that was given in \a buttons.
    If \a defaultButton is QMessageBox::NoButton, QMessageBox
    chooses a suitable default automatically.

    Returns the identity of the standard button that was clicked. If
    \key Esc was pressed instead, the \l{Default and Escape Keys}
    {escape button} is returned.

    The message box is an \l{Qt::ApplicationModal} {application modal}
    dialog box.

    \warning Do not delete \a parent during the execution of the dialog.
             If you want to do this, you should create the dialog
             yourself using one of the QMessageBox constructors.

    \sa question(), warning(), information()
*/
QMessageBox::StandardButton QMessageBox::critical(QWidget *parent, const QString &title,
                         const QString& text, StandardButtons buttons,
                         StandardButton defaultButton)
{
    return showNewMessageBox(parent, Critical, title, text, buttons, defaultButton);
}

/*!
    Displays a simple about box with title \a title and text \a
    text. The about box's parent is \a parent.

    about() looks for a suitable icon in four locations:

    \list 1
    \o It prefers \link QWidget::windowIcon() parent->icon() \endlink
    if that exists.
    \o If not, it tries the top-level widget containing \a parent.
    \o If that fails, it tries the \link
    QApplication::activeWindow() active window. \endlink
    \o As a last resort it uses the Information icon.
    \endlist

    The about box has a single button labelled "OK". On Mac OS X, the
    about box is popped up as a modeless window; on other platforms,
    it is currently application modal.

    \sa QWidget::windowIcon(), QApplication::activeWindow()
*/
void QMessageBox::about(QWidget *parent, const QString &title, const QString &text)
{
    QMessageBox *msgBox = new QMessageBox(Information, title, text, 0, parent);
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    QIcon icon = msgBox->windowIcon();
    QSize size = icon.actualSize(QSize(64, 64));
    msgBox->setIconPixmap(icon.pixmap(size));

    msgBox->exec();
}

/*!
    Displays a simple message box about Qt, with the given \a title
    and centered over \a parent (if \a parent is not 0). The message
    includes the version number of Qt being used by the application.

    This is useful for inclusion in the \gui Help menu of an application,
    as shown in the \l{mainwindows/menus}{Menus} example.

    QApplication provides this functionality as a slot.

    \sa QApplication::aboutQt()
*/
void QMessageBox::aboutQt(QWidget *parent, const QString &title)
{
    QString translatedTextAboutQtCaption = QMessageBox::tr(
        "<h3>About Katie</h3>"
        "<p>This program uses Katie version %1.</p>"
        ).arg(QLatin1String(QT_VERSION_STR));
    QString translatedTextAboutQtText = QMessageBox::tr(
        "<p>Katie is a C++ toolkit derived from the Qt 4.8 framework.</p>"
        "<p>Katie is licensed under the GNU LGPL version 2.1</p>"
        "<p>Copyright (C) 2015 The Qt Company Ltd and other contributors.</p>"
        "<p>Copyright (C) 2016 Ivailo Monev</p>"
        "<p>See <a href=\"https://github.com/fluxer/katie\">github.com/fluxer/katie</a> for more information.</p>"
        );
    QMessageBox *msgBox = new QMessageBox(parent);
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setWindowTitle(title.isEmpty() ? tr("About Katie") : title);
    msgBox->setText(translatedTextAboutQtCaption);
    msgBox->setInformativeText(translatedTextAboutQtText);

    QPixmap pm;
    pm.loadFromData(qrc_katie(), qrc_katie_size, "PNG");
    if (!pm.isNull())
        msgBox->setIconPixmap(pm);

    msgBox->exec();
}

/////////////////////////////////////////////////////////////////////////////////////////
// Source and binary compatibility routines for 4.0 and 4.1

void QMessageBoxPrivate::retranslateStrings()
{
#ifndef QT_NO_TEXTEDIT
    if (detailsButton)
        detailsButton->setLabel(detailsText->isHidden() ? ShowLabel : HideLabel);
#endif
}

#ifndef QT_NO_TEXTEDIT
/*!
  \property QMessageBox::detailedText
  \brief the text to be displayed in the details area.
  \since 4.2

  The text will be interpreted as a plain text.

  By default, this property contains an empty string.

  \sa QMessageBox::text, QMessageBox::informativeText
*/
QString QMessageBox::detailedText() const
{
    Q_D(const QMessageBox);
    return d->detailsText ? d->detailsText->text() : QString();
}

void QMessageBox::setDetailedText(const QString &text)
{
    Q_D(QMessageBox);
    if (text.isEmpty()) {
        delete d->detailsText;
        d->detailsText = 0;
        removeButton(d->detailsButton);
        delete d->detailsButton;
        d->detailsButton = 0;
        return;
    }

    if (!d->detailsText) {
        d->detailsText = new QMessageBoxDetailsText(this);
        QGridLayout* grid = qobject_cast<QGridLayout*>(layout());
        if (grid)
            grid->addWidget(d->detailsText, grid->rowCount(), 0, 1, grid->columnCount());
        d->detailsText->hide();
    }
    if (!d->detailsButton)
        d->detailsButton = new DetailButton(this);
    d->detailsText->setText(text);
}
#endif // QT_NO_TEXTEDIT

/*!
  \property QMessageBox::informativeText

  \brief the informative text that provides a fuller description for
  the message

  \since 4.2

  Infromative text can be used to expand upon the text() to give more
  information to the user. On the Mac, this text appears in small
  system font below the text().  On other platforms, it is simply
  appended to the existing text.

  By default, this property contains an empty string.

  \sa QMessageBox::text, QMessageBox::detailedText
*/
QString QMessageBox::informativeText() const
{
    Q_D(const QMessageBox);
    return d->informativeLabel ? d->informativeLabel->text() : QString();
}

void QMessageBox::setInformativeText(const QString &text)
{
    Q_D(QMessageBox);
    if (text.isEmpty()) {
        layout()->removeWidget(d->informativeLabel);
        delete d->informativeLabel;
        d->informativeLabel = 0;
        d->label->setContentsMargins(2, 0, 0, 0);
        d->updateSize();
        return;
    }

    if (!d->informativeLabel) {
        QLabel *label = new QLabel(this);
        label->setObjectName(QLatin1String("qt_msgbox_informativelabel"));
        label->setTextInteractionFlags(Qt::TextInteractionFlags(style()->styleHint(QStyle::SH_MessageBox_TextInteractionFlags, 0, this)));
        label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        label->setOpenExternalLinks(true);
        label->setWordWrap(true);
        d->label->setContentsMargins(2, 0, 0, 0);
        label->setContentsMargins(2, 0, 0, 6);
        label->setIndent(9);
        label->setWordWrap(true);
        QGridLayout *grid = static_cast<QGridLayout *>(layout());
        grid->addWidget(label, 1, 1, 1, 1);
        d->informativeLabel = label;
    }
    d->informativeLabel->setText(text);

    d->updateSize();
}

/*!
    \since 4.2

    This function shadows QWidget::setWindowModality().

    Sets the modality of the message box to \a windowModality.

    On Mac OS X, if the modality is set to Qt::WindowModal and the message box
    has a parent, then the message box will be a Qt::Sheet, otherwise the
    message box will be a standard dialog.
*/
void QMessageBox::setWindowModality(Qt::WindowModality windowModality)
{
    QDialog::setWindowModality(windowModality);

    if (parentWidget() && windowModality == Qt::WindowModal)
        setParent(parentWidget(), Qt::Sheet);
    else
        setParent(parentWidget(), Qt::Dialog);
    setDefaultButton(d_func()->defaultButton);
}


QPixmap QMessageBoxPrivate::standardIcon(QMessageBox::Icon icon, QMessageBox *mb)
{
    QStyle *style = mb ? mb->style() : QApplication::style();
    int iconSize = style->pixelMetric(QStyle::PM_MessageBoxIconSize, 0, mb);
    QIcon tmpIcon;
    switch (icon) {
    case QMessageBox::Information:
        tmpIcon = style->standardIcon(QStyle::SP_MessageBoxInformation, 0, mb);
        break;
    case QMessageBox::Warning:
        tmpIcon = style->standardIcon(QStyle::SP_MessageBoxWarning, 0, mb);
        break;
    case QMessageBox::Critical:
        tmpIcon = style->standardIcon(QStyle::SP_MessageBoxCritical, 0, mb);
        break;
    case QMessageBox::Question:
        tmpIcon = style->standardIcon(QStyle::SP_MessageBoxQuestion, 0, mb);
    default:
        break;
    }
    if (!tmpIcon.isNull())
        return tmpIcon.pixmap(iconSize, iconSize);
    return QPixmap();
}

/*!
    \obsolete

    Returns the pixmap used for a standard icon. This allows the
    pixmaps to be used in more complex message boxes. \a icon
    specifies the required icon, e.g. QMessageBox::Question,
    QMessageBox::Information, QMessageBox::Warning or
    QMessageBox::Critical.

    Call QStyle::standardIcon() with QStyle::SP_MessageBoxInformation etc.
    instead.
*/

QPixmap QMessageBox::standardIcon(Icon icon)
{
    return QMessageBoxPrivate::standardIcon(icon, 0);
}

/*!
    \typedef QMessageBox::Button
    \obsolete

    Use QMessageBox::StandardButton instead.
*/

/*!
  \fn int QMessageBox::exec()

  Shows the message box as a \l{QDialog#Modal Dialogs}{modal dialog},
  blocking until the user closes it.

  When using a QMessageBox with standard buttons, this functions returns a
  \l StandardButton value indicating the standard button that was clicked.
  When using QMessageBox with custom buttons, this function returns an
  opaque value; use clickedButton() to determine which button was clicked.

  \note The result() function returns also \l StandardButton value instead of \l QDialog::DialogCode

  Users cannot interact with any other window in the same
  application until they close the dialog, either by clicking a
  button or by using a mechanism provided by the window system.

  \sa show(), result()
*/

QT_END_NAMESPACE

#include "moc_qmessagebox.h"
#include "qrc_qmessagebox.cpp"

#endif // QT_NO_MESSAGEBOX
