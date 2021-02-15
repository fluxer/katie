/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtTest module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QTESTXUNITSTREAMER_H
#define QTESTXUNITSTREAMER_H

#include <QtTest/qtestbasicstreamer.h>


QT_BEGIN_NAMESPACE

class QTestLogger;

class QTestXunitStreamer: public QTestBasicStreamer
{
    public:
        QTestXunitStreamer();
        ~QTestXunitStreamer();

        void formatStart(const QTestElement *element, QTestCharBuffer *formatted) const;
        void formatEnd(const QTestElement *element, QTestCharBuffer *formatted) const;
        void formatAfterAttributes(const QTestElement *element, QTestCharBuffer *formatted) const;
        void formatAttributes(const QTestElement *element, const QTestElementAttribute *attribute, QTestCharBuffer *formatted) const;
        void output(QTestElement *element) const;
        void outputElements(QTestElement *element, bool isChildElement = false) const;

    private:
        void displayXunitXmlHeader() const;
        static void indentForElement(const QTestElement* element, char* buf, int size);
};

QT_END_NAMESPACE


#endif
