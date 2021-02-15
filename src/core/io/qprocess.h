/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#ifndef QPROCESS_H
#define QPROCESS_H

#include <QtCore/qstringlist.h>
#include <QtCore/qshareddata.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_PROCESS

typedef qint64 Q_PID;

class QProcessPrivate;
class QProcessEnvironmentPrivate;

class Q_CORE_EXPORT QProcessEnvironment
{
public:
    QProcessEnvironment();
    QProcessEnvironment(const QProcessEnvironment &other);
    ~QProcessEnvironment();
    QProcessEnvironment &operator=(const QProcessEnvironment &other);

    bool operator==(const QProcessEnvironment &other) const;
    inline bool operator!=(const QProcessEnvironment &other) const
    { return !(*this == other); }

    bool isEmpty() const;
    void clear();

    bool contains(const QString &name) const;
    void insert(const QString &name, const QString &value);
    void remove(const QString &name);
    QString value(const QString &name, const QString &defaultValue = QString()) const;

    QStringList toStringList() const;

    QStringList keys() const;

    void insert(const QProcessEnvironment &e);

    static QProcessEnvironment systemEnvironment();

private:
    friend class QProcessPrivate;
    friend class QProcessEnvironmentPrivate;
    QSharedDataPointer<QProcessEnvironmentPrivate> d;
};

class Q_CORE_EXPORT QProcess : public QIODevice
{
    Q_OBJECT
public:
    enum ProcessError {
        FailedToStart, //### file not found, resource error
        Crashed,
        Timedout,
        ReadError,
        WriteError,
        UnknownError
    };
    enum ProcessState {
        NotRunning,
        Starting,
        Running
    };
    enum ProcessChannel {
        StandardOutput,
        StandardError
    };
    enum ProcessChannelMode {
        SeparateChannels,
        MergedChannels,
        ForwardedChannels
    };
    enum ExitStatus {
        NormalExit,
        CrashExit
    };

    explicit QProcess(QObject *parent = Q_NULLPTR);
    virtual ~QProcess();

    void start(const QString &program, const QStringList &arguments, OpenMode mode = ReadWrite);
    void start(const QString &program, OpenMode mode = ReadWrite);

    ProcessChannelMode processChannelMode() const;
    void setProcessChannelMode(ProcessChannelMode mode);

    ProcessChannel readChannel() const;
    void setReadChannel(ProcessChannel channel);

    void closeReadChannel(ProcessChannel channel);
    void closeWriteChannel();

    void setStandardInputFile(const QString &fileName);
    void setStandardOutputFile(const QString &fileName, OpenMode mode = Truncate);
    void setStandardErrorFile(const QString &fileName, OpenMode mode = Truncate);
    void setStandardOutputProcess(QProcess *destination);

    QString workingDirectory() const;
    void setWorkingDirectory(const QString &dir);

    void setEnvironment(const QStringList &environment);
    QStringList environment() const;
    void setProcessEnvironment(const QProcessEnvironment &environment);
    QProcessEnvironment processEnvironment() const;

    QProcess::ProcessError error() const;
    QProcess::ProcessState state() const;

    Q_PID pid() const;

    bool waitForStarted(int msecs = 30000);
    bool waitForReadyRead(int msecs = 30000);
    bool waitForBytesWritten(int msecs = 30000);
    bool waitForFinished(int msecs = 30000);

    QByteArray readAllStandardOutput();
    QByteArray readAllStandardError();

    int exitCode() const;
    QProcess::ExitStatus exitStatus() const;

    // QIODevice
    qint64 bytesAvailable() const;
    qint64 bytesToWrite() const;
    bool isSequential() const;
    bool canReadLine() const;
    void close();
    bool atEnd() const;

    static int execute(const QString &program, const QStringList &arguments);
    static int execute(const QString &program);

    static bool startDetached(const QString &program, const QStringList &arguments, const QString &workingDirectory,
                              Q_PID *pid = 0);
    static bool startDetached(const QString &program, const QStringList &arguments);
    static bool startDetached(const QString &program);

    static QStringList systemEnvironment();

public Q_SLOTS:
    void terminate();
    void kill();

Q_SIGNALS:
    void started();
    void finished(int exitCode);
    void finished(int exitCode, QProcess::ExitStatus exitStatus);
    void error(QProcess::ProcessError error);
    void stateChanged(QProcess::ProcessState state);

    void readyReadStandardOutput();
    void readyReadStandardError();

protected:
    void setProcessState(ProcessState state);

    virtual void setupChildProcess();

    // QIODevice
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

private:
    Q_DECLARE_PRIVATE(QProcess)
    Q_DISABLE_COPY(QProcess)

    Q_PRIVATE_SLOT(d_func(), bool _q_canReadStandardOutput())
    Q_PRIVATE_SLOT(d_func(), bool _q_canReadStandardError())
    Q_PRIVATE_SLOT(d_func(), bool _q_canWrite())
    Q_PRIVATE_SLOT(d_func(), bool _q_startupNotification())
    Q_PRIVATE_SLOT(d_func(), bool _q_processDied())
    friend class QProcessManager;
};

#endif // QT_NO_PROCESS

QT_END_NAMESPACE


#endif // QPROCESS_H
