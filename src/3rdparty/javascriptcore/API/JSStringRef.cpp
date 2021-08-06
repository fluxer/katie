/*
 * Copyright (C) 2006, 2007 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "Platform.h"
#include "JSStringRef.h"

#include "InitializeThreading.h"
#include "OpaqueJSString.h"
#include <wtf/unicode/UTF8.h>

#include <QTextCodec>

using namespace JSC;
using namespace WTF::Unicode;

JSStringRef JSStringCreateWithCharacters(const JSChar* chars, size_t numChars)
{
    initializeThreading();
    return OpaqueJSString::create(chars, numChars).releaseRef();
}

JSStringRef JSStringCreateWithUTF8CString(const char* string)
{
    initializeThreading();
    if (string) {
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        QTextDecoder decoder(codec, QTextCodec::DefaultConversion);
        QString result = decoder.toUnicode(string, strlen(string));
        if (!decoder.hasFailure()) {
            return OpaqueJSString::create(reinterpret_cast<const UChar*>(result.unicode()), result.size()).releaseRef();
        }
    }

    // Null string.
    return OpaqueJSString::create().releaseRef();
}

JSStringRef JSStringRetain(JSStringRef string)
{
    string->ref();
    return string;
}

void JSStringRelease(JSStringRef string)
{
    string->deref();
}

size_t JSStringGetLength(JSStringRef string)
{
    return string->length();
}

const JSChar* JSStringGetCharactersPtr(JSStringRef string)
{
    return string->characters();
}

size_t JSStringGetMaximumUTF8CStringSize(JSStringRef string)
{
    // Any UTF8 character > 3 bytes encodes as a UTF16 surrogate pair.
    return string->length() * 3 + 1; // + 1 for terminating '\0'
}

size_t JSStringGetUTF8CString(JSStringRef string, char* buffer, size_t bufferSize)
{
    if (!bufferSize)
        return 0;

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextEncoder encoder(codec, QTextCodec::DefaultConversion);
    QByteArray result = encoder.fromUnicode(reinterpret_cast<const QChar*>(string->characters()), string->length());
    if (encoder.hasFailure()) {
        buffer = nullptr;
        return 0;
    }
    buffer = result.data();
    return result.size();
}

bool JSStringIsEqual(JSStringRef a, JSStringRef b)
{
    unsigned len = a->length();
    return len == b->length() && 0 == memcmp(a->characters(), b->characters(), len * sizeof(UChar));
}

bool JSStringIsEqualToUTF8CString(JSStringRef a, const char* b)
{
    JSStringRef bBuf = JSStringCreateWithUTF8CString(b);
    bool result = JSStringIsEqual(a, bBuf);
    JSStringRelease(bBuf);
    
    return result;
}
