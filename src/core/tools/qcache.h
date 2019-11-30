/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QCACHE_H
#define QCACHE_H

#include <QtCore/qhash.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

template <class Key, class T>
class QCache
{
    QHash<Key, T*> hash;
    int mx;

    Q_DISABLE_COPY(QCache)

public:
    inline explicit QCache(int maxCost = 100);
    inline ~QCache() { clear(); }

    inline int maxCost() const { return mx; }
    inline void setMaxCost(int m);

    inline int size() const { return hash.size(); }
    inline int count() const { return hash.count(); }
    inline bool isEmpty() const { return hash.isEmpty(); }
    inline QList<Key> keys() const { return hash.keys(); }
    inline void clear() { hash.clear(); }

    inline bool insert(const Key &key, T *object, int cost = 1);
    inline T *object(const Key &key) const { return hash.value(key, Q_NULLPTR); }
    inline bool contains(const Key &key) const { return hash.contains(key); }
    inline T *operator[](const Key &key) const { return object(key); }

    inline bool remove(const Key &key) { return hash.remove(key) != 0; }
    inline T *take(const Key &key) { return hash.take(key); }
};

template <class Key, class T>
inline QCache<Key, T>::QCache(int amaxCost)
    : mx(amaxCost)
{
    hash.reserve(amaxCost);
}

template <class Key, class T>
inline void QCache<Key,T>::setMaxCost(int m)
{
    mx = m;
    hash.clear();
    hash.reserve(m);
}

template <class Key, class T>
inline bool QCache<Key,T>::insert(const Key &akey, T *aobject, int acost)
{
    if (acost > mx) {
        delete aobject;
        return false;
    } else if (hash.size() >= mx) {
        hash.erase(hash.begin());
    }
    hash.insert(akey, aobject);
    return true;
}

QT_END_NAMESPACE

QT_END_HEADER

#endif // QCACHE_H
