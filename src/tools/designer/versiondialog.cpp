/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
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

#include <QtCore/QVector>
#include <QtGui/qevent.h>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QStyleOption>
#include "versiondialog.h"

QT_BEGIN_NAMESPACE

class VersionLabel : public QLabel
{
    Q_OBJECT
public:
    VersionLabel(QWidget *parent = Q_NULLPTR);

signals:
    void triggered();

protected:
    void mousePressEvent(QMouseEvent *me);
    void mouseMoveEvent(QMouseEvent *me);
    void mouseReleaseEvent(QMouseEvent *me);
    void paintEvent(QPaintEvent *pe);
private:
    QVector<QPoint> hitPoints;
    QVector<QPoint> missPoints;
    QPainterPath m_path;
    bool secondStage;
    bool m_pushed;
};

VersionLabel::VersionLabel(QWidget *parent)
        : QLabel(parent), secondStage(false), m_pushed(false)
{
    setPixmap(QPixmap(QLatin1String(":/trolltech/designer/designer.png")));
    hitPoints.append(QPoint(56, 25));
    hitPoints.append(QPoint(29, 55));
    hitPoints.append(QPoint(56, 87));
    hitPoints.append(QPoint(82, 55));
    hitPoints.append(QPoint(58, 56));

    secondStage = false;
    m_pushed = false;
}

void VersionLabel::mousePressEvent(QMouseEvent *me)
{
    if (me->button() == Qt::LeftButton) {
        if (!secondStage) {
            m_path = QPainterPath(me->pos());
        } else {
            m_pushed = true;
            update();
        }
    }
}

void VersionLabel::mouseMoveEvent(QMouseEvent *me)
{
    if (me->buttons() & Qt::LeftButton)
        if (!secondStage)
            m_path.lineTo(me->pos());
}

void VersionLabel::mouseReleaseEvent(QMouseEvent *me)
{
    if (me->button() == Qt::LeftButton) {
        if (!secondStage) {
            m_path.lineTo(me->pos());
            bool gotIt = true;
            foreach(const QPoint &pt, hitPoints) {
                if (!m_path.contains(pt)) {
                    gotIt = false;
                    break;
                }
            }
            if (gotIt) {
                foreach(const QPoint &pt, missPoints) {
                    if (m_path.contains(pt)) {
                        gotIt = false;
                        break;
                    }
                }
            }
            if (gotIt && !secondStage) {
                secondStage = true;
                m_path = QPainterPath();
                update();
            }
        } else {
            m_pushed = false;
            update();
            emit triggered();
        }
    }
}

void VersionLabel::paintEvent(QPaintEvent *pe)
{
    if (secondStage) {
        QPainter p(this);
        QStyleOptionButton opt;
        opt.init(this);
        if (!m_pushed)
            opt.state |= QStyle::State_Raised;
        else
            opt.state |= QStyle::State_Sunken;
        opt.state &= ~QStyle::State_HasFocus;
        style()->drawControl(QStyle::CE_PushButtonBevel, &opt, &p, this);
    }
    QLabel::paintEvent(pe);
}

VersionDialog::VersionDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QGridLayout *layout = new QGridLayout(this);
    VersionLabel *label = new VersionLabel;
    QLabel *lbl = new QLabel;
    QString version = tr("<h3>%1</h3><br/><br/>Version %2");
    version = version.arg(tr("Katie Designer")).arg(QLatin1String(QT_VERSION_STR));
    version.append(tr("<br/>Katie Designer is a graphical user interface designer for Qt applications.<br/>"));

    lbl->setText(tr("%1"
                    "<br/>Copyright (C) 2015 The Qt Company Ltd."
                    "<br/>Copyright (C) 2016-2020 Ivailo Monev"
                    ).arg(version));

    lbl->setWordWrap(true);
    lbl->setOpenExternalLinks(true);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttonBox , SIGNAL(rejected()), this, SLOT(reject()));
    connect(label, SIGNAL(triggered()), this, SLOT(accept()));
    layout->addWidget(label, 0, 0, 1, 1);
    layout->addWidget(lbl, 0, 1, 4, 4);
    layout->addWidget(buttonBox, 4, 2, 1, 1);
}

QT_END_NAMESPACE

#include "moc_versiondialog.cpp"
#include "moc_versiondialog.h"
