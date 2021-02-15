/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the FOO module of the Katie Toolkit.
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


#ifndef QDBUSUNIXFILEDESCRIPTOR_H
#define QDBUSUNIXFILEDESCRIPTOR_H

#include <QtCore/qshareddata.h>
#include <QtCore/qmetatype.h>


#ifdef Q_COMPILER_RVALUE_REFS
# include <utility>
#endif


QT_BEGIN_NAMESPACE


class QDBusUnixFileDescriptorPrivate;
template<> QExplicitlySharedDataPointer<QDBusUnixFileDescriptorPrivate>::~QExplicitlySharedDataPointer();

class Q_DBUS_EXPORT QDBusUnixFileDescriptor
{
public:
    QDBusUnixFileDescriptor();
    explicit QDBusUnixFileDescriptor(int fileDescriptor);
    QDBusUnixFileDescriptor(const QDBusUnixFileDescriptor &other);
    QDBusUnixFileDescriptor &operator=(const QDBusUnixFileDescriptor &other);
    ~QDBusUnixFileDescriptor();

    bool isValid() const;

    int fileDescriptor() const;
    void setFileDescriptor(int fileDescriptor);

    void giveFileDescriptor(int fileDescriptor);
    int takeFileDescriptor();

#if defined(Q_COMPILER_RVALUE_REFS)
    QDBusUnixFileDescriptor(QDBusUnixFileDescriptor &&other) : d(static_cast<Data &&>(other.d))
    { }
    inline QDBusUnixFileDescriptor &operator=(QDBusUnixFileDescriptor &&other)
    { d.swap(other.d); return *this; }
#endif

protected:
    typedef QExplicitlySharedDataPointer<QDBusUnixFileDescriptorPrivate>  Data;
    Data d;
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QDBusUnixFileDescriptor)
Q_DECLARE_METATYPE(QList<QDBusUnixFileDescriptor>)


#endif // QDBUSUNIXFILEDESCRIPTOR_H
