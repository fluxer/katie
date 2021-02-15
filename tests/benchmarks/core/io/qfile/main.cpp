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
#include <QTemporaryFile>
#include <QFSFileEngine>
#include <QString>
#include <QDirIterator>
#include <qtest.h>
#include "qplatformdefs.h"

QT_USE_NAMESPACE

#define QFILE_BENCH_BUFSIZE 1024*512
#define QFILE_BENCH_FACTOR 1024*512
#define QFILE_BENCH_TF_SIZE QFILE_BENCH_FACTOR*81

// 10 predefined (but random() seek positions
// hardcoded to be comparable over several runs
const int seekpos[] = {int(QFILE_BENCH_TF_SIZE*0.52),
                       int(QFILE_BENCH_TF_SIZE*0.23),
                       int(QFILE_BENCH_TF_SIZE*0.73),
                       int(QFILE_BENCH_TF_SIZE*0.77),
                       int(QFILE_BENCH_TF_SIZE*0.80),
                       int(QFILE_BENCH_TF_SIZE*0.12),
                       int(QFILE_BENCH_TF_SIZE*0.53),
                       int(QFILE_BENCH_TF_SIZE*0.21),
                       int(QFILE_BENCH_TF_SIZE*0.27),
                       int(QFILE_BENCH_TF_SIZE*0.78)};

const int sp_size = sizeof(seekpos)/sizeof(int);

class tst_qfile: public QObject
{
Q_ENUMS(BenchmarkType)
Q_OBJECT
public:
    enum BenchmarkType {
        QFileBenchmark = 1,
        QFSFileEngineBenchmark,
        PosixBenchmark,
        QFileFromPosixBenchmark
    };
private slots:
    void initTestCase();
    void cleanupTestCase();

    void open_data();
    void open();
    void seek_data();
    void seek();

    void readSmallFiles_QFile();
    void readSmallFiles_QFSFileEngine();
    void readSmallFiles_posix();

    void readSmallFiles_QFile_data();
    void readSmallFiles_QFSFileEngine_data();
    void readSmallFiles_posix_data();

    void readBigFile_QFile_data();
    void readBigFile_QFSFileEngine_data();
    void readBigFile_posix_data();

    void readBigFile_QFile();
    void readBigFile_QFSFileEngine();
    void readBigFile_posix();

    void writeFileSequential_data();
    void writeFileSequential();
    void writeFileBackwards_data();
    void writeFileBackwards();
    void writeFileSequentialWithSeeks_data();
    void writeFileSequentialWithSeeks();

private:
    void readBigFile_data(BenchmarkType type, QIODevice::OpenModeFlag t, QIODevice::OpenModeFlag b);
    void readBigFile();
    void readSmallFiles_data(BenchmarkType type, QIODevice::OpenModeFlag t, QIODevice::OpenModeFlag b);
    void readSmallFiles();
    void createFile();
    void fillFile(int factor=QFILE_BENCH_FACTOR);
    void removeFile();
    void createSmallFiles();
    void removeSmallFiles();
    QString filename;
    QString tmpDirName;
};

Q_DECLARE_METATYPE(tst_qfile::BenchmarkType)
Q_DECLARE_METATYPE(QIODevice::OpenMode)
Q_DECLARE_METATYPE(QIODevice::OpenModeFlag)

void tst_qfile::createFile()
{
    removeFile();  // Cleanup in case previous test case aborted before cleaning up

    QTemporaryFile tmpFile;
    tmpFile.setAutoRemove(false);
    if (!tmpFile.open())
        ::exit(1);
    filename = tmpFile.fileName();
    tmpFile.close();
}

void tst_qfile::removeFile()
{
    if (!filename.isEmpty())
        QFile::remove(filename);
}

void tst_qfile::fillFile(int factor)
{
    QFile tmpFile(filename);
    tmpFile.open(QIODevice::WriteOnly);
    //for (int row=0; row<factor; ++row) {
    //    tmpFile.write(QByteArray().fill('0'+row%('0'-'z'), 80));
    //    tmpFile.write("\n");
    //}
    tmpFile.seek(factor*80);
    tmpFile.putChar('\n');
    tmpFile.close();
    // let IO settle
    QTest::qSleep(2000);
}

void tst_qfile::initTestCase()
{
}

void tst_qfile::cleanupTestCase()
{
}

void tst_qfile::readBigFile_QFile() { readBigFile(); }
void tst_qfile::readBigFile_QFSFileEngine() { readBigFile(); }
void tst_qfile::readBigFile_posix() 
{ 
    readBigFile(); 
}

void tst_qfile::readBigFile_QFile_data()
{
    readBigFile_data(QFileBenchmark, QIODevice::NotOpen, QIODevice::NotOpen);
    readBigFile_data(QFileBenchmark, QIODevice::NotOpen, QIODevice::Unbuffered);
    readBigFile_data(QFileBenchmark, QIODevice::Text, QIODevice::NotOpen);
    readBigFile_data(QFileBenchmark, QIODevice::Text, QIODevice::Unbuffered);

}

void tst_qfile::readBigFile_QFSFileEngine_data()
{
    readBigFile_data(QFSFileEngineBenchmark, QIODevice::NotOpen, QIODevice::NotOpen);
    readBigFile_data(QFSFileEngineBenchmark, QIODevice::NotOpen, QIODevice::Unbuffered);
    readBigFile_data(QFSFileEngineBenchmark, QIODevice::Text, QIODevice::NotOpen);
    readBigFile_data(QFSFileEngineBenchmark, QIODevice::Text, QIODevice::Unbuffered);
}

void tst_qfile::readBigFile_posix_data()
{
    readBigFile_data(PosixBenchmark, QIODevice::NotOpen, QIODevice::NotOpen);
}

void tst_qfile::readBigFile_data(BenchmarkType type, QIODevice::OpenModeFlag t, QIODevice::OpenModeFlag b)
{
    QTest::addColumn<tst_qfile::BenchmarkType>("testType");
    QTest::addColumn<int>("blockSize");
    QTest::addColumn<QFile::OpenModeFlag>("textMode");
    QTest::addColumn<QFile::OpenModeFlag>("bufferedMode");

    const int bs[] = {1024, 1024*2, 1024*8, 1024*16, 1024*32,1024*512};
    int bs_entries = sizeof(bs)/sizeof(const int);

    QString flagstring;
    if (t & QIODevice::Text)       flagstring += QLatin1String("textMode ");
    if (b & QIODevice::Unbuffered) flagstring += QLatin1String("unbuffered ");
    if (flagstring.isEmpty())      flagstring = QLatin1String("none");

    for (int i=0; i<bs_entries; ++i)
        QTest::newRow((QString("BS: %1, Flags: %2" )).arg(bs[i]).arg(flagstring).toLatin1().constData()) << type << bs[i] << t << b;
}

void tst_qfile::readBigFile()
{
    QFETCH(tst_qfile::BenchmarkType, testType);
    QFETCH(int, blockSize);
    QFETCH(QFile::OpenModeFlag, textMode);
    QFETCH(QFile::OpenModeFlag, bufferedMode);

    char *buffer = new char[QFILE_BENCH_BUFSIZE];
    createFile();
    fillFile();

    switch (testType) {
        case(QFileBenchmark): {
            QFile file(filename);
            file.open(QIODevice::ReadOnly|textMode|bufferedMode);
            QBENCHMARK {
                while(!file.atEnd())
                    file.read(blockSize);
                file.reset();
            }
            file.close();
        }
        break;
        case(QFSFileEngineBenchmark): {
            QFSFileEngine fse(filename);
            fse.open(QIODevice::ReadOnly|textMode|bufferedMode);
            QBENCHMARK {
                //qWarning() << fse.supportsExtension(QAbstractFileEngine::MapExtension);
                while(fse.read(buffer, blockSize));
                fse.seek(0);
            }
            fse.close();
        }
        break;
        case(PosixBenchmark): {
            QByteArray data = filename.toLocal8Bit();
            const char* cfilename = data.constData();
            FILE* cfile = QT_FOPEN(cfilename, "rb");
            QBENCHMARK {
                while(!::feof(cfile))
                    ::fread(buffer, blockSize, 1, cfile);
                QT_FSEEK(cfile, 0, SEEK_SET);
            }
            ::fclose(cfile);
        }
        break;
        case(QFileFromPosixBenchmark): {
            // No gain in benchmarking this case
        }
        break;
    }

    removeFile();
    delete[] buffer;
}

void tst_qfile::seek_data()
{
    QTest::addColumn<tst_qfile::BenchmarkType>("testType");
    QTest::newRow("QFile") << QFileBenchmark;
    QTest::newRow("QFSFileEngine") << QFSFileEngineBenchmark;
    QTest::newRow("Posix FILE*") << PosixBenchmark;
}

void tst_qfile::seek()
{
    QFETCH(tst_qfile::BenchmarkType, testType);
    int i = 0;

    createFile();
    fillFile();

    switch (testType) {
        case(QFileBenchmark): {
            QFile file(filename);
            file.open(QIODevice::ReadOnly);
            QBENCHMARK {
                i=(i+1)%sp_size;
                file.seek(seekpos[i]);
            }
            file.close();
        }
        break;
        case(QFSFileEngineBenchmark): {
            QFSFileEngine fse(filename);
            fse.open(QIODevice::ReadOnly);
            QBENCHMARK {
                i=(i+1)%sp_size;
                fse.seek(seekpos[i]);
            }
            fse.close();
        }
        break;
        case(PosixBenchmark): {
            QByteArray data = filename.toLocal8Bit();
            const char* cfilename = data.constData();
            FILE* cfile = QT_FOPEN(cfilename, "rb");
            QBENCHMARK {
                i=(i+1)%sp_size;
                QT_FSEEK(cfile, seekpos[i], SEEK_SET);
            }
            ::fclose(cfile);
        }
        break;
        case(QFileFromPosixBenchmark): {
            // No gain in benchmarking this case
        }
        break;
    }

    removeFile();
}

void tst_qfile::open_data()
{
    QTest::addColumn<tst_qfile::BenchmarkType>("testType");
    QTest::newRow("QFile") << QFileBenchmark;
    QTest::newRow("QFSFileEngine") << QFSFileEngineBenchmark;
    QTest::newRow("Posix FILE*") << PosixBenchmark;
    QTest::newRow("QFile from FILE*") << QFileFromPosixBenchmark;
}

void tst_qfile::open()
{
    QFETCH(tst_qfile::BenchmarkType, testType);

    createFile();

    switch (testType) {
        case(QFileBenchmark): {
            QBENCHMARK {
                QFile file( filename );
                file.open( QIODevice::ReadOnly );
                file.close();
            }
        }
        break;
        case(QFSFileEngineBenchmark): {
            QBENCHMARK {
                QFSFileEngine fse(filename);
                fse.open(QIODevice::ReadOnly);
                fse.close();
            }
        }
        break;

        case(PosixBenchmark): {
            // ensure we don't account toLocal8Bit()
            QByteArray data = filename.toLocal8Bit();
            const char* cfilename = data.constData();

            QBENCHMARK {
                FILE* cfile = QT_FOPEN(cfilename, "rb");
                ::fclose(cfile);
            }
        }
        break;
        case(QFileFromPosixBenchmark): {
            // ensure we don't account toLocal8Bit()
            QByteArray data = filename.toLocal8Bit();
            const char* cfilename = data.constData();
            FILE* cfile = QT_FOPEN(cfilename, "rb");

            QBENCHMARK {
                QFile file;
                file.open(cfile, QIODevice::ReadOnly);
                file.close();
            }
            ::fclose(cfile);
        }
        break;
    }

    removeFile();
}


void tst_qfile::readSmallFiles_QFile() { readSmallFiles(); }
void tst_qfile::readSmallFiles_QFSFileEngine() { readSmallFiles(); }
void tst_qfile::readSmallFiles_posix() 
{
    readSmallFiles(); 
}

void tst_qfile::readSmallFiles_QFile_data()
{
    readSmallFiles_data(QFileBenchmark, QIODevice::NotOpen, QIODevice::NotOpen);
    readSmallFiles_data(QFileBenchmark, QIODevice::NotOpen, QIODevice::Unbuffered);
    readSmallFiles_data(QFileBenchmark, QIODevice::Text, QIODevice::NotOpen);
    readSmallFiles_data(QFileBenchmark, QIODevice::Text, QIODevice::Unbuffered);

}

void tst_qfile::readSmallFiles_QFSFileEngine_data()
{
    readSmallFiles_data(QFSFileEngineBenchmark, QIODevice::NotOpen, QIODevice::NotOpen);
    readSmallFiles_data(QFSFileEngineBenchmark, QIODevice::NotOpen, QIODevice::Unbuffered);
    readSmallFiles_data(QFSFileEngineBenchmark, QIODevice::Text, QIODevice::NotOpen);
    readSmallFiles_data(QFSFileEngineBenchmark, QIODevice::Text, QIODevice::Unbuffered);
}

void tst_qfile::readSmallFiles_posix_data()
{
    readSmallFiles_data(PosixBenchmark, QIODevice::NotOpen, QIODevice::NotOpen);
}

void tst_qfile::readSmallFiles_data(BenchmarkType type, QIODevice::OpenModeFlag t, QIODevice::OpenModeFlag b)
{
    QTest::addColumn<tst_qfile::BenchmarkType>("testType");
    QTest::addColumn<int>("blockSize");
    QTest::addColumn<QFile::OpenModeFlag>("textMode");
    QTest::addColumn<QFile::OpenModeFlag>("bufferedMode");

    const int bs[] = {1024, 1024*2, 1024*8, 1024*16, 1024*32,1024*512};
    int bs_entries = sizeof(bs)/sizeof(const int);

    QString flagstring;
    if (t & QIODevice::Text)       flagstring += QLatin1String("textMode ");
    if (b & QIODevice::Unbuffered) flagstring += QLatin1String("unbuffered ");
    if (flagstring.isEmpty())      flagstring = QLatin1String("none");

    for (int i=0; i<bs_entries; ++i)
        QTest::newRow((QString("BS: %1, Flags: %2" )).arg(bs[i]).arg(flagstring).toLatin1().constData()) << type << bs[i] << t << b;

}

void tst_qfile::createSmallFiles()
{
    QDir dir = QDir::temp();
    dir.mkdir(QLatin1String("tst"));
    dir.cd(QLatin1String("tst"));
    tmpDirName = dir.absolutePath();

    for (int i = 0; i < 1000; ++i)
    {
        QFile f(tmpDirName + QLatin1Char('/') + QString::number(i));
        f.open(QIODevice::WriteOnly);
        f.seek(511);
        f.putChar('\n');
        f.close();
    }
}

void tst_qfile::removeSmallFiles()
{
    QDirIterator it(tmpDirName, QDirIterator::FollowSymlinks);
    while (it.hasNext())
        QFile::remove(it.next());
    QDir::temp().rmdir(QLatin1String("tst"));
}


void tst_qfile::readSmallFiles()
{
    QFETCH(tst_qfile::BenchmarkType, testType);
    QFETCH(int, blockSize);
    QFETCH(QFile::OpenModeFlag, textMode);
    QFETCH(QFile::OpenModeFlag, bufferedMode);

    createSmallFiles();

    QDir dir(tmpDirName);
    const QStringList files = dir.entryList(QDir::NoDotAndDotDot|QDir::NoSymLinks|QDir::Files);
    char *buffer = new char[QFILE_BENCH_BUFSIZE];

    switch (testType) {
        case(QFileBenchmark): {
            QList<QFile*> fileList;
            Q_FOREACH(QString file, files) {
                QFile *f = new QFile(tmpDirName + QLatin1Char('/') + file);
                f->open(QIODevice::ReadOnly|textMode|bufferedMode);
                fileList.append(f);
            }

            QBENCHMARK {
                Q_FOREACH(QFile *file, fileList) {
                    while (!file->atEnd()) {
                       file->read(buffer, blockSize);
                    }
                }
            }

            Q_FOREACH(QFile *file, fileList) {
                file->close();
                delete file;
            }
        }
        break;
        case(QFSFileEngineBenchmark): {
            QList<QFSFileEngine*> fileList;
            Q_FOREACH(QString file, files) {
                QFSFileEngine *fse = new QFSFileEngine(tmpDirName + QLatin1Char('/') + file);
                fse->open(QIODevice::ReadOnly|textMode|bufferedMode);
                fileList.append(fse);
            }

            QBENCHMARK {
                Q_FOREACH(QFSFileEngine *fse, fileList) {
                    while (fse->read(buffer, blockSize));
                }
            }

            Q_FOREACH(QFSFileEngine *fse, fileList) {
                fse->close();
                delete fse;
            }
        }
        break;
        case(PosixBenchmark): {
            QList<FILE*> fileList;
            Q_FOREACH(QString file, files) {
                fileList.append(QT_FOPEN(QFile::encodeName(tmpDirName + QLatin1Char('/') + file).constData(), "rb"));
            }

            QBENCHMARK {
                Q_FOREACH(FILE* cfile, fileList) {
                    while(!::feof(cfile))
                        ::fread(buffer, blockSize, 1, cfile);
                    QT_FSEEK(cfile, 0, SEEK_SET);
                }
            }

            Q_FOREACH(FILE* cfile, fileList) {
                ::fclose(cfile);
            }
        }
        break;
        case(QFileFromPosixBenchmark): {
            // No gain in benchmarking this case
        }
        break;
    }

    removeSmallFiles();
    delete[] buffer;
}

void tst_qfile::writeFileSequential_data()
{
    QTest::addColumn<int>("blockSize");

    QTest::newRow("internal 16b") << 16;
    QTest::newRow("internal 512b") << 512;
    QTest::newRow("internal 4k") << 4096;
    QTest::newRow("internal 16k") << 16384;
    QTest::newRow("internal 64k") << 65536;
}

void tst_qfile::writeFileSequential()
{
    const qint64 limit = 1024 * 1024;
    QFETCH(int, blockSize);
    QTemporaryFile f;
    QByteArray block;
    block.fill('@', blockSize);
    QBENCHMARK {
        QVERIFY(f.open());
        for (qint64 pos = 0; pos < limit; pos += blockSize) {
            QVERIFY(f.write(block));
        }
        QVERIFY(f.flush());
        QCOMPARE(f.size(), limit);
        f.close();
    }
}

void tst_qfile::writeFileBackwards_data()
{
    writeFileSequential_data();
}

void tst_qfile::writeFileBackwards()
{
    const qint64 limit = 1024 * 1024;
    QFETCH(int, blockSize);
    QTemporaryFile f;
    QByteArray block;
    block.fill('@', blockSize);
    QBENCHMARK {
        QVERIFY(f.open());
        for (qint64 pos = limit - blockSize; pos >= 0; pos -= blockSize) {
            QVERIFY(f.seek(pos));
            QVERIFY(f.write(block));
        }
        QVERIFY(f.flush());
        QCOMPARE(f.size(), limit);
        f.close();
    }
}

void tst_qfile::writeFileSequentialWithSeeks_data()
{
    writeFileSequential_data();
}

void tst_qfile::writeFileSequentialWithSeeks()
{
    const qint64 limit = 1024 * 1024;
    QFETCH(int, blockSize);
    QTemporaryFile f;
    QByteArray block;
    block.fill('@', blockSize);
    QBENCHMARK {
        QVERIFY(f.open());
        for (qint64 pos = 0; pos < limit; pos += blockSize) {
            QVERIFY(f.seek(pos));
            QVERIFY(f.write(block));
        }
        QVERIFY(f.flush());
        QCOMPARE(f.size(), limit);
        f.close();
    }
}

QTEST_MAIN(tst_qfile)

#include "moc_main.cpp"
