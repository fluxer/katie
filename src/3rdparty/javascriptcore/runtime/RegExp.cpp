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

// for reference:
// https://en.cppreference.com/w/cpp/regex/basic_regex

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
    m_constructionError.clear();
    m_numSubpatterns = 0;

    Qt::CaseSensitivity regexOptions = Qt::CaseSensitive;
    if (ignoreCase())
        regexOptions = Qt::CaseInsensitive;

    m_regExp = QRegExp(QString(m_pattern), regexOptions);
    if (!m_regExp.isValid()) {
        m_constructionError = m_regExp.errorString().toLatin1();
        return;
    }
    m_numSubpatterns = m_regExp.captureCount();
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

        const QString qstring(s);
#if 0
        if (multiline()) {
            // TODO: split lines and match
        }
#endif
        const bool didmatch = (m_regExp.indexIn(qstring, startOffset) != -1);

        if (!didmatch) {
#ifndef QT_NO_DEBUG
            fprintf(stderr, "jsRegExpExecute failed with result\n");
#endif
            if (ovector)
                ovector->clear();
            return -1;
        }

        size_t nummatches = 0;
        for (int i = 0; i < m_regExp.matchedLength(); i++) {
            offsetVector[nummatches] = m_regExp.pos(i);
            offsetVector[nummatches + 1] = m_regExp.cap(i).length();
            offsetVector[nummatches + 2] = 0;
            nummatches++;
        }

        return offsetVector[0];
    }

    return -1;
}

} // namespace JSC
