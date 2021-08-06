/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the examples of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtGui/QApplication>
#include <QtScript/QScriptEngine>
#include <QtScriptTools/QScriptEngineDebugger>

#include <stdlib.h>

QT_USE_NAMESPACE

static bool wantsToQuit;

static QScriptValue qtscript_quit(QScriptContext *ctx, QScriptEngine *eng)
{
    Q_UNUSED(ctx);
    wantsToQuit = true;
    return eng->undefinedValue();
}

static void interactive(QScriptEngine *eng)
{
    QScriptValue global = eng->globalObject();
    QScriptValue quitFunction = eng->newFunction(qtscript_quit);
    if (!global.property(QLatin1String("exit")).isValid())
        global.setProperty(QLatin1String("exit"), quitFunction);
    if (!global.property(QLatin1String("quit")).isValid())
        global.setProperty(QLatin1String("quit"), quitFunction);
    wantsToQuit = false;

    QFile qin;
    qin.open(stdin, QFile::ReadOnly);

    const char *qscript_prompt = "qs> ";
    const char *dot_prompt = ".... ";
    const char *prompt = qscript_prompt;

    QString code;

    forever {

        printf("%s", prompt);
        fflush(stdout);

        QString line = qin.readLine();
        if (line.isNull())
            break;

        code += line;
        code += QLatin1Char('\n');

        if (line.trimmed().isEmpty()) {
            continue;

        } else if (! eng->canEvaluate(code)) {
            prompt = dot_prompt;

        } else {
            QScriptValue result = eng->evaluate(code, QLatin1String("typein"));

            code.clear();
            prompt = qscript_prompt;

            if (! result.isUndefined())
                fprintf(stderr, "%s\n", qPrintable(result.toString()));

            if (wantsToQuit)
                break;
        }
    }
}

static QScriptValue importExtension(QScriptContext *context, QScriptEngine *engine)
{
    return engine->importExtension(context->argument(0).toString());
}

static QScriptValue loadScripts(QScriptContext *context, QScriptEngine *engine)
{
    for (int i = 0; i < context->argumentCount(); ++i) {
        QString fileName = context->argument(0).toString();
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
            return context->throwError(QString::fromLatin1("could not open %0 for reading").arg(fileName));
        QByteArray contents = file.readAll();
        file.close();
        QScriptContext *pc = context->parentContext();
        context->setActivationObject(pc->activationObject());
        context->setThisObject(pc->thisObject());
        QScriptValue ret = engine->evaluate(QString::fromLocal8Bit(contents));
        if (engine->hasUncaughtException())
            return ret;
    }
    return engine->undefinedValue();
}

int main(int argc, char *argv[])
{
    QApplication *app = new QApplication(argc, argv);
    QScriptEngine *eng = new QScriptEngine(app);
    QScriptEngineDebugger *debugger = nullptr;

    QScriptValue globalObject = eng->globalObject();

    globalObject.setProperty(QLatin1String("load"), eng->newFunction(loadScripts, /*length=*/1));

    {
        if (!globalObject.property(QLatin1String("qt")).isObject())
            globalObject.setProperty(QLatin1String("qt"), eng->newObject());
        QScriptValue qscript = eng->newObject();
        qscript.setProperty(QLatin1String("importExtension"), eng->newFunction(importExtension));
        globalObject.property(QLatin1String("qt")).setProperty(QLatin1String("script"), qscript);
    }

    if (! *++argv) {
        interactive(eng);
        return EXIT_SUCCESS;
    }

    while (const char *arg = *argv++) {
        QString fn = QString::fromLocal8Bit(arg);

        if (fn == QLatin1String("-d")) {
            debugger = new QScriptEngineDebugger(app);
            debugger->attachTo(eng);
            debugger->setAutoShowStandardWindow(true);
            continue;
        } else if (fn == QLatin1String("-i")) {
            interactive(eng);
            break;
        }

        QByteArray contents;
        int lineNumber = 1;

        if (fn == QLatin1String("-")) {
            QFile qin;
            qin.open(stdin, QFile::ReadOnly);
            contents = qin.readAll();
        }

        else {
            QFile file(fn);

            if (file.open(QFile::ReadOnly)) {
                contents = file.readAll();
                file.close();

                // strip off #!/usr/bin/env qscript line
                if (contents.startsWith("#!")) {
                    contents.remove(0, contents.indexOf("\n"));
                    ++lineNumber;
                }
            }
        }

        if (contents.isEmpty())
            continue;

        QScriptValue r = eng->evaluate(QString::fromLocal8Bit(contents), fn, lineNumber);
        if (!debugger && eng->hasUncaughtException()) {
            QStringList backtrace = eng->uncaughtExceptionBacktrace();
            fprintf (stderr, "    %s\n%s\n\n", qPrintable(r.toString()),
                     qPrintable(backtrace.join(QLatin1String("\n"))));
            return EXIT_FAILURE;
        }
    }

    // do not exit until the debugger has been closed
    if (debugger && eng->hasUncaughtException()) {
        app->processEvents();
    }

    delete debugger;
    delete eng;
    delete app;

    return EXIT_SUCCESS;
}
