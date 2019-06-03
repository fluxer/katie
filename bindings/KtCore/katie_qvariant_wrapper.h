/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the FOO module of the Katie Toolkit.
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

#ifndef SBK_QVARIANTWRAPPER_H
#define SBK_QVARIANTWRAPPER_H

#include <qvariant.h>

class QVariantWrapper : public Katie::QVariant
{
public:
    QVariantWrapper();
    QVariantWrapper(Katie::QVariant::Type type);
    QVariantWrapper(bool b);
    QVariantWrapper(const Katie::QByteArray & bytearray);
    QVariantWrapper(const Katie::QChar & qchar);
    QVariantWrapper(const Katie::QLatin1String & string);
    QVariantWrapper(const Katie::QRegExp & regExp);
    QVariantWrapper(const Katie::QString & string);
    QVariantWrapper(const Katie::QStringList & stringlist);
    QVariantWrapper(const Katie::QVariant& self) : Katie::QVariant(self)
    {
    }

    QVariantWrapper(const char * str);
    QVariantWrapper(double d);
    QVariantWrapper(float f);
    QVariantWrapper(int i);
    QVariantWrapper(int typeOrUserType, const void * copy);
    QVariantWrapper(int typeOrUserType, const void * copy, uint flags);
    QVariantWrapper(long long ll);
    QVariantWrapper(uint ui);
    QVariantWrapper(unsigned long long ull);
    inline bool cmp_protected(const Katie::QVariant & other) const { return Katie::QVariant::cmp(other); }
    inline void create_protected(int type, const void * copy) { Katie::QVariant::create(type, copy); }
    ~QVariantWrapper();
};

#endif // SBK_QVARIANTWRAPPER_H

