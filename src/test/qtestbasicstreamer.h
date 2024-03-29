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

#ifndef QTESTBASICSTREAMER_H
#define QTESTBASICSTREAMER_H

#include <QtCore/qglobal.h>


QT_BEGIN_NAMESPACE

class QTestElement;
class QTestElementAttribute;
class QTestLogger;
struct QTestCharBuffer;

class Q_TEST_EXPORT QTestBasicStreamer
{
public:
    QTestBasicStreamer();
    virtual ~QTestBasicStreamer();

    virtual void output(QTestElement *element) const;

    void outputString(const char *msg) const;
    bool isTtyOutput();
    void startStreaming();
    void stopStreaming();

    void setLogger(const QTestLogger *tstLogger);
    const QTestLogger *logger() const;

protected:
    virtual void formatStart(const QTestElement *element, QTestCharBuffer *formatted) const;
    virtual void formatEnd(const QTestElement *element, QTestCharBuffer *formatted) const;
    virtual void formatBeforeAttributes(const QTestElement *element, QTestCharBuffer *formatted) const;
    virtual void formatAfterAttributes(const QTestElement *element, QTestCharBuffer *formatted) const;
    virtual void formatAttributes(const QTestElement *element, const QTestElementAttribute *attribute, QTestCharBuffer *formatted) const;
    virtual void outputElements(QTestElement *element, bool isChildElement = false) const;
    virtual void outputElementAttributes(const QTestElement *element, QTestElementAttribute *attribute) const;

private:
    const QTestLogger *testLogger;
};

QT_END_NAMESPACE

#endif
