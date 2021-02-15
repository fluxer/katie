/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the test suite module of the Katie Toolkit.
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

#include <QtTest/QtTest>
#include <QtCore/QDirIterator>
#include "qplatformdefs.h"

QT_USE_NAMESPACE

class bench_QDir_10000 : public QObject{
  Q_OBJECT
public slots:
    void initTestCase() {
        QDir testdir = QDir::tempPath();

        const QString subfolder_name = QLatin1String("test_speed");
        QVERIFY(testdir.mkdir(subfolder_name));
        QVERIFY(testdir.cd(subfolder_name));

        for (uint i=0; i<10000; ++i) {
            QFile file(testdir.absolutePath() + QLatin1String("/testfile_") + QString::number(i));
            file.open(QIODevice::WriteOnly);
        }
    }
    void cleanupTestCase() {
        {
            QDir testdir(QDir::tempPath() + QLatin1String("/test_speed"));
            testdir.setSorting(QDir::Unsorted);
            testdir.setFilter(QDir::AllEntries | QDir::System | QDir::Hidden);
            foreach (const QString &filename, testdir.entryList()) {
                testdir.remove(filename);
            }
        }
        const QDir temp = QDir(QDir::tempPath());
        temp.rmdir(QLatin1String("test_speed"));
    }
private slots:
    void baseline() {}

    void sizeSpeed() {
        QDir testdir(QDir::tempPath() + QLatin1String("/test_speed"));
        QBENCHMARK {
            QFileInfoList fileInfoList = testdir.entryInfoList(QDir::Files, QDir::Unsorted);
            foreach (const QFileInfo &fileInfo, fileInfoList) {
                fileInfo.isDir();
                fileInfo.size();
            }
        }
    }
    void sizeSpeedIterator() {
        QDir testdir(QDir::tempPath() + QLatin1String("/test_speed"));
        QBENCHMARK {
            QDirIterator dit(testdir.path(), QDir::Files);
            while (dit.hasNext()) {
                dit.next();
                dit.fileInfo().isDir();
                dit.fileInfo().size();
            }
        }
    }

    void sizeSpeedWithoutFilter() {
        QDir testdir(QDir::tempPath() + QLatin1String("/test_speed"));
        QBENCHMARK {
            QFileInfoList fileInfoList = testdir.entryInfoList(QDir::NoFilter, QDir::Unsorted);
            foreach (const QFileInfo &fileInfo, fileInfoList) {
                fileInfo.size();
            }
        }
    }
    void sizeSpeedWithoutFilterIterator() {
        QDir testdir(QDir::tempPath() + QLatin1String("/test_speed"));
        QBENCHMARK {
            QDirIterator dit(testdir.path());
            while (dit.hasNext()) {
                dit.next();
                dit.fileInfo().isDir();
                dit.fileInfo().size();
            }
        }
    }

    void sizeSpeedWithoutFileInfoList() {
        QDir testdir(QDir::tempPath() + QLatin1String("/test_speed"));
        testdir.setSorting(QDir::Unsorted);
        QBENCHMARK {
            QStringList fileList = testdir.entryList(QDir::NoFilter, QDir::Unsorted);
            foreach (const QString &filename, fileList) {
                QFileInfo fileInfo(filename);
                fileInfo.size();
            }
        }
    }

    void iDontWantAnyStat() {
        QDir testdir(QDir::tempPath() + QLatin1String("/test_speed"));
        testdir.setSorting(QDir::Unsorted);
        testdir.setFilter(QDir::AllEntries | QDir::System | QDir::Hidden);
        QBENCHMARK {
            testdir.entryList(QDir::NoFilter, QDir::Unsorted);
        }
    }
    void iDontWantAnyStatIterator() {
        QBENCHMARK {
            QDirIterator dit(QDir::tempPath() + QLatin1String("/test_speed"));
            while (dit.hasNext()) {
                dit.next();
            }
        }
    }

    void sizeSpeedWithoutFilterLowLevel() {
        QDir testdir(QDir::tempPath() + QLatin1String("/test_speed"));
        DIR *dir = QT_OPENDIR(qPrintable(testdir.absolutePath()));
        QVERIFY(dir);

        QVERIFY(!QT_CHDIR(qPrintable(testdir.absolutePath())));
        QBENCHMARK {
            QT_DIRENT *item = QT_READDIR(dir);
            while (item) {
                char *fileName = item->d_name;

                QT_STATBUF fileStat;
                QVERIFY(!QT_STAT(fileName, &fileStat));

                item = QT_READDIR(dir);
            }
        }
        QT_CLOSEDIR(dir);
    }
};

QTEST_MAIN(bench_QDir_10000)

#include "moc_bench_qdir_10000.cpp"
