/*
 *  Copyright (C) 1999-2001, 2004 Harri Porten (porten@kde.org)
 *  Copyright (c) 2007, 2008 Apple Inc. All rights reserved.
 *  Copyright (C) 2009 Torch Mobile, Inc.
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

#include "Platform.h"
#include "RegExp.h"
#include "Lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wtf/Assertions.h>

#include <pcre.h>

namespace JSC {

inline RegExp::RegExp(const UString& pattern)
    : m_pattern(pattern)
    , m_flagBits(0)
    , m_constructionError(0)
    , m_numSubpatterns(0)
{
    compile();
}

inline RegExp::RegExp(const UString& pattern, const UString& flags)
    : m_pattern(pattern)
    , m_flagBits(0)
    , m_constructionError(0)
    , m_numSubpatterns(0)
{
    // NOTE: The global flag is handled on a case-by-case basis by functions like
    // String::match and RegExpObject::match.
    //Invalid flags should throw a SyntaxError (ECMA Script 15.10.4.1)
    static const char flagError[] = "invalid regular expression flag";
    for (int i = 0; i < flags.size(); i++) {
        switch (flags.data()[i]) {
        case 'g':
            m_flagBits |= Global;
            break;
        case 'i':
            m_flagBits |= IgnoreCase;
            break;
        case 'm':
            m_flagBits |= Multiline;
            break;
        default:
            m_constructionError = flagError;
            m_regExp = 0;
            return;
        }
    }

    compile();
}

RegExp::~RegExp()
{
    pcre_free(m_regExp);
}

PassRefPtr<RegExp> RegExp::create(const UString& pattern)
{
    return adoptRef(new RegExp(pattern));
}

PassRefPtr<RegExp> RegExp::create(const UString& pattern, const UString& flags)
{
    return adoptRef(new RegExp(pattern, flags));
}

void RegExp::compile()
{
    m_regExp = nullptr;

    int regexOptions = PCRE_JAVASCRIPT_COMPAT | PCRE_NO_UTF8_CHECK;
    if (ignoreCase())
        regexOptions |= PCRE_CASELESS;
    if (multiline())
        regexOptions |= PCRE_MULTILINE;
    int errorOffset;
    m_regExp = pcre_compile(m_pattern.ascii(), regexOptions, &m_constructionError, &errorOffset, nullptr);

    pcre_fullinfo(m_regExp, nullptr, PCRE_INFO_CAPTURECOUNT, &m_numSubpatterns);
}

int RegExp::match(const UString& s, int startOffset, Vector<int, 32>* ovector)
{
    if (startOffset < 0)
        startOffset = 0;
    if (ovector)
        ovector->clear();

    if (startOffset > s.size() || s.isNull())
        return -1;

    if (m_regExp) {
        // Set up the offset vector for the result.
        // First 2/3 used for result, the last third used by PCRE.
        int* offsetVector;
        int offsetVectorSize;
        int fixedSizeOffsetVector[3];
        if (!ovector) {
            offsetVectorSize = 3;
            offsetVector = fixedSizeOffsetVector;
        } else {
            offsetVectorSize = (m_numSubpatterns + 1) * 3;
            ovector->resize(offsetVectorSize);
            offsetVector = ovector->data();
        }

        const int numMatches = pcre_exec(m_regExp, nullptr, s.ascii(), s.size(), startOffset, 0, offsetVector, offsetVectorSize);

        if (numMatches < 0) {
#ifndef QT_NO_DEBUG
            if (numMatches != PCRE_ERROR_NOMATCH)
                fprintf(stderr, "jsRegExpExecute failed with result %d\n", numMatches);
#endif
            if (ovector)
                ovector->clear();
            return -1;
        }

        return offsetVector[0];
    }

    return -1;
}

} // namespace JSC
