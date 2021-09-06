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

#include "../../shared/util.h"
#include <QtTest/QtTest>
#include <QtCore/QProcess>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QThread>
#include <QtCore/QRegExp>
#include <QtCore/QDebug>
#include <QtCore/QMetaType>
// Network test unnecessary?
#include <QtNetwork/QHostInfo>
#include <stdlib.h>

#ifdef QT_NO_PROCESS
QTEST_NOOP_MAIN
#else

//TESTED_CLASS=
//TESTED_FILES=

Q_DECLARE_METATYPE(QList<QProcess::ExitStatus>);
Q_DECLARE_METATYPE(QProcess::ExitStatus);
Q_DECLARE_METATYPE(QProcess::ProcessState);

#define QPROCESS_VERIFY(Process, Fn) \
{ \
const bool ret = Process.Fn; \
if (ret == false) \
    qWarning("QProcess error: %d: %s", Process.error(), qPrintable(Process.errorString())); \
QVERIFY(ret); \
}

class tst_QProcess : public QObject
{
    Q_OBJECT

public:
    tst_QProcess();
    virtual ~tst_QProcess();

public slots:
    void init();
    void cleanup();

private slots:
    void getSetCheck();
    void constructing();
    void simpleStart();
    void execute();
    void startDetached();
    void crashTest();
    void crashTest2();
    void echoTest_data();
    void echoTest();
    void echoTest2();
    void echoTest_performance();
    void exitStatus_data();
    void exitStatus();
    void loopBackTest();
    void readTimeoutAndThenCrash();
    void waitForFinished();
    void deadWhileReading();
    void restartProcessDeadlock();
    void closeWriteChannel();
    void closeReadChannel();
    void openModes();
    void emitReadyReadOnlyWhenNewDataArrives();
    void hardExit();
    void softExit();
    void softExitInSlots_data();
    void softExitInSlots();
    void mergedChannels();
    void forwardedChannels();
    void atEnd();
    void atEnd2();
    void processInAThread();
    void processesInMultipleThreads();
    void waitForFinishedWithTimeout();
    void waitForReadyReadInAReadyReadSlot();
    void waitForBytesWrittenInABytesWrittenSlot();
    void spaceArgsTest_data();
    void spaceArgsTest();
    void exitCodeTest();
    void setEnvironment_data();
    void setEnvironment();
    void setProcessEnvironment_data();
    void setProcessEnvironment();
    void systemEnvironment();
    void spaceInName();
    void lockupsInStartDetached();
    void waitForReadyReadForNonexistantProcess();
    void setStandardInputFile();
    void setStandardOutputFile_data();
    void setStandardOutputFile();
    void setStandardOutputProcess_data();
    void setStandardOutputProcess();
    void removeFileWhileProcessIsRunning();
    void fileWriterProcess();
    void detachedWorkingDirectoryAndPid();
    void switchReadChannels();
    void setWorkingDirectory();
    void startFinishStartFinish();
    void invalidProgramString_data();
    void invalidProgramString();
    void onlyOneStartedSignal();
    void waitForStartedWithoutStart();

    // keep these at the end, since they use lots of processes and sometimes
    // caused obscure failures to occur in tests that followed them (esp. on the Mac)
    void failToStart();
    void failToStartWithWait();
    void failToStartWithEventLoop();

protected slots:
    void readFromProcess();
    void exitLoopSlot();
    void restartProcess();
    void waitForReadyReadInAReadyReadSlotSlot();
    void waitForBytesWrittenInABytesWrittenSlotSlot();

private:
    QProcess *process;
    qint64 bytesAvailable;
};

static inline QByteArray msgStartProcessFailed(const QString &binary,
                                               const QString &errorMessage)
{
    const QString message =
            QString::fromLatin1("Unable to start '%1' from '%2': %3")
            .arg(binary, QDir::currentPath(), errorMessage);
    return message.toLocal8Bit();
}

// Testing get/set functions
void tst_QProcess::getSetCheck()
{
    QProcess obj1;
    // ProcessChannelMode QProcess::processChannelMode()
    // void QProcess::setProcessChannelMode(ProcessChannelMode)
    obj1.setProcessChannelMode(QProcess::ProcessChannelMode(QProcess::SeparateChannels));
    QCOMPARE(QProcess::ProcessChannelMode(QProcess::SeparateChannels), obj1.processChannelMode());
    obj1.setProcessChannelMode(QProcess::ProcessChannelMode(QProcess::MergedChannels));
    QCOMPARE(QProcess::ProcessChannelMode(QProcess::MergedChannels), obj1.processChannelMode());
    obj1.setProcessChannelMode(QProcess::ProcessChannelMode(QProcess::ForwardedChannels));
    QCOMPARE(QProcess::ProcessChannelMode(QProcess::ForwardedChannels), obj1.processChannelMode());

    // ProcessChannel QProcess::readChannel()
    // void QProcess::setReadChannel(ProcessChannel)
    obj1.setReadChannel(QProcess::ProcessChannel(QProcess::StandardOutput));
    QCOMPARE(QProcess::ProcessChannel(QProcess::StandardOutput), obj1.readChannel());
    obj1.setReadChannel(QProcess::ProcessChannel(QProcess::StandardError));
    QCOMPARE(QProcess::ProcessChannel(QProcess::StandardError), obj1.readChannel());
}

tst_QProcess::tst_QProcess()
{
#if 0 // TODO?
    // Set current directory such that the test binaries are found.
#endif


    QVERIFY(QFile::copy("./test Space In Name/qprocess_testSpaceInName", "./test Space In Name/qprocess_testSpaceInName_one space"));
    QVERIFY(QFile::copy("./test Space In Name/qprocess_testSpaceInName", "./test Space In Name/qprocess_testSpaceInName_two space s"));
}

tst_QProcess::~tst_QProcess()
{
}

void tst_QProcess::init()
{
}

void tst_QProcess::cleanup()
{
}

//-----------------------------------------------------------------------------
void tst_QProcess::constructing()
{
    QProcess process;
    QCOMPARE(process.readChannel(), QProcess::StandardOutput);
    QCOMPARE(process.workingDirectory(), QString());
    QCOMPARE(process.environment(), QStringList());
    QCOMPARE(process.error(), QProcess::UnknownError);
    QCOMPARE(process.state(), QProcess::NotRunning);
    QCOMPARE(process.pid(), Q_PID(0));
    QCOMPARE(process.readAllStandardOutput(), QByteArray());
    QCOMPARE(process.readAllStandardError(), QByteArray());
    QCOMPARE(process.canReadLine(), false);

    // QIODevice
    QCOMPARE(process.openMode(), QIODevice::NotOpen);
    QVERIFY(!process.isOpen());
    QVERIFY(!process.isReadable());
    QVERIFY(!process.isWritable());
    QVERIFY(process.isSequential());
    QCOMPARE(process.pos(), qlonglong(0));
    QCOMPARE(process.size(), qlonglong(0));
    QVERIFY(process.atEnd());
    QCOMPARE(process.bytesAvailable(), qlonglong(0));
    QCOMPARE(process.bytesToWrite(), qlonglong(0));
    QVERIFY(!process.errorString().isEmpty());

    char c;
    QCOMPARE(process.read(&c, 1), qlonglong(-1));
    QCOMPARE(process.write(&c, 1), qlonglong(-1));

    QProcess proc2;
}

void tst_QProcess::simpleStart()
{
    qRegisterMetaType<QProcess::ProcessState>("QProcess::ProcessState");

    process = new QProcess;
    QSignalSpy spy(process, SIGNAL(stateChanged(QProcess::ProcessState)));
    connect(process, SIGNAL(readyRead()), this, SLOT(readFromProcess()));

    /* valgrind dislike SUID binaries(those that have the `s'-flag set), which
     * makes it fail to start the process. For this reason utilities like `ping' won't
     * start, when the auto test is run through `valgrind'. */
    const QString binary = QLatin1String("./qprocess_testProcessNormal");
    process->start(binary);
    if (process->state() != QProcess::Starting)
        QCOMPARE(process->state(), QProcess::Running);
    QVERIFY2(process->waitForStarted(5000), qPrintable(process->errorString()));
    QCOMPARE(process->state(), QProcess::Running);
    process->waitForReadyRead(5000);
    QCOMPARE(int(process->state()), int(QProcess::NotRunning));

    delete process;
    process = 0;

    QCOMPARE(spy.count(), 3);
    QCOMPARE(qvariant_cast<QProcess::ProcessState>(spy.at(0).at(0)), QProcess::Starting);
    QCOMPARE(qvariant_cast<QProcess::ProcessState>(spy.at(1).at(0)), QProcess::Running);
    QCOMPARE(qvariant_cast<QProcess::ProcessState>(spy.at(2).at(0)), QProcess::NotRunning);
}
//-----------------------------------------------------------------------------
void tst_QProcess::execute()
{
    QCOMPARE(QProcess::execute("./qprocess_testProcessNormal",
                               QStringList() << "arg1" << "arg2"), 0);
    QCOMPARE(QProcess::execute("nonexistingexe"), -2);
}

//-----------------------------------------------------------------------------
void tst_QProcess::startDetached()
{
    QProcess proc;
    QVERIFY(proc.startDetached("./qprocess_testProcessNormal",
                               QStringList() << "arg1" << "arg2"));
    QCOMPARE(QProcess::startDetached("nonexistingexe"), false);
}

//-----------------------------------------------------------------------------
void tst_QProcess::readFromProcess()
{
    int lines = 0;
    while (process->canReadLine()) {
        ++lines;
        QByteArray line = process->readLine();
    }
}

//-----------------------------------------------------------------------------
void tst_QProcess::crashTest()
{
    qRegisterMetaType<QProcess::ProcessState>("QProcess::ProcessState");
    process = new QProcess;
    QSignalSpy stateSpy(process, SIGNAL(stateChanged(QProcess::ProcessState)));
    const QString binary = QLatin1String("./qprocess_testProcessCrash");
    process->start(binary);
    QVERIFY2(process->waitForStarted(5000),
             msgStartProcessFailed(binary, process->errorString()).constData());

    qRegisterMetaType<QProcess::ProcessError>("QProcess::ProcessError");
    qRegisterMetaType<QProcess::ProcessError>("QProcess::ExitStatus");

    QSignalSpy spy(process, SIGNAL(error(QProcess::ProcessError)));
    QSignalSpy spy2(process, SIGNAL(finished(int, QProcess::ExitStatus)));

    QVERIFY(process->waitForFinished(5000));

    QCOMPARE(spy.count(), 1);
    QCOMPARE(*static_cast<const QProcess::ProcessError *>(spy.at(0).at(0).constData()), QProcess::Crashed);

    QCOMPARE(spy2.count(), 1);
    QCOMPARE(*static_cast<const QProcess::ExitStatus *>(spy2.at(0).at(1).constData()), QProcess::CrashExit);

    QCOMPARE(process->exitStatus(), QProcess::CrashExit);

    delete process;
    process = 0;

    QCOMPARE(stateSpy.count(), 3);
    QCOMPARE(qvariant_cast<QProcess::ProcessState>(stateSpy.at(0).at(0)), QProcess::Starting);
    QCOMPARE(qvariant_cast<QProcess::ProcessState>(stateSpy.at(1).at(0)), QProcess::Running);
    QCOMPARE(qvariant_cast<QProcess::ProcessState>(stateSpy.at(2).at(0)), QProcess::NotRunning);
}

//-----------------------------------------------------------------------------
void tst_QProcess::crashTest2()
{
    process = new QProcess;
    const QString binary = QLatin1String("./qprocess_testProcessCrash");
    process->start(binary);
    QVERIFY2(process->waitForStarted(5000),
             msgStartProcessFailed(binary, process->errorString()).constData());

    qRegisterMetaType<QProcess::ProcessError>("QProcess::ProcessError");
    qRegisterMetaType<QProcess::ProcessError>("QProcess::ExitStatus");

    QSignalSpy spy(process, SIGNAL(error(QProcess::ProcessError)));
    QSignalSpy spy2(process, SIGNAL(finished(int, QProcess::ExitStatus)));

    QObject::connect(process, SIGNAL(finished(int)), this, SLOT(exitLoopSlot()));

    QTestEventLoop::instance().enterLoop(5);
    if (QTestEventLoop::instance().timeout())
        QFAIL("Failed to detect crash : operation timed out");

    QCOMPARE(spy.count(), 1);
    QCOMPARE(*static_cast<const QProcess::ProcessError *>(spy.at(0).at(0).constData()), QProcess::Crashed);

    QCOMPARE(spy2.count(), 1);
    QCOMPARE(*static_cast<const QProcess::ExitStatus *>(spy2.at(0).at(1).constData()), QProcess::CrashExit);

    QCOMPARE(process->exitStatus(), QProcess::CrashExit);

    delete process;
    process = 0;
}

//-----------------------------------------------------------------------------
void tst_QProcess::echoTest_data()
{
    QTest::addColumn<QByteArray>("input");

    QTest::newRow("1") << QByteArray("H");
    QTest::newRow("2") << QByteArray("He");
    QTest::newRow("3") << QByteArray("Hel");
    QTest::newRow("4") << QByteArray("Hell");
    QTest::newRow("5") << QByteArray("Hello");
    QTest::newRow("100 bytes") << QByteArray(100, '@');
    QTest::newRow("1000 bytes") << QByteArray(1000, '@');
    QTest::newRow("10000 bytes") << QByteArray(10000, '@');
}

//-----------------------------------------------------------------------------
void tst_QProcess::echoTest()
{
    QFETCH(QByteArray, input);

    process = new QProcess;
    connect(process, SIGNAL(readyRead()), this, SLOT(exitLoopSlot()));

    const QString binary = QLatin1String("./qprocess_testProcessEcho");

    process->start(binary);
    QVERIFY2(process->waitForStarted(5000),
             msgStartProcessFailed(binary, process->errorString()).constData());
    process->write(input);

    QTime stopWatch;
    stopWatch.start();
    do {
        QVERIFY(process->isOpen());
        QTestEventLoop::instance().enterLoop(2);
    } while (stopWatch.elapsed() < 60000 && process->bytesAvailable() < input.size());
    if (stopWatch.elapsed() >= 60000)
        QFAIL("Timed out");

    QByteArray message = process->readAll();
    QCOMPARE(message.size(), input.size());

    char *c1 = message.data();
    char *c2 = input.data();
    while (*c1 && *c2) {
        if (*c1 != *c2)
            QCOMPARE(*c1, *c2);
        ++c1;
        ++c2;
    }
    QCOMPARE(*c1, *c2);

    process->write("", 1);

    QVERIFY(process->waitForFinished(5000));


    delete process;
    process = 0;
}

//-----------------------------------------------------------------------------
void tst_QProcess::exitLoopSlot()
{
    QTestEventLoop::instance().exitLoop();
}

//-----------------------------------------------------------------------------
void tst_QProcess::echoTest2()
{
    process = new QProcess;
    connect(process, SIGNAL(readyRead()), this, SLOT(exitLoopSlot()));

    const QString binary = QLatin1String("./qprocess_testProcessEcho2");

    process->start(binary);
    QVERIFY2(process->waitForStarted(5000),
             msgStartProcessFailed(binary, process->errorString()).constData());
    QVERIFY(!process->waitForReadyRead(250));
    QCOMPARE(process->error(), QProcess::Timedout);

    process->write("Hello");
    QSignalSpy spy1(process, SIGNAL(readyReadStandardOutput()));
    QSignalSpy spy2(process, SIGNAL(readyReadStandardError()));

    QTime stopWatch;
    stopWatch.start();
    forever {
        QTestEventLoop::instance().enterLoop(1);
        if (stopWatch.elapsed() >= 30000)
            QFAIL("Timed out");
        process->setReadChannel(QProcess::StandardOutput);
        qint64 baso = process->bytesAvailable();

        process->setReadChannel(QProcess::StandardError);
        qint64 base = process->bytesAvailable();
        if (baso == 5 && base == 5)
            break;
    }

    QVERIFY(spy1.count() > 0);
    QVERIFY(spy2.count() > 0);

    QCOMPARE(process->readAllStandardOutput(), QByteArray("Hello"));
    QCOMPARE(process->readAllStandardError(), QByteArray("Hello"));

    process->write("", 1);
    QVERIFY(process->waitForFinished(5000));

    delete process;
    process = 0;
}

//-----------------------------------------------------------------------------
void tst_QProcess::echoTest_performance()
{
    QProcess process;

    const QString binary = QLatin1String("./qprocess_testProcessLoopback");

    process.start(binary);
    QVERIFY2(process.waitForStarted(5000),
             msgStartProcessFailed(binary, process.errorString()).constData());

    QByteArray array;
    array.resize(1024 * 1024);
    for (int j = 0; j < array.size(); ++j)
        array[j] = 'a' + (j % 20);

    QVERIFY(process.waitForStarted());

    QTime stopWatch;
    stopWatch.start();

    qint64 totalBytes = 0;
    QByteArray dump;
    QSignalSpy readyReadSpy(&process, SIGNAL(readyRead()));
    while (stopWatch.elapsed() < 2000) {
        process.write(array);
        while (process.bytesToWrite() > 0) {
            int readCount = readyReadSpy.count();
            QVERIFY(process.waitForBytesWritten(5000));
            if (readyReadSpy.count() == readCount)
                QVERIFY(process.waitForReadyRead(5000));
        }

        while (process.bytesAvailable() < array.size())
            QVERIFY2(process.waitForReadyRead(5000), qPrintable(process.errorString()));
        dump = process.readAll();
        totalBytes += dump.size();
    }

    qDebug() << "Elapsed time:" << stopWatch.elapsed() << "ms;"
             << "transfer rate:" << totalBytes / (1048.576) / stopWatch.elapsed()
             << "MB/s";

    for (int j = 0; j < array.size(); ++j)
        QCOMPARE(char(dump.at(j)), char('a' + (j % 20)));

    process.closeWriteChannel();
    QVERIFY(process.waitForFinished());
}

//-----------------------------------------------------------------------------
void tst_QProcess::exitStatus_data()
{
    QTest::addColumn<QStringList>("processList");
    QTest::addColumn<QList<QProcess::ExitStatus> >("exitStatus");

    QTest::newRow("normal") << (QStringList() << "./qprocess_testProcessNormal")
                            << (QList<QProcess::ExitStatus>() << QProcess::NormalExit);
    QTest::newRow("crash") << (QStringList() << "./qprocess_testProcessCrash")
                            << (QList<QProcess::ExitStatus>() << QProcess::CrashExit);

    QTest::newRow("normal-crash") << (QStringList()
                                      << "./qprocess_testProcessNormal"
                                      << "./qprocess_testProcessCrash")
                                  << (QList<QProcess::ExitStatus>()
                                      << QProcess::NormalExit
                                      << QProcess::CrashExit);
    QTest::newRow("crash-normal") << (QStringList()
                                      << "./qprocess_testProcessCrash"
                                      << "./qprocess_testProcessNormal")
                                  << (QList<QProcess::ExitStatus>()
                                      << QProcess::CrashExit
                                      << QProcess::NormalExit);
}

void tst_QProcess::exitStatus()
{
    process = new QProcess;
    QFETCH(QStringList, processList);
    QFETCH(QList<QProcess::ExitStatus>, exitStatus);

    QCOMPARE(exitStatus.count(), processList.count());
    for (int i = 0; i < processList.count(); ++i) {
        process->start(processList.at(i));
        QVERIFY2(process->waitForStarted(5000),
                 msgStartProcessFailed(processList.at(i), process->errorString()).constData());
        QVERIFY(process->waitForFinished(5000));

        QCOMPARE(process->exitStatus(), exitStatus.at(i));
    }

    process->deleteLater();
    process = 0;
}
//-----------------------------------------------------------------------------
void tst_QProcess::loopBackTest()
{
    process = new QProcess;

    const QString binary = QLatin1String("./qprocess_testProcessEcho");

    process->start(binary);
    QVERIFY2(process->waitForStarted(5000),
             msgStartProcessFailed(binary, process->errorString()).constData());
    for (int i = 0; i < 100; ++i) {
        process->write("Hello");
        do {
            QVERIFY(process->waitForReadyRead(5000));
        } while (process->bytesAvailable() < 5);
        QCOMPARE(process->readAll(), QByteArray("Hello"));
    }

    process->write("", 1);
    QVERIFY(process->waitForFinished(5000));

    delete process;
    process = 0;
}

//-----------------------------------------------------------------------------
void tst_QProcess::readTimeoutAndThenCrash()
{
    process = new QProcess;

    const QString binary = QLatin1String("./qprocess_testProcessEcho");

    process->start(binary);
    if (process->state() != QProcess::Starting)
        QCOMPARE(process->state(), QProcess::Running);

    QVERIFY(process->waitForStarted(5000));
    QCOMPARE(process->state(), QProcess::Running);

    QVERIFY(!process->waitForReadyRead(5000));
    QCOMPARE(process->error(), QProcess::Timedout);

    qRegisterMetaType<QProcess::ProcessError>("QProcess::ProcessError");
    QSignalSpy spy(process, SIGNAL(error(QProcess::ProcessError)));

    process->kill();

    QVERIFY(process->waitForFinished(5000));
    QCOMPARE(process->state(), QProcess::NotRunning);

    QCOMPARE(spy.count(), 1);
    QCOMPARE(*static_cast<const QProcess::ProcessError *>(spy.at(0).at(0).constData()), QProcess::Crashed);

    delete process;
    process = 0;
}

void tst_QProcess::waitForFinished()
{
    QProcess process;

    const QString binary = QLatin1String("./qprocess_testProcessOutput");

    process.start(binary);

    QVERIFY(process.waitForFinished(5000));
    QCOMPARE(process.exitStatus(), QProcess::NormalExit);

    QString output = process.readAll();
    QCOMPARE(output.count("\n"), 10*1024);

    process.start("blurdybloop");
    QVERIFY(!process.waitForFinished());
    QCOMPARE(process.error(), QProcess::FailedToStart);
}


void tst_QProcess::deadWhileReading()
{
    QProcess process;

    const QString binary = QLatin1String("./qprocess_testProcessDeadWhileReading");

    QString output;

    process.start(binary);
    QVERIFY2(process.waitForStarted(5000),
             msgStartProcessFailed(binary, process.errorString()).constData());
    while (process.waitForReadyRead(5000))
        output += process.readAll();

    QCOMPARE(output.count("\n"), 10*1024);
    process.waitForFinished();
}

//-----------------------------------------------------------------------------
void tst_QProcess::restartProcessDeadlock()
{
    // The purpose of this test is to detect whether restarting a
    // process in the finished() connected slot causes a deadlock
    // because of the way QProcessManager uses its locks.
    QProcess proc;
    process = &proc;
    connect(process, SIGNAL(finished(int)), this, SLOT(restartProcess()));

    const QString binary = QLatin1String("./qprocess_testProcessEcho");

    process->start(binary);
    QVERIFY2(process->waitForStarted(5000),
             msgStartProcessFailed(binary, process->errorString()).constData());
    QCOMPARE(process->write("", 1), qlonglong(1));
    QVERIFY(process->waitForFinished(5000));

    process->disconnect(SIGNAL(finished(int)));

    QCOMPARE(process->write("", 1), qlonglong(1));
    QVERIFY(process->waitForFinished(5000));
}

void tst_QProcess::restartProcess()
{
    const QString binary = QLatin1String("./qprocess_testProcessEcho");

    process->start(binary);
    QVERIFY2(process->waitForStarted(5000),
             msgStartProcessFailed(binary, process->errorString()).constData());
}

//-----------------------------------------------------------------------------
void tst_QProcess::closeWriteChannel()
{
    QProcess more;
    const QString binary = QLatin1String("./qprocess_testProcessEOF");
    more.start(binary);
    QVERIFY2(more.waitForStarted(5000),
             msgStartProcessFailed(binary, more.errorString()).constData());
    QVERIFY(!more.waitForReadyRead(250));
    QCOMPARE(more.error(), QProcess::Timedout);

    QVERIFY(more.write("Data to read") != -1);

    QVERIFY(!more.waitForReadyRead(250));
    QCOMPARE(more.error(), QProcess::Timedout);

    more.closeWriteChannel();

    QVERIFY(more.waitForReadyRead(5000));
    QVERIFY(more.readAll().startsWith("Data to read"));

    if (more.state() == QProcess::Running)
        more.write("q");
    QVERIFY(more.waitForFinished(5000));
}

//-----------------------------------------------------------------------------
void tst_QProcess::closeReadChannel()
{
    for (int i = 0; i < 10; ++i) {
        QProcess::ProcessChannel channel1 = QProcess::StandardOutput;
        QProcess::ProcessChannel channel2 = QProcess::StandardError;

        QProcess proc;

        const QString binary = QLatin1String("./qprocess_testProcessEcho2");

        proc.start(binary);
        QVERIFY2(proc.waitForStarted(5000),
                 msgStartProcessFailed(binary, proc.errorString()).constData());
        proc.closeReadChannel(i&1 ? channel2 : channel1);
        proc.setReadChannel(i&1 ? channel2 : channel1);
        proc.write("Data");

        QVERIFY(!proc.waitForReadyRead(5000));
        QVERIFY(proc.readAll().isEmpty());

        proc.setReadChannel(i&1 ? channel1 : channel2);

        while (proc.bytesAvailable() < 4 && proc.waitForReadyRead(5000))
        { }

        QCOMPARE(proc.readAll(), QByteArray("Data"));

        proc.write("", 1);
        QVERIFY(proc.waitForFinished(5000));
    }
}

//-----------------------------------------------------------------------------
void tst_QProcess::openModes()
{
    QProcess proc;
    QVERIFY(!proc.isOpen());
    QVERIFY(proc.openMode() == QProcess::NotOpen);

    const QString binary = QLatin1String("./qprocess_testProcessEcho3");

    proc.start(binary);
    QVERIFY2(proc.waitForStarted(5000),
             msgStartProcessFailed(binary, proc.errorString()).constData());
    QVERIFY(proc.isOpen());
    QVERIFY(proc.openMode() == QProcess::ReadWrite);
    QVERIFY(proc.isReadable());
    QVERIFY(proc.isWritable());

    proc.write("Data");

    proc.closeWriteChannel();

    QVERIFY(proc.isWritable());
    QVERIFY(proc.openMode() == QProcess::ReadWrite);

    while (proc.bytesAvailable() < 4 && proc.waitForReadyRead(5000))
    { }

    QCOMPARE(proc.readAll().constData(), QByteArray("Data").constData());

    proc.closeReadChannel(QProcess::StandardOutput);

    QVERIFY(proc.openMode() == QProcess::ReadWrite);
    QVERIFY(proc.isReadable());

    proc.closeReadChannel(QProcess::StandardError);

    QVERIFY(proc.openMode() == QProcess::ReadWrite);
    QVERIFY(proc.isReadable());

    proc.close();
    QVERIFY(!proc.isOpen());
    QVERIFY(!proc.isReadable());
    QVERIFY(!proc.isWritable());
    QCOMPARE(proc.state(), QProcess::NotRunning);
}

//-----------------------------------------------------------------------------
void tst_QProcess::emitReadyReadOnlyWhenNewDataArrives()
{
    QProcess proc;
    connect(&proc, SIGNAL(readyRead()), this, SLOT(exitLoopSlot()));
    QSignalSpy spy(&proc, SIGNAL(readyRead()));

    const QString binary = QLatin1String("./qprocess_testProcessEcho");

    proc.start(binary);
    QVERIFY2(proc.waitForStarted(5000),
             msgStartProcessFailed(binary, proc.errorString()).constData());
    QCOMPARE(spy.count(), 0);

    proc.write("A");

    QTestEventLoop::instance().enterLoop(5);
    if (QTestEventLoop::instance().timeout())
        QFAIL("Operation timed out");

    QCOMPARE(spy.count(), 1);

    QTestEventLoop::instance().enterLoop(1);
    QVERIFY(QTestEventLoop::instance().timeout());
    QVERIFY(!proc.waitForReadyRead(250));

    QObject::disconnect(&proc, SIGNAL(readyRead()), 0, 0);
    proc.write("B");
    QVERIFY(proc.waitForReadyRead(5000));

    proc.write("", 1);
    QVERIFY(proc.waitForFinished(5000));
}

//-----------------------------------------------------------------------------
void tst_QProcess::hardExit()
{
    static const int timeOutMs = 5000;

    QProcess proc;

    const QString binary = QLatin1String("./qprocess_testProcessEcho");

    proc.start(binary);
    QVERIFY2(proc.waitForStarted(timeOutMs),
             msgStartProcessFailed(binary, proc.errorString()).constData());
    proc.kill();

    QVERIFY(proc.waitForFinished(5000));
    QCOMPARE(int(proc.state()), int(QProcess::NotRunning));
    QCOMPARE(int(proc.error()), int(QProcess::Crashed));
}

//-----------------------------------------------------------------------------
void tst_QProcess::softExit()
{
    QProcess proc;
    const QString binary = QLatin1String("./qprocess_testSoftExit");
    proc.start(binary);
    QVERIFY2(proc.waitForStarted(10000),
             msgStartProcessFailed(binary, proc.errorString()).constData());

    QVERIFY(proc.waitForReadyRead(10000));

    proc.terminate();

    QVERIFY(proc.waitForFinished(10000));
    QCOMPARE(int(proc.state()), int(QProcess::NotRunning));
    QCOMPARE(int(proc.error()), int(QProcess::UnknownError));
}

class SoftExitProcess : public QProcess
{
    Q_OBJECT
public:
    bool waitedForFinished;

    SoftExitProcess(int n) : waitedForFinished(false), n(n), killing(false)
    {
        connect(this, SIGNAL(finished(int, QProcess::ExitStatus)),
                this, SLOT(finishedSlot(int, QProcess::ExitStatus)));

        switch (n) {
        case 0:
            setProcessChannelMode(QProcess::MergedChannels);
            connect(this, SIGNAL(readyRead()), this, SLOT(terminateSlot()));
            break;
        case 1:
            connect(this, SIGNAL(readyReadStandardOutput()),
                    this, SLOT(terminateSlot()));
            break;
        case 2:
            connect(this, SIGNAL(readyReadStandardError()),
                    this, SLOT(terminateSlot()));
            break;
        case 3:
            connect(this, SIGNAL(started()),
                    this, SLOT(terminateSlot()));
            break;
        case 4:
        default:
            connect(this, SIGNAL(stateChanged(QProcess::ProcessState)),
                    this, SLOT(terminateSlot()));
            break;
        }
    }

    void writeAfterStart(const char *buf, int count)
    {
        dataToWrite = QByteArray(buf, count);
    }

    void start(const QString &program)
    {
        QProcess::start(program);
        writePendingData();
    }

public slots:
    void terminateSlot()
    {
        writePendingData(); // In cases 3 and 4 we haven't written the data yet.
        if (killing || (n == 4 && state() != Running)) {
            // Don't try to kill the process before it is running - that can
            // be hazardous, as the actual child process might not be running
            // yet. Also, don't kill it "recursively".
            return;
        }
        killing = true;
        readAll();
        terminate();
        if ((waitedForFinished = waitForFinished(5000)) == false) {
            kill();
            if (state() != NotRunning)
                waitedForFinished = waitForFinished(5000);
        }
    }

    void finishedSlot(int, QProcess::ExitStatus)
    {
        waitedForFinished = true;
    }

private:
    void writePendingData()
    {
        if (!dataToWrite.isEmpty()) {
            write(dataToWrite);
            dataToWrite.clear();
        }
    }

private:
    int n;
    bool killing;
    QByteArray dataToWrite;
};

//-----------------------------------------------------------------------------
void tst_QProcess::softExitInSlots_data()
{
    QTest::addColumn<QString>("appName");

    QTest::newRow("gui app") << "./qprocess_testGuiProcess";
    QTest::newRow("console app") << "./qprocess_testProcessEcho2";
}

//-----------------------------------------------------------------------------
void tst_QProcess::softExitInSlots()
{
    QFETCH(QString, appName);

    for (int i = 0; i < 5; ++i) {
        SoftExitProcess proc(i);
        proc.writeAfterStart("OLEBOLE", 8); // include the \0
        proc.start(appName);
        QTRY_VERIFY(proc.waitedForFinished);
        QCOMPARE(proc.state(), QProcess::NotRunning);
    }
}

//-----------------------------------------------------------------------------
void tst_QProcess::mergedChannels()
{
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    QCOMPARE(process.processChannelMode(), QProcess::MergedChannels);


    const QString binary = QLatin1String("./qprocess_testProcessEcho2");

    process.start(binary);
    QVERIFY2(process.waitForStarted(5000),
             msgStartProcessFailed(binary, process.errorString()).constData());

    for (int i = 0; i < 100; ++i) {
        QCOMPARE(process.write("abc"), qlonglong(3));
        while (process.bytesAvailable() < 6)
            QVERIFY(process.waitForReadyRead(5000));
        QCOMPARE(process.readAll(), QByteArray("aabbcc"));
    }

    process.closeWriteChannel();
    QVERIFY(process.waitForFinished(5000));
}

//-----------------------------------------------------------------------------
void tst_QProcess::forwardedChannels()
{
    QProcess process;
    process.setProcessChannelMode(QProcess::ForwardedChannels);
    QCOMPARE(process.processChannelMode(), QProcess::ForwardedChannels);

    const QString binary = QLatin1String("./qprocess_testProcessEcho2");

    process.start(binary);
    QVERIFY2(process.waitForStarted(5000),
             msgStartProcessFailed(binary, process.errorString()).constData());
    QCOMPARE(process.write("forwarded\n"), qlonglong(10));
    QVERIFY(!process.waitForReadyRead(250));
    QCOMPARE(process.bytesAvailable(), qlonglong(0));

    process.closeWriteChannel();
    QVERIFY(process.waitForFinished(5000));
}


//-----------------------------------------------------------------------------
void tst_QProcess::atEnd()
{
    QProcess process;

    const QString binary = QLatin1String("./qprocess_testProcessEcho");

    process.start(binary);
    QVERIFY2(process.waitForStarted(5000),
             msgStartProcessFailed(binary, process.errorString()).constData());
    process.write("abcdefgh\n");

    while (process.bytesAvailable() < 8)
        QVERIFY(process.waitForReadyRead(5000));

    QTextStream stream(&process);
    QVERIFY(!stream.atEnd());
    QString tmp = stream.readLine();
    QVERIFY(stream.atEnd());
    QCOMPARE(tmp, QString::fromLatin1("abcdefgh"));

    process.write("", 1);
    QVERIFY(process.waitForFinished(5000));
}

class TestThread : public QThread
{
    Q_OBJECT
public:
    inline int code()
    {
        return exitCode;
    }

protected:
    inline void run()
    {
        exitCode = 90210;

        QProcess process;
        connect(&process, SIGNAL(finished(int)), this, SLOT(catchExitCode(int)),
                Qt::DirectConnection);

        QStringList args;
        const QString binary = QLatin1String("./qprocess_testProcessEcho");

        process.start(binary);
        QVERIFY2(process.waitForStarted(5000),
                 msgStartProcessFailed(binary, process.errorString()).constData());

        QCOMPARE(process.write("abc\0", 4), qint64(4));
        exitCode = exec();
    }

protected slots:
    inline void catchExitCode(int exitCode)
    {
        this->exitCode = exitCode;
        exit(exitCode);
    }

private:
    int exitCode;
};

//-----------------------------------------------------------------------------
void tst_QProcess::processInAThread()
{
    for (int i = 0; i < 10; ++i) {
        TestThread thread;
        thread.start();
        QVERIFY(thread.wait(10000));
        QCOMPARE(thread.code(), 0);
    }
}

//-----------------------------------------------------------------------------
void tst_QProcess::processesInMultipleThreads()
{
    for (int i = 0; i < 10; ++i) {
        TestThread thread1;
        TestThread thread2;
        TestThread thread3;

        thread1.start();
        thread2.start();
        thread3.start();

        QVERIFY(thread2.wait(10000));
        QVERIFY(thread3.wait(10000));
        QVERIFY(thread1.wait(10000));

        QCOMPARE(thread1.code(), 0);
        QCOMPARE(thread2.code(), 0);
        QCOMPARE(thread3.code(), 0);
    }
}

//-----------------------------------------------------------------------------
void tst_QProcess::waitForFinishedWithTimeout()
{
    static const int timeOutMs = 5000;

    process = new QProcess(this);

    const QString binary = QLatin1String("./qprocess_testProcessEcho");

    process->start(binary);
    QVERIFY2(process->waitForStarted(timeOutMs),
             msgStartProcessFailed(binary, process->errorString()).constData());
    QVERIFY(!process->waitForFinished(1));

    process->write("", 1);

    QVERIFY(process->waitForFinished());

    delete process;
    process = 0;
}

//-----------------------------------------------------------------------------
void tst_QProcess::waitForReadyReadInAReadyReadSlot()
{
    process = new QProcess(this);
    connect(process, SIGNAL(readyRead()), this, SLOT(waitForReadyReadInAReadyReadSlotSlot()));
    connect(process, SIGNAL(finished(int)), this, SLOT(exitLoopSlot()));
    bytesAvailable = 0;


    const QString binary = QLatin1String("./qprocess_testProcessEcho");

    process->start(binary);
    QVERIFY2(process->waitForStarted(5000),
             msgStartProcessFailed(binary, process->errorString()).constData());

    QSignalSpy spy(process, SIGNAL(readyRead()));
    process->write("foo");
    QTestEventLoop::instance().enterLoop(30);
    QVERIFY(!QTestEventLoop::instance().timeout());

    QCOMPARE(spy.count(), 1);

    process->disconnect();
    QVERIFY(process->waitForFinished(5000));
    QVERIFY(process->bytesAvailable() > bytesAvailable);
    delete process;
    process = 0;
}

//-----------------------------------------------------------------------------
void tst_QProcess::waitForReadyReadInAReadyReadSlotSlot()
{
    bytesAvailable = process->bytesAvailable();
    process->write("bar", 4);
    QVERIFY(process->waitForReadyRead(5000));
    QTestEventLoop::instance().exitLoop();
}

//-----------------------------------------------------------------------------
void tst_QProcess::waitForBytesWrittenInABytesWrittenSlot()
{
    process = new QProcess(this);
    connect(process, SIGNAL(bytesWritten(qint64)), this, SLOT(waitForBytesWrittenInABytesWrittenSlotSlot()));
    bytesAvailable = 0;

    const QString binary = QLatin1String("./qprocess_testProcessEcho");

    process->start(binary);
    QVERIFY2(process->waitForStarted(5000),
             msgStartProcessFailed(binary, process->errorString()).constData());
    qRegisterMetaType<qint64>("qint64");
    QSignalSpy spy(process, SIGNAL(bytesWritten(qint64)));
    process->write("f");
    QTestEventLoop::instance().enterLoop(30);
    QVERIFY(!QTestEventLoop::instance().timeout());

    QCOMPARE(spy.count(), 1);
    process->write("", 1);
    process->disconnect();
    QVERIFY(process->waitForFinished());
    delete process;
    process = 0;
}

//-----------------------------------------------------------------------------
void tst_QProcess::waitForBytesWrittenInABytesWrittenSlotSlot()
{
    process->write("b");
    QVERIFY(process->waitForBytesWritten(5000));
    QTestEventLoop::instance().exitLoop();
}

//-----------------------------------------------------------------------------
void tst_QProcess::spaceArgsTest_data()
{
    QTest::addColumn<QStringList>("args");
    QTest::addColumn<QString>("stringArgs");

    // arg1 | arg2
    QTest::newRow("arg1 arg2") << (QStringList() << QString::fromLatin1("arg1") << QString::fromLatin1("arg2"))
                               << QString::fromLatin1("arg1 arg2");
    // "arg1" | ar "g2
    QTest::newRow("\"\"\"\"arg1\"\"\"\" \"ar \"\"\"g2\"") << (QStringList() << QString::fromLatin1("\"arg1\"") << QString::fromLatin1("ar \"g2"))
                                                          << QString::fromLatin1("\"\"\"\"arg1\"\"\"\" \"ar \"\"\"g2\"");
    // ar g1 | a rg 2
    QTest::newRow("\"ar g1\" \"a rg 2\"") << (QStringList() << QString::fromLatin1("ar g1") << QString::fromLatin1("a rg 2"))
                                          << QString::fromLatin1("\"ar g1\" \"a rg 2\"");
    // -lar g1 | -l"ar g2"
    QTest::newRow("\"-lar g1\" \"-l\"\"\"ar g2\"\"\"\"") << (QStringList() << QString::fromLatin1("-lar g1") << QString::fromLatin1("-l\"ar g2\""))
                                                         << QString::fromLatin1("\"-lar g1\" \"-l\"\"\"ar g2\"\"\"\"");
    // ar"g1
    QTest::newRow("ar\"\"\"\"g1") << (QStringList() << QString::fromLatin1("ar\"g1"))
                                  << QString::fromLatin1("ar\"\"\"\"g1");
    // ar/g1
    QTest::newRow("ar\\g1") << (QStringList() << QString::fromLatin1("ar\\g1"))
                            << QString::fromLatin1("ar\\g1");
    // ar\g"1
    QTest::newRow("ar\\g\"\"\"\"1") << (QStringList() << QString::fromLatin1("ar\\g\"1"))
                                    << QString::fromLatin1("ar\\g\"\"\"\"1");
    // arg\"1
    QTest::newRow("arg\\\"\"\"1") << (QStringList() << QString::fromLatin1("arg\\\"1"))
                                  << QString::fromLatin1("arg\\\"\"\"1");
    // """"
    QTest::newRow("\"\"\"\"\"\"\"\"\"\"\"\"") << (QStringList() << QString::fromLatin1("\"\"\"\""))
                                              << QString::fromLatin1("\"\"\"\"\"\"\"\"\"\"\"\"");
    // """" | "" ""
    QTest::newRow("\"\"\"\"\"\"\"\"\"\"\"\" \"\"\"\"\"\"\" \"\"\"\"\"\"\"") << (QStringList() << QString::fromLatin1("\"\"\"\"") << QString::fromLatin1("\"\" \"\""))
                                                                            << QString::fromLatin1("\"\"\"\"\"\"\"\"\"\"\"\" \"\"\"\"\"\"\" \"\"\"\"\"\"\"");
    // ""  ""
    QTest::newRow("\"\"\"\"\"\"\" \"\" \"\"\"\"\"\"\" (bogus double quotes)") << (QStringList() << QString::fromLatin1("\"\"  \"\""))
                                                                              << QString::fromLatin1("\"\"\"\"\"\"\" \"\" \"\"\"\"\"\"\"");
    // ""  ""
    QTest::newRow(" \"\"\"\"\"\"\" \"\" \"\"\"\"\"\"\"   (bogus double quotes)") << (QStringList() << QString::fromLatin1("\"\"  \"\""))
                                                                                 << QString::fromLatin1(" \"\"\"\"\"\"\" \"\" \"\"\"\"\"\"\"   ");
}

//-----------------------------------------------------------------------------
void tst_QProcess::spaceArgsTest()
{
    QFETCH(QStringList, args);
    QFETCH(QString, stringArgs);

    static const int timeOutMs = 5000;

    QStringList programs;
    programs << QString::fromLatin1("./test Space In Name/qprocess_testSpaceInName")
             << QString::fromLatin1("./test Space In Name/qprocess_testSpaceInName_one space")
             << QString::fromLatin1("./test Space In Name/qprocess_testSpaceInName_two space s");

    process = new QProcess(this);

    for (int i = 0; i < programs.size(); ++i) {
        QString program = programs.at(i);
        process->start(program, args);
        QVERIFY2(process->waitForStarted(timeOutMs),
                 msgStartProcessFailed(program, process->errorString()).constData());

        QVERIFY(process->waitForFinished(5000));

        QStringList actual = QString::fromLatin1(process->readAll()).split("|");
        QVERIFY(!actual.isEmpty());
        // not interested in the program name, it might be different.
        actual.removeFirst();

        QCOMPARE(actual, args);

        if (program.contains(" "))
            program = "\"" + program + "\"";

        if (!stringArgs.isEmpty())
            program += QString::fromLatin1(" ") + stringArgs;

        process->start(program);

        QVERIFY2(process->waitForStarted(5000),
                 msgStartProcessFailed(program, process->errorString()).constData());
        QVERIFY(process->waitForFinished(5000));

        actual = QString::fromLatin1(process->readAll()).split("|");
        QVERIFY(!actual.isEmpty());
        // not interested in the program name, it might be different.
        actual.removeFirst();

        QCOMPARE(actual, args);
    }

    delete process;
    process = 0;
}

//-----------------------------------------------------------------------------
void tst_QProcess::exitCodeTest()
{
    for (int i = 0; i < 255; ++i) {
        QProcess process;
        const QString binary = QLatin1String("./qprocess_testExitCodes");
        process.start(binary, QStringList(QString::number(i)));
        QVERIFY2(process.waitForStarted(5000),
                 msgStartProcessFailed(binary, process.errorString()).constData());

        QVERIFY(process.waitForFinished(5000));
        QCOMPARE(process.exitCode(), i);
        QCOMPARE(process.error(), QProcess::UnknownError);
    }
}

//-----------------------------------------------------------------------------
void tst_QProcess::failToStart()
{
    qRegisterMetaType<QProcess::ProcessError>("QProcess::ProcessError");
    qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
    qRegisterMetaType<QProcess::ProcessState>("QProcess::ProcessState");

    QProcess process;
    QSignalSpy stateSpy(&process, SIGNAL(stateChanged(QProcess::ProcessState)));
    QSignalSpy errorSpy(&process, SIGNAL(error(QProcess::ProcessError)));
    QSignalSpy finishedSpy(&process, SIGNAL(finished(int)));
    QSignalSpy finishedSpy2(&process, SIGNAL(finished(int, QProcess::ExitStatus)));

   const int attempts = 50;

    for (int j = 0; j < 8; ++j) {
        for (int i = 0; i < attempts; ++i) {
            QCOMPARE(errorSpy.count(), j * attempts + i);
            process.start("/blurp");

            switch (j) {
            case 0:
            case 1:
                QVERIFY(!process.waitForStarted());
                break;
            case 2:
            case 3:
                QVERIFY(!process.waitForFinished());
                break;
            case 4:
            case 5:
                QVERIFY(!process.waitForReadyRead());
                break;
            case 6:
            case 7:
            default:
                QVERIFY(!process.waitForBytesWritten());
                break;
            }

            QCOMPARE(process.error(), QProcess::FailedToStart);
            QCOMPARE(errorSpy.count(), j * attempts + i + 1);
            QCOMPARE(finishedSpy.count(), 0);
            QCOMPARE(finishedSpy2.count(), 0);

            int it = j * attempts + i + 1;

            QCOMPARE(stateSpy.count(), it * 2);
            QCOMPARE(qvariant_cast<QProcess::ProcessState>(stateSpy.at(it * 2 - 2).at(0)), QProcess::Starting);
            QCOMPARE(qvariant_cast<QProcess::ProcessState>(stateSpy.at(it * 2 - 1).at(0)), QProcess::NotRunning);
        }
    }
}

//-----------------------------------------------------------------------------
void tst_QProcess::failToStartWithWait()
{
    qRegisterMetaType<QProcess::ProcessError>("QProcess::ProcessError");
    qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");

    QProcess process;
    QEventLoop loop;
    QSignalSpy errorSpy(&process, SIGNAL(error(QProcess::ProcessError)));
    QSignalSpy finishedSpy(&process, SIGNAL(finished(int)));
    QSignalSpy finishedSpy2(&process, SIGNAL(finished(int, QProcess::ExitStatus)));

    for (int i = 0; i < 50; ++i) {
        process.start("/blurp", QStringList() << "-v" << "-debug");
        process.waitForStarted();

        QCOMPARE(process.error(), QProcess::FailedToStart);
        QCOMPARE(errorSpy.count(), i + 1);
        QCOMPARE(finishedSpy.count(), 0);
        QCOMPARE(finishedSpy2.count(), 0);
    }
}

//-----------------------------------------------------------------------------
void tst_QProcess::failToStartWithEventLoop()
{
    qRegisterMetaType<QProcess::ProcessError>("QProcess::ProcessError");
    qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");

    QProcess process;
    QEventLoop loop;
    QSignalSpy errorSpy(&process, SIGNAL(error(QProcess::ProcessError)));
    QSignalSpy finishedSpy(&process, SIGNAL(finished(int)));
    QSignalSpy finishedSpy2(&process, SIGNAL(finished(int, QProcess::ExitStatus)));

    // The error signal may be emitted before start() returns
    connect(&process, SIGNAL(error(QProcess::ProcessError)), &loop, SLOT(quit()), Qt::QueuedConnection);


    for (int i = 0; i < 50; ++i) {
        process.start("/blurp", QStringList() << "-v" << "-debug");

        loop.exec();

        QCOMPARE(process.error(), QProcess::FailedToStart);
        QCOMPARE(errorSpy.count(), i + 1);
        QCOMPARE(finishedSpy.count(), 0);
        QCOMPARE(finishedSpy2.count(), 0);
    }
}

//-----------------------------------------------------------------------------
void tst_QProcess::removeFileWhileProcessIsRunning()
{
    QFile file("removeFile.txt");
    QVERIFY(file.open(QFile::WriteOnly));

    QProcess process;

    const QString binary = QLatin1String("./qprocess_testProcessEcho");

    process.start(binary);
    QVERIFY2(process.waitForStarted(5000),
             msgStartProcessFailed(binary, process.errorString()).constData());

    QVERIFY(file.remove());

    process.write("", 1);
    QVERIFY(process.waitForFinished(5000));
}

//-----------------------------------------------------------------------------
void tst_QProcess::setEnvironment_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("value");

    QTest::newRow("setting-empty") << "tst_QProcess" << "";
    QTest::newRow("setting") << "tst_QProcess" << "value";

    QTest::newRow("unsetting") << "PATH" << QString();
    QTest::newRow("overriding") << "PATH" << "value";
}

void tst_QProcess::setEnvironment()
{
    // make sure our environment variables are correct
    QVERIFY(qgetenv("tst_QProcess").isEmpty());
    QVERIFY(!qgetenv("PATH").isEmpty());

    QFETCH(QString, name);
    QFETCH(QString, value);
    QString executable = QDir::currentPath() + "/qprocess_testProcessEnvironment";

    {
        QProcess process;
        QStringList environment = QProcess::systemEnvironment();
        if (value.isNull()) {
            int pos;
            QRegExp rx(name + "=.*");
            while ((pos = environment.indexOf(rx)) != -1)
                environment.removeAt(pos);
        } else {
            environment.append(name + '=' + value);
        }
        process.setEnvironment(environment);
        process.start(executable, QStringList() << name);
        QVERIFY2(process.waitForStarted(5000),
                 msgStartProcessFailed(executable, process.errorString()).constData());
        QVERIFY(process.waitForFinished());
        if (value.isNull())
            QCOMPARE(process.exitCode(), 1);
        else if (!value.isEmpty())
            QCOMPARE(process.exitCode(), 0);

        QCOMPARE(process.readAll(), value.toLocal8Bit());
    }

    // re-do the test but set the environment twice, to make sure
    // that the latter addition overrides
    // this test doesn't make sense in unsetting
    if (!value.isNull()) {
        QProcess process;
        QStringList environment = QProcess::systemEnvironment();
        environment.prepend(name + "=This is not the right value");
        environment.append(name + '=' + value);
        process.setEnvironment(environment);
        process.start(executable, QStringList() << name);

        QVERIFY2(process.waitForStarted(5000),
                 msgStartProcessFailed(executable, process.errorString()).constData());
        QVERIFY(process.waitForFinished());
        if (!value.isEmpty())
            QCOMPARE(process.exitCode(), 0);

        QCOMPARE(process.readAll(), value.toLocal8Bit());
    }
}

//-----------------------------------------------------------------------------
void tst_QProcess::setProcessEnvironment_data()
{
    setEnvironment_data();
}

void tst_QProcess::setProcessEnvironment()
{
    // make sure our environment variables are correct
    QVERIFY(qgetenv("tst_QProcess").isEmpty());
    QVERIFY(!qgetenv("PATH").isEmpty());

    QFETCH(QString, name);
    QFETCH(QString, value);
    QString executable = QDir::currentPath() + "/qprocess_testProcessEnvironment";

    {
        QProcess process;
        QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();
        if (value.isNull())
            environment.remove(name);
        else
            environment.insert(name, value);
        process.setProcessEnvironment(environment);
        process.start(executable, QStringList() << name);
        QVERIFY2(process.waitForStarted(5000),
                 msgStartProcessFailed(executable, process.errorString()).constData());
        QVERIFY(process.waitForFinished());
        if (value.isNull())
            QCOMPARE(process.exitCode(), 1);
        else if (!value.isEmpty())
            QCOMPARE(process.exitCode(), 0);

        QCOMPARE(process.readAll(), value.toLocal8Bit());
    }
}
//-----------------------------------------------------------------------------
void tst_QProcess::systemEnvironment()
{
    QVERIFY(!QProcess::systemEnvironment().isEmpty());
    QVERIFY(!QProcessEnvironment::systemEnvironment().isEmpty());

    QVERIFY(QProcessEnvironment::systemEnvironment().contains("PATH"));
    QVERIFY(!QProcess::systemEnvironment().filter(QRegExp("^PATH=", Qt::CaseInsensitive)).isEmpty());
}

//-----------------------------------------------------------------------------
void tst_QProcess::spaceInName()
{
    QProcess process;
    const QString binary = QLatin1String("test Space In Name/qprocess_testSpaceInName");
    process.start(binary, QStringList());
    QVERIFY2(process.waitForStarted(),
             msgStartProcessFailed(binary, process.errorString()).constData());
    process.write("", 1);
    QVERIFY(process.waitForFinished());
}

//-----------------------------------------------------------------------------
void tst_QProcess::lockupsInStartDetached()
{
    // Check that QProcess doesn't cause a lock up at this program's
    // exit if a thread was started and we tried to run a program that
    // doesn't exist. Before Qt 4.2, this used to lock up on Unix due
    // to calling ::exit instead of ::_exit if execve failed.

    QHostInfo::lookupHost(QString("something.invalid"), 0, 0);
    QProcess::execute("yjhbrty");
    QProcess::startDetached("yjhbrty");
}

//-----------------------------------------------------------------------------
void tst_QProcess::atEnd2()
{
    QProcess process;

    const QString binary = QLatin1String("./qprocess_testProcessEcho");

    process.start(binary);
    QVERIFY2(process.waitForStarted(),
             msgStartProcessFailed(binary, process.errorString()).constData());
    process.write("Foo\nBar\nBaz\nBodukon\nHadukan\nTorwukan\nend\n");
    process.putChar('\0');
    QVERIFY(process.waitForFinished());
    QList<QByteArray> lines;
    while (!process.atEnd()) {
        lines << process.readLine();
    }
    QCOMPARE(lines.size(), 7);
}

//-----------------------------------------------------------------------------
void tst_QProcess::waitForReadyReadForNonexistantProcess()
{
    // This comes from task 108968
    // Start a program that doesn't exist, process events and then try to waitForReadyRead
    qRegisterMetaType<QProcess::ProcessError>("QProcess::ProcessError");
    qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");

    QProcess process;
    QSignalSpy errorSpy(&process, SIGNAL(error(QProcess::ProcessError)));
    QSignalSpy finishedSpy1(&process, SIGNAL(finished(int)));
    QSignalSpy finishedSpy2(&process, SIGNAL(finished(int, QProcess::ExitStatus)));
    QVERIFY(!process.waitForReadyRead()); // used to crash
    process.start("doesntexist");
    QVERIFY(!process.waitForReadyRead());
    QCOMPARE(errorSpy.count(), 1);
    QCOMPARE(errorSpy.at(0).at(0).toInt(), 0);
    QCOMPARE(finishedSpy1.count(), 0);
    QCOMPARE(finishedSpy2.count(), 0);
}

//-----------------------------------------------------------------------------
void tst_QProcess::setStandardInputFile()
{
    static const char data[] = "A bunch\1of\2data\3\4\5\6\7...";
    QProcess process;
    QFile file("data");

    QVERIFY(file.open(QIODevice::WriteOnly));
    file.write(data, sizeof data);
    file.close();

    process.setStandardInputFile("data");

    const QString binary = QLatin1String("./qprocess_testProcessEcho");

    process.start(binary);
    QVERIFY2(process.waitForStarted(),
             msgStartProcessFailed(binary, process.errorString()).constData());
    QPROCESS_VERIFY(process, waitForFinished());
        QByteArray all = process.readAll();
    QCOMPARE(all.size(), int(sizeof data) - 1); // testProcessEcho drops the ending \0
    QVERIFY(all == data);
}

//-----------------------------------------------------------------------------
void tst_QProcess::setStandardOutputFile_data()
{
    QTest::addColumn<int>("channelToTest");
    QTest::addColumn<int>("_channelMode");
    QTest::addColumn<bool>("append");

    QTest::newRow("stdout-truncate") << int(QProcess::StandardOutput)
                                     << int(QProcess::SeparateChannels)
                                     << false;
    QTest::newRow("stdout-append") << int(QProcess::StandardOutput)
                                   << int(QProcess::SeparateChannels)
                                   << true;

    QTest::newRow("stderr-truncate") << int(QProcess::StandardError)
                                     << int(QProcess::SeparateChannels)
                                     << false;
    QTest::newRow("stderr-append") << int(QProcess::StandardError)
                                   << int(QProcess::SeparateChannels)
                                   << true;

    QTest::newRow("merged-truncate") << int(QProcess::StandardOutput)
                                     << int(QProcess::MergedChannels)
                                     << false;
    QTest::newRow("merged-append") << int(QProcess::StandardOutput)
                                   << int(QProcess::MergedChannels)
                                   << true;
}

void tst_QProcess::setStandardOutputFile()
{
    static const char data[] = "Original data. ";
    static const char testdata[] = "Test data.";

    QFETCH(int, channelToTest);
    QFETCH(int, _channelMode);
    QFETCH(bool, append);

    QProcess::ProcessChannelMode channelMode = QProcess::ProcessChannelMode(_channelMode);
    QIODevice::OpenMode mode = append ? QIODevice::Append : QIODevice::Truncate;

    // create the destination file with data
    QFile file("data");
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.write(data, sizeof data - 1);
    file.close();

    // run the process
    QProcess process;
    process.setProcessChannelMode(channelMode);
    if (channelToTest == QProcess::StandardOutput)
        process.setStandardOutputFile("data", mode);
    else
        process.setStandardErrorFile("data", mode);

    const QString binary = QLatin1String("./qprocess_testProcessEcho2");

    process.start(binary);
    QVERIFY2(process.waitForStarted(),
             msgStartProcessFailed(binary, process.errorString()).constData());
    process.write(testdata, sizeof testdata);
    QPROCESS_VERIFY(process,waitForFinished());

    // open the file again and verify the data
    QVERIFY(file.open(QIODevice::ReadOnly));
    QByteArray all = file.readAll();
    file.close();

    int expectedsize = sizeof testdata - 1;
    if (mode == QIODevice::Append) {
        QVERIFY(all.startsWith(data));
        expectedsize += sizeof data - 1;
    }
    if (channelMode == QProcess::MergedChannels) {
        expectedsize += sizeof testdata - 1;
    } else {
        QVERIFY(all.endsWith(testdata));
    }

    QCOMPARE(all.size(), expectedsize);
}

//-----------------------------------------------------------------------------
void tst_QProcess::setStandardOutputProcess_data()
{
    QTest::addColumn<bool>("merged");
    QTest::newRow("separate") << false;
    QTest::newRow("merged") << true;
}

void tst_QProcess::setStandardOutputProcess()
{
    QProcess source;
    QProcess sink;

    QFETCH(bool, merged);
    source.setProcessChannelMode(merged ? QProcess::MergedChannels : QProcess::SeparateChannels);
    source.setStandardOutputProcess(&sink);

    const QString sourceBinary = QLatin1String("./qprocess_testProcessEcho2");
    const QString sinkBinary = QLatin1String("./qprocess_testProcessEcho2");

    source.start(sourceBinary);
    QVERIFY2(source.waitForStarted(),
             msgStartProcessFailed(sourceBinary, source.errorString()).constData());
    sink.start(sinkBinary);
    QVERIFY2(sink.waitForStarted(),
             msgStartProcessFailed(sinkBinary, sink.errorString()).constData());

    QByteArray data("Hello, World");
    source.write(data);
    source.closeWriteChannel();
    QPROCESS_VERIFY(source, waitForFinished());
    QPROCESS_VERIFY(sink, waitForFinished());
    QByteArray all = sink.readAll();

    if (!merged)
        QCOMPARE(all, data);
    else
        QCOMPARE(all, QByteArray("HHeelllloo,,  WWoorrlldd"));
}

//-----------------------------------------------------------------------------
void tst_QProcess::fileWriterProcess()
{
    QString stdinStr;
    for (int i = 0; i < 5000; ++i)
        stdinStr += QString::fromLatin1("%1 -- testing testing 1 2 3\n").arg(i);

    QTime stopWatch;
    stopWatch.start();
    do {
        QFile::remove("fileWriterProcess.txt");
        QProcess process;
        const QString binary = QLatin1String("./qprocess_fileWriterProcess");
        process.start(binary, QIODevice::ReadWrite | QIODevice::Text);
        QVERIFY2(process.waitForStarted(),
                 msgStartProcessFailed(binary, process.errorString()).constData());
        process.write(stdinStr.toLatin1());
        process.closeWriteChannel();
        while (process.bytesToWrite()) {
            QVERIFY(stopWatch.elapsed() < 3500);
            QVERIFY(process.waitForBytesWritten(2000));
        }
        QVERIFY(process.waitForFinished());
        QCOMPARE(QFile("fileWriterProcess.txt").size(), qint64(stdinStr.size()));
    } while (stopWatch.elapsed() < 3000);
}

//-----------------------------------------------------------------------------
void tst_QProcess::detachedWorkingDirectoryAndPid()
{
    qint64 pid;

    QFile infoFile(QDir::currentPath() + QLatin1String("/detachedinfo.txt"));

    infoFile.remove();

    QString workingDir = QDir::tempPath();
    QVERIFY(QDir(workingDir).exists());

    QStringList args;
    args << infoFile.fileName();
    QVERIFY(QProcess::startDetached(QDir::currentPath() + QLatin1String("/qprocess_testDetached"), args, workingDir, &pid));

    QFileInfo fi(infoFile);
    fi.setCaching(false);
    //The guard counter ensures the test does not hang if the sub process fails.
    //Instead, the test will fail when trying to open & verify the sub process output file.
    for (int guard = 0; guard < 100 && fi.size() == 0; guard++) {
        QTest::qSleep(100);
    }

    QVERIFY(infoFile.open(QIODevice::ReadOnly | QIODevice::Text));
    QString actualWorkingDir = QString::fromUtf8(infoFile.readLine());
    actualWorkingDir.chop(1); // strip off newline
    QByteArray processIdString = infoFile.readLine();
    processIdString.chop(1);
    infoFile.close();
    infoFile.remove();

    bool ok = false;
    qint64 actualPid = processIdString.toLongLong(&ok);
    QVERIFY(ok);

    QCOMPARE(actualWorkingDir, workingDir);
    QCOMPARE(actualPid, pid);
}

//-----------------------------------------------------------------------------
void tst_QProcess::switchReadChannels()
{
    const char data[] = "ABCD";

    QProcess process;

    const QString binary = QLatin1String("./qprocess_testProcessEcho2");

    process.start(binary);
    QVERIFY2(process.waitForStarted(),
             msgStartProcessFailed(binary, process.errorString()).constData());
    process.write(data);
    process.closeWriteChannel();
    QVERIFY(process.waitForFinished(5000));

    for (int i = 0; i < 4; ++i) {
        process.setReadChannel(QProcess::StandardOutput);
        QCOMPARE(process.read(1), QByteArray(&data[i], 1));
        process.setReadChannel(QProcess::StandardError);
        QCOMPARE(process.read(1), QByteArray(&data[i], 1));
    }

    process.ungetChar('D');
    process.setReadChannel(QProcess::StandardOutput);
    process.ungetChar('D');
    process.setReadChannel(QProcess::StandardError);
    QCOMPARE(process.read(1), QByteArray("D"));
    process.setReadChannel(QProcess::StandardOutput);
    QCOMPARE(process.read(1), QByteArray("D"));
}

//-----------------------------------------------------------------------------
void tst_QProcess::setWorkingDirectory()
{
    process = new QProcess;
    process->setWorkingDirectory("test");

    const QString binary = QLatin1String("./qprocess_testSetWorkingDirectory");

    QSKIP("setWorkingDirectory will chdir before starting the process on unices", SkipAll);
    process->start(binary);
    QVERIFY2(process->waitForStarted(),
             msgStartProcessFailed(binary, process->errorString()).constData());
    QVERIFY(process->waitForFinished());

    QByteArray workingDir = process->readAllStandardOutput();
    QCOMPARE(QDir("test").canonicalPath(), QDir(workingDir.constData()).canonicalPath());

    delete process;
    process = 0;
}

//-----------------------------------------------------------------------------
void tst_QProcess::startFinishStartFinish()
{
    QProcess process;

    for (int i = 0; i < 3; ++i) {
        QCOMPARE(process.state(), QProcess::NotRunning);

        const QString binary = QLatin1String("./qprocess_testProcessOutput");

        process.start(binary);
        QVERIFY2(process.waitForStarted(),
                 msgStartProcessFailed(binary, process.errorString()).constData());
        QVERIFY(process.waitForReadyRead(10000));
        QCOMPARE(QString::fromLatin1(process.readLine().trimmed()),
                 QString("0 -this is a number"));
        if (process.state() != QProcess::NotRunning)
            QVERIFY(process.waitForFinished(10000));
    }
}

//-----------------------------------------------------------------------------
void tst_QProcess::invalidProgramString_data()
{
    QTest::addColumn<QString>("programString");
    QTest::newRow("null string") << QString();
    QTest::newRow("empty string") << QString("");
    QTest::newRow("only blank string") << QString("  ");
}

void tst_QProcess::invalidProgramString()
{
    QFETCH(QString, programString);
    QProcess process;

    qRegisterMetaType<QProcess::ProcessError>("QProcess::ProcessError");
    QSignalSpy spy(&process, SIGNAL(error(QProcess::ProcessError)));

    process.start(programString);
    QCOMPARE(process.error(), QProcess::FailedToStart);
    QCOMPARE(spy.count(), 1);

    QVERIFY(!QProcess::startDetached(programString));
}

//-----------------------------------------------------------------------------
void tst_QProcess::onlyOneStartedSignal()
{
    QProcess process;

    QSignalSpy spyStarted(&process,  SIGNAL(started()));
    QSignalSpy spyFinished(&process, SIGNAL(finished(int, QProcess::ExitStatus)));

    const QString binary = QLatin1String("./qprocess_testProcessNormal");
    process.start(binary);
    QVERIFY2(process.waitForStarted(),
             msgStartProcessFailed(binary, process.errorString()).constData());

    QVERIFY(process.waitForStarted(5000));
    QVERIFY(process.waitForFinished(5000));
    QCOMPARE(spyStarted.count(), 1);
    QCOMPARE(spyFinished.count(), 1);

    spyStarted.clear();
    spyFinished.clear();

    process.start(binary);
    QVERIFY(process.waitForStarted(5000));
    QVERIFY(process.waitForFinished(5000));
    QCOMPARE(spyStarted.count(), 1);
    QCOMPARE(spyFinished.count(), 1);
}

void tst_QProcess::waitForStartedWithoutStart()
{
    QProcess process;
    QVERIFY(!process.waitForStarted(5000));
}

QTEST_MAIN(tst_QProcess)

#include "moc_tst_qprocess.cpp"

#endif

