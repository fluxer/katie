#ifndef QSCRIPTTOOLSCOMMON_H
#define QSCRIPTTOOLSCOMMON_H

#include "qscriptcontextinfo.h"
#include "qscriptscriptdata_p.h"
#include "qscriptdebuggerconsolecommand_p.h"
#include "qscriptdebuggerconsolecommandgroupdata_p.h"
#include "qscriptdebuggervalueproperty_p.h"
#include "qscriptdebuggerobjectsnapshotdelta_p.h"
#include "qscriptdebuggerresponse_p.h"

QT_BEGIN_NAMESPACE
typedef QPair<QList<qint64>, QList<qint64> > QScriptScriptsDelta;
typedef QPair<QList<qint64>, QList<qint64> > QScriptContextsDelta;
class QScriptDebuggerBackendPrivate;
class QScriptDebuggerResponse;
QT_END_NAMESPACE

Q_DECLARE_METATYPE(QScriptScriptsDelta)
Q_DECLARE_METATYPE(QScriptDebuggerValue)
Q_DECLARE_METATYPE(QScriptDebuggerBackendPrivate*)
Q_DECLARE_METATYPE(QScriptBreakpointData)
Q_DECLARE_METATYPE(QScriptDebuggerValueProperty)
Q_DECLARE_METATYPE(QScriptDebuggerValuePropertyList)
Q_DECLARE_METATYPE(QScriptDebuggerObjectSnapshotDelta)
Q_DECLARE_METATYPE(QScriptDebuggerResponse)
Q_DECLARE_METATYPE(QScriptBreakpointMap)
Q_DECLARE_METATYPE(QScriptScriptData)
Q_DECLARE_METATYPE(QScriptScriptMap)
Q_DECLARE_METATYPE(QScriptContextInfo)
Q_DECLARE_METATYPE(QScriptDebuggerConsoleCommand*)
Q_DECLARE_METATYPE(QScriptDebuggerConsoleCommandList)
Q_DECLARE_METATYPE(QScriptDebuggerConsoleCommandGroupData)
Q_DECLARE_METATYPE(QScriptDebuggerConsoleCommandGroupMap)
Q_DECLARE_METATYPE(QScriptDebuggerValueList)

QT_BEGIN_NAMESPACE

static inline bool isAlmostIdentChar(const QChar &ch)
{
    static QChar underscore = QLatin1Char('_');
    return ch.isLetterOrNumber() || (ch == underscore);
}

static inline bool isIdentChar(const QChar &ch)
{
    static QChar underscore = QLatin1Char('_');
    return ch.isLetter() || (ch == underscore);
}

static inline bool isPrefixOf(const QString &prefix, const QString &what)
{
    return ((what.length() > prefix.length()) && what.startsWith(prefix));
}

QT_END_NAMESPACE

#endif // QSCRIPTTOOLSCOMMON_H
