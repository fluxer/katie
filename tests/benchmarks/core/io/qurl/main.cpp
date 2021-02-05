/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the test suite of the Katie Toolkit.
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

#include <qurl.h>
#include <qtest.h>

QT_USE_NAMESPACE

class tst_qurl: public QObject
{
    Q_OBJECT

private slots:
    void emptyUrl();
    void relativeUrl();
    void absoluteUrl();
    void isRelative_data();
    void isRelative();
    void toLocalFile_data();
    void toLocalFile();
    void toString_data();
    void toString();
    void toEncoded_data();
    void toEncoded();
    void resolved_data();
    void resolved();
    void equality_data();
    void equality();
    void qmlPropertyWriteUseCase();

private:
    void generateFirstRunData();
};

void tst_qurl::emptyUrl()
{
    QBENCHMARK {
        QUrl url;
    }
}

void tst_qurl::relativeUrl()
{
    QBENCHMARK {
        QUrl url(QLatin1String("pics/avatar.png"));
    }
}

void tst_qurl::absoluteUrl()
{
    QBENCHMARK {
        QUrl url(QLatin1String("/tmp/avatar.png"));
    }
}

void tst_qurl::generateFirstRunData()
{
    QTest::addColumn<bool>("firstRun");

    QTest::newRow("construction + first run") << true;
    QTest::newRow("subsequent runs") << false;
}

void tst_qurl::isRelative_data()
{
    generateFirstRunData();
}

void tst_qurl::isRelative()
{
    QFETCH(bool, firstRun);
    if (firstRun) {
        QBENCHMARK {
            QUrl url(QLatin1String("pics/avatar.png"));
            url.isRelative();
        }
    } else {
        QUrl url(QLatin1String("pics/avatar.png"));
        QBENCHMARK {
            url.isRelative();
        }
    }
}

void tst_qurl::toLocalFile_data()
{
    generateFirstRunData();
}

void tst_qurl::toLocalFile()
{
    QFETCH(bool, firstRun);
    if (firstRun) {
        QBENCHMARK {
            QUrl url(QLatin1String("/tmp/avatar.png"));
            url.toLocalFile();
        }
    } else {
        QUrl url(QLatin1String("/tmp/avatar.png"));
        QBENCHMARK {
            url.toLocalFile();
        }
    }
}

void tst_qurl::toString_data()
{
    generateFirstRunData();
}

void tst_qurl::toString()
{
    QFETCH(bool, firstRun);
    if(firstRun) {
        QBENCHMARK {
            QUrl url(QLatin1String("pics/avatar.png"));
            url.toString();
        }
    } else {
        QUrl url(QLatin1String("pics/avatar.png"));
        QBENCHMARK {
            url.toString();
        }
    }
}

void tst_qurl::toEncoded_data()
{
   generateFirstRunData();
}

void tst_qurl::toEncoded()
{
   QFETCH(bool, firstRun);
   if(firstRun) {
       QBENCHMARK {
           QUrl url(QLatin1String("pics/avatar.png"));
           url.toEncoded(QUrl::FormattingOption(0x100));
       }
   } else {
       QUrl url(QLatin1String("pics/avatar.png"));
       QBENCHMARK {
           url.toEncoded(QUrl::FormattingOption(0x100));
       }
   }
}

void tst_qurl::resolved_data()
{
   generateFirstRunData();
}

void tst_qurl::resolved()
{
    QFETCH(bool, firstRun);
    if(firstRun) {
        QBENCHMARK {
            QUrl baseUrl(QLatin1String("/home/user/"));
            QUrl url(QLatin1String("pics/avatar.png"));
            baseUrl.resolved(url);
        }
    } else {
        QUrl baseUrl(QLatin1String("/home/user/"));
        QUrl url(QLatin1String("pics/avatar.png"));
        QBENCHMARK {
            baseUrl.resolved(url);
        }
    }
}

void tst_qurl::equality_data()
{
   generateFirstRunData();
}

void tst_qurl::equality()
{
    QFETCH(bool, firstRun);
    if(firstRun) {
        QBENCHMARK {
            QUrl url(QLatin1String("pics/avatar.png"));
            QUrl url2(QLatin1String("pics/avatar2.png"));
            //url == url2;
        }
    } else {
        QUrl url(QLatin1String("pics/avatar.png"));
        QUrl url2(QLatin1String("pics/avatar2.png"));
        QBENCHMARK {
            (void)(url == url2);
        }
    }
}

void tst_qurl::qmlPropertyWriteUseCase()
{
    QUrl base(QLatin1String("file:///home/user/qt/demos/declarative/samegame/SamegameCore/"));
    QString str(QLatin1String("pics/redStar.png"));

    QBENCHMARK {
        QUrl u = QUrl(str);
        if (!u.isEmpty() && u.isRelative())
            u = base.resolved(u);
    }
}

QTEST_MAIN(tst_qurl)

#include "moc_main.cpp"
