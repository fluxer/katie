/*
 * Copyright (C) 2009 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef RepatchBuffer_h
#define RepatchBuffer_h

#include <wtf/Platform.h>

#if ENABLE(ASSEMBLER)

#include <MacroAssembler.h>
#include <wtf/Noncopyable.h>

namespace JSC {

// RepatchBuffer:
//
// This class is used to modify code after code generation has been completed,
// and after the code has potentially already been executed.  This mechanism is
// used to apply optimizations to the code.
//
class RepatchBuffer {
public:
    static inline void relink(CodeLocationJump jump, CodeLocationLabel destination)
    {
        MacroAssembler::repatchJump(jump, destination);
    }

    static inline void relink(CodeLocationCall call, CodeLocationLabel destination)
    {
        MacroAssembler::repatchCall(call, destination);
    }

    static inline void relink(CodeLocationCall call, FunctionPtr destination)
    {
        MacroAssembler::repatchCall(call, destination);
    }

    static inline void relink(CodeLocationNearCall nearCall, MacroAssemblerCodePtr destination)
    {
        MacroAssembler::repatchNearCall(nearCall, CodeLocationLabel(destination));
    }

    static inline void relink(CodeLocationNearCall nearCall, CodeLocationLabel destination)
    {
        MacroAssembler::repatchNearCall(nearCall, destination);
    }

    static inline void repatch(CodeLocationDataLabel32 dataLabel32, int32_t value)
    {
        MacroAssembler::repatchInt32(dataLabel32, value);
    }

    static inline void repatch(CodeLocationDataLabelPtr dataLabelPtr, void* value)
    {
        MacroAssembler::repatchPointer(dataLabelPtr, value);
    }

    static inline void repatchLoadPtrToLEA(CodeLocationInstruction instruction)
    {
        MacroAssembler::repatchLoadPtrToLEA(instruction);
    }

    static inline void relinkCallerToTrampoline(ReturnAddressPtr returnAddress, CodeLocationLabel label)
    {
        relink(CodeLocationCall(MacroAssemblerCodePtr(returnAddress)), label);
    }
    
    static inline void relinkCallerToTrampoline(ReturnAddressPtr returnAddress, MacroAssemblerCodePtr newCalleeFunction)
    {
        relinkCallerToTrampoline(returnAddress, CodeLocationLabel(newCalleeFunction));
    }

    static inline void relinkCallerToFunction(ReturnAddressPtr returnAddress, FunctionPtr function)
    {
        relink(CodeLocationCall(MacroAssemblerCodePtr(returnAddress)), function);
    }
    
    static inline void relinkNearCallerToTrampoline(ReturnAddressPtr returnAddress, CodeLocationLabel label)
    {
        relink(CodeLocationNearCall(MacroAssemblerCodePtr(returnAddress)), label);
    }
    
    static inline void relinkNearCallerToTrampoline(ReturnAddressPtr returnAddress, MacroAssemblerCodePtr newCalleeFunction)
    {
        relinkNearCallerToTrampoline(returnAddress, CodeLocationLabel(newCalleeFunction));
    }
};

} // namespace JSC

#endif // ENABLE(ASSEMBLER)

#endif // RepatchBuffer_h
