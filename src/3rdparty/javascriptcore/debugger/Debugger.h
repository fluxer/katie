/*
 *  Copyright (C) 1999-2001 Harri Porten (porten@kde.org)
 *  Copyright (C) 2001 Peter Kelly (pmk@post.com)
 *  Copyright (C) 2008, 2009 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef Debugger_h
#define Debugger_h

#include <debugger/DebuggerCallFrame.h>
#include <wtf/HashSet.h>

namespace JSC {

    class ExecState;
    class JSGlobalData;
    class JSGlobalObject;
    class JSValue;
    class SourceCode;
    class UString;

    class Debugger {
    public:
        virtual ~Debugger();

        void attach(JSGlobalObject*);
        virtual void detach(JSGlobalObject*);

        virtual void scriptUnload(QT_PREPEND_NAMESPACE(qint64) id)
        {
            Q_UNUSED(id);
        };
        virtual void scriptLoad(QT_PREPEND_NAMESPACE(qint64) id, const UString &program,
                            const UString &fileName, int baseLineNumber)
        {
            Q_UNUSED(id);
            Q_UNUSED(program);
            Q_UNUSED(fileName);
            Q_UNUSED(baseLineNumber);
        };
        virtual void contextPush() {};
        virtual void contextPop() {};

        virtual void evaluateStart(intptr_t sourceID)
        {
            Q_UNUSED(sourceID);
        }
        virtual void evaluateStop(const JSC::JSValue& returnValue, intptr_t sourceID)
        {
            Q_UNUSED(sourceID);
            Q_UNUSED(returnValue);
        }

        virtual void exceptionThrow(const JSC::DebuggerCallFrame& frame, intptr_t sourceID, bool hasHandler)
        {
            Q_UNUSED(frame);
            Q_UNUSED(sourceID);
            Q_UNUSED(hasHandler);
        };
        virtual void exceptionCatch(const JSC::DebuggerCallFrame& frame, intptr_t sourceID)
        {
            Q_UNUSED(frame);
            Q_UNUSED(sourceID);
        };

        virtual void functionExit(const JSC::JSValue& returnValue, intptr_t sourceID)
        {
            Q_UNUSED(returnValue);
            Q_UNUSED(sourceID);
        };

        virtual void sourceParsed(ExecState*, const SourceCode&, int errorLineNumber, const UString& errorMessage) = 0;
        virtual void exception(const DebuggerCallFrame&, intptr_t sourceID, int lineNumber, bool hasHandler) = 0;
        virtual void atStatement(const DebuggerCallFrame&, intptr_t sourceID, int lineNumber) = 0;
        virtual void callEvent(const DebuggerCallFrame&, intptr_t sourceID, int lineNumber) = 0;
        virtual void returnEvent(const DebuggerCallFrame&, intptr_t sourceID, int lineNumber) = 0;

        virtual void willExecuteProgram(const DebuggerCallFrame&, intptr_t sourceID, int lineNumber) = 0;
        virtual void didExecuteProgram(const DebuggerCallFrame&, intptr_t sourceID, int lineNumber) = 0;
        virtual void didReachBreakpoint(const DebuggerCallFrame&, intptr_t sourceID, int lineNumber) = 0;

        void recompileAllJSFunctions(JSGlobalData*);

    private:
        HashSet<JSGlobalObject*> m_globalObjects;
    };
} // namespace JSC

#endif // Debugger_h
