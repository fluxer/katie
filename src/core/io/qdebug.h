/****************************************************************************
**
** Copyright (C) 2022 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#ifndef QDEBUG_H
#define QDEBUG_H

#include <QtCore/qvector.h>
#include <QtCore/qset.h>

QT_BEGIN_NAMESPACE

class QDebugPrivate;

class Q_CORE_EXPORT QDebug
{
public:
    QDebug(QIODevice *device);
    QDebug(QtMsgType type);
    QDebug(const QDebug &o);
    QDebug &operator=(const QDebug &other);
    ~QDebug();

    QDebug &space();
    QDebug &nospace();
    QDebug &maybeSpace();

    QDebug &operator<<(QChar t);
    QDebug &operator<<(bool t);
    QDebug &operator<<(char t);
    QDebug &operator<<(qint8 t);
    QDebug &operator<<(quint8 t);
    QDebug &operator<<(qint16 t);
    QDebug &operator<<(quint16 t);
    QDebug &operator<<(qint32 t);
    QDebug &operator<<(quint32 t);
    QDebug &operator<<(qint64 t);
    QDebug &operator<<(quint64 t);
    QDebug &operator<<(long int t);
    QDebug &operator<<(unsigned long int t);
    QDebug &operator<<(float t);
    QDebug &operator<<(double t);
    QDebug &operator<<(const char* t);
    QDebug &operator<<(const QString &t);
    QDebug &operator<<(const QStringRef &t);
    QDebug &operator<<(const QLatin1String &t);
    QDebug &operator<<(const QByteArray &t);
    QDebug &operator<<(const void* t);

private:
    QDebugPrivate *d_ptr;
};

template <class T>
inline QDebug operator<<(QDebug debug, const QList<T> &list)
{
    debug.nospace() << '(';
    for (typename QList<T>::size_type i = 0; i < list.count(); ++i) {
        if (i)
            debug << ", ";
        debug << list.at(i);
    }
    debug << ')';
    return debug.space();
}

template <typename T>
inline QDebug operator<<(QDebug debug, const QVector<T> &vec)
{
    debug.nospace() << "QVector";
    return operator<<(debug, vec.toList());
}

template <class aKey, class aT>
inline QDebug operator<<(QDebug debug, const QMap<aKey, aT> &map)
{
    debug.nospace() << "QMap(";
    for (typename QMap<aKey, aT>::const_iterator it = map.constBegin();
         it != map.constEnd(); ++it) {
        debug << '(' << it.key() << ", " << it.value() << ')';
    }
    debug << ')';
    return debug.space();
}

template <class aKey, class aT>
inline QDebug operator<<(QDebug debug, const QHash<aKey, aT> &hash)
{
    debug.nospace() << "QHash(";
    for (typename QHash<aKey, aT>::const_iterator it = hash.constBegin();
            it != hash.constEnd(); ++it)
        debug << '(' << it.key() << ", " << it.value() << ')';
    debug << ')';
    return debug.space();
}

template <class T1, class T2>
inline QDebug operator<<(QDebug debug, const QPair<T1, T2> &pair)
{
    debug.nospace() << "QPair(" << pair.first << ',' << pair.second << ')';
    return debug.space();
}

template <typename T>
inline QDebug operator<<(QDebug debug, const QSet<T> &set)
{
    debug.nospace() << "QSet";
    return operator<<(debug, set.toList());
}

template <class T>
inline QDebug operator<<(QDebug debug, const QFlags<T> &flags)
{
    debug.nospace() << "QFlags(";
    bool needSeparator = false;
    for (uint i = 0; i < sizeof(T) * 8; ++i) {
        if (flags.testFlag(T(1 << i))) {
            if (needSeparator)
                debug.nospace() << '|';
            else
                needSeparator = true;
            debug.nospace() << "0x" << QByteArray::number(T(1 << i), 16).constData();
        }
    }
    debug << ')';
    return debug.space();
}

Q_CORE_EXPORT_INLINE QDebug qDebug() { return QDebug(QtDebugMsg); }
Q_CORE_EXPORT_INLINE QDebug qWarning() { return QDebug(QtWarningMsg); }
Q_CORE_EXPORT_INLINE QDebug qCritical() { return QDebug(QtCriticalMsg); }

QT_END_NAMESPACE


#endif // QDEBUG_H
