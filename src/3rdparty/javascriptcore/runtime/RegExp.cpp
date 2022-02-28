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

namespace JSC {

inline RegExp::RegExp(const UString& pattern)
    : m_pattern(pattern)
    , m_flagBits(0)
    , m_numSubpatterns(0)
{
    compile();
}

inline RegExp::RegExp(const UString& pattern, const UString& flags)
    : m_pattern(pattern)
    , m_flagBits(0)
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
            return;
        }
    }

    compile();
}

RegExp::~RegExp()
{
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
    m_constructionError = std::string();
    m_regExp = std::regex();
    m_numSubpatterns = 0;

    int regexOptions = std::regex_constants::ECMAScript;
    if (ignoreCase())
        regexOptions |= std::regex_constants::icase;
    if (multiline())
#if __cplusplus >= 201703L
        regexOptions |= std::regex_constants::multiline;
#endif

#ifndef QT_NO_EXCEPTIONS
    try {
        const std::string regexpattern = m_pattern.ascii();
        m_regExp = std::regex(regexpattern.c_str(), regexOptions);
        std::sregex_iterator matchbegin = std::sregex_iterator(regexpattern.begin(), regexpattern.end(), m_regExp);
        std::sregex_iterator matchend = std::sregex_iterator();
        m_numSubpatterns = std::distance(matchbegin, matchend);
    } catch (const std::regex_error &err) {
        m_constructionError = err.what();
    } catch (...) {
        m_constructionError = "Exception caught during regex compilation";
    }
#else
    // no exceptions, no way to find out if error occured
    const std::string regexpattern = m_pattern.ascii();
    m_regExp = std::regex(regexpattern.c_str(), regexOptions);
    std::sregex_iterator matchbegin = std::sregex_iterator(regexpattern.begin(), regexpattern.end(), m_regExp);
    std::sregex_iterator matchend = std::sregex_iterator();
    m_numSubpatterns = std::distance(matchbegin, matchend);
#endif
}

int RegExp::match(const UString& s, int startOffset, Vector<int, 32>* ovector)
{
    if (startOffset < 0)
        startOffset = 0;
    if (ovector)
        ovector->clear();

    if (startOffset > s.size() || s.isNull())
        return -1;

    if (isValid()) {
        // Set up the offset vector for the result.
        // First 2/3 used for result, the last third unused but there for compatibility.
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

#ifndef QT_NO_EXCEPTIONS
        bool didmatch = false;
        try {
            didmatch = std::regex_match(s.ascii() + startOffset, m_regExp);
        } catch (const std::regex_error &err) {
            m_constructionError = err.what();
        } catch (...) {
            m_constructionError = "Exception caught during regex matching";
        }
#else
        const bool didmatch = std::regex_match(s.ascii() + startOffset, m_regExp);
#endif

        if (!didmatch) {
#ifndef QT_NO_DEBUG
            fprintf(stderr, "jsRegExpExecute failed with result\n");
#endif
            if (ovector)
                ovector->clear();
            return -1;
        }

        const std::string regexpattern = m_pattern.ascii();
        std::sregex_iterator matchbegin = std::sregex_iterator(regexpattern.begin(), regexpattern.end(), m_regExp);
        std::sregex_iterator matchend = std::sregex_iterator();
        size_t nummatches = 0;
        for (std::sregex_iterator iter = matchbegin; iter != matchend; iter++) {
            const std::smatch itermatch = *iter;
            offsetVector[nummatches] = itermatch.position();
            offsetVector[nummatches + 1] = itermatch.length();
            offsetVector[nummatches + 2] = 0;
            nummatches++;
        }

        return offsetVector[0];
    }

    return -1;
}

} // namespace JSC
