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


#include <QtTest/QtTest>
#include <qplatformdefs.h>

#include <QAbstractFileEngine>
#include <QFSFileEngine>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHostInfo>
#include <QProcess>

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#ifndef NO_NETWORK_TEST
#  include "../network-settings.h"
#endif

// #define TEST_LFS

Q_DECLARE_METATYPE(QFile::FileError)

//TESTED_CLASS=
//TESTED_FILES=

static const bool currentuserisroot = (::getuid() == 0);

class tst_QFile : public QObject
{
    Q_OBJECT

public:
    tst_QFile();
    virtual ~tst_QFile();


public slots:
    void init();
    void cleanup();
private slots:
    void initTestCase();
    void cleanupTestCase();
    void exists();
    void open_data();
    void open();
    void openUnbuffered();
    void size_data();
    void size();
    void sizeNoExist();
    void seek();
    void setSize();
    void setSizeSeek();
    void seekToSamePosition();
    void atEnd();
    void readLine();
    void readLine2();
    void readLineNullInLine();
    void readAll_data();
    void readAll();
    void readAllBuffer();
    void readAllStdin();
    void readLineStdin();
    void readLineStdin_lineByLine();
    void text();
    void missingEndOfLine();
    void readBlock();
    void getch();
    void ungetChar();
    void createFile();
    void append();
    void permissions_data();
    void permissions();
    void setPermissions();
    void copy();
    void copyAfterFail();
    void copyRemovesTemporaryFile() const;
    void copyShouldntOverwrite();
    void link();
    void linkToDir();
    void absolutePathLinkToRelativePath();
    void readBrokenLink();
    void readTextFile_data();
    void readTextFile();
    void readTextFile2();
    void writeTextFile_data();
    void writeTextFile();
#ifdef TEST_LFS
    void largeFileSupport();
#endif
    void tailFile();
    void flush();
    void bufferedRead();
    void isSequential();
    void encodeName();
    void truncate();
    void seekToPos();
    void seekAfterEndOfFile();
    void FILEReadWrite();
    void i18nFileName_data();
    void i18nFileName();
    void longFileName_data();
    void longFileName();
    void getCharFF();
    void remove_and_exists();
    void removeOpenFile();
    void fullDisk();
    void writeLargeDataBlock_data();
    void writeLargeDataBlock();
    void readFromWriteOnlyFile();
    void writeToReadOnlyFile();
    void virtualFile();
    void textFile();
    void rename_data();
    void rename();
    void renameWithAtEndSpecialFile() const;
    void resourceCopyRename();
    void renameMultiple();
    void appendAndRead();
    void standarderror();
    void handle();
    void nativeHandleLeaks();

    void readEof_data();
    void readEof();

    void map_data();
    void map();
    void mapResource_data();
    void mapResource();
    void mapOpenMode_data();
    void mapOpenMode();

    void openStandardStreams();

    void resize_data();
    void resize();

    void objectConstructors();
    void caseSensitivity();

    void autocloseHandle();

    // --- Task related tests below this line
    void task167217();

    void openDirectory();
    void writeNothing();

public:
// disabled this test for the moment... it hangs
    void invalidFile_data();
    void invalidFile();

private:
    enum FileType {
        OpenQFile,
        OpenFd,
        OpenStream,
        NumberOfFileTypes
    };

    void openStandardStreamsFileDescriptors();
    void openStandardStreamsBufferedStreams();

    bool openFd(QFile &file, QIODevice::OpenMode mode, QFile::FileHandleFlags handleFlags)
    {
        int fdMode = 0;

        // File will be truncated if in Write mode.
        if (mode & QIODevice::WriteOnly)
            fdMode |= QT_OPEN_WRONLY | QT_OPEN_TRUNC;
        if (mode & QIODevice::ReadOnly)
            fdMode |= QT_OPEN_RDONLY;

        fd_ = QT_OPEN(qPrintable(file.fileName()), fdMode);

        return (-1 != fd_) && file.open(fd_, mode, handleFlags);
    }

    bool openStream(QFile &file, QIODevice::OpenMode mode, QFile::FileHandleFlags handleFlags)
    {
        char const *streamMode = "";

        // File will be truncated if in Write mode.
        if (mode & QIODevice::WriteOnly)
            streamMode = "wb+";
        else if (mode & QIODevice::ReadOnly)
            streamMode = "rb";

        stream_ = QT_FOPEN(qPrintable(file.fileName()), streamMode);

        return stream_ && file.open(stream_, mode, handleFlags);
    }

    bool openFile(QFile &file, QIODevice::OpenMode mode, FileType type = OpenQFile, QFile::FileHandleFlags handleFlags = QFile::DontCloseHandle)
    {
        if (mode & QIODevice::WriteOnly && !file.exists())
        {
            // Make sure the file exists
            QFile createFile(file.fileName());
            if (!createFile.open(QIODevice::ReadWrite))
                return false;
        }

        // Note: openFd and openStream will truncate the file if write mode.
        switch (type)
        {
            case OpenQFile:
                return file.open(mode);

            case OpenFd:
                return openFd(file, mode, handleFlags);

            case OpenStream:
                return openStream(file, mode, handleFlags);
        }

        return false;
    }

    void closeFile(QFile &file)
    {
        file.close();

        if (-1 != fd_)
            QT_CLOSE(fd_);
        if (stream_)
            ::fclose(stream_);

        fd_ = -1;
        stream_ = 0;
    }

    int fd_;
    FILE *stream_;

    const QString m_srcDir;
    const QString m_stdinProcess;
};

static inline QString findStdinProcess()
{
    QString result = QCoreApplication::applicationDirPath();
    result += QLatin1String("/qfile_stdinprocess");
    const QFileInfo fi(result);
    if (fi.exists() && fi.isExecutable())
        return fi.absoluteFilePath();
    return QString();
}

tst_QFile::tst_QFile()
    : m_srcDir(QLatin1String(SRCDIR))
    , m_stdinProcess(findStdinProcess())
{
    Q_INIT_RESOURCE(copy_rename);
    Q_INIT_RESOURCE(qfile);
}

tst_QFile::~tst_QFile()
{

}

void tst_QFile::init()
{
// TODO: Add initialization code here.
// This will be executed immediately before each test is run.
    fd_ = -1;
    stream_ = 0;
}

void tst_QFile::cleanup()
{
// TODO: Add cleanup code here.
// This will be executed immediately after each test is run.

    // for resourceCopyRename()
    QFile::remove("file-copy-destination.txt");
    QFile::remove("file-rename-destination.txt");

    // for copyAfterFail()
    QFile::remove("file-to-be-copied.txt");
    QFile::remove("existing-file.txt");
    QFile::remove("copied-file-1.txt");
    QFile::remove("copied-file-2.txt");

    // for renameMultiple()
    QFile::remove("file-to-be-renamed.txt");
    QFile::remove("existing-file.txt");
    QFile::remove("file-renamed-once.txt");
    QFile::remove("file-renamed-twice.txt");

    if (-1 != fd_)
        QT_CLOSE(fd_);
    if (stream_)
        ::fclose(stream_);
}

void tst_QFile::initTestCase()
{
    QDir srcDir(m_srcDir);
    QVERIFY2(srcDir.exists(), qPrintable(m_srcDir + QLatin1String(" does not exist.")));
    QDir::setCurrent(srcDir.absolutePath());
    QVERIFY2(!m_stdinProcess.isEmpty(),
             qPrintable("Cannot locate stdinprocess from " + QCoreApplication::applicationDirPath()));

    QFile::remove("noreadfile");

    // create a file and make it read-only
    QFile file("readonlyfile");
    file.open(QFile::WriteOnly);
    file.write("a", 1);
    file.close();
    file.setPermissions(QFile::ReadOwner);

    // create another file and make it not readable
    file.setFileName("noreadfile");
    file.open(QFile::WriteOnly);
    file.write("b", 1);
    file.close();
    file.setPermissions(0);
}

void tst_QFile::cleanupTestCase()
{
    // clean up the files we created
    QFile::remove("readonlyfile");
    QFile::remove("noreadfile");
    QFile::remove("myLink.lnk");
    QFile::remove("appendme.txt");
    QFile::remove("createme.txt");
    QFile::remove("file.txt");
    QFile::remove("genfile.txt");
    QFile::remove("seekToPos.txt");
    QFile::remove("setsizeseek.txt");
    QFile::remove("stdfile.txt");
    QFile::remove("textfile.txt");
    QFile::remove("truncate.txt");
    QFile::remove("winfile.txt");
    QFile::remove("writeonlyfile");
    QFile::remove("largeblockfile.txt");
    QFile::remove("tst_qfile_copy.cpp");
    QFile::remove("nullinline.txt");
    QFile::remove("myLink2.lnk");
    QFile::remove("resources");
    QFile::remove("qfile_map_testfile");
    QFile::remove("readAllBuffer.txt");
    QFile::remove("qt_file.tmp");
    QFile::remove("File.txt");
    QFile::remove("seekToSamePosition.txt");
}

//------------------------------------------
// The 'testfile' is currently just a
// testfile. The path of this file, the
// attributes and the contents itself
// will be changed as far as we have a
// proper way to handle files in the
// testing environment.
//------------------------------------------

void tst_QFile::exists()
{
    QFile f( SRCDIR "testfile.txt" );
    QCOMPARE( f.exists(), true );

    QFile file("nobodyhassuchafile");
    file.remove();
    QVERIFY(!file.exists());

    QFile file2("nobodyhassuchafile");
    QVERIFY(file2.open(QIODevice::WriteOnly));
    file2.close();

    QVERIFY(file.exists());

    QVERIFY(file.open(QIODevice::WriteOnly));
    file.close();
    QVERIFY(file.exists());

    file.remove();
    QVERIFY(!file.exists());
}

void tst_QFile::open_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<int>("mode");
    QTest::addColumn<bool>("ok");
    QTest::addColumn<QFile::FileError>("status");

    static const QString denied("Permission denied");
    QTest::newRow( "exist_readOnly"  )
        << QString(SRCDIR "testfile.txt") << int(QIODevice::ReadOnly)
        << true << QFile::NoError;

    QTest::newRow( "exist_writeOnly" )
        << QString("readonlyfile")
        << int(QIODevice::WriteOnly)
        << false
        << QFile::OpenError;

    QTest::newRow( "exist_append"    )
        << QString("readonlyfile") << int(QIODevice::Append)
        << false << QFile::OpenError;

    QTest::newRow( "nonexist_readOnly"  )
        << QString("nonExist.txt") << int(QIODevice::ReadOnly)
        << false << QFile::OpenError;

    QTest::newRow("emptyfile")
        << QString("")
        << int(QIODevice::ReadOnly)
        << false
        << QFile::OpenError;

    QTest::newRow("nullfile") << QString() << int(QIODevice::ReadOnly) << false
        << QFile::OpenError;

    QTest::newRow("two-dots") << QString(SRCDIR "two.dots.file") << int(QIODevice::ReadOnly) << true
        << QFile::NoError;

    QTest::newRow("readonlyfile") << QString("readonlyfile") << int(QIODevice::WriteOnly)
                                  << false << QFile::OpenError;
    QTest::newRow("noreadfile") << QString("noreadfile") << int(QIODevice::ReadOnly)
                                << false << QFile::OpenError;
}

void tst_QFile::open()
{
    QFETCH( QString, filename );
    QFETCH( int, mode );

    QFile f( filename );

    QFETCH( bool, ok );

    if (currentuserisroot) {
        // root and Chuck Norris don't care for file permissions. Skip.
        QSKIP("Running this test as root doesn't make sense", SkipAll);
    }

    if (filename.isEmpty())
        QTest::ignoreMessage(QtWarningMsg, "QFSFileEngine::open: No file name specified");

    if (ok) {
        QVERIFY2(f.open(QIODevice::OpenMode(mode)),
                 qPrintable(QString::fromLatin1("Cannot open %1 in mode %2: %3")
                            .arg(filename).arg(mode).arg(f.errorString())));
    } else {
        QVERIFY(!f.open(QIODevice::OpenMode(mode)));
    }

    QTEST( f.error(), "status" );
}

void tst_QFile::openUnbuffered()
{
    QFile file(SRCDIR "testfile.txt");
    QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Unbuffered));
    char c = '\0';
    QVERIFY(file.seek(1));
    QCOMPARE(file.pos(), qint64(1));
    QVERIFY(file.getChar(&c));
    QCOMPARE(file.pos(), qint64(2));
    char d = '\0';
    QVERIFY(file.seek(3));
    QCOMPARE(file.pos(), qint64(3));
    QVERIFY(file.getChar(&d));
    QCOMPARE(file.pos(), qint64(4));
    QVERIFY(file.seek(1));
    QCOMPARE(file.pos(), qint64(1));
    char c2 = '\0';
    QVERIFY(file.getChar(&c2));
    QCOMPARE(file.pos(), qint64(2));
    QVERIFY(file.seek(3));
    QCOMPARE(file.pos(), qint64(3));
    char d2 = '\0';
    QVERIFY(file.getChar(&d2));
    QCOMPARE(file.pos(), qint64(4));
    QCOMPARE(c, c2);
    QCOMPARE(d, d2);
    QCOMPARE(c, '-');
    QCOMPARE(d, '-');
}

void tst_QFile::size_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<qint64>("size");

    QTest::newRow( "exist01" ) << QString(SRCDIR "testfile.txt") << (qint64)245;
}

void tst_QFile::size()
{
    QFETCH( QString, filename );
    QFETCH( qint64, size );

    {
        QFile f( filename );
        QCOMPARE( f.size(), size );

        QVERIFY( f.open(QIODevice::ReadOnly) );
        QCOMPARE( f.size(), size );
    }

    {
        QFile f;
        FILE* stream = QT_FOPEN(filename.toLocal8Bit().constData(), "rb");
        QVERIFY( stream );
        QVERIFY( f.open(stream, QIODevice::ReadOnly) );
        QCOMPARE( f.size(), size );

        f.close();
        fclose(stream);
    }

    {
        QFile f;

        int fd = QT_OPEN(filename.toLocal8Bit().constData(), QT_OPEN_RDONLY);

        QVERIFY( fd != -1 );
        QVERIFY( f.open(fd, QIODevice::ReadOnly) );
        QCOMPARE( f.size(), size );

        f.close();
        QT_CLOSE(fd);
    }
}

void tst_QFile::sizeNoExist()
{
    QFile file("nonexist01");
    QVERIFY( !file.exists() );
    QCOMPARE( file.size(), (qint64)0 );
    QVERIFY( !file.open(QIODevice::ReadOnly) );
}

void tst_QFile::seek()
{
    QFile::remove("newfile.txt");
    QFile file("newfile.txt");
    file.open(QIODevice::WriteOnly);
    QCOMPARE(file.size(), qint64(0));
    QCOMPARE(file.pos(), qint64(0));
    QVERIFY(file.seek(10));
    QCOMPARE(file.pos(), qint64(10));
    QCOMPARE(file.size(), qint64(0));
    file.close();
    QFile::remove("newfile.txt");
}

void tst_QFile::setSize()
{
    DEPENDS_ON( "size" );

    if ( QFile::exists( "createme.txt" ) )
        QFile::remove( "createme.txt" );
    QVERIFY( !QFile::exists( "createme.txt" ) );

    QFile f("createme.txt");
    QVERIFY(f.open(QIODevice::Truncate | QIODevice::ReadWrite));
    f.putChar('a');

    f.seek(0);
    char c = '\0';
    f.getChar(&c);
    QCOMPARE(c, 'a');

    QCOMPARE(f.size(), (qlonglong)1);
    bool ok = f.resize(99);
    QVERIFY(ok);
    QCOMPARE(f.size(), (qlonglong)99);

    f.seek(0);
    c = '\0';
    f.getChar(&c);
    QCOMPARE(c, 'a');

    QVERIFY(f.resize(1));
    QCOMPARE(f.size(), (qlonglong)1);

    f.seek(0);
    c = '\0';
    f.getChar(&c);
    QCOMPARE(c, 'a');

    f.close();

    QCOMPARE(f.size(), (qlonglong)1);
    QVERIFY(f.resize(100));
    QCOMPARE(f.size(), (qlonglong)100);
    QVERIFY(f.resize(50));
    QCOMPARE(f.size(), (qlonglong)50);
}

void tst_QFile::setSizeSeek()
{
    QFile::remove("setsizeseek.txt");
    QFile f("setsizeseek.txt");
    QVERIFY(f.open(QFile::WriteOnly));
    f.write("ABCD");

    QCOMPARE(f.pos(), qint64(4));
    f.resize(2);
    QCOMPARE(f.pos(), qint64(2));
    f.resize(4);
    QCOMPARE(f.pos(), qint64(2));
    f.resize(0);
    QCOMPARE(f.pos(), qint64(0));
    f.resize(4);
    QCOMPARE(f.pos(), qint64(0));

    f.seek(3);
    QCOMPARE(f.pos(), qint64(3));
    f.resize(2);
    QCOMPARE(f.pos(), qint64(2));
}

void tst_QFile::seekToSamePosition()
{
    QFile in(SRCDIR "testfile.txt");
    QFile out("seekToSamePosition.txt");
    QVERIFY(in.open(QFile::ReadOnly));
    QVERIFY(out.open(QFile::WriteOnly));
    QByteArray plusses;
    plusses.fill('+', 58);

    qint64 cursor = 0;
    QVERIFY(in.seek(cursor));
    QVERIFY(out.seek(cursor));

    QVERIFY(out.write(plusses));

    cursor += 58;
    QVERIFY(in.seek(cursor));
    QVERIFY(out.seek(cursor));

    QByteArray copy = in.read(60);
    QVERIFY(out.write(copy));

    cursor += 60;
    QVERIFY(in.seek(cursor));
    QVERIFY(out.seek(cursor));

    QVERIFY(out.write(plusses));

    cursor += 58;
    QVERIFY(in.seek(cursor));
    QVERIFY(out.seek(cursor));

    copy = in.readAll();
    QVERIFY(out.write(copy));

    //compare
    out.close();
    QVERIFY(out.open(QFile::ReadOnly));
    QVERIFY(in.seek(0));

    QByteArray clean = in.readAll();
    QByteArray dirty = out.readAll();
    out.close();

    QVERIFY(clean.size() == dirty.size());
    for (int i=0;i<clean.size();i++) {
        if (clean[i] == '-')
            QVERIFY2(dirty[i] == '+', qPrintable(QString("no + at pos %1").arg(i)));
        else
            QVERIFY2(dirty[i] == clean[i], qPrintable(QString("char at pos %1 mismatched, %2 vs %3").arg(i).arg(clean[i]).arg(dirty[i])));
    }
}

void tst_QFile::atEnd()
{
    QFile f( SRCDIR "testfile.txt" );
    QVERIFY(f.open( QIODevice::ReadOnly ));

    int size = f.size();
    f.seek( size );

    bool end = f.atEnd();
    f.close();
    QCOMPARE( end, true );
}

void tst_QFile::readLine()
{
    QFile f( SRCDIR "testfile.txt" );
    QVERIFY(f.open( QIODevice::ReadOnly ));

    int i = 0;
    char p[128];
    int foo;
    while ( (foo=f.readLine( p, 128 )) > 0 ) {
        ++i;
        if ( i == 5 ) {
            QCOMPARE( p[0], 'T' );
            QCOMPARE( p[3], 's' );
            QCOMPARE( p[11], 'i' );
        }
    }
    f.close();
    QCOMPARE( i, 6 );
}

void tst_QFile::readLine2()
{
    QFile f( SRCDIR "testfile.txt" );
    f.open( QIODevice::ReadOnly );

    char p[128];
    QCOMPARE(f.readLine(p, 60), qlonglong(59));
    QCOMPARE(f.readLine(p, 60), qlonglong(59));
    memset(p, '@', sizeof(p));
    QCOMPARE(f.readLine(p, 60), qlonglong(59));

    QCOMPARE(p[57], '-');
    QCOMPARE(p[58], '\n');
    QCOMPARE(p[59], '\0');
    QCOMPARE(p[60], '@');
}

void tst_QFile::readLineNullInLine()
{
    QFile::remove("nullinline.txt");
    QFile file("nullinline.txt");
    QVERIFY(file.open(QIODevice::ReadWrite));
    QVERIFY(file.write("linewith\0null\nanotherline\0withnull\n\0\nnull\0", 42) > 0);
    QVERIFY(file.flush());
    file.reset();

    QCOMPARE(file.readLine(), QByteArray("linewith\0null\n", 14));
    QCOMPARE(file.readLine(), QByteArray("anotherline\0withnull\n", 21));
    QCOMPARE(file.readLine(), QByteArray("\0\n", 2));
    QCOMPARE(file.readLine(), QByteArray("null\0", 5));
    QCOMPARE(file.readLine(), QByteArray());
}

void tst_QFile::readAll_data()
{
    QTest::addColumn<bool>("textMode");
    QTest::addColumn<QString>("fileName");
    QTest::newRow( "TextMode unixfile" ) <<  true << SRCDIR "testfile.txt";
    QTest::newRow( "BinaryMode unixfile" ) <<  false << SRCDIR "testfile.txt";
    QTest::newRow( "TextMode dosfile" ) <<  true << SRCDIR "dosfile.txt";
    QTest::newRow( "BinaryMode dosfile" ) <<  false << SRCDIR "dosfile.txt";
    QTest::newRow( "TextMode bigfile" ) <<  true << SRCDIR "tst_qfile.cpp";
    QTest::newRow( "BinaryMode  bigfile" ) <<  false << SRCDIR "tst_qfile.cpp";
    QVERIFY(QFile(SRCDIR "tst_qfile.cpp").size() > 64*1024);
}

void tst_QFile::readAll()
{
    QFETCH( bool, textMode );
    QFETCH( QString, fileName );

    QFile file(fileName);
    if (textMode)
        QVERIFY(file.open(QFile::Text | QFile::ReadOnly));
    else
        QVERIFY(file.open(QFile::ReadOnly));

    QByteArray a = file.readAll();
    file.reset();
    QVERIFY(file.pos() == 0);

    QVERIFY(file.bytesAvailable() > 7);
    QByteArray b = file.read(1);
    char x;
    file.getChar(&x);
    b.append(x);
    b.append(file.read(5));
    b.append(file.readAll());

    QCOMPARE(a, b);
}

void tst_QFile::readAllBuffer()
{
    QString fileName = QLatin1String("readAllBuffer.txt");

    QFile::remove(fileName);

    QFile writer(fileName);
    QFile reader(fileName);

    QByteArray data1("This is arguably a very simple text.");
    QByteArray data2("This is surely not as simple a test.");

    QVERIFY( writer.open(QIODevice::ReadWrite | QIODevice::Unbuffered) );
    QVERIFY( reader.open(QIODevice::ReadOnly) );

    QCOMPARE( writer.write(data1), qint64(data1.size()) );
    QVERIFY( writer.seek(0) );

    QByteArray result;
    result = reader.read(18);
    QCOMPARE( result.size(), 18 );

    QCOMPARE( writer.write(data2), qint64(data2.size()) ); // new data, old version buffered in reader
    QCOMPARE( writer.write(data2), qint64(data2.size()) ); // new data, unbuffered in reader

    result += reader.readAll();

    QCOMPARE( result, data1 + data2 );

    QFile::remove(fileName);
}

void tst_QFile::readAllStdin()
{
    QByteArray lotsOfData(1024, '@'); // 10 megs

    QProcess process;
    process.start(m_stdinProcess + QLatin1String(" all"));
    QVERIFY( process.waitForStarted() );
    for (int i = 0; i < 5; ++i) {
        QTest::qWait(1000);
        process.write(lotsOfData);
        while (process.bytesToWrite() > 0) {
            QVERIFY(process.waitForBytesWritten());
        }
    }

    process.closeWriteChannel();
    process.waitForFinished();
    QCOMPARE(process.readAll().size(), lotsOfData.size() * 5);
}

void tst_QFile::readLineStdin()
{
    QByteArray lotsOfData(1024, '@'); // 10 megs
    for (int i = 0; i < lotsOfData.size(); ++i) {
        if ((i % 32) == 31)
            lotsOfData[i] = '\n';
        else
            lotsOfData[i] = char('0' + i % 32);
    }

    for (int i = 0; i < 2; ++i) {
        QProcess process;
        process.start(m_stdinProcess + QString::fromLatin1(" line %1").arg(i), QIODevice::Text | QIODevice::ReadWrite);
        for (int i = 0; i < 5; ++i) {
            QTest::qWait(1000);
            process.write(lotsOfData);
            while (process.bytesToWrite() > 0) {
                QVERIFY(process.waitForBytesWritten());
            }
        }

        process.closeWriteChannel();
        QVERIFY(process.waitForFinished(5000));

        QByteArray array = process.readAll();
        QCOMPARE(array.size(), lotsOfData.size() * 5);
        for (int i = 0; i < array.size(); ++i) {
            if ((i % 32) == 31)
                QCOMPARE(char(array[i]), '\n');
            else
                QCOMPARE(char(array[i]), char('0' + i % 32));
        }
    }
}

void tst_QFile::readLineStdin_lineByLine()
{
    for (int i = 0; i < 2; ++i) {
        QProcess process;
        process.start(m_stdinProcess + QString::fromLatin1(" line %1").arg(i), QIODevice::Text | QIODevice::ReadWrite);
        QVERIFY(process.waitForStarted());

        for (int j = 0; j < 3; ++j) {
            QByteArray line = "line " + QByteArray::number(j) + "\n";
            QCOMPARE(process.write(line), qint64(line.size()));
            QVERIFY(process.waitForBytesWritten(2000));
            if (process.bytesAvailable() == 0)
                QVERIFY(process.waitForReadyRead(2000));
            QCOMPARE(process.readAll(), line);
        }

        process.closeWriteChannel();
        QVERIFY(process.waitForFinished(5000));
    }
}

void tst_QFile::text()
{
    // dosfile.txt is a binary CRLF file
    QFile file(SRCDIR "dosfile.txt");
    QVERIFY(file.open(QFile::Text | QFile::ReadOnly));
    QCOMPARE(file.readLine(),
            QByteArray("/dev/system/root     /                    reiserfs   acl,user_xattr        1 1\n"));
    QCOMPARE(file.readLine(),
            QByteArray("/dev/sda1            /boot                ext3       acl,user_xattr        1 2\n"));
    file.ungetChar('\n');
    file.ungetChar('2');
    QCOMPARE(file.readLine().constData(), QByteArray("2\n").constData());
}

void tst_QFile::missingEndOfLine()
{
    QFile file(SRCDIR "noendofline.txt");
    QVERIFY(file.open(QFile::ReadOnly));

    int nlines = 0;
    while (!file.atEnd()) {
        ++nlines;
        file.readLine();
    }

    QCOMPARE(nlines, 3);
}

void tst_QFile::readBlock()
{
    QFile f( SRCDIR "testfile.txt" );
    f.open( QIODevice::ReadOnly );

    int length = 0;
    char p[256];
    length = f.read( p, 256 );
    f.close();
    QCOMPARE( length, 245 );
    QCOMPARE( p[59], 'D' );
    QCOMPARE( p[178], 'T' );
    QCOMPARE( p[199], 'l' );
}

void tst_QFile::getch()
{
    QFile f( SRCDIR "testfile.txt" );
    f.open( QIODevice::ReadOnly );

    char c;
    int i = 0;
    while (f.getChar(&c)) {
        QCOMPARE(f.pos(), qint64(i + 1));
        if ( i == 59 )
            QCOMPARE( c, 'D' );
        ++i;
    }
    f.close();
    QCOMPARE( i, 245 );
}

void tst_QFile::ungetChar()
{
    QFile f(SRCDIR "testfile.txt");
    QVERIFY(f.open(QIODevice::ReadOnly));

    QByteArray array = f.readLine();
    QCOMPARE(array.constData(), "----------------------------------------------------------\n");
    f.ungetChar('\n');

    array = f.readLine();
    QCOMPARE(array.constData(), "\n");

    f.ungetChar('\n');
    f.ungetChar('-');
    f.ungetChar('-');

    array = f.readLine();
    QCOMPARE(array.constData(), "--\n");

    QFile::remove("genfile.txt");
    QFile out("genfile.txt");
    QVERIFY(out.open(QIODevice::ReadWrite));
    out.write("123");
    out.seek(0);
    QCOMPARE(out.readAll().constData(), "123");
    out.ungetChar('3');
    out.write("4");
    out.seek(0);
    QCOMPARE(out.readAll().constData(), "124");
    out.ungetChar('4');
    out.ungetChar('2');
    out.ungetChar('1');
    char buf[3];
    QCOMPARE(out.read(buf, sizeof(buf)), qint64(3));
    QCOMPARE(buf[0], '1');
    QCOMPARE(buf[1], '2');
    QCOMPARE(buf[2], '4');
}

void tst_QFile::invalidFile_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::newRow( "x11" ) << QString( "qwe//" );
}

void tst_QFile::invalidFile()
{
    QFETCH( QString, fileName );
    QFile f( fileName );
    QVERIFY( !f.open( QIODevice::ReadWrite ) );
}

void tst_QFile::createFile()
{
    if ( QFile::exists( "createme.txt" ) )
        QFile::remove( "createme.txt" );
    QVERIFY( !QFile::exists( "createme.txt" ) );

    QFile f( "createme.txt" );
    QVERIFY( f.open( QIODevice::WriteOnly ) );
    f.close();
    QVERIFY( QFile::exists( "createme.txt" ) );
}

void tst_QFile::append()
{
    const QString name("appendme.txt");
    if (QFile::exists(name))
        QFile::remove(name);
    QVERIFY(!QFile::exists(name));

    QFile f(name);
    QVERIFY(f.open(QIODevice::WriteOnly | QIODevice::Truncate));
    f.putChar('a');
    f.close();

    QVERIFY(f.open(QIODevice::Append));
    QVERIFY(f.pos() == 1);
    f.putChar('a');
    f.close();
    QCOMPARE(int(f.size()), 2);
}

void tst_QFile::permissions_data()
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<uint>("perms");
    QTest::addColumn<bool>("expected");

    QTest::newRow("data0") << QCoreApplication::instance()->applicationFilePath() << uint(QFile::ExeUser) << true;
    QTest::newRow("data1") << SRCDIR "tst_qfile.cpp" << uint(QFile::ReadUser) << true;
//    QTest::newRow("data2") << "tst_qfile.cpp" << int(QFile::WriteUser) << false;
    QTest::newRow("resource1") << ":/tst_qfileinfo/resources/file1.ext1" << uint(QFile::ReadUser) << true;
    QTest::newRow("resource2") << ":/tst_qfileinfo/resources/file1.ext1" << uint(QFile::WriteUser) << false;
    QTest::newRow("resource3") << ":/tst_qfileinfo/resources/file1.ext1" << uint(QFile::ExeUser) << false;
}

void tst_QFile::permissions()
{
    QFETCH(QString, file);
    QFETCH(uint, perms);
    QFETCH(bool, expected);
    QFile f(file);
    QCOMPARE(((f.permissions() & perms) == QFile::Permissions(perms)), expected);
    QCOMPARE(((QFile::permissions(file) & perms) == QFile::Permissions(perms)), expected);
}

void tst_QFile::setPermissions()
{
    DEPENDS_ON( "permissions" ); //if that doesn't work...

    if ( QFile::exists( "createme.txt" ) )
        QFile::remove( "createme.txt" );
    QVERIFY( !QFile::exists( "createme.txt" ) );

    QFile f("createme.txt");
    QVERIFY(f.open(QIODevice::WriteOnly | QIODevice::Truncate));
    f.putChar('a');
    f.close();

    QFile::Permissions perms(QFile::WriteUser | QFile::ReadUser);
    QVERIFY(f.setPermissions(perms));
    QVERIFY((f.permissions() & perms) == perms);

}

void tst_QFile::copy()
{
    QFile::setPermissions("tst_qfile_copy.cpp", QFile::WriteUser);
    QFile::remove("tst_qfile_copy.cpp");
    QFile::remove("test2");
    QVERIFY(QFile::copy(SRCDIR "tst_qfile.cpp", "tst_qfile_copy.cpp"));
    QFile in1(SRCDIR "tst_qfile.cpp"), in2("tst_qfile_copy.cpp");
    QVERIFY(in1.open(QFile::ReadOnly));
    QVERIFY(in2.open(QFile::ReadOnly));
    QByteArray data1 = in1.readAll(), data2 = in2.readAll();
    QCOMPARE(data1, data2);
    QFile::remove( "main_copy.cpp" );

    QFile::copy(QDir::currentPath(), QDir::currentPath() + QLatin1String("/test2"));
}

void tst_QFile::copyAfterFail()
{
    QFile file1("file-to-be-copied.txt");
    QFile file2("existing-file.txt");

    QVERIFY(file1.open(QIODevice::ReadWrite));
    QVERIFY(file2.open(QIODevice::ReadWrite));
    file2.close();
    QVERIFY(!QFile::exists("copied-file-1.txt"));
    QVERIFY(!QFile::exists("copied-file-2.txt"));

    QVERIFY(file1.copy("existing-file.txt"));
    QCOMPARE(file1.error(), QFile::NoError);

    QVERIFY(file1.copy("copied-file-1.txt"));
    QVERIFY(!file1.isOpen());
    QCOMPARE(file1.error(), QFile::NoError);

    QVERIFY(file1.copy("existing-file.txt"));
    QCOMPARE(file1.error(), QFile::NoError);

    QVERIFY(file1.copy("copied-file-2.txt"));
    QVERIFY(!file1.isOpen());
    QCOMPARE(file1.error(), QFile::NoError);

    QVERIFY(QFile::exists("copied-file-1.txt"));
    QVERIFY(QFile::exists("copied-file-2.txt"));

    QVERIFY(QFile::remove("file-to-be-copied.txt"));
    QVERIFY(QFile::remove("existing-file.txt"));
    QVERIFY(QFile::remove("copied-file-1.txt"));
    QVERIFY(QFile::remove("copied-file-2.txt"));
}

void tst_QFile::copyRemovesTemporaryFile() const
{
    const QString newName(QLatin1String("copyRemovesTemporaryFile"));
    QVERIFY(QFile::copy(SRCDIR "forCopying.txt", newName));

    QVERIFY(!QFile::exists(QLatin1String( SRCDIR "qt_temp.XXXXXX")));
    QVERIFY(QFile::remove(newName));
}

void tst_QFile::copyShouldntOverwrite()
{
    if (currentuserisroot) {
        // root and Chuck Norris don't care for file permissions. Skip.
        QSKIP("Running this test as root doesn't make sense", SkipAll);
    }

    // Copy should not overwrite existing files.
    QFile::remove("tst_qfile.cpy");
    QFile file(SRCDIR "tst_qfile.cpp");
    QVERIFY(file.copy("tst_qfile.cpy"));
    bool ok = QFile::setPermissions("tst_qfile.cpy", QFile::WriteOther);
    QVERIFY(ok);
    QVERIFY(!file.copy("tst_qfile.cpy"));
    QFile::remove("tst_qfile.cpy");
}

void tst_QFile::link()
{
    QFile::remove("myLink.lnk");

    QFileInfo info1(SRCDIR "tst_qfile.cpp");
    QString referenceTarget = QDir::cleanPath(info1.absoluteFilePath());

    QVERIFY(QFile::link(SRCDIR "tst_qfile.cpp", "myLink.lnk"));

    QFileInfo info2("myLink.lnk");
    QVERIFY(info2.isSymLink());
    QCOMPARE(info2.readLink(), referenceTarget);

    QFile link("myLink.lnk");
    QVERIFY(link.open(QIODevice::ReadOnly));
    QCOMPARE(link.readLink(), referenceTarget);
    link.close();

    QCOMPARE(QFile("myLink.lnk").readLink(), referenceTarget);

    QVERIFY(QFile::remove(info2.absoluteFilePath()));
}

void tst_QFile::linkToDir()
{
    QFile::remove("myLinkToDir.lnk");
    QDir dir;
    dir.mkdir("myDir");
    QFileInfo info1("myDir");
    QVERIFY(QFile::link("myDir", "myLinkToDir.lnk"));
    QFileInfo info2("myLinkToDir.lnk");
    QVERIFY(info2.isSymLink());
    QCOMPARE(info2.readLink(), info1.absoluteFilePath());
    QVERIFY(QFile::remove(info2.absoluteFilePath()));
    QFile::remove("myLinkToDir.lnk");
    dir.rmdir("myDir");
}

void tst_QFile::absolutePathLinkToRelativePath()
{
    QFile::remove("myDir/test.txt");
    QFile::remove("myDir/myLink.lnk");
    QDir dir;
    dir.mkdir("myDir");
    QFile("myDir/test.txt").open(QFile::WriteOnly);

    QVERIFY(QFile::link("myDir/test.txt", "myDir/myLink.lnk"));
    QEXPECT_FAIL("", "Symlinking using relative paths is currently different on Windows and Unix/Symbian", Continue);
    QCOMPARE(QFileInfo(QFile(QFileInfo("myDir/myLink.lnk").absoluteFilePath()).readLink()).absoluteFilePath(),
             QFileInfo("myDir/test.txt").absoluteFilePath());

    QFile::remove("myDir/test.txt");
    QFile::remove("myDir/myLink.lnk");
    dir.rmdir("myDir");
}

void tst_QFile::readBrokenLink()
{
    QFile::remove("myLink2.lnk");
    QFileInfo info1("file12");
    QVERIFY(QFile::link("file12", "myLink2.lnk"));
    QFileInfo info2("myLink2.lnk");
    QVERIFY(info2.isSymLink());
    QCOMPARE(info2.readLink(), info1.absoluteFilePath());
    QVERIFY(QFile::remove(info2.absoluteFilePath()));

    QVERIFY(QFile::link("ole/..", "myLink2.lnk"));
    QCOMPARE(QFileInfo("myLink2.lnk").readLink(), QDir::currentPath());
}

void tst_QFile::readTextFile_data()
{
    QTest::addColumn<QByteArray>("in");
    QTest::addColumn<QByteArray>("out");

    QTest::newRow("empty") << QByteArray() << QByteArray();
    QTest::newRow("a") << QByteArray("a") << QByteArray("a");
    QTest::newRow("a\\rb") << QByteArray("a\rb") << QByteArray("ab");
    QTest::newRow("\\n") << QByteArray("\n") << QByteArray("\n");
    QTest::newRow("\\r\\n") << QByteArray("\r\n") << QByteArray("\n");
    QTest::newRow("\\r") << QByteArray("\r") << QByteArray();
    QTest::newRow("twolines") << QByteArray("Hello\r\nWorld\r\n") << QByteArray("Hello\nWorld\n");
    QTest::newRow("twolines no endline") << QByteArray("Hello\r\nWorld") << QByteArray("Hello\nWorld");
}

void tst_QFile::readTextFile()
{
    QFETCH(QByteArray, in);
    QFETCH(QByteArray, out);

    QFile winfile("winfile.txt");
    QVERIFY(winfile.open(QFile::WriteOnly | QFile::Truncate));
    winfile.write(in);
    winfile.close();

    QVERIFY(winfile.open(QFile::ReadOnly));
    QCOMPARE(winfile.readAll(), in);
    winfile.close();

    QVERIFY(winfile.open(QFile::ReadOnly | QFile::Text));
    QCOMPARE(winfile.readAll(), out);
}

void tst_QFile::readTextFile2()
{
    {
        QFile file(SRCDIR "testlog.txt");
        QVERIFY(file.open(QIODevice::ReadOnly));
        file.read(4097);
    }

    {
        QFile file(SRCDIR "testlog.txt");
        QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));
        file.read(4097);
    }
}

void tst_QFile::writeTextFile_data()
{
    QTest::addColumn<QByteArray>("in");

    QTest::newRow("empty") << QByteArray();
    QTest::newRow("a") << QByteArray("a");
    QTest::newRow("a\\rb") << QByteArray("a\rb");
    QTest::newRow("\\n") << QByteArray("\n");
    QTest::newRow("\\r\\n") << QByteArray("\r\n");
    QTest::newRow("\\r") << QByteArray("\r");
    QTest::newRow("twolines crlf") << QByteArray("Hello\r\nWorld\r\n");
    QTest::newRow("twolines crlf no endline") << QByteArray("Hello\r\nWorld");
    QTest::newRow("twolines lf") << QByteArray("Hello\nWorld\n");
    QTest::newRow("twolines lf no endline") << QByteArray("Hello\nWorld");
    QTest::newRow("mixed") << QByteArray("this\nis\r\na\nmixed\r\nfile\n");
}

void tst_QFile::writeTextFile()
{
    QFETCH(QByteArray, in);

    QFile file("textfile.txt");
    QVERIFY(file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text));
    QCOMPARE(file.write(in), qlonglong(in.size()));
    file.close();

    file.open(QFile::ReadOnly);
    QCOMPARE(file.readAll(), in);
}

void tst_QFile::tailFile()
{
    QSKIP("File change notifications are so far unsupported.", SkipAll);

    QFile file("tail.txt");
    QVERIFY(file.open(QFile::WriteOnly | QFile::Append));

    QFile tailFile("tail.txt");
    QVERIFY(tailFile.open(QFile::ReadOnly));
    tailFile.seek(file.size());

    QSignalSpy readSignal(&tailFile, SIGNAL(readyRead()));

    file.write("", 1);

    QTestEventLoop::instance().enterLoop(5);

    QVERIFY(!QTestEventLoop::instance().timeout());
    QCOMPARE(readSignal.count(), 1);
}

void tst_QFile::flush()
{
	QString fileName("stdfile.txt");

	QFile::remove(fileName);

	{
		QFile file(fileName);
		QVERIFY(file.open(QFile::WriteOnly));
		QCOMPARE(file.write("abc", 3),qint64(3));
	}

	{
		QFile file(fileName);
		QVERIFY(file.open(QFile::WriteOnly | QFile::Append));
        QCOMPARE(file.pos(), qlonglong(3));
        QCOMPARE(file.write("def", 3), qlonglong(3));
        QCOMPARE(file.pos(), qlonglong(6));
    }

    {
        QFile file("stdfile.txt");
        QVERIFY(file.open(QFile::ReadOnly));
        QCOMPARE(file.readAll(), QByteArray("abcdef"));
    }

	QFile::remove(fileName);
}

void tst_QFile::bufferedRead()
{
    QFile::remove("stdfile.txt");

    QFile file("stdfile.txt");
    QVERIFY(file.open(QFile::WriteOnly));
    file.write("abcdef");
    file.close();


    FILE *stdFile = fopen("stdfile.txt", "r");
    QVERIFY(stdFile);
    char c;
    QCOMPARE(int(fread(&c, 1, 1, stdFile)), 1);
    QCOMPARE(c, 'a');
    QCOMPARE(int(ftell(stdFile)), 1);

    {
        QFile file;
        QVERIFY(file.open(stdFile, QFile::ReadOnly));
        QCOMPARE(file.pos(), qlonglong(1));
        QCOMPARE(file.read(&c, 1), qlonglong(1));
        QCOMPARE(c, 'b');
        QCOMPARE(file.pos(), qlonglong(2));
    }

    fclose(stdFile);
}

void tst_QFile::isSequential()
{
    QFile zero("/dev/null");
    QVERIFY(zero.open(QFile::ReadOnly));
    QVERIFY(zero.isSequential());
}

void tst_QFile::encodeName()
{
    QCOMPARE(QFile::encodeName(QString()), QByteArray());
}

void tst_QFile::truncate()
{
    for (int i = 0; i < 2; ++i) {
        QFile file("truncate.txt");
        QVERIFY(file.open(QFile::WriteOnly));
        file.write(QByteArray(200, '@'));
        file.close();

        QVERIFY(file.open((i ? QFile::WriteOnly : QFile::ReadWrite) | QFile::Truncate));
        file.write(QByteArray(100, '$'));
        file.close();

        QVERIFY(file.open(QFile::ReadOnly));
        QCOMPARE(file.readAll(), QByteArray(100, '$'));
    }
}

void tst_QFile::seekToPos()
{
    {
        QFile file("seekToPos.txt");
        QVERIFY(file.open(QFile::WriteOnly));
        file.write("a\r\nb\r\nc\r\n");
        file.flush();
    }

    QFile file("seekToPos.txt");
    QVERIFY(file.open(QFile::ReadOnly | QFile::Text));
    file.seek(1);
    char c;
    QVERIFY(file.getChar(&c));
    QCOMPARE(c, '\n');

    QCOMPARE(file.pos(), qint64(3));
    file.seek(file.pos());
    QCOMPARE(file.pos(), qint64(3));

    file.seek(1);
    file.seek(file.pos());
    QCOMPARE(file.pos(), qint64(1));

}

void tst_QFile::seekAfterEndOfFile()
{
    QLatin1String filename("seekAfterEof.dat");
    QFile::remove(filename);
    {
        QFile file(filename);
        QVERIFY(file.open(QFile::WriteOnly));
        file.write("abcd");
        QCOMPARE(file.size(), qint64(4));
        file.seek(8);
        file.write("ijkl");
        QCOMPARE(file.size(), qint64(12));
        file.seek(4);
        file.write("efgh");
        QCOMPARE(file.size(), qint64(12));
        file.seek(16);
        file.write("----");
        QCOMPARE(file.size(), qint64(20));
        file.flush();
    }

    QFile file(filename);
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray contents = file.readAll();
    QCOMPARE(contents.left(12), QByteArray("abcdefghijkl", 12));
    //bytes 12-15 are uninitialised so we don't care what they read as.
    QCOMPARE(contents.mid(16), QByteArray("----", 4));
    file.close();
    QFile::remove(filename);
}

void tst_QFile::FILEReadWrite()
{
    // Tests modifying a file. First creates it then reads in 4 bytes and then overwrites these
    // 4 bytes with new values. At the end check to see the file contains the new values.

    QFile::remove("FILEReadWrite.txt");

    // create test file
    {
        QFile f("FILEReadWrite.txt");
        QVERIFY(f.open(QFile::WriteOnly));
        QDataStream ds(&f);
        qint8 c = 0;
        ds << c;
        c = 1;
        ds << c;
        c = 2;
        ds << c;
        c = 3;
        ds << c;
        c = 4;
        ds << c;
        c = 5;
        ds << c;
        c = 6;
        ds << c;
        c = 7;
        ds << c;
        c = 8;
        ds << c;
        c = 9;
        ds << c;
        c = 10;
        ds << c;
        c = 11;
        ds << c;
        f.close();
    }

    FILE *fp = fopen("FILEReadWrite.txt", "r+b");
    QVERIFY(fp);
    QFile file;
    QVERIFY(file.open(fp, QFile::ReadWrite));
    QDataStream sfile(&file) ;

    qint8 var1,var2,var3,var4;
    while (!sfile.atEnd())
    {
        qint64 base = file.pos();

        QCOMPARE(file.pos(), base + 0);
        sfile >> var1;
        QCOMPARE(file.pos(), base + 1);
        file.flush(); // flushing should not change the base
        QCOMPARE(file.pos(), base + 1);
        sfile >> var2;
        QCOMPARE(file.pos(), base + 2);
        sfile >> var3;
        QCOMPARE(file.pos(), base + 3);
        sfile >> var4;
        QCOMPARE(file.pos(), base + 4);
        file.seek(file.pos() - 4) ;   // Move it back 4, for we are going to write new values based on old ones
        QCOMPARE(file.pos(), base + 0);
        sfile << qint8(var1 + 5);
        QCOMPARE(file.pos(), base + 1);
        sfile << qint8(var2 + 5);
        QCOMPARE(file.pos(), base + 2);
        sfile << qint8(var3 + 5);
        QCOMPARE(file.pos(), base + 3);
        sfile << qint8(var4 + 5);
        QCOMPARE(file.pos(), base + 4);

    }
    file.close();
    fclose(fp);

    // check modified file
    {
        QFile f("FILEReadWrite.txt");
        QVERIFY(f.open(QFile::ReadOnly));
        QDataStream ds(&f);
        qint8 c = 0;
        ds >> c;
        QCOMPARE(c, (qint8)5);
        ds >> c;
        QCOMPARE(c, (qint8)6);
        ds >> c;
        QCOMPARE(c, (qint8)7);
        ds >> c;
        QCOMPARE(c, (qint8)8);
        ds >> c;
        QCOMPARE(c, (qint8)9);
        ds >> c;
        QCOMPARE(c, (qint8)10);
        ds >> c;
        QCOMPARE(c, (qint8)11);
        ds >> c;
        QCOMPARE(c, (qint8)12);
        ds >> c;
        QCOMPARE(c, (qint8)13);
        ds >> c;
        QCOMPARE(c, (qint8)14);
        ds >> c;
        QCOMPARE(c, (qint8)15);
        ds >> c;
        QCOMPARE(c, (qint8)16);
        f.close();
    }

    QFile::remove("FILEReadWrite.txt");
}


#ifdef TEST_LFS
void tst_QFile::largeFileSupport()
{
    static const QString largefilesrc(QDir::currentPath() + "/largefilesrc.txt");
    static const QString largefilerename(QDir::currentPath() + "/largefilerename.txt");
    static const QString largefilecopy(QDir::currentPath() + "/largefilecopy.txt");

    QFile largefile(largefilesrc);
    QVERIFY(largefile.open(QIODevice::Truncate | QIODevice::ReadWrite));
    QVERIFY(largefile.write("foo"));
    QVERIFY(largefile.resize(4000000000));
    largefile.close();

    QVERIFY(QFile::rename(largefilesrc, largefilerename));
    QVERIFY(QFile::copy(largefilerename, largefilecopy));
    QVERIFY(QFile::remove(largefilerename));
    QVERIFY(QFile::remove(largefilecopy));
}
#endif // TEST_LFS

void tst_QFile::i18nFileName_data()
{
    QTest::addColumn<QString>("fileName");

    QTest::newRow( "01" ) << QString::fromUtf8("xxxxxxx.txt");
}

void tst_QFile::i18nFileName()
{
     QFETCH(QString, fileName);
     if (QFile::exists(fileName)) {
         QVERIFY(QFile::remove(fileName));
     }
     {
        QFile file(fileName);
        QVERIFY(file.open(QFile::WriteOnly | QFile::Text));
        QTextStream ts(&file);
        ts.setCodec("UTF-8");
        ts << fileName << endl;
     }
     {
        QFile file(fileName);
        QVERIFY(file.open(QFile::ReadOnly | QFile::Text));
        QTextStream ts(&file);
        ts.setCodec("UTF-8");
        QString line = ts.readLine();
        QCOMPARE(line, fileName);
     }
     QVERIFY(QFile::remove(fileName));
}


void tst_QFile::longFileName_data()
{
    QTest::addColumn<QString>("fileName");

    QTest::newRow( "16 chars" ) << QString::fromLatin1("longFileName.txt");
    QTest::newRow( "52 chars" ) << QString::fromLatin1("longFileNamelongFileNamelongFileNamelongFileName.txt");
    QTest::newRow( "148 chars" ) << QString::fromLatin1("longFileNamelongFileNamelongFileNamelongFileName"
                                                     "longFileNamelongFileNamelongFileNamelongFileName"
                                                     "longFileNamelongFileNamelongFileNamelongFileName.txt");
    QTest::newRow( "244 chars" ) << QString::fromLatin1("longFileNamelongFileNamelongFileNamelongFileName"
                                                     "longFileNamelongFileNamelongFileNamelongFileName"
                                                     "longFileNamelongFileNamelongFileNamelongFileName"
                                                     "longFileNamelongFileNamelongFileNamelongFileName"
                                                     "longFileNamelongFileNamelongFileNamelongFileName.txt");
    QTest::newRow( "244 chars to absolutepath" ) << QFileInfo(QString::fromLatin1("longFileNamelongFileNamelongFileNamelongFileName"
                                                     "longFileNamelongFileNamelongFileNamelongFileName"
                                                     "longFileNamelongFileNamelongFileNamelongFileName"
                                                     "longFileNamelongFileNamelongFileNamelongFileName"
                                                     "longFileNamelongFileNamelongFileNamelongFileName.txt")).absoluteFilePath();
  /* needs to be put on a windows 2000 > test machine
  QTest::newRow( "244 chars on UNC" ) <<  QString::fromLatin1("//arsia/D/troll/tmp/longFileNamelongFileNamelongFileNamelongFileName"
                                                     "longFileNamelongFileNamelongFileNamelongFileName"
                                                     "longFileNamelongFileNamelongFileNamelongFileName"
                                                     "longFileNamelongFileNamelongFileNamelongFileName"
                                                     "longFileNamelongFileNamelongFileNamelongFileName.txt");*/
}

void tst_QFile::longFileName()
{
    QFETCH(QString, fileName);
    if (QFile::exists(fileName)) {
        QVERIFY(QFile::remove(fileName));
    }
    {
        QFile file(fileName);
        QVERIFY(file.open(QFile::WriteOnly | QFile::Text));
        QTextStream ts(&file);
        ts << fileName << endl;
    }
    {
        QFile file(fileName);
        QVERIFY(file.open(QFile::ReadOnly | QFile::Text));
        QTextStream ts(&file);
        QString line = ts.readLine();
        QCOMPARE(line, fileName);
    }
    QString newName = fileName + QLatin1String("1");
    {
        QVERIFY(QFile::copy(fileName, newName));
        QFile file(newName);
        QVERIFY(file.open(QFile::ReadOnly | QFile::Text));
        QTextStream ts(&file);
        QString line = ts.readLine();
        QCOMPARE(line, fileName);

    }
    QVERIFY(QFile::remove(newName));
    {
        QVERIFY(QFile::rename(fileName, newName));
        QFile file(newName);
        QVERIFY(file.open(QFile::ReadOnly | QFile::Text));
        QTextStream ts(&file);
        QString line = ts.readLine();
        QCOMPARE(line, fileName);
    }
    QVERIFY(QFile::exists(newName));
    QVERIFY(QFile::remove(newName));
}

class MyEngine : public QAbstractFileEngine
{
public:
    MyEngine(int n) { number = n; }
    virtual ~MyEngine() {}

    void setFileName(const QString &) {}
    bool open(QIODevice::OpenMode) { return false; }
    bool close() { return false; }
    bool flush() { return false; }
    qint64 size() const { return 123 + number; }
    qint64 at() const { return -1; }
    bool seek(qint64) { return false; }
    bool isSequential() const { return false; }
    qint64 read(char *, qint64) { return -1; }
    qint64 write(const char *, qint64) { return -1; }
    bool remove() { return false; }
    bool copy(const QString &) { return false; }
    bool rename(const QString &) { return false; }
    bool link(const QString &) { return false; }
    bool mkdir(const QString &, bool) const { return false; }
    bool rmdir(const QString &, bool) const { return false; }
    bool setSize(qint64) { return false; }
    QStringList entryList(QDir::Filters, const QStringList &) const { return QStringList(); }
    bool caseSensitive() const { return false; }
    bool isRelativePath() const { return false; }
    FileFlags fileFlags(FileFlags) const { return 0; }
    bool chmod(uint) { return false; }
    QString fileName(FileName) const { return name; }
    uint ownerId(FileOwner) const { return 0; }
    QString owner(FileOwner) const { return QString(); }
    QDateTime fileTime(FileTime) const { return QDateTime(); }

private:
    int number;
    QString name;
};

void tst_QFile::getCharFF()
{
    QFile file("file.txt");
    file.open(QFile::ReadWrite);
    file.write("\xff\xff\xff");
    file.flush();
    file.seek(0);

    char c;
    QVERIFY(file.getChar(&c));
    QVERIFY(file.getChar(&c));
    QVERIFY(file.getChar(&c));
}

void tst_QFile::remove_and_exists()
{
    QFile::remove("tull_i_grunn.txt");
    QFile f("tull_i_grunn.txt");

    QVERIFY(!f.exists());

    bool opened = f.open(QIODevice::WriteOnly);
    QVERIFY(opened);

    f.write(QString("testing that remove/exists work...").toLatin1());
    f.close();

    QVERIFY(f.exists());

    f.remove();
    QVERIFY(!f.exists());
}

void tst_QFile::removeOpenFile()
{
    {
        // remove an opened, write-only file
        QFile::remove("remove_unclosed.txt");
        QFile f("remove_unclosed.txt");

        QVERIFY(!f.exists());
        bool opened = f.open(QIODevice::WriteOnly);
        QVERIFY(opened);
        f.write(QString("testing that remove closes the file first...").toLatin1());

        bool removed = f.remove(); // remove should both close and remove the file
        QVERIFY(removed);
        QVERIFY(!f.isOpen());
        QVERIFY(!f.exists());
        QVERIFY(f.error() == QFile::NoError);
    }

    {
        // remove an opened, read-only file
        QFile::remove("remove_unclosed.txt");

        // first, write a file that we can remove
        {
            QFile f("remove_unclosed.txt");
            QVERIFY(!f.exists());
            bool opened = f.open(QIODevice::WriteOnly);
            QVERIFY(opened);
            f.write(QString("testing that remove closes the file first...").toLatin1());
            f.close();
        }

        QFile f("remove_unclosed.txt");
        bool opened = f.open(QIODevice::ReadOnly);
        QVERIFY(opened);
        f.readAll();
        // this used to only fail on FreeBSD (and Mac OS X)
        QVERIFY(f.flush());
        bool removed = f.remove(); // remove should both close and remove the file
        QVERIFY(removed);
        QVERIFY(!f.isOpen());
        QVERIFY(!f.exists());
        QVERIFY(f.error() == QFile::NoError);
    }
}

void tst_QFile::fullDisk()
{
    QFile file("/dev/full");
    if (!file.exists())
        QSKIP("/dev/full doesn't exist on this system", SkipAll);

    QVERIFY(file.open(QIODevice::WriteOnly));
    file.write("foobar", 6);

    QVERIFY(!file.flush());
    QCOMPARE(file.error(), QFile::ResourceError);
    QVERIFY(!file.flush());
    QCOMPARE(file.error(), QFile::ResourceError);

    char c = 0;
    file.write(&c, 0);
    QVERIFY(!file.flush());
    QCOMPARE(file.error(), QFile::ResourceError);
    QCOMPARE(file.write(&c, 1), qint64(1));
    QVERIFY(!file.flush());
    QCOMPARE(file.error(), QFile::ResourceError);

    file.close();
    QVERIFY(!file.isOpen());
    QCOMPARE(file.error(), QFile::ResourceError);

    file.open(QIODevice::WriteOnly);
    QCOMPARE(file.error(), QFile::NoError);
    QVERIFY(file.flush()); // Shouldn't inherit write buffer
    file.close();
    QCOMPARE(file.error(), QFile::NoError);

    // try again without flush:
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.write("foobar", 6);
    file.close();
    QVERIFY(file.error() != QFile::NoError);
}

void tst_QFile::writeLargeDataBlock_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<int>("type");

    QTest::newRow("localfile-QFile")  << "./largeblockfile.txt" << (int)OpenQFile;
    QTest::newRow("localfile-Fd")     << "./largeblockfile.txt" << (int)OpenFd;
    QTest::newRow("localfile-Stream") << "./largeblockfile.txt" << (int)OpenStream;
}

static QByteArray getLargeDataBlock()
{
    static QByteArray array;

    if (array.isNull())
    {
        int resizeSize = 64 * 1024 * 1024;
        array.resize(resizeSize);
        for (int i = 0; i < array.size(); ++i)
            array[i] = uchar(i);
    }

    return array;
}

void tst_QFile::writeLargeDataBlock()
{
    QFETCH(QString, fileName);
    QFETCH( int, type );

    QByteArray const originalData = getLargeDataBlock();

    {
        QFile file(fileName);

        QVERIFY2( openFile(file, QIODevice::WriteOnly, (FileType)type),
            qPrintable(QString("Couldn't open file for writing: [%1]").arg(fileName)) );
        qint64 fileWriteOriginalData = file.write(originalData);
        qint64 originalDataSize      = (qint64)originalData.size();
        QCOMPARE( fileWriteOriginalData, originalDataSize );
        QVERIFY( file.flush() );

        closeFile(file);
    }

    QByteArray readData;

    {
        QFile file(fileName);

        QVERIFY2( openFile(file, QIODevice::ReadOnly, (FileType)type),
            qPrintable(QString("Couldn't open file for reading: [%1]").arg(fileName)) );
        readData = file.readAll();
        closeFile(file);
    }

    QCOMPARE( readData, originalData );
    QVERIFY( QFile::remove(fileName) );
}

void tst_QFile::readFromWriteOnlyFile()
{
    QFile file("writeonlyfile");
    QVERIFY(file.open(QFile::WriteOnly));
    char c;
    QTest::ignoreMessage(QtWarningMsg, "QIODevice::read: WriteOnly device");
    QCOMPARE(file.read(&c, 1), qint64(-1));
}

void tst_QFile::writeToReadOnlyFile()
{
    QFile file("readonlyfile");
    QVERIFY(file.open(QFile::ReadOnly));
    char c = 0;
    QTest::ignoreMessage(QtWarningMsg, "QIODevice::write: ReadOnly device");
    QCOMPARE(file.write(&c, 1), qint64(-1));
}

void tst_QFile::virtualFile()
{
    // test if QFile works with virtual files
    QString fname;
#if defined(Q_OS_LINUX)
    fname = "/proc/self/maps";
#else
    QSKIP("This platform does not have 0-sized virtual files", SkipAll);
#endif

    // consistency check
    QFileInfo fi(fname);
    QVERIFY(fi.exists());
    QVERIFY(fi.isFile());
    QCOMPARE(fi.size(), Q_INT64_C(0));

    // open the file
    QFile f(fname);
    QVERIFY(f.open(QIODevice::ReadOnly));
    QCOMPARE(f.size(), Q_INT64_C(0));
    QVERIFY(f.atEnd());

    // read data
    QByteArray data = f.read(16);
    QCOMPARE(data.size(), 16);
    QCOMPARE(f.pos(), Q_INT64_C(16));

    // line-reading
    data = f.readLine();
    QVERIFY(!data.isEmpty());

    // read all:
    data = f.readAll();
    QVERIFY(f.pos() != 0);
    QVERIFY(!data.isEmpty());

    // seeking
    QVERIFY(f.seek(1));
    QCOMPARE(f.pos(), Q_INT64_C(1));
}

void tst_QFile::textFile()
{
    FILE *fs = ::fopen("writeabletextfile", "w");
    QFile f;
    QByteArray part1("This\nis\na\nfile\nwith\nnewlines\n");
    QByteArray part2("Add\nsome\nmore\nnewlines\n");

    QVERIFY(f.open(fs, QIODevice::WriteOnly));
    f.write(part1);
    f.write(part2);
    f.close();
    ::fclose(fs);

    QFile file("writeabletextfile");
    QVERIFY(file.open(QIODevice::ReadOnly));

    QByteArray data = file.readAll();

    QByteArray expected = part1 + part2;
    QCOMPARE(data, expected);
    file.close();
    file.remove();
}

void tst_QFile::rename_data()
{
    QTest::addColumn<QString>("source");
    QTest::addColumn<QString>("destination");
    QTest::addColumn<bool>("result");

    QTest::newRow("a -> b") << QString("a") << QString("b") << false;
    QTest::newRow("a -> .") << QString("a") << QString(".") << false;
    QTest::newRow("renamefile -> renamefile") << QString("renamefile") << QString("renamefile") << false;
    QTest::newRow("renamefile -> noreadfile") << QString("renamefile") << QString("noreadfile") << false;
    QTest::newRow("renamefile -> renamedfile") << QString("renamefile") << QString("renamedfile") << true;
    QTest::newRow("renamefile -> ..") << QString("renamefile") << QString("..") << false;

    if (!currentuserisroot) {
        // root and Chuck Norris don't care for permissions.
        QTest::newRow("renamefile -> /etc/renamefile") << QString("renamefile") << QString("/etc/renamefile") << false;
    }
}

void tst_QFile::rename()
{
    QFETCH(QString, source);
    QFETCH(QString, destination);
    QFETCH(bool, result);

    QFile::remove("renamedfile");
    QFile f("renamefile");
    f.open(QFile::WriteOnly);
    f.close();

    QFile file(source);
    QCOMPARE(file.rename(destination), result);

    if (result)
        QCOMPARE(file.error(), QFile::NoError);
    else
        QCOMPARE(file.error(), QFile::RenameError);

    QFile::remove("renamefile");
}

/*!
 \since 4.5

 Some special files have QFile::atEnd() returning true, even though there is
 more data available. True for corner cases, as well as some mounts on OS X.

 Here, we reproduce that condition by having a QFile sub-class with this
 peculiar atEnd() behavior.

 See task 231583.
 */
void tst_QFile::renameWithAtEndSpecialFile() const
{
    class PeculiarAtEnd : public QFile
    {
    public:
        virtual bool atEnd() const
        {
            return true;
        }
    };

    const QString newName(QLatin1String("newName.txt"));
    /* Cleanup, so we're a bit more robust. */
    QFile::remove(newName);

    const QString originalName(QString(SRCDIR "forRenaming.txt"));

    PeculiarAtEnd file;
    file.setFileName(originalName);
    QVERIFY(file.open(QIODevice::ReadOnly));

    QVERIFY(file.rename(newName));

    file.close();
    /* Guess what, we have to rename it back, otherwise we'll fail on second
     * invocation. */
    QVERIFY(QFile::rename(newName, originalName));
}

void tst_QFile::resourceCopyRename()
{
    // Using a resource file as a *read-only* source whose move and copy should fail.
    QFile file(":/copy-rename.qrc");
    QVERIFY(file.exists());
    QFile::remove("file-rename-destination.txt");

    QVERIFY(!file.copy("file-copy-destination.txt"));
    QVERIFY(!QFile::exists("file-copy-destination.txt"));
    QVERIFY(!file.isOpen());

    QVERIFY(!file.rename("file-rename-destination.txt"));
    QVERIFY(!QFile::exists("file-rename-destination.txt"));
    QVERIFY(!file.isOpen());
}

void tst_QFile::renameMultiple()
{
    // create the file if it doesn't exist
    QFile file("file-to-be-renamed.txt");
    QFile file2("existing-file.txt");
    QVERIFY(file.open(QIODevice::ReadWrite));
    QVERIFY(file2.open(QIODevice::ReadWrite));

    // any stale files from previous test failures?
    QFile::remove("file-renamed-once.txt");
    QFile::remove("file-renamed-twice.txt");

    // begin testing
    QVERIFY(QFile::exists("existing-file.txt"));
    QVERIFY(!file.rename("existing-file.txt"));
    QCOMPARE(file.error(), QFile::RenameError);
    QCOMPARE(file.fileName(), QString("file-to-be-renamed.txt"));

    QVERIFY(file.rename("file-renamed-once.txt"));
    QVERIFY(!file.isOpen());
    QCOMPARE(file.fileName(), QString("file-renamed-once.txt"));

    QVERIFY(QFile::exists("existing-file.txt"));
    QVERIFY(!file.rename("existing-file.txt"));
    QCOMPARE(file.error(), QFile::RenameError);
    QCOMPARE(file.fileName(), QString("file-renamed-once.txt"));

    QVERIFY(file.rename("file-renamed-twice.txt"));
    QVERIFY(!file.isOpen());
    QCOMPARE(file.fileName(), QString("file-renamed-twice.txt"));

    QVERIFY(QFile::exists("existing-file.txt"));
    QVERIFY(!QFile::exists("file-to-be-renamed.txt"));
    QVERIFY(!QFile::exists("file-renamed-once.txt"));
    QVERIFY(QFile::exists("file-renamed-twice.txt"));

    file.remove();
    file2.remove();
    QVERIFY(!QFile::exists("file-renamed-twice.txt"));
    QVERIFY(!QFile::exists("existing-file.txt"));
}

void tst_QFile::appendAndRead()
{
    QFile writeFile(QLatin1String("appendfile.txt"));
    QVERIFY(writeFile.open(QIODevice::WriteOnly | QIODevice::Truncate));

    QFile readFile(QLatin1String("appendfile.txt"));
    QVERIFY(readFile.open(QIODevice::ReadOnly));

    // Write to the end of the file, then read that character back, and so on.
    for (int i = 0; i < 100; ++i) {
        char c = '\0';
        writeFile.putChar(char(i % 256));
        writeFile.flush();
        QVERIFY(readFile.getChar(&c));
        QCOMPARE(c, char(i % 256));
        QCOMPARE(readFile.pos(), writeFile.pos());
    }

    // Write blocks and read them back
    for (int j = 0; j < 18; ++j) {
        writeFile.write(QByteArray(1 << j, '@'));
        writeFile.flush();
        QCOMPARE(readFile.read(1 << j).size(), 1 << j);
    }

    readFile.close();
    QFile::remove(QLatin1String("appendfile.txt"));
}

void tst_QFile::standarderror()
{
    QFile f;
    bool ok = f.open(stderr, QFile::WriteOnly);
    QVERIFY(ok);
    f.close();
}

void tst_QFile::handle()
{
    int fd;
    QFile file(SRCDIR "tst_qfile.cpp");
    QVERIFY(file.open(QIODevice::ReadOnly));
    fd = int(file.handle());
    QVERIFY(fd > 2);
    QCOMPARE(int(file.handle()), fd);
    char c = '\0';
    QT_READ(int(file.handle()), &c, 1);
    QCOMPARE(c, '/');

    // test if the QFile and the handle remain in sync
    QVERIFY(file.getChar(&c));
    QCOMPARE(c, '*');

    // same, but read from QFile first now
    file.close();
    QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Unbuffered));
    fd = int(file.handle());
    QVERIFY(fd > 2);
    QVERIFY(file.getChar(&c));
    QCOMPARE(c, '/');
    QCOMPARE(QT_READ(fd, &c, 1), ssize_t(1));

    QCOMPARE(c, '*');

    //test round trip of adopted stdio file handle
    QFile file2;
    FILE *fp = fopen(SRCDIR "tst_qfile.cpp", "r");
    file2.open(fp, QIODevice::ReadOnly);
    QCOMPARE(int(file2.handle()), int(fileno(fp)));
    QCOMPARE(int(file2.handle()), int(fileno(fp)));
    fclose(fp);

    //test round trip of adopted posix file handle
    QFile file3;
    fd = QT_OPEN(SRCDIR "tst_qfile.cpp", QT_OPEN_RDONLY);
    file3.open(fd, QIODevice::ReadOnly);
    QCOMPARE(int(file3.handle()), fd);
    QT_CLOSE(fd);
}

void tst_QFile::nativeHandleLeaks()
{
    int fd1, fd2;

    {
        QFile file("qt_file.tmp");
        QVERIFY( file.open(QIODevice::ReadWrite) );

        fd1 = file.handle();
        QVERIFY( -1 != fd1 );
    }

    {
        QFile file("qt_file.tmp");
        QVERIFY( file.open(QIODevice::ReadOnly) );

        fd2 = file.handle();
        QVERIFY( -1 != fd2 );
    }

    QCOMPARE( fd2, fd1 );
}

void tst_QFile::readEof_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<int>("imode");

    QTest::newRow("buffered") << SRCDIR "testfile.txt" << 0;
    QTest::newRow("unbuffered") << SRCDIR "testfile.txt" << int(QIODevice::Unbuffered);

    QTest::newRow("sequential,buffered") << "/dev/null" << 0;
    QTest::newRow("sequential,unbuffered") << "/dev/null" << int(QIODevice::Unbuffered);
}

void tst_QFile::readEof()
{
    QFETCH(QString, filename);
    QFETCH(int, imode);
    QIODevice::OpenMode mode = QIODevice::OpenMode(imode);

    {
        QFile file(filename);
        QVERIFY(file.open(QIODevice::ReadOnly | mode));
        bool isSequential = file.isSequential();
        if (!isSequential) {
            QVERIFY(file.seek(245));
            QVERIFY(file.atEnd());
        }

        char buf[10];
        int ret = file.read(buf, sizeof buf);
        QCOMPARE(ret, 0);
        QVERIFY(file.error() == QFile::NoError);
        QVERIFY(file.atEnd());

        // Do it again to ensure that we get the same result
        ret = file.read(buf, sizeof buf);
        QCOMPARE(ret, 0);
        QVERIFY(file.error() == QFile::NoError);
        QVERIFY(file.atEnd());
    }

    {
        QFile file(filename);
        QVERIFY(file.open(QIODevice::ReadOnly | mode));
        bool isSequential = file.isSequential();
        if (!isSequential) {
            QVERIFY(file.seek(245));
            QVERIFY(file.atEnd());
        }

        QByteArray ret = file.read(10);
        QVERIFY(ret.isEmpty());
        QVERIFY(file.error() == QFile::NoError);
        QVERIFY(file.atEnd());

        // Do it again to ensure that we get the same result
        ret = file.read(10);
        QVERIFY(ret.isEmpty());
        QVERIFY(file.error() == QFile::NoError);
        QVERIFY(file.atEnd());
    }

    {
        QFile file(filename);
        QVERIFY(file.open(QIODevice::ReadOnly | mode));
        bool isSequential = file.isSequential();
        if (!isSequential) {
            QVERIFY(file.seek(245));
            QVERIFY(file.atEnd());
        }

        char buf[10];
        int ret = file.readLine(buf, sizeof buf);
        QCOMPARE(ret, -1);
        QVERIFY(file.error() == QFile::NoError);
        QVERIFY(file.atEnd());

        // Do it again to ensure that we get the same result
        ret = file.readLine(buf, sizeof buf);
        QCOMPARE(ret, -1);
        QVERIFY(file.error() == QFile::NoError);
        QVERIFY(file.atEnd());
    }

    {
        QFile file(filename);
        QVERIFY(file.open(QIODevice::ReadOnly | mode));
        bool isSequential = file.isSequential();
        if (!isSequential) {
            QVERIFY(file.seek(245));
            QVERIFY(file.atEnd());
        }

        QByteArray ret = file.readLine();
        QVERIFY(ret.isNull());
        QVERIFY(file.error() == QFile::NoError);
        QVERIFY(file.atEnd());

        // Do it again to ensure that we get the same result
        ret = file.readLine();
        QVERIFY(ret.isNull());
        QVERIFY(file.error() == QFile::NoError);
        QVERIFY(file.atEnd());
    }

    {
        QFile file(filename);
        QVERIFY(file.open(QIODevice::ReadOnly | mode));
        bool isSequential = file.isSequential();
        if (!isSequential) {
            QVERIFY(file.seek(245));
            QVERIFY(file.atEnd());
        }

        char c;
        QVERIFY(!file.getChar(&c));
        QVERIFY(file.error() == QFile::NoError);
        QVERIFY(file.atEnd());

        // Do it again to ensure that we get the same result
        QVERIFY(!file.getChar(&c));
        QVERIFY(file.error() == QFile::NoError);
        QVERIFY(file.atEnd());
    }
}

void tst_QFile::task167217()
{
    // Regression introduced in 4.3.0; after a failed stat, pos() could no
    // longer be calculated correctly.
    QFile::remove("tmp.txt");
    QFile file("tmp.txt");
    QVERIFY(!file.exists());
    QVERIFY(file.open(QIODevice::Append));
    QVERIFY(file.exists());
    file.write("qt430", 5);
    QVERIFY(!file.isSequential());
    QCOMPARE(file.pos(), qint64(5));
    file.remove();
}

#define FILESIZE 65536 * 3

void tst_QFile::map_data()
{
    QTest::addColumn<int>("fileSize");
    QTest::addColumn<int>("offset");
    QTest::addColumn<int>("size");
    QTest::addColumn<QFile::FileError>("error");

    QTest::newRow("zero")         << FILESIZE << 0     << FILESIZE         << QFile::NoError;
    QTest::newRow("small, but 0") << FILESIZE << 30    << FILESIZE - 30    << QFile::NoError;
    QTest::newRow("a page")       << FILESIZE << 4096  << FILESIZE - 4096  << QFile::NoError;
    QTest::newRow("+page")        << FILESIZE << 5000  << FILESIZE - 5000  << QFile::NoError;
    QTest::newRow("++page")       << FILESIZE << 65576 << FILESIZE - 65576 << QFile::NoError;
    QTest::newRow("bad size")     << FILESIZE << 0     << -1               << QFile::ResourceError;
    QTest::newRow("bad offset")   << FILESIZE << -1    << 1                << QFile::UnspecifiedError;
    QTest::newRow("zerozero")     << FILESIZE << 0     << 0                << QFile::UnspecifiedError;
}

void tst_QFile::map()
{
    QFETCH(int, fileSize);
    QFETCH(int, offset);
    QFETCH(int, size);
    QFETCH(QFile::FileError, error);

    QString fileName = QDir::currentPath() + '/' + "qfile_map_testfile";

    if (QFile::exists(fileName)) {
        QVERIFY(QFile::setPermissions(fileName,
            QFile::WriteOwner | QFile::ReadOwner | QFile::WriteUser | QFile::ReadUser));
        QFile::remove(fileName);
    }
    QFile file(fileName);

    // invalid, not open
    uchar *memory = file.map(0, size);
    QVERIFY(!memory);
    QCOMPARE(file.error(), QFile::PermissionsError);
    QVERIFY(!file.unmap(memory));
    QCOMPARE(file.error(), QFile::PermissionsError);

    // make a file
    QVERIFY(file.open(QFile::ReadWrite));
    QVERIFY(file.resize(fileSize));
    QVERIFY(file.flush());
    file.close();
    QVERIFY(file.open(QFile::ReadWrite));
    memory = file.map(offset, size);
    if (error != QFile::NoError) {

	QVERIFY(file.error() != QFile::NoError);
        return;
    }

    QCOMPARE(file.error(), error);
    QVERIFY(memory);
    memory[0] = 'Q';
    QVERIFY(file.unmap(memory));
    QCOMPARE(file.error(), QFile::NoError);

    // Verify changes were saved
    memory = file.map(offset, size);
    QCOMPARE(file.error(), QFile::NoError);
    QVERIFY(memory);
    QVERIFY(memory[0] == 'Q');
    QVERIFY(file.unmap(memory));
    QCOMPARE(file.error(), QFile::NoError);

    // exotic test to make sure that multiple maps work

    // note: windows ce does not reference count mutliple maps
    // it's essentially just the same reference but it 
    // cause a resource lock on the file which prevents it 
    // from being removed    uchar *memory1 = file.map(0, file.size());
    uchar *memory1 = file.map(0, file.size());
    QCOMPARE(file.error(), QFile::NoError);
    uchar *memory2 = file.map(0, file.size());
    QCOMPARE(file.error(), QFile::NoError);
    QVERIFY(memory1);
    QVERIFY(memory2);
    QVERIFY(file.unmap(memory1));
    QCOMPARE(file.error(), QFile::NoError);
    QVERIFY(file.unmap(memory2));
    QCOMPARE(file.error(), QFile::NoError);
    memory1 = file.map(0, file.size());
    QCOMPARE(file.error(), QFile::NoError);
    QVERIFY(memory1);
    QVERIFY(file.unmap(memory1));
    QCOMPARE(file.error(), QFile::NoError);

    file.close();

    if (!currentuserisroot)
        // root always has permissions
    {
        // Change permissions on a file, just to confirm it would fail
        QFile::Permissions originalPermissions = file.permissions();
        QVERIFY(file.setPermissions(QFile::ReadOther));
        QVERIFY(!file.open(QFile::ReadWrite));
        memory = file.map(offset, size);
        QCOMPARE(file.error(), QFile::PermissionsError);
        QVERIFY(!memory);
        QVERIFY(file.setPermissions(originalPermissions));
    }
    QVERIFY(file.remove());
}

void tst_QFile::mapResource_data()
{
    QTest::addColumn<int>("offset");
    QTest::addColumn<int>("size");
    QTest::addColumn<QFile::FileError>("error");
    QTest::addColumn<QString>("fileName");

    QString validFile = ":/tst_qfileinfo/resources/file1.ext1";
    QString invalidFile = ":/tst_qfileinfo/resources/filefoo.ext1";

    for (int i = 0; i < 2; ++i) {
        QString file = (i == 0) ? validFile : invalidFile;
        QTest::newRow("0, 0") << 0 << 0 << QFile::UnspecifiedError << file;
        QTest::newRow("0, BIG") << 0 << 4096 << QFile::UnspecifiedError << file;
        QTest::newRow("-1, 0") << -1 << 0 << QFile::UnspecifiedError << file;
        QTest::newRow("0, -1") << 0 << -1 << QFile::UnspecifiedError << file;
    }

    QTest::newRow("0, 1") << 0 << 1 << QFile::NoError << validFile;
}

void tst_QFile::mapResource()
{
    QFETCH(QString, fileName);
    QFETCH(int, offset);
    QFETCH(int, size);
    QFETCH(QFile::FileError, error);

    QFile file(fileName);
    uchar *memory = file.map(offset, size);
    QCOMPARE(file.error(), error);
    QVERIFY((error == QFile::NoError) ? (memory != 0) : (memory == 0));
    if (error == QFile::NoError)
        QCOMPARE(QString(memory[0]), QString::number(offset + 1));
    QVERIFY(file.unmap(memory));
}

void tst_QFile::mapOpenMode_data()
{
    QTest::addColumn<int>("openMode");

    QTest::newRow("ReadOnly") << int(QIODevice::ReadOnly);
    //QTest::newRow("WriteOnly") << int(QIODevice::WriteOnly); // this doesn't make sense
    QTest::newRow("ReadWrite") << int(QIODevice::ReadWrite);
    QTest::newRow("ReadOnly,Unbuffered") << int(QIODevice::ReadOnly | QIODevice::Unbuffered);
    QTest::newRow("ReadWrite,Unbuffered") << int(QIODevice::ReadWrite | QIODevice::Unbuffered);
}

void tst_QFile::mapOpenMode()
{
    QFETCH(int, openMode);
    static const qint64 fileSize = 4096;

    QByteArray pattern(fileSize, 'A');

    QString fileName = QDir::currentPath() + '/' + "qfile_map_testfile";
    if (QFile::exists(fileName)) {
        QVERIFY(QFile::setPermissions(fileName,
            QFile::WriteOwner | QFile::ReadOwner | QFile::WriteUser | QFile::ReadUser));
	QFile::remove(fileName);
    }
    QFile file(fileName);

    // make a file
    QVERIFY(file.open(QFile::ReadWrite));
    QVERIFY(file.write(pattern));
    QVERIFY(file.flush());
    file.close();

    // open according to our mode
    QVERIFY(file.open(QIODevice::OpenMode(openMode)));

    uchar *memory = file.map(0, fileSize);
    QVERIFY(memory);
    QVERIFY(memcmp(memory, pattern, fileSize) == 0);

    if (openMode & QIODevice::WriteOnly) {
        // try to write to the file
        *memory = 'a';
        file.unmap(memory);
        file.close();
        file.open(QIODevice::OpenMode(openMode));
        file.seek(0);
        char c;
        QVERIFY(file.getChar(&c));
        QCOMPARE(c, 'a');
    }

    file.close();
}

void tst_QFile::openDirectory()
{
    QFile f1(SRCDIR "resources");
    // it's a directory, it must not exist
    QVERIFY(!f1.exists());

    // ...but not be openable
    QVERIFY(!f1.open(QIODevice::ReadOnly));
    f1.close();
    QVERIFY(!f1.open(QIODevice::ReadOnly|QIODevice::Unbuffered));
    f1.close();
    QVERIFY(!f1.open(QIODevice::ReadWrite));
    f1.close();
    QVERIFY(!f1.open(QIODevice::WriteOnly));
    f1.close();
    QVERIFY(!f1.open(QIODevice::WriteOnly|QIODevice::Unbuffered));
    f1.close();
}

static qint64 streamExpectedSize(int fd)
{
    QT_STATBUF sb;
    if (QT_FSTAT(fd, &sb) != -1)
        return sb.st_size;
    return 0;
}

static qint64 streamCurrentPosition(int fd)
{
    QT_OFF_T pos = QT_LSEEK(fd, 0, SEEK_CUR);
    if (pos != -1)
        return pos;
    return 0;
}

static qint64 streamCurrentPosition(FILE *f)
{
    QT_OFF_T pos = QT_FTELL(f);
    if (pos != -1)
        return pos;
    return 0;
}

void tst_QFile::openStandardStreamsFileDescriptors()
{
    {
        QFile in;
        in.open(STDIN_FILENO, QIODevice::ReadOnly);
        QCOMPARE( in.pos(), streamCurrentPosition(STDIN_FILENO) );
        QCOMPARE( in.size(), streamExpectedSize(STDIN_FILENO) );
    }

    {
        QFile out;
        QVERIFY(out.open(STDOUT_FILENO, QIODevice::WriteOnly));
        QCOMPARE( out.pos(), streamCurrentPosition(STDOUT_FILENO) );
        QCOMPARE( out.size(), streamExpectedSize(STDOUT_FILENO) );
    }

    {
        QFile err;
        err.open(STDERR_FILENO, QIODevice::WriteOnly);
        QCOMPARE( err.pos(), streamCurrentPosition(STDERR_FILENO) );
        QCOMPARE( err.size(), streamExpectedSize(STDERR_FILENO) );
    }
}

void tst_QFile::openStandardStreamsBufferedStreams()
{
    // Using streams
    {
        /* in/out/err.isSequential() are only true when run in a console (CI);
        * it is false when they are redirected from/to files.
        * Prevent failures in case someone runs tests with stdout/stderr redirected. */
        QFile in;
        in.open(stdin, QIODevice::ReadOnly);
        QCOMPARE( in.pos(), streamCurrentPosition(stdin) );
        QCOMPARE( in.size(), streamExpectedSize(QT_FILENO(stdin)) );
    }

    {
        QFile out;
        out.open(stdout, QIODevice::WriteOnly);
        QCOMPARE( out.pos(), streamCurrentPosition(stdout) );
        QCOMPARE( out.size(), streamExpectedSize(QT_FILENO(stdout)) );
    }

    {
        QFile err;
        err.open(stderr, QIODevice::WriteOnly);
        QCOMPARE( err.pos(), streamCurrentPosition(stderr) );
        QCOMPARE( err.size(), streamExpectedSize(QT_FILENO(stderr)) );
    }
}

void tst_QFile::openStandardStreams()
{
    openStandardStreamsFileDescriptors();
    openStandardStreamsBufferedStreams();
}

void tst_QFile::writeNothing()
{
    for (int i = 0; i < NumberOfFileTypes; ++i) {
        QFile file("file.txt");
        QVERIFY( openFile(file, QIODevice::WriteOnly | QIODevice::Unbuffered, FileType(i)) );
        QVERIFY( 0 == file.write((char *)0, 0) );
        QCOMPARE( file.error(), QFile::NoError );
        closeFile(file);
    }
}

void tst_QFile::resize_data()
{
    QTest::addColumn<int>("filetype");

    QTest::newRow("native") << int(OpenQFile);
    QTest::newRow("fileno") << int(OpenFd);
    QTest::newRow("stream") << int(OpenStream);
}

void tst_QFile::resize()
{
    QFETCH(int, filetype);
    QString filename(QLatin1String("file.txt"));
    QFile file(filename);
    QVERIFY(openFile(file, QIODevice::ReadWrite, FileType(filetype)));
    QVERIFY(file.resize(8));
    QCOMPARE(file.size(), qint64(8));
    closeFile(file);
    QFile::resize(filename, 4);
    QCOMPARE(QFileInfo(filename).size(), qint64(4));
    QVERIFY(QFile::remove(filename));
}

void tst_QFile::objectConstructors()
{
    QObject ob;
    QFile* file1 = new QFile(SRCDIR "testfile.txt", &ob);
    QFile* file2 = new QFile(&ob);
    QVERIFY(file1->exists());
    QVERIFY(!file2->exists());
}

void tst_QFile::caseSensitivity()
{
    const bool caseSensitive = true;
    QByteArray testData("a little test");
    QString filename("File.txt");
    {
        QFile f(filename);
        QVERIFY(f.open(QIODevice::WriteOnly));
        QVERIFY(f.write(testData));
        f.close();
    }
    QStringList alternates;
    QFileInfo fi(filename);
    QVERIFY(fi.exists());
    alternates << "file.txt" << "File.TXT" << "fIlE.TxT" << fi.absoluteFilePath().toUpper() << fi.absoluteFilePath().toLower();
    foreach (QString alt, alternates) {
        QFileInfo fi2(alt);
        QCOMPARE(fi2.exists(), !caseSensitive);
        QCOMPARE(fi.size() == fi2.size(), !caseSensitive);
        QFile f2(alt);
        QCOMPARE(f2.open(QIODevice::ReadOnly), !caseSensitive);
        if (!caseSensitive)
            QCOMPARE(f2.readAll(), testData);
    }
}

void tst_QFile::autocloseHandle()
{
    {
        QFile file("readonlyfile");
        QVERIFY(openFile(file, QIODevice::ReadOnly, OpenFd, QFile::AutoCloseHandle));
        int fd = fd_;
        QCOMPARE(file.handle(), fd);
        file.close();
        fd_ = -1;
        QCOMPARE(file.handle(), -1);
        //file is closed, read should fail
        char buf;
        QCOMPARE((int)QT_READ(fd, &buf, 1), -1);
        QVERIFY(errno = EBADF);
    }

    {
        QFile file("readonlyfile");
        QVERIFY(openFile(file, QIODevice::ReadOnly, OpenFd, QFile::DontCloseHandle));
        QCOMPARE(file.handle(), fd_);
        file.close();
        QCOMPARE(file.handle(), -1);
        //file is not closed, read should succeed
        char buf;
        QCOMPARE((int)QT_READ(fd_, &buf, 1), 1);
        ::close(fd_);
        fd_ = -1;
    }

    {
        QFile file("readonlyfile");
        QVERIFY(openFile(file, QIODevice::ReadOnly, OpenStream, QFile::AutoCloseHandle));
        int fd = fileno(stream_);
        QCOMPARE(file.handle(), fd);
        file.close();
        stream_ = 0;
        QCOMPARE(file.handle(), -1);
        //file is closed, read should fail
        char buf;
        QCOMPARE((int)QT_READ(fd, &buf, 1), -1); //not using fread because the FILE* was freed by fclose
    }

    {
        QFile file("readonlyfile");
        QVERIFY(openFile(file, QIODevice::ReadOnly, OpenStream, QFile::DontCloseHandle));
        QCOMPARE(file.handle(), fileno(stream_));
        file.close();
        QCOMPARE(file.handle(), -1);
        //file is not closed, read should succeed
        char buf;
        QCOMPARE(int(::fread(&buf, 1, 1, stream_)), 1);
        ::fclose(stream_);
        stream_ = 0;
    }
}

QTEST_MAIN(tst_QFile)

#include "qrc_copy-rename.cpp"
#include "qrc_qfile.cpp"
#include "moc_tst_qfile.cpp"
