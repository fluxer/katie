/*
 * Copyright (C) 2007 Apple Inc.  All rights reserved.
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
#include "UTF8.h"

namespace WTF {
namespace Unicode {

inline int inlineUTF8SequenceLengthNonASCII(char b0)
{
    if ((b0 & 0xC0) != 0xC0)
        return 0;
    if ((b0 & 0xE0) == 0xC0)
        return 2;
    if ((b0 & 0xF0) == 0xE0)
        return 3;
    if ((b0 & 0xF8) == 0xF0)
        return 4;
    return 0;
}

inline int inlineUTF8SequenceLength(char b0)
{
    return (b0 & 0x80) == 0 ? 1 : inlineUTF8SequenceLengthNonASCII(b0);
}

int UTF8SequenceLength(char b0)
{
    return (b0 & 0x80) == 0 ? 1 : inlineUTF8SequenceLengthNonASCII(b0);
}

int decodeUTF8Sequence(const char* sequence)
{
    // Handle 0-byte sequences (never valid).
    const unsigned char b0 = sequence[0];
    const int length = inlineUTF8SequenceLength(b0);
    if (length == 0)
        return -1;

    // Handle 1-byte sequences (plain ASCII).
    const unsigned char b1 = sequence[1];
    if (length == 1) {
        if (b1)
            return -1;
        return b0;
    }

    // Handle 2-byte sequences.
    if ((b1 & 0xC0) != 0x80)
        return -1;
    const unsigned char b2 = sequence[2];
    if (length == 2) {
        if (b2)
            return -1;
        const int c = ((b0 & 0x1F) << 6) | (b1 & 0x3F);
        if (c < 0x80)
            return -1;
        return c;
    }

    // Handle 3-byte sequences.
    if ((b2 & 0xC0) != 0x80)
        return -1;
    const unsigned char b3 = sequence[3];
    if (length == 3) {
        if (b3)
            return -1;
        const int c = ((b0 & 0xF) << 12) | ((b1 & 0x3F) << 6) | (b2 & 0x3F);
        if (c < 0x800)
            return -1;
        // UTF-16 surrogates should never appear in UTF-8 data.
        if (c >= 0xD800 && c <= 0xDFFF)
            return -1;
        return c;
    }

    // Handle 4-byte sequences.
    if ((b3 & 0xC0) != 0x80)
        return -1;
    const unsigned char b4 = sequence[4];
    if (length == 4) {
        if (b4)
            return -1;
        const int c = ((b0 & 0x7) << 18) | ((b1 & 0x3F) << 12) | ((b2 & 0x3F) << 6) | (b3 & 0x3F);
        if (c < 0x10000 || c > 0x10FFFF)
            return -1;
        return c;
    }

    return -1;
}

}
}
