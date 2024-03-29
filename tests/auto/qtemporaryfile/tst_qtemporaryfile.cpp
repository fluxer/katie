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
#include <qcoreapplication.h>
#include <qstring.h>
#include <qtemporaryfile.h>
#include <qfile.h>
#include <qdir.h>
#include <qset.h>
#include <qcore_unix_p.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>             // open(2)
#include <unistd.h>            // close(2)

//TESTED_CLASS=
//TESTED_FILES=

#ifndef QT_NO_TEMPORARYFILE

class tst_QTemporaryFile : public QObject
{
    Q_OBJECT
public slots:
    void initTestCase();
    void cleanupTestCase();

private slots:
    void construction();
    void fileTemplate();
    void fileTemplate_data();
    void getSetCheck();
    void fileName();
    void fileNameIsEmpty();
    void autoRemove();
    void write();
    void size();
    void resize();
    void openOnRootDrives();
    void stressTest();
    void rename();
    void renameFdLeak();
    void keepOpenMode();
    void resetTemplateAfterError();
    void setTemplateAfterOpen();
    void autoRemoveAfterFailedRename();
};

void tst_QTemporaryFile::initTestCase()
{
    // For fileTemplate tests
    QVERIFY(QDir("test-XXXXXX").exists() || QDir().mkdir("test-XXXXXX"));
}

void tst_QTemporaryFile::cleanupTestCase()
{
    // From fileTemplate tests
    QVERIFY(QDir().rmdir("test-XXXXXX"));
}

void tst_QTemporaryFile::construction()
{
    QTemporaryFile file(0);
    QString tmp = QDir::tempPath();
    QCOMPARE(file.fileTemplate().left(tmp.size()), tmp);
    QCOMPARE(file.fileTemplate().at(tmp.size()), QChar('/'));
}

// Testing get/set functions
void tst_QTemporaryFile::getSetCheck()
{
    QTemporaryFile obj1;
    // bool QTemporaryFile::autoRemove()
    // void QTemporaryFile::setAutoRemove(bool)
    obj1.setAutoRemove(false);
    QCOMPARE(false, obj1.autoRemove());
    obj1.setAutoRemove(true);
    QCOMPARE(true, obj1.autoRemove());
}

/*
    This tests whether the temporary file really gets placed in QDir::tempPath
*/
void tst_QTemporaryFile::fileName()
{
    // Get QDir::tempPath and make an absolute path.
    QString tempPath = QDir::tempPath();
    QString absoluteTempPath = QDir(tempPath).absolutePath();
    QTemporaryFile file;
    file.setAutoRemove(true);
    file.open();
    QString fileName = file.fileName();
    QVERIFY(QFile::exists(fileName));
    // Get path to the temp file, whithout the file name.
    QString absoluteFilePath = QFileInfo(fileName).absolutePath();
    QCOMPARE(absoluteFilePath, absoluteTempPath);
}

void tst_QTemporaryFile::fileNameIsEmpty()
{
    QString filename;
    {
        QTemporaryFile file;
        QVERIFY(file.fileName().isEmpty());

        QVERIFY(file.open());
        QVERIFY(!file.fileName().isEmpty());

        filename = file.fileName();
        QVERIFY(QFile::exists(filename));

        file.close();
        QVERIFY(!file.isOpen());
        QVERIFY(QFile::exists(filename));
        QVERIFY(!file.fileName().isEmpty());
    }
    QVERIFY(!QFile::exists(filename));
}

void tst_QTemporaryFile::autoRemove()
{
    // Test auto remove
    QString fileName;
    {
        QTemporaryFile file("tempXXXXXX");
        file.setAutoRemove(true);
        QVERIFY(file.open());
        fileName = file.fileName();
        file.close();
    }
    QVERIFY(!QFile::exists(fileName));

    // Test if disabling auto remove works.
    {
        QTemporaryFile file("tempXXXXXX");
        file.setAutoRemove(false);
        QVERIFY(file.open());
        fileName = file.fileName();
        file.close();
    }
    QVERIFY(QFile::exists(fileName));
    QVERIFY(QFile::remove(fileName));


    // Do not explicitly call setAutoRemove (tests if it really is the default as documented)
    {
        QTemporaryFile file("tempXXXXXX");
        QVERIFY(file.open());
        fileName = file.fileName();
        file.close();
    }
    QVERIFY(!QFile::exists(fileName));

}

void tst_QTemporaryFile::write()
{
    QByteArray data("OLE\nOLE\nOLE");
    QTemporaryFile file;
    QVERIFY(file.open());
    QCOMPARE((int)file.write(data), data.size());
    file.reset();
    QFile compare(file.fileName());
    compare.open(QIODevice::ReadOnly);
    QCOMPARE(compare.readAll() , data);
    file.close();
}

void tst_QTemporaryFile::size()
{
    QTemporaryFile file;
    QVERIFY(file.open());
    QVERIFY(file.exists());
    QVERIFY(!file.isSequential());
    QByteArray str("foobar");
    file.write(str);
    QVERIFY(QFile::exists(file.fileName()));
    QCOMPARE(file.size(), qint64(6));
    QCOMPARE(file.size(), qint64(6));
}

void tst_QTemporaryFile::resize()
{
    QTemporaryFile file;
    file.setAutoRemove(true);
    QVERIFY(file.open());
    QVERIFY(file.resize(100));

    QCOMPARE(QFileInfo(file.fileName()).size(), qint64(100));

    file.close();
}

void tst_QTemporaryFile::openOnRootDrives()
{
    // If it's possible to create a file in the root directory, it
    // must be possible to create a temp file there too.
    QFile testFile("/XXXXXX.txt");
    if (testFile.open(QIODevice::ReadWrite)) {
        testFile.remove();
        QTemporaryFile file("/XXXXXX.txt");
        file.setAutoRemove(true);
        QVERIFY(file.open());
    }
}

void tst_QTemporaryFile::stressTest()
{
    QSet<QString> names;
    for (int i = 0; i < 1000; ++i) {
        QTemporaryFile file;
        file.setAutoRemove(false);
        QVERIFY2(file.open(), qPrintable(file.errorString()));
        QVERIFY(!names.contains(file.fileName()));
        names.insert(file.fileName());
    }
    foreach (const QString &it, names) {
        QFile::remove(it);
    }
}

void tst_QTemporaryFile::rename()
{
    // This test checks that the temporary file is deleted, even after a
    // rename.

    QFile file;
    QVERIFY(!file.exists("temporary-file.txt"));

    QString tempname;
    {
        QTemporaryFile tempfile;

        QVERIFY2(tempfile.open(), qPrintable(tempfile.errorString()));
        tempname = tempfile.fileName();
        QVERIFY(file.exists(tempname));

        QVERIFY2(tempfile.rename("temporary-file.txt"), qPrintable(tempfile.errorString()));
        QVERIFY(!file.exists(tempname));
        QVERIFY(file.exists("temporary-file.txt"));
        QCOMPARE(tempfile.fileName(), QString("temporary-file.txt"));
    }

    QVERIFY(!file.exists(tempname));
    QVERIFY(!file.exists("temporary-file.txt"));
}

void tst_QTemporaryFile::renameFdLeak()
{
    // Open a bunch of files to force the fd count to go up
    static const int count = 10;
    int bunch_of_files[count];
    for (int i = 0; i < count; ++i) {
        bunch_of_files[i] = qt_safe_open(SRCDIR "tst_qtemporaryfile.cpp", O_RDONLY);
        QVERIFY(bunch_of_files[i] != -1);
    }

    int fd;
    {
        QTemporaryFile file;
        file.setAutoRemove(false);
        QVERIFY(file.open());

        // close the bunch of files
        for (int i = 0; i < count; ++i)
            qt_safe_close(bunch_of_files[i]);

        // save the file descriptor for later
        fd = file.handle();

        // rename the file to something
        QString newPath = QDir::tempPath() + "/tst_qtemporaryfile-renameFdLeak-" + QString::number(::getpid());
        file.rename(newPath);
        QFile::remove(newPath);
    }

    // check if QTemporaryFile closed the file
    QVERIFY(qt_safe_close(fd) == -1 && errno == EBADF);
}

void tst_QTemporaryFile::keepOpenMode()
{
    QByteArray data("abcdefghij");

    {
        QTemporaryFile tmpfile;
        QVERIFY(tmpfile.open());
        QCOMPARE(tmpfile.write(data), (qint64)data.size());
        QVERIFY(tmpfile.rename("temporary-file.txt"));

        QFile file("temporary-file.txt");
        QVERIFY(file.open(QIODevice::ReadOnly));
        QVERIFY(QIODevice::ReadOnly == file.openMode());
        QCOMPARE(file.readAll(), data);
    }
}

void tst_QTemporaryFile::resetTemplateAfterError()
{
    // calling setFileTemplate on a failed open

    QString tempPath = QDir::tempPath();

    QString const fileTemplate("destination/qt_temp_file_test.XXXXXXXXXX");
    QString const fileTemplate2(tempPath + "/qt_temp_file_test.XXXXXXXXXX");

    QVERIFY2( QDir(tempPath).exists() || QDir().mkpath(tempPath), "Test precondition" );
    QVERIFY2( !QFile::exists("destination"), "Test precondition" );
    QVERIFY2( !QFile::exists(fileTemplate2) || QFile::remove(fileTemplate2), "Test precondition" );

    QFile file(fileTemplate2);
    QByteArray fileContent("This file is intentionally NOT left empty.");

    QVERIFY( file.open(QIODevice::ReadWrite | QIODevice::Truncate) );
    QCOMPARE( file.write(fileContent), (qint64)fileContent.size() );
    QVERIFY( file.flush() );

    QString fileName;
    {
        QTemporaryFile temp;

        QVERIFY( temp.fileName().isEmpty() );
        QVERIFY( !temp.fileTemplate().isEmpty() );

        temp.setFileTemplate( fileTemplate );

        QVERIFY( temp.fileName().isEmpty() );
        QCOMPARE( temp.fileTemplate(), fileTemplate );

        QVERIFY( !temp.open() );

        QVERIFY( temp.fileName().isEmpty() );
        QCOMPARE( temp.fileTemplate(), fileTemplate );

        temp.setFileTemplate( fileTemplate2 );
        QVERIFY( temp.open() );

        fileName = temp.fileName();
        QVERIFY( QFile::exists(fileName) );
        QVERIFY( !fileName.isEmpty() );
        QVERIFY2( fileName != fileTemplate2,
            ("Generated name shouldn't be same as template: " + fileTemplate2).toLocal8Bit().constData() );
    }

    QVERIFY( !QFile::exists(fileName) );

    file.seek(0);
    QCOMPARE( QString(file.readAll()), QString(fileContent) );
    QVERIFY( file.remove() );
}

void tst_QTemporaryFile::setTemplateAfterOpen()
{
    QTemporaryFile temp;

    QVERIFY( temp.fileName().isEmpty() );
    QVERIFY( !temp.fileTemplate().isEmpty() );

    QVERIFY( temp.open() );

    QString const fileName = temp.fileName();
    QString const newTemplate("funny-name-XXXXXX.tmp");

    QVERIFY( !fileName.isEmpty() );
    QVERIFY( QFile::exists(fileName) );
    QVERIFY( !temp.fileTemplate().isEmpty() );
    QVERIFY( temp.fileTemplate() != newTemplate );

    temp.close(); // QTemporaryFile::setFileTemplate will assert on isOpen() up to 4.5.2
    temp.setFileTemplate(newTemplate);
    QCOMPARE( temp.fileTemplate(), newTemplate );

    QVERIFY( temp.open() );
    QCOMPARE( temp.fileTemplate(), newTemplate );
}

void tst_QTemporaryFile::autoRemoveAfterFailedRename()
{
    struct CleanOnReturn
    {
        ~CleanOnReturn()
        {
            if (!tempName.isEmpty())
                QFile::remove(tempName);
        }

        void reset()
        {
            tempName.clear();
        }

        QString tempName;
    };

    CleanOnReturn cleaner;

    {
        QTemporaryFile file;
        QVERIFY( file.open() );
        cleaner.tempName = file.fileName();

        QVERIFY( QFile::exists(cleaner.tempName) );
        QVERIFY( !QFileInfo("i-do-not-exist").isDir() );
        QVERIFY( !file.rename("i-do-not-exist/file.txt") );
        QVERIFY( QFile::exists(cleaner.tempName) );
    }

    QVERIFY( !QFile::exists(cleaner.tempName) );
    cleaner.reset();
}

void tst_QTemporaryFile::fileTemplate_data()
{
    QTest::addColumn<QString>("prefix");
    QTest::addColumn<QString>("suffix");
    QTest::addColumn<bool>("openResult");

    QString unicode = QString::fromUtf8("\xc3\xa5\xc3\xa6\xc3\xb8");

    QTest::newRow("<empty>") << QString() << QString() << true;

    QTest::newRow(".") << QString(".") << QString() << true;
    QTest::newRow("..") << QString("..") << QString() << true;

    QTest::newRow("foo") << QString("foo") << QString() << true;
    QTest::newRow("qt_ ... xxxxxx") << QString("qt_") << QString("xxxxxx") << true;
    QTest::newRow("qt_ ... xxx") << QString("qt_") << QString("xxx") << true;
    QTest::newRow("qt_ ... xXx") << QString("qt_") << QString("xXx") << true;
    QTest::newRow("qt_ ...") << QString("qt_") << QString() << true;
    QTest::newRow("qt_ ... _XXXX") << QString("qt_") << QString("_XXXX") << true;
    QTest::newRow("qt_ ... _XXXXX") << QString("qt_") << QString("_XXXXX") << true;
    QTest::newRow("qt_XXXX_ ...") << QString("qt_XXXX_") << QString() << true;
    QTest::newRow("qt_XXXXX_ ...") << QString("qt_XXXXX_") << QString() << true;
    QTest::newRow("qt_XXXX_ ... _XXXX") << QString("qt_XXXX_") << QString("_XXXX") << true;
    QTest::newRow("qt_XXXXX_ ... _XXXXX") << QString("qt_XXXXX_") << QString("_XXXXX") << true;

    QTest::newRow("blaXXXXXX") << QString("bla") << QString() << true;
    QTest::newRow("XXXXXXbla") << QString() << QString("bla") << true;

    QTest::newRow("does-not-exist/qt_temp.XXXXXX") << QString("does-not-exist/qt_temp") << QString() << false;

    QTest::newRow("XXXXXX<unicode>") << QString() << unicode << true;
    QTest::newRow("<unicode>XXXXXX") << unicode << QString() << true;
    QTest::newRow("<unicode>XXXXXX<unicode>") << unicode << unicode << true;
}

void tst_QTemporaryFile::fileTemplate()
{
    QVERIFY(QDir("test-XXXXXX").exists());

    struct CleanOnReturn
    {
        ~CleanOnReturn()
        {
            Q_FOREACH(QString tempName, tempNames)
                QFile::remove(tempName);
        }

        void reset()
        {
            tempNames.clear();
        }

        QStringList tempNames;
    };

    CleanOnReturn cleaner;

    QFETCH(QString, prefix);
    QFETCH(QString, suffix);
    QFETCH(bool, openResult);

    enum IterationType {
        UseConstructor,
        UseSetFileTemplate,
        Done
    };

    for (IterationType setFileTemplate = UseConstructor; setFileTemplate != Done;
            setFileTemplate = IterationType(int(setFileTemplate) + 1))
    {
        Q_FOREACH(QString const &tempName, cleaner.tempNames)
            QVERIFY( !QFile::exists(tempName) );

        cleaner.reset();

        QString fileTemplate1 = prefix + QString("XX") + suffix;
        QString fileTemplate2 = prefix + QString("XXXX") + suffix;
        QString fileTemplate3 = prefix + QString("XXXXXX") + suffix;
        QString fileTemplate4 = prefix + QString("XXXXXXXX") + suffix;

        QTemporaryFile file1(setFileTemplate ? QString() : fileTemplate1);
        QTemporaryFile file2(setFileTemplate ? QString() : fileTemplate2);
        QTemporaryFile file3(setFileTemplate ? QString() : fileTemplate3);
        QTemporaryFile file4(setFileTemplate ? QString() : fileTemplate4);
        QTemporaryFile file5(setFileTemplate ? QString() : "test-XXXXXX/" + fileTemplate1);
        QTemporaryFile file6(setFileTemplate ? QString() : "test-XXXXXX/" + fileTemplate3);

        if (setFileTemplate) {
            file1.setFileTemplate(fileTemplate1);
            file2.setFileTemplate(fileTemplate2);
            file3.setFileTemplate(fileTemplate3);
            file4.setFileTemplate(fileTemplate4);
            file5.setFileTemplate("test-XXXXXX/" + fileTemplate1);
            file6.setFileTemplate("test-XXXXXX/" + fileTemplate3);
        }

        if (openResult) {
            QVERIFY2(file1.open(), qPrintable(file1.errorString()));
            QVERIFY2(file2.open(), qPrintable(file2.errorString()));
            QVERIFY2(file3.open(), qPrintable(file3.errorString()));
            QVERIFY2(file4.open(), qPrintable(file4.errorString()));
            QVERIFY2(file5.open(), qPrintable(file5.errorString()));
            QVERIFY2(file6.open(), qPrintable(file6.errorString()));
        } else {
            QVERIFY(!file1.open());
            QVERIFY(!file2.open());
            QVERIFY(!file3.open());
            QVERIFY(!file4.open());
            QVERIFY(!file5.open());
            QVERIFY(!file6.open());
        }

        QCOMPARE(file1.exists(), openResult);
        QCOMPARE(file2.exists(), openResult);
        QCOMPARE(file3.exists(), openResult);
        QCOMPARE(file4.exists(), openResult);
        QCOMPARE(file5.exists(), openResult);
        QCOMPARE(file6.exists(), openResult);

        // make sure the file exists under the *correct* name
        if (openResult) {
            cleaner.tempNames << file1.fileName()
                << file2.fileName()
                << file3.fileName()
                << file4.fileName()
                << file5.fileName()
                << file6.fileName();

            QDir currentDir;
            QString fileName1 = currentDir.relativeFilePath(file1.fileName());
            QString fileName2 = currentDir.relativeFilePath(file2.fileName());
            QString fileName3 = currentDir.relativeFilePath(file3.fileName());
            QString fileName4 = currentDir.relativeFilePath(file4.fileName());
            QString fileName5 = currentDir.relativeFilePath(file5.fileName());
            QString fileName6 = currentDir.relativeFilePath(file6.fileName());

            QVERIFY2(fileName1.startsWith(fileTemplate1 + QLatin1Char('.')), qPrintable(file1.fileName()));
            QVERIFY2(fileName2.startsWith(fileTemplate2 + QLatin1Char('.')), qPrintable(file2.fileName()));
            QVERIFY2(fileName5.startsWith("test-XXXXXX/" + fileTemplate1 + QLatin1Char('.')), qPrintable(file5.fileName()));
            QVERIFY2(fileName6.startsWith("test-XXXXXX/" + prefix), qPrintable(file6.fileName()));

            if (!prefix.isEmpty()) {
                QVERIFY2(fileName3.startsWith(prefix), qPrintable(file3.fileName()));
                QVERIFY2(fileName4.startsWith(prefix), qPrintable(file4.fileName()));
            }

            if (!suffix.isEmpty()) {
                QVERIFY2(fileName3.endsWith(suffix), qPrintable(file3.fileName()));
                QVERIFY2(fileName4.endsWith(suffix), qPrintable(file4.fileName()));
                QVERIFY2(fileName6.endsWith(suffix), qPrintable(file6.fileName()));
            }
        }
    }
}

QTEST_MAIN(tst_QTemporaryFile)

#include "moc_tst_qtemporaryfile.cpp"

#else // QT_NO_TEMPORARYFILE

QTEST_NOOP_MAIN

#endif // QT_NO_TEMPORARYFILE
