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

#ifndef QPROCESS_P_H
#define QPROCESS_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qprocess.h"
#include "qiodevice_p.h"
#include "qringbuffer_p.h"
#include "qorderedmutexlocker_p.h"

typedef int Q_PIPE;
#define INVALID_Q_PIPE -1

#ifndef QT_NO_PROCESS

QT_BEGIN_NAMESPACE

class QSocketNotifier;
class QTimer;

class QProcessPrivate : public QIODevicePrivate
{
public:
    Q_DECLARE_PUBLIC(QProcess)

    struct Channel {
        enum ProcessChannelType {
            Normal = 0,
            PipeSource = 1,
            PipeSink = 2,
            Redirect = 3
        };

        Channel() : process(0), notifier(0), type(Normal), closed(false), append(false)
        {
            pipe[0] = INVALID_Q_PIPE;
            pipe[1] = INVALID_Q_PIPE;
        }

        void clear();

        Channel &operator=(const QString &fileName)
        {
            clear();
            file = fileName;
            type = fileName.isEmpty() ? Normal : Redirect;
            return *this;
        }

        void pipeTo(QProcessPrivate *other)
        {
            clear();
            process = other;
            type = PipeSource;
        }

        void pipeFrom(QProcessPrivate *other)
        {
            clear();
            process = other;
            type = PipeSink;
        }

        QString file;
        QProcessPrivate *process;
        QSocketNotifier *notifier;
        Q_PIPE pipe[2];

        ProcessChannelType type;
        bool closed;
        bool append;
    };

    QProcessPrivate();
    virtual ~QProcessPrivate();

    // private slots
    bool _q_canReadStandardOutput();
    bool _q_canReadStandardError();
    bool _q_canWrite();
    bool _q_startupNotification();
    bool _q_processDied();

    QProcess::ProcessChannel processChannel;
    QProcess::ProcessChannelMode processChannelMode;
    QProcess::ProcessError processError;
    QProcess::ProcessState processState;
    QString workingDirectory;
    pid_t pid;

    bool dying;
    bool emittedReadyRead;
    bool emittedBytesWritten;

    Channel stdinChannel;
    Channel stdoutChannel;
    Channel stderrChannel;
    bool createChannel(Channel &channel);
    void closeWriteChannel();

    QString program;
    QStringList arguments;
    QProcessEnvironment environment;

    QRingBuffer outputReadBuffer;
    QRingBuffer errorReadBuffer;
    QRingBuffer writeBuffer;

    Q_PIPE childStartedPipe[2];
    Q_PIPE deathPipe[2];
    void destroyPipe(Q_PIPE pipe[2]);

    QSocketNotifier *startupSocketNotifier;
    QSocketNotifier *deathNotifier;

    void startProcess();
    void execChild(const char *workingDirectory, char **path, char **argv, char **envp);
    bool processStarted();
    void terminateProcess();
    void killProcess();
    void findExitCode();
    bool waitForDeadChild();

    static bool startDetached(const QString &program, const QStringList &arguments, const QString &workingDirectory = QString(),
                              qint64 *pid = 0);

    int exitCode;
    QProcess::ExitStatus exitStatus;
    bool crashed;

    bool waitForStarted(int msecs);
    bool waitForReadyRead(int msecs);
    bool waitForBytesWritten(int msecs);
    bool waitForFinished(int msecs);

    qint64 bytesAvailableFromStdout() const;
    qint64 bytesAvailableFromStderr() const;
    qint64 readFromStdout(char *data, qint64 maxlen);
    qint64 readFromStderr(char *data, qint64 maxlen);
    qint64 writeToStdin(const char *data, qint64 maxlen);

    void cleanup();
    static void initializeProcessManager();

};

QT_END_NAMESPACE

#endif // QT_NO_PROCESS

#endif // QPROCESS_P_H
