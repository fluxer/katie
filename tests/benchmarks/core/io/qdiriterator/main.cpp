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
** $QT_END_LICENSE$
**
****************************************************************************/
#include <QDebug>
#include <QDirIterator>
#include <QString>
#include <qtest.h>
#include "qplatformdefs.h"
#include "qfilesystemiterator.h"

QT_USE_NAMESPACE

class tst_qdiriterator : public QObject
{
    Q_OBJECT
private slots:
    void posix();
    void posix_data() { data(); }
    void diriterator();
    void diriterator_data() { data(); }
    void fsiterator();
    void fsiterator_data() { data(); }
    void data();
};


void tst_qdiriterator::data()
{
    QTest::addColumn<QString>("dirpath");
    QString absolute = QDir::cleanPath(QString::fromLatin1(SRCDIR "/../.."));
    QTest::newRow(absolute.toLatin1()) << absolute;
}

static int posix_helper(const char *dirpath)
{
    //qDebug() << "DIR" << dirpath;
    DIR *dir = QT_OPENDIR(dirpath);
    if (!dir)
        return 0;

    QT_DIRENT *entry = 0;

    int count = 0;
    while ((entry = QT_READDIR(dir))) {
        if (qstrcmp(entry->d_name, ".") == 0)
            continue;
        if (qstrcmp(entry->d_name, "..") == 0)
            continue;
        ++count;
        QByteArray ba = dirpath;
        ba += '/';
        ba += entry->d_name;
        QT_STATBUF st;
        QT_LSTAT(ba.constData(), &st);
        if (S_ISDIR(st.st_mode))
            count += posix_helper(ba.constData());
    }

    QT_CLOSEDIR(dir);
    return count;
}


void tst_qdiriterator::posix()
{
    QFETCH(QString, dirpath);

    int count = 0;
    QByteArray path = dirpath.toLatin1();
    QBENCHMARK {
        count = posix_helper(path.constData());
    }
    qDebug() << count;
}

void tst_qdiriterator::diriterator()
{
    QFETCH(QString, dirpath);

    int count = 0;

    QBENCHMARK {
        int c = 0;

        QDirIterator dir(dirpath,
            //QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot,
            //QDir::AllEntries | QDir::Hidden,
            QDir::Files,
            QDirIterator::Subdirectories);

        while (dir.hasNext()) {
            dir.next();
            //printf("%s\n", qPrintable(dir.fileName()));
            0 && printf("%d %s\n",
                dir.fileInfo().isDir(),
                //qPrintable(dir.fileInfo().absoluteFilePath()),
                //qPrintable(dir.path()),
                qPrintable(dir.filePath()));
            ++c;
        }
        count = c;
    }
    qDebug() << count;
}

void tst_qdiriterator::fsiterator()
{
    QFETCH(QString, dirpath);

    int count = 0;
    int dump = 0;

    QBENCHMARK {
        int c = 0;

        dump && printf("\n\n\n\n");
        QFileSystemIterator dir(dirpath,
            //QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot,
            //QDir::AllEntries | QDir::Hidden,
            //QDir::Files | QDir::NoDotAndDotDot,
            QDir::Files,
            QFileSystemIterator::Subdirectories);

        for (; !dir.atEnd(); dir.next()) {
            dump && printf("%d %s\n",
                dir.fileInfo().isDir(),
                //qPrintable(dir.fileInfo().absoluteFilePath()),
                //qPrintable(dir.path()),
                qPrintable(dir.filePath())
            );
            ++c;
        }
        count = c;
    }
    qDebug() << count;
}

QTEST_MAIN(tst_qdiriterator)

#include "moc_main.cpp"
